#include "Weapon_RailGun.h"
#include "../Raven_Bot.h"
#include "misc/Cgdi.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../lua/Raven_Scriptor.h"
#include "fuzzy/FuzzyOperators.h"


//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
RailGun::RailGun(Raven_Bot*   owner):

                      Raven_Weapon(type_rail_gun,
                                   script->GetInt("RailGun_DefaultRounds"),
                                   script->GetInt("RailGun_MaxRoundsCarried"),
                                   script->GetDouble("RailGun_FiringFreq"),
                                   script->GetDouble("RailGun_IdealRange"),
                                   script->GetDouble("Slug_MaxSpeed"),
                                   owner)
{

    //setup the vertex buffer
  const int NumWeaponVerts = 4;
  const Vector2D weapon[NumWeaponVerts] = {Vector2D(0, -1),
                                           Vector2D(10, -1),
                                           Vector2D(10, 1),
                                           Vector2D(0, 1)
                                           };

  
  for (int vtx=0; vtx<NumWeaponVerts; ++vtx)
  {
    m_vecWeaponVB.push_back(weapon[vtx]);
  }

  //setup the fuzzy module
  InitializeFuzzyModule();

}


//------------------------------ ShootAt --------------------------------------

inline void RailGun::ShootAt(Vector2D pos)
{ 
  if (NumRoundsRemaining() > 0 && isReadyForNextShot())
  {
	  double timeTargetHasBeenVisible = m_pOwner->GetTargetSys()->GetTimeTargetHasBeenVisible(); // a modifier
	  double ShootTime = Clock->GetCurrentTime();
	  double precision = getPrecision((Vec2DDistance(m_pOwner->Pos(), m_pOwner->GetTargetSys()->GetTarget()->Pos())), m_pOwner->Velocity(), ShootTime - timeTargetHasBeenVisible);
    //fire a round
    m_pOwner->GetWorld()->AddRailGunSlug(m_pOwner, pos*(precision / 100));

    UpdateTimeWeaponIsNextAvailable();

    m_iNumRoundsLeft--;

    //add a trigger to the game so that the other bots can hear this shot
    //(provided they are within range)
    m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner, script->GetDouble("RailGun_SoundRange"));
  }
}

//---------------------------- Desirability -----------------------------------
//
//-----------------------------------------------------------------------------
double RailGun::GetDesirability(double DistanceToTarget)
{
  if (m_iNumRoundsLeft == 0)
  {
    m_dLastDesirabilityScore = 0;
  }
  else
  {
    //fuzzify distance and amount of ammo
    m_FuzzyModule.Fuzzify("DistanceToTarget", DistanceToTarget);
    m_FuzzyModule.Fuzzify("AmmoStatus", (double)m_iNumRoundsLeft);

    m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);
  }

  return m_dLastDesirabilityScore;
}

//---------------------------- Precision -----------------------------------
//
//-----------------------------------------------------------------------------
double RailGun::getPrecision(double DistanceToTarget, Vector2D Velocity, double ViewTime)
{
	double velocitylenght = Velocity.Length();
	std::cerr << "distance : " << DistanceToTarget << " velocity : " << Velocity.Length() << " time : " << ViewTime << std::endl;
	//fuzzify distance and amount of DistanceToTarget
	m_FuzzyModule.Fuzzify("DistanceToTarget", DistanceToTarget);
	m_FuzzyModule.Fuzzify("Velocity", Velocity.Length());
	m_FuzzyModule.Fuzzify("ViewTime", ViewTime);
	m_Precision = m_FuzzyModule.DeFuzzify("Precision", FuzzyModule::max_av);


	return m_Precision;
}


//----------------------- InitializeFuzzyModule -------------------------------
//
//  set up some fuzzy variables and rules
//-----------------------------------------------------------------------------
void RailGun::InitializeFuzzyModule()
{ 

  FuzzyVariable& DistanceToTarget = m_FuzzyModule.CreateFLV("DistanceToTarget");
  
  FzSet& Target_Close = DistanceToTarget.AddLeftShoulderSet("Target_Close", 0, 25, 150);
  FzSet& Target_Medium = DistanceToTarget.AddTriangularSet("Target_Medium", 25, 150, 300);
  FzSet& Target_Far = DistanceToTarget.AddRightShoulderSet("Target_Far", 150, 300, 1000);

  FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability");
  
  FzSet& VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 50, 75, 100);
  FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 25, 50, 75);
  FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);

  FuzzyVariable& AmmoStatus = m_FuzzyModule.CreateFLV("AmmoStatus");
  FzSet& Ammo_Loads = AmmoStatus.AddRightShoulderSet("Ammo_Loads", 15, 30, 100);
  FzSet& Ammo_Okay = AmmoStatus.AddTriangularSet("Ammo_Okay", 0, 15, 30);
  FzSet& Ammo_Low = AmmoStatus.AddTriangularSet("Ammo_Low", 0, 0, 15);


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
  m_FuzzyModule.AddRule(FzAND(Target_Close, Velocity_High, ViewTime_High), Precision_Low);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Velocity_Low, ViewTime_High), Precision_Low);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Velocity_Low, ViewTime_Low), Precision_Medium);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Velocity_High, ViewTime_High), Precision_Low);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Velocity_High, ViewTime_Low), Precision_High);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Velocity_Low, ViewTime_High), Precision_Medium);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Velocity_Low, ViewTime_Low), Precision_High);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Velocity_High, ViewTime_High), Precision_High);
  m_FuzzyModule.AddRule(FzAND(Target_Far, Velocity_High, ViewTime_Low), Precision_High);


  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Loads), FzFairly(Desirable));
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Okay),  FzFairly(Desirable));
  m_FuzzyModule.AddRule(FzAND(Target_Close, Ammo_Low), Undesirable);

  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Loads), VeryDesirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Okay), Desirable);
  m_FuzzyModule.AddRule(FzAND(Target_Medium, Ammo_Low), Desirable);

  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Loads), FzVery(VeryDesirable));
  m_FuzzyModule.AddRule(FzAND(Target_Far, Ammo_Okay), FzVery(VeryDesirable));
  m_FuzzyModule.AddRule(FzAND(Target_Far, FzFairly(Ammo_Low)), VeryDesirable);
}

//-------------------------------- Render -------------------------------------
//-----------------------------------------------------------------------------
void RailGun::Render()
{
    m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
                                   m_pOwner->Pos(),
                                   m_pOwner->Facing(),
                                   m_pOwner->Facing().Perp(),
                                   m_pOwner->Scale());

  gdi->BluePen();

  gdi->ClosedShape(m_vecWeaponVBTrans);
}