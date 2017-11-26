#include "Weapon_Blaster.h"
#include "../Raven_Bot.h"
#include "misc/Cgdi.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../lua/Raven_Scriptor.h"
#include "fuzzy/FuzzyOperators.h"


//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
Blaster::Blaster(Raven_Bot*   owner):

                      Raven_Weapon(type_blaster,
                                   script->GetInt("Blaster_DefaultRounds"),
                                   script->GetInt("Blaster_MaxRoundsCarried"),
                                   script->GetDouble("Blaster_FiringFreq"),
                                   script->GetDouble("Blaster_IdealRange"),
                                   script->GetDouble("Bolt_MaxSpeed"),
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

inline void Blaster::ShootAt(Vector2D pos)
{ 
  if (isReadyForNextShot())
  {
	  double timeTargetHasBeenVisible = m_pOwner->GetTargetSys()->GetTimeTargetHasBeenVisible(); // a modifier
	  double ShootTime = Clock->GetCurrentTime();
	  double precision = getPrecision((Vec2DDistance(m_pOwner->Pos(), m_pOwner->GetTargetSys()->GetTarget()->Pos())), m_pOwner->Velocity(), ShootTime - timeTargetHasBeenVisible);
    //fire!
    m_pOwner->GetWorld()->AddBolt(m_pOwner, pos*(precision/100));

    UpdateTimeWeaponIsNextAvailable();

    //add a trigger to the game so that the other bots can hear this shot
    //(provided they are within range)
    m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner, script->GetDouble("Blaster_SoundRange"));
  }
}



//---------------------------- Desirability -----------------------------------
//
//-----------------------------------------------------------------------------
double Blaster::GetDesirability(double DistToTarget)
{
  //fuzzify distance and amount of ammo
  m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);

  m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);

  return m_dLastDesirabilityScore;
}


//---------------------------- Precision -----------------------------------
//
//-----------------------------------------------------------------------------
double Blaster::getPrecision(double DistToTarget, Vector2D Velocity, double ViewTime)
{
	double velocitylenght = Velocity.Length();
	std::cerr << "distance : " << DistToTarget << " velocity : " << Velocity.Length() << " time : " << ViewTime<<std::endl;
	//fuzzify distance and amount of ammo
	m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);
	m_FuzzyModule.Fuzzify("Velocity", Velocity.Length());
	m_FuzzyModule.Fuzzify("ViewTime", ViewTime);
	m_Precision = m_FuzzyModule.DeFuzzify("Precision", FuzzyModule::max_av);


	return m_Precision;
}


//----------------------- InitializeFuzzyModule -------------------------------
//
//  set up some fuzzy variables and rules
//-----------------------------------------------------------------------------
void Blaster::InitializeFuzzyModule()
{
	FuzzyVariable& DistToTarget = m_FuzzyModule.CreateFLV("DistToTarget");

	FzSet& Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close", 0, 25, 150);
	FzSet& Target_Medium = DistToTarget.AddTriangularSet("Target_Medium", 25, 150, 300);
	FzSet& Target_Far = DistToTarget.AddRightShoulderSet("Target_Far", 150, 300, 1000);

	FuzzyVariable& Velocity = m_FuzzyModule.CreateFLV("Velocity");
	FzSet& Velocity_Low = Velocity.AddLeftShoulderSet("Velocity_Low", 0, 0.2, 0.6);
	FzSet& Velocity_High = Velocity.AddRightShoulderSet("Velocity_High", 0.4, 0.7, 2);

	FuzzyVariable& ViewTime = m_FuzzyModule.CreateFLV("ViewTime");
	FzSet& ViewTime_Low = ViewTime.AddLeftShoulderSet("ViewTime_Low", 0, 50, 100);
	FzSet& ViewTime_High = ViewTime.AddRightShoulderSet("ViewTime_High", 10, 350, 10000);

	FuzzyVariable& Precision = m_FuzzyModule.CreateFLV("Precision");
	FzSet& Precision_Low = Precision.AddLeftShoulderSet("Precision_High", 0, 10, 30);
	FzSet& Precision_Medium = Precision.AddTriangularSet("Precision_Medium", 20, 40, 60);

	FzSet& Precision_High = Precision.AddRightShoulderSet("Precision_Low", 40, 70, 100);

	m_FuzzyModule.AddRule(FzAND(Target_Close, Velocity_Low, ViewTime_High), Precision_Low);
	m_FuzzyModule.AddRule(FzAND(Target_Close, Velocity_Low,  ViewTime_Low), Precision_Medium);
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

  FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability"); 
  FzSet& VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 50, 75, 100);
  FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 25, 50, 75);
  FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);

  m_FuzzyModule.AddRule(Target_Close, Desirable);
  m_FuzzyModule.AddRule(Target_Medium, FzVery(Undesirable));
  m_FuzzyModule.AddRule(Target_Far, FzVery(Undesirable));
}


//-------------------------------- Render -------------------------------------
//-----------------------------------------------------------------------------
void Blaster::Render()
{
   m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
                                   m_pOwner->Pos(),
                                   m_pOwner->Facing(),
                                   m_pOwner->Facing().Perp(),
                                   m_pOwner->Scale());

  gdi->GreenPen();

  gdi->ClosedShape(m_vecWeaponVBTrans);
}