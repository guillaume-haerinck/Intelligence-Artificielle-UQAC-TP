#pragma once

#include "Triggers/Trigger_Respawning.h"
#include "../Raven_Bot.h"
#include "../Raven_ObjectEnumerations.h"

#include <map>

class Trigger_WeaponCache : public Trigger_Respawning<Raven_Bot>
{
private:
	std::vector<Vector2D> points;
	Raven_Game *gameWorld;

	int teamCache;
	std::map<int, int> weapons;

	bool activated;

public:
	Trigger_WeaponCache(Vector2D pos, int teamCache);
	~Trigger_WeaponCache();

	void Try(Raven_Bot* pBot);
	void AddWeapon(int weapon);
	void Clear();
	void Render();

	int GetTeamCache() const { return teamCache; }

	bool IsActivated() const { return activated; }
};
