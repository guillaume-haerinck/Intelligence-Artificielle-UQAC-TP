#ifndef VEHICLELEADER_H
#define VEHICLELEADER_H
#pragma once

//------------------------------------------------------------------------
//
//  Name:   VehicleLeader.h
//
//  Desc:   Definition of a simple vehicle that uses steering behaviors
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

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
	bool		m_isManuallyControlled;
	int			m_acceleration;


public:

	VehicleLeader(GameWorld* world,
		Vector2D position,
		double    rotation,
		Vector2D velocity,
		double    mass,
		double    max_force,
		double    max_speed,
		double    max_turn_rate,
		double    scale);

	//Manual Control
	void MoveLeft();
	void MoveUp();
	void MoveRight();
	void MoveDown();

	~VehicleLeader();

	//-------------------------------------------accessor methods
	bool isControlled() { return m_isManuallyControlled; }
	void setControlled(bool controlled) { m_isManuallyControlled = controlled; }
};

#endif