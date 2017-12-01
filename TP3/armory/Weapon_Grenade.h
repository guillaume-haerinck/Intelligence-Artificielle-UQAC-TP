#ifndef GRENADE_H
#define GRENADE_H

#include "Raven_Weapon.h"

class Raven_Bot;

class Grenade : public Raven_Weapon {
private:

	void     InitializeFuzzyModule();

public:

	Grenade(Raven_Bot* owner);


	void Render();

	void ShootAt(Vector2D pos);

	double GetDesirability(double DistToTarget);

	double getPrecision(double DistToTarget, Vector2D Velocity, double ViewTime);
};

#endif