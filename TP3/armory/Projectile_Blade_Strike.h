#pragma once

#include "Raven_Projectile.h"

class Blade_Strike : public Raven_Projectile {
private:
	double m_dBlastRadius;
	Raven_Bot *swordsman;

	void	TestForImpact();
	void	InflictDamageOnBotsWithinBlastRadius();

public:
	Blade_Strike(Raven_Bot* swordsman, Vector2D target);

	void Render();

	void Update();
};

