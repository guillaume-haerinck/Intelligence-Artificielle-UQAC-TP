#include "Trigger_WeaponCache.h"
#include "../Raven_WeaponSystem.h"

Trigger_WeaponCache::Trigger_WeaponCache(Vector2D pos, int teamCache) : Trigger_Respawning<Raven_Bot>(GetNextValidID()), teamCache(teamCache), activated(false) {
	SetEntityType(type_team_cache);
	m_vPosition = pos;

	const int NumRocketVerts = 8;
	const Vector2D rip[NumRocketVerts] = { Vector2D(0, 3),
		Vector2D(1, 2),
		Vector2D(1, 0),
		Vector2D(2, -2),
		Vector2D(-2, -2),
		Vector2D(-1, 0),
		Vector2D(-1, 2),
		Vector2D(0, 3) };

	for (int i = 0; i<NumRocketVerts; ++i)
	{
		points.push_back(rip[i]);
	}

	weapons.clear();

	weapons[type_shotgun] = 0;
	weapons[type_rail_gun] = 0;
	weapons[type_rocket_launcher] = 0;
}

Trigger_WeaponCache::~Trigger_WeaponCache() {

}

void Trigger_WeaponCache::Try(Raven_Bot *pBot) {
	if (pBot->EntityType() == this->EntityType() && this->isTouchingTrigger(pBot->Pos(), pBot->BRadius()))
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

	activated = weapons[type_shotgun] != 0 || weapons[type_rail_gun] != 0 || weapons[type_rocket_launcher] != 0;
}

void Trigger_WeaponCache::AddWeapon(int weapon) {
	weapons[weapon]++;

	activated = true;
}

void Trigger_WeaponCache::Clear() {
	weapons[type_shotgun] = 0;
	weapons[type_rail_gun] = 0;
	weapons[type_rocket_launcher] = 0;

	activated = false;
}

void Trigger_WeaponCache::Render()
{
	if (activated) {
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