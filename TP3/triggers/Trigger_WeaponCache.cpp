#include "Trigger_WeaponCache.h"
#include "../Raven_WeaponSystem.h"

Trigger_WeaponCache::Trigger_WeaponCache(Vector2D pos, int teamCache) : Trigger_Respawning<Raven_Bot>(GetNextValidID()), teamCache(teamCache) {
	SetEntityType(type_team_cache + teamCache);
	SetInactive();
	m_vPosition = pos;

	weapons.clear();

	weapons[type_shotgun] = 0;
	weapons[type_rail_gun] = 0;
	weapons[type_rocket_launcher] = 0;

	AddCircularTriggerRegion(Pos(), 5.0 * Scale().x);
}

Trigger_WeaponCache::~Trigger_WeaponCache() {

}

void Trigger_WeaponCache::Try(Raven_Bot *pBot) {
	if (isActive() && pBot->EntityType() == this->teamCache && this->isTouchingTrigger(pBot->Pos(), pBot->BRadius()))
	{
		if (weapons[type_shotgun] != 0 && pBot->GetWeaponSys()->GetWeaponFromInventory(type_shotgun) == nullptr) {
			pBot->GetWeaponSys()->AddWeapon(type_shotgun);
			weapons[type_shotgun]--;
		}

		if (weapons[type_rail_gun] != 0 && pBot->GetWeaponSys()->GetWeaponFromInventory(type_rail_gun) == nullptr) {
			pBot->GetWeaponSys()->AddWeapon(type_rail_gun);
			weapons[type_rail_gun]--;
		}

		if (weapons[type_rocket_launcher] != 0 && pBot->GetWeaponSys()->GetWeaponFromInventory(type_rocket_launcher) == nullptr) {
			pBot->GetWeaponSys()->AddWeapon(type_rocket_launcher);
			weapons[type_rocket_launcher]--;
		}
	}

	(weapons[type_shotgun] != 0 || weapons[type_rail_gun] != 0 || weapons[type_rocket_launcher] != 0) ? SetActive() : SetInactive();
}

void Trigger_WeaponCache::AddWeapon(int weapon) {
	weapons[weapon]++;

	SetActive();
}

void Trigger_WeaponCache::Clear() {
	weapons[type_shotgun] = 0;
	weapons[type_rail_gun] = 0;
	weapons[type_rocket_launcher] = 0;

	SetInactive();
}

void Trigger_WeaponCache::Update() {

}

void Trigger_WeaponCache::Render()
{
	if (isActive()) {
		gdi->BlackPen();

		switch (teamCache) {
		case type_bot_blue_team:
			gdi->BlueBrush();
			break;

		case type_bot_red_team:
			gdi->RedBrush();
			break;

		case type_bot_green_team:
			gdi->GreenBrush();
			break;

		case type_bot_yellow_team:
			gdi->YellowBrush();
			break;

		default:
			gdi->WhiteBrush();
		}

		gdi->Circle(m_vPosition, 5.0 *Scale().x);
	}
}