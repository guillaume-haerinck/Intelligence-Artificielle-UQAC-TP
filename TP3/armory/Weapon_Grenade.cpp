#include "Weapon_Grenade.h"
#include "../Raven_Bot.h"
#include "misc/Cgdi.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../lua/Raven_Scriptor.h"
#include "fuzzy/FuzzyOperators.h"


//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
Grenade::Grenade(Raven_Bot*   owner) :

	Raven_Weapon(type_grenade,
		script->GetInt("Grenade_DefaultRounds"),
		script->GetInt("Grenade_MaxRoundsCarried"),
		script->GetDouble("Grenade_FiringFreq"),
		script->GetDouble("Grenade_IdealRange"),
		script->GetDouble("Grenade_MaxSpeed"),
		owner)
{
	//setup the vertex buffer
	const int NumWeaponVerts = 1;
	const Vector2D weapon[NumWeaponVerts] = { Vector2D(0, 7)
	};
	for (int vtx = 0; vtx<NumWeaponVerts; ++vtx)
	{
		m_vecWeaponVB.push_back(weapon[vtx]);
	}

	//setup the fuzzy module
	InitializeFuzzyModule();

}


//------------------------------ ShootAt --------------------------------------
//-----------------------------------------------------------------------------
inline void Grenade::ShootAt(Vector2D pos)
{
	if (NumRoundsRemaining() > 0 && isReadyForNextShot())
	{
		double timeTargetHasBeenVisible = m_pOwner->GetTargetSys()->GetTimeTargetHasBeenVisible(); // a modifier
		double ShootTime = Clock->GetCurrentTime();

		Vector2D target;
		if (m_pOwner->isPossessed()) {
			target = pos;
		}
		else {
			target = m_pOwner->GetTargetSys()->GetTarget()->Pos();
		}
		double precision = getPrecision(Vec2DDistance(m_pOwner->Pos(), target), m_pOwner->Velocity(), ShootTime - timeTargetHasBeenVisible);
		//fire off a rocket!
		m_pOwner->GetWorld()->AddGrenade(m_pOwner, pos + getVectorPrecision(precision));

		UpdateTimeWeaponIsNextAvailable();

		//add a trigger to the game so that the other bots can hear this shot
		//(provided they are within range)
		m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner, script->GetDouble("Grenade_SoundRange"));
	}
}

//---------------------------- Desirability -----------------------------------
//
//-----------------------------------------------------------------------------
double Grenade::GetDesirability(double DistToTarget)
{
	if (m_iNumRoundsLeft == 0 || !isReadyForNextShot())
	{
		m_dLastDesirabilityScore = 0;
	}
	else
	{
		//fuzzify distance and amount of ammo
		m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);
		m_FuzzyModule.Fuzzify("ViewTimeDesirability", (double) Clock->GetCurrentTime() - m_pOwner->GetTargetSys()->GetTimeTargetHasBeenVisible());

		m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);
	}

	return m_dLastDesirabilityScore;
}

//---------------------------- Precision -----------------------------------
//
//-----------------------------------------------------------------------------
double Grenade::getPrecision(double DistToTarget, Vector2D Velocity, double ViewTime)
{
	double velocitylenght = Velocity.Length();
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
void Grenade::InitializeFuzzyModule()
{
	FuzzyVariable& DistToTarget = m_FuzzyModule.CreateFLV("DistToTarget");
	FzSet& Target_Close = DistToTarget.AddLeftShoulderSet("Target_Close",0,25,150);
	FzSet& Target_Medium = DistToTarget.AddTriangularSet("Target_Medium",55,150,300);
	FzSet& Target_Far = DistToTarget.AddRightShoulderSet("Target_Far",150,300,1000);

	FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability");
	FzSet& VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 50, 75, 100);
	FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 25, 50, 75);
	FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);

	FuzzyVariable& ViewTimeDesirability = m_FuzzyModule.CreateFLV("ViewTimeDesirability");
	FzSet& NoVisibility = ViewTimeDesirability.AddSingletonSet("No_Visibility", 0, 0, 1);
	FzSet& ShortVisibility = ViewTimeDesirability.AddLeftShoulderSet("Short_Visibility", 1, 100, 400);
	FzSet& MediumVisibility = ViewTimeDesirability.AddTriangularSet("Medium_Visibility", 300, 1000, 2000);
	FzSet& LongVisibility = ViewTimeDesirability.AddRightShoulderSet("Long_Visibility", 1500, 1700, 10000);

	m_FuzzyModule.AddRule(FzAND(Target_Close, NoVisibility), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, NoVisibility), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, NoVisibility), Undesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Close, ShortVisibility), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, MediumVisibility), Undesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Close, LongVisibility), Undesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Medium, ShortVisibility), VeryDesirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, MediumVisibility), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, LongVisibility), Undesirable);

	m_FuzzyModule.AddRule(FzAND(Target_Far, ShortVisibility), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, MediumVisibility), Desirable);
	m_FuzzyModule.AddRule(FzAND(Target_Far, LongVisibility), Undesirable);


	// A modifier
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
	m_FuzzyModule.AddRule(FzAND(Target_Close, Velocity_Low, ViewTime_Low), Precision_Low);
	m_FuzzyModule.AddRule(FzAND(Target_Close, Velocity_High, ViewTime_Low), Precision_Low);
	m_FuzzyModule.AddRule(FzAND(Target_Close, Velocity_High, ViewTime_High), Precision_Low);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, Velocity_Low, ViewTime_High), Precision_Low);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, Velocity_Low, ViewTime_Low), Precision_Low);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, Velocity_High, ViewTime_High), Precision_Medium);
	m_FuzzyModule.AddRule(FzAND(Target_Medium, Velocity_High, ViewTime_Low), Precision_High);
	m_FuzzyModule.AddRule(FzAND(Target_Far, Velocity_Low, ViewTime_High), Precision_Medium);
	m_FuzzyModule.AddRule(FzAND(Target_Far, Velocity_Low, ViewTime_Low), Precision_Medium);
	m_FuzzyModule.AddRule(FzAND(Target_Far, Velocity_High, ViewTime_High), Precision_Medium);
	m_FuzzyModule.AddRule(FzAND(Target_Far, Velocity_High, ViewTime_Low), Precision_High);
}


//-------------------------------- Render -------------------------------------
//-----------------------------------------------------------------------------
void Grenade::Render()
{
	m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
		m_pOwner->Pos(),
		m_pOwner->Facing(),
		m_pOwner->Facing().Perp(),
		m_pOwner->Scale());

	gdi->DarkGreenPen();
	gdi->DarkGreenBrush();
	gdi->Circle(m_vecWeaponVBTrans[0], 4);
}