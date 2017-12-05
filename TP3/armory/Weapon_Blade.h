#pragma once
#include "Raven_Weapon.h"

class Blade : public Raven_Weapon {
private:

	void     InitializeFuzzyModule();

public:

	Blade(Raven_Bot* owner);


	void Render();

	void ShootAt(Vector2D pos);

	double GetDesirability(double DistToTarget);

	double getPrecision(double DistToTarget, Vector2D Velocity, double ViewTime);
};

