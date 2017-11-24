#include "Weapon_RocketLauncher.h"
#include "../Raven_Bot.h"
#include "misc/Cgdi.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../lua/Raven_Scriptor.h"
#include "fuzzy/FuzzyOperators.h"


//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
RocketLauncher::RocketLauncher(Raven_Bot*   owner):

                      Raven_Weapon(type_rocket_launcher,
                                   script->GetInt("RocketLauncher_DefaultRounds"),
                                   script->GetInt("RocketLauncher_MaxRoundsCarried"),
                                   script->GetDouble("RocketLauncher_FiringFreq"),
                                   script->GetDouble("RocketLauncher_IdealRange"),
                                   script->GetDouble("Rocket_MaxSpeed"),
                                   owner)
{
    //setup the vertex buffer
  const int NumWeaponVerts = 8;
  const Vector2D weapon[NumWeaponVerts] = {Vector2D(0, -3),
                                           Vector2D(6, -3),
                                           Vector2D(6, -1),
                                           Vector2D(15, -1),
                                           Vector2D(15, 1),
                                           Vector2D(6, 1),
                                           Vector2D(6, 3),
                                           Vector2D(0, 3)
                                           };
  for (int vtx=0; vtx<NumWeaponVerts; ++vtx)
  {
    m_vecWeaponVB.push_back(weapon[vtx]);
  }

  //setup the fuzzy module
  InitializeFuzzyModule();

}


//------------------------------ ShootAt --------------------------------------
//-----------------------------------------------------------------------------
inline void RocketLauncher::ShootAt(Vector2D pos)
{ 
  if (NumRoundsRemaining() > 0 && isReadyForNextShot())
  {
	  double timeTargetHasBeenVisible = m_pOwner->GetTargetSys()->GetTimeTargetHasBeenVisible(); // a modifier
	  double ShootTime = Clock->GetCurrentTime();
	  double precision = getPrecision((Vec2DDistance(m_pOwner->Pos(), m_pOwner->GetTargetSys()->GetTarget()->Pos())), m_pOwner->Velocity(), ShootTime - timeTargetHasBeenVisible);
    //fire off a rocket!
    m_pOwner->GetWorld()->AddRocket(m_pOwner, pos*(precision / 100));

    m_iNumRoundsLeft--;

    UpdateTimeWeaponIsNextAvailable();

    //add a trigger to the game so that the other bots can hear this shot
    //(provided they are within range)
    m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner, script->GetDouble("RocketLauncher_SoundRange"));
  }
}

//---------------------------- Desirability -----------------------------------
//
//-----------------------------------------------------------------------------
double RocketLauncher::GetDesirability(double DistToTarget)
{
  if (m_iNumRoundsLeft == 0)
  {
    m_dLastDesirabilityScore = 0;
  }
  else
  {
    //fuzzify distance and amount of ammo
    m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);
    m_FuzzyModule.Fuzzify("AmmoStatus", (double)m_iNumRoundsLeft);

    m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);
  }

  return m_dLastDesirabilityScore;
}

//---------------------------- Precision -----------------------------------
//
//-----------------------------------------------------------------------------
double RocketLauncher::getPrecision(double DistToTarget, Vector2D Velocity, double ViewTime)
{
	double velocitylenght = Velocity.Length();
	std::cerr << "distance : " << DistToTarget << " velocity : " << Velocity.Length() << " time : " << ViewTime << std::endl;
	//fuzzify distance and amount of ammo
	m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);
	m_FuzzyModule.Fuzzify("Velocity", Velocity.Length());
	m_FuzzyModule.Fuzzify("ViewTime", ViewTime);
	m_Precision = m_FuzzyModule.DeFuzzify("Precision", FuzzyModule::max_av);


	return m_Precision;
}

