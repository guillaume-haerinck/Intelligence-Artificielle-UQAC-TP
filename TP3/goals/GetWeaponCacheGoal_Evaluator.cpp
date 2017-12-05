#include "GetWeaponCacheGoal_Evaluator.h"
#include "../Raven_ObjectEnumerations.h"
#include "misc/Stream_Utility_Functions.h"
#include "../Raven_Game.h"
#include "../Raven_Map.h"
#include "Goal_Think.h"
#include "Raven_Goal_Types.h"
#include "Raven_Feature.h"

#include "../Raven_WeaponSystem.h"

#include <string>



//------------------- CalculateDesirability ---------------------------------
//-----------------------------------------------------------------------------
double GetWeaponCacheGoal_Evaluator::CalculateDesirability(Raven_Bot* pBot)
{
	// Si on n'est pas en teamMode, pas de caches d'armes
	if (!pBot->GetWorld()->isTeamMode()) {
		return 0;
	}
	// On récupère la distance avec la cache la plus proche
	double Distance = Raven_Feature::DistanceToItem(pBot, type_team_cache);

	//if the distance feature is rated with a value of 1 it means that the
	//item is either not present on the map or too far away to be worth 
	//considering, therefore the desirability is zero
	if (Distance == 1)
	{
		return 0;
	}
	else
	{
		//value used to tweak the desirability
		const double Tweaker = 0.3;

		double Health;
		std::vector<double> WeaponsStrength;

		Health = Raven_Feature::Health(pBot);

		int missingWeapons = 0;
		for (int weapon = type_rail_gun; weapon < type_blaster; ++weapon) {
			if (pBot->GetWeaponSys()->GetWeaponFromInventory(weapon) == nullptr && pBot->GetWorld()->GetMap()->GetWeaponCache(pBot->EntityType())->GetNumberWeapons(weapon) != 0) {
				missingWeapons++;
			}
		}

		double Desirability = (Tweaker * Health * missingWeapons / (type_blaster - type_rail_gun)) / Distance;

		//ensure the value is in the range 0 to 1
		Clamp(Desirability, 0, 1);

		Desirability *= m_dCharacterBias;

		return Desirability;
	}
}

//------------------------------ SetGoal --------------------------------------
void GetWeaponCacheGoal_Evaluator::SetGoal(Raven_Bot* pBot)
{
	pBot->GetBrain()->AddGoal_MoveToPosition(pBot->GetWorld()->GetMap()->GetWeaponCache(pBot->EntityType())->Pos());
}

//-------------------------- RenderInfo ---------------------------------------
//-----------------------------------------------------------------------------
void GetWeaponCacheGoal_Evaluator::RenderInfo(Vector2D Position, Raven_Bot* pBot)
{
	gdi->TextAtPos(Position, "WC: " + ttos(CalculateDesirability(pBot), 2));
}