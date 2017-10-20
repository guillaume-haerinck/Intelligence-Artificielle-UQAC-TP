#ifndef VEHICLECHASER_H
#define VEHICLECHASER_H
#pragma once


#include "Vehicle.h"
#include "2d/Vector2D.h"
#include "misc/Smoother.h"

#include <vector>
#include <list>
#include <string>

class GameWorld;
class SteeringBehavior;



class VehicleLeader : public Vehicle
{
private: 

	//define if the vehicle is controlled or not
	bool		m_isManuallyControled;


public:

	VehicleLeader(GameWorld* world,
		Vector2D position,
		double    rotation,
		Vector2D velocity,
		double    mass,
		double    max_force,
		double    max_speed,
		double    max_turn_rate,
		double    scale,
		bool isManuallyControled);

	~VehicleLeader();

	//-------------------------------------------accessor methods
	bool Controlled() { return m_isManuallyControled; }

};

#endif