//-------------------------  InitializeFuzzyModule ----------------------------
//
//  set up some fuzzy variables and rules
//-----------------------------------------------------------------------------
void RocketLauncher::InitializeFuzzyModule()
{
  FuzzyVariable& DistToTarget = m_FuzzyModule.CreateFLV("DistToTarget");

  /*FzSet& Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close",0,25,150);
  FzSet& Target_Medium = DistToTarget.AddTriangularSet("Target_Medium",25,150,300);
  FzSet& Target_Far = DistToTarget.AddRightShoulderSet("Target_Far",150,300,1000);*/
  FzSet& Target_Melee = DistToTarget.AddLeftShoulderSet("Target_Melee", 0, 5, 10);
  FzSet& Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close", 10, 25, 150);
  FzSet& Target_Medium = DistToTarget.AddTriangularSet("Target_Medium", 25, 150, 300);
  FzSet& Target_Far = DistToTarget.AddRightShoulderSet("Target_Far", 150, 300, 800);
  FzSet& Target_VeryFar = DistToTarget.AddRightShoulderSet("Target_VeryFar", 800, 950, 1000);

  FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability"); 
  /*FzSet& VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 50, 75, 100);
  FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 25, 50, 75);
  FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);*/
  FzSet& Necessary = Desirability.AddRightShoulderSet("Necessary", 95, 97.5, 100);
  FzSet& VeryDesirable = Desirability.AddTriangularSet("VeryDesirable", 60, 75, 95);
  FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 40, 50, 60);
  FzSet& Undesirable = Desirability.AddTriangularSet("Undesirable", 5, 25, 40);
  FzSet& Unthinkable = Desirability.AddLeftShoulderSet("Unthinkable", 0, 2.5, 5);

  FuzzyVariable& AmmoStatus = m_FuzzyModule.CreateFLV("AmmoStatus");
  /*FzSet& Ammo_Loads = AmmoStatus.AddRightShoulderSet("Ammo_Loads", 10, 30, 100);
  FzSet& Ammo_Okay = AmmoStatus.AddTriangularSet("Ammo_Okay", 0, 10, 30);
  FzSet& Ammo_Low = AmmoStatus.AddTriangularSet("Ammo_Low", 0, 0, 10);*/
  FzSet& Ammo_Full = AmmoStatus.AddRightShoulderSet("Ammo_Full", 45, 47.5, 50);
  FzSet& Ammo_Loads = AmmoStatus.AddTriangularSet("Ammo_Loads", 25, 30, 45);
  FzSet& Ammo_Okay = AmmoStatus.AddTriangularSet("Ammo_Okay", 10, 20, 25);
  FzSet& Ammo_Low = AmmoStatus.AddTriangularSet("Ammo_Low", 2, 3, 10);
  FzSet& Ammo_Empty = AmmoStatus.AddLeftShoulderSet("Ammo_Empty", 0, 1, 2);


  FuzzyVariable& Velocity = m_FuzzyModule.CreateFLV("Velocity");
  FzSet& Velocity_Low = Velocity.AddLeftShoulderSet("Velocity_Low", 0, 0.2, 0.6);
  FzSet& Velocity_High = Velocity.AddRightShoulderSet("Velocity_High", 0.4, 0.7, 1);

  FuzzyVariable& ViewTime = m_FuzzyModule.CreateFLV("ViewTime");
  FzSet& ViewTime_Low = ViewTime.AddLeftShoulderSet("ViewTime_Low", 0, 50, 100);
  //FzSet& ViewTime_Medium = DistToTarget.AddTriangularSet("ViewTime_Medium", 0.5, 2, 3.5);
  FzSet& ViewTime_High = ViewTime.AddRightShoulderSet("ViewTime_High", 10, 350, 10000);

  FuzzyVariable& Precision = m_FuzzyModule.CreateFLV("Precision");
  FzSet& Precision_Low = Precision.AddLeftShoulderSet("Precision_High", 0, 10, 30);
  FzSet& Precision_Medium = Precision.AddTriangularSet("Precision_Medium", 20, 40, 60);

  FzSet& Precision_High = Precision.AddRightShoulderSet("Precision_Low", 40, 70, 100);

  m_FuzzyModule.AddRule(FzAND(Target_Close, Velocity_Low, ViewTime_High), Precision_Low);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Velocity_Low, ViewTime_Low), Precision_Medium);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Velocity_High, ViewTime_Low), Precision_Medium);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Velocity_High, ViewTime_High), Precision_Medium);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Velocity_Low, ViewTime_High), Precision_Low);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Velocity_Low, ViewTime_Low), Precision_Medium);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Velocity_High, ViewTime_High), Precision_Medium);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Velocity_High, ViewTime_Low), Precision_High);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Velocity_Low, ViewTime_High), Precision_Medium);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Velocity_Low, ViewTime_Low), Precision_High);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Velocity_High, ViewTime_High), Precision_High);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Velocity_High, ViewTime_Low), Precision_High);
  m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Velocity_Low, ViewTime_High), Precision_High);
  m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Velocity_Low, ViewTime_Low), Precision_High);
  m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Velocity_High, ViewTime_High), Precision_High);
  m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Velocity_High, ViewTime_Low), Precision_High);

  m_FuzzyModule.AddRule(FzAND(Target_Melee, Ammo_Full), Unthinkable);
  m_FuzzyModule.AddRule(FzAND(Target_Melee, Ammo_Loads), Unthinkable);
  m_FuzzyModule.AddRule(FzAND(Target_Melee, Ammo_Okay), Unthinkable);
  m_FuzzyModule.AddRule(FzAND(Target_Melee, Ammo_Low), Unthinkable);
  m_FuzzyModule.AddRule(FzAND(Target_Melee, Ammo_Empty), Unthinkable);

  /*m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Loads), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Okay), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Low), Undesirable);*/
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Full), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Loads), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Okay), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Low), Unthinkable);
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Empty), Unthinkable);

  /*m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Loads), VeryDesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Okay), VeryDesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Low), Desirable);*/
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Full), Necessary);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Loads), Necessary);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Okay), VeryDesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Low), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Empty), Undesirable);

  /*m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Loads), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Okay), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Low), Undesirable);*/
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Full), VeryDesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Loads), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Okay), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Low), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Empty), Undesirable);

  m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Ammo_Full), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Ammo_Loads), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Ammo_Okay), Undesirable);
  m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Ammo_Low), Unthinkable);
  m_FuzzyModule.AddRule(FzAND(Target_VeryFar, Ammo_Empty), Unthinkable);
}


//-------------------------------- Render -------------------------------------
//-----------------------------------------------------------------------------
void RocketLauncher::Render()
{
    m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
                                   m_pOwner->Pos(),
                                   m_pOwner->Facing(),
                                   m_pOwner->Facing().Perp(),
                                   m_pOwner->Scale());

  gdi->RedPen();

  gdi->ClosedShape(m_vecWeaponVBTrans);
}