#ifndef GRENADEPROJECTILE_H
#define GRENADEPROJECTILE_H

#include "Raven_Projectile.h"

class Raven_Bot;

class Grenade_Projectile : public Raven_Projectile
{
private:
	bool exploded;

	double explosionTime;

	//the radius of damage, once the rocket has impacted
	double    m_dBlastRadius;

	//this is used to render the splash when the rocket impacts
	double    m_dCurrentBlastRadius;

	//If the rocket has impacted we test all bots to see if they are within the 
	//blast radius and reduce their health accordingly
	void InflictDamageOnBotsWithinBlastRadius();

	//tests the trajectory of the shell for an impact
	void TestForImpact();

public:

	Grenade_Projectile(Raven_Bot* shooter, Vector2D target);

	void Render();

	void Update();

	inline bool Grenade_Projectile::isExploded()
	{
		if (Clock->GetCurrentTime() > explosionTime)
		{
			return true;
		}

		return false;
	}

};


#endif