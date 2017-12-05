#include "Weapon_Blade.h"
#include "../Raven_Bot.h"
#include "misc/Cgdi.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "../lua/Raven_Scriptor.h"
#include "fuzzy/FuzzyOperators.h"


//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
Blade::Blade(Raven_Bot*   owner) :

	Raven_Weapon(type_blade,
		script->GetInt("Blade_DefaultRounds"),
		script->GetInt("Blade_MaxRoundsCarried"),
		script->GetDouble("Blade_AttackFreq"),
		script->GetDouble("Blade_IdealRange"),
		script->GetDouble("Blade_MaxSpeed"),
		owner)
{
	//setup the vertex buffer
	const int NumWeaponVerts = 11;
	const Vector2D weapon[NumWeaponVerts] = {	Vector2D(0, -6),
												Vector2D(5, -6), 
												Vector2D(6, 0), 
												Vector2D(7, -3), 
												Vector2D(18, -3), 
												Vector2D(23, -6), 
												Vector2D(18, -9), 
												Vector2D(7, -9), 
												Vector2D(6, -12), 
												Vector2D(5, -6),
												Vector2D(15, -6)
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
inline void Blade::ShootAt(Vector2D pos)
{
	if (isReadyForNextShot())
	{
		//fire off a rocket!
		m_pOwner->GetWorld()->AddBladeStrike(m_pOwner, pos);

		UpdateTimeWeaponIsNextAvailable();

		//add a trigger to the game so that the other bots can hear this shot
		//(provided they are within range)
		m_pOwner->GetWorld()->GetMap()->AddSoundTrigger(m_pOwner, script->GetDouble("Blade_SoundRange"));
	}
}

//---------------------------- Desirability -----------------------------------
//
//-----------------------------------------------------------------------------
double Blade::GetDesirability(double DistToTarget)
{
	m_FuzzyModule.Fuzzify("DistToTarget", DistToTarget);

	m_dLastDesirabilityScore = m_FuzzyModule.DeFuzzify("Desirability", FuzzyModule::max_av);

	return m_dLastDesirabilityScore;
}

//-------------------------  InitializeFuzzyModule ----------------------------
//
//  set up some fuzzy variables and rules
//-----------------------------------------------------------------------------
void Blade::InitializeFuzzyModule()
{
	FuzzyVariable& DistToTarget = m_FuzzyModule.CreateFLV("DistToTarget");
	FzSet& Target_Melee = DistToTarget.AddLeftShoulderSet("Target_Melee", 0, 40, 50);
	FzSet& Target_Close = DistToTarget.AddTriangularSet("Target_Close", 50, 75, 100);
	FzSet& Target_Far = DistToTarget.AddRightShoulderSet("Target_Far", 100, 300, 1000);

	FuzzyVariable& Desirability = m_FuzzyModule.CreateFLV("Desirability");
	FzSet& VeryDesirable = Desirability.AddRightShoulderSet("VeryDesirable", 80, 90, 100);
	FzSet& Desirable = Desirability.AddTriangularSet("Desirable", 10, 25, 30);
	FzSet& Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 5, 10);

	m_FuzzyModule.AddRule(Target_Melee, VeryDesirable);
	m_FuzzyModule.AddRule(Target_Close, Desirable);
	m_FuzzyModule.AddRule(Target_Far, Undesirable);
}


//-------------------------------- Render -------------------------------------
//-----------------------------------------------------------------------------
void Blade::Render()
{
	m_vecWeaponVBTrans = WorldTransform(m_vecWeaponVB,
		m_pOwner->Pos(),
		m_pOwner->Facing(),
		m_pOwner->Facing().Perp(),
		m_pOwner->Scale());

	gdi->BlackPen();
	gdi->GreyBrush();
	gdi->ClosedShape(m_vecWeaponVBTrans);
}