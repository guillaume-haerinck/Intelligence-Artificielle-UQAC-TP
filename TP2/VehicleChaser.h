#ifndef VEHICLECHASER_H
#define VEHICLECHASER_H
#pragma once

//------------------------------------------------------------------------
//
//  Name:   VehicleChaser.h
//
//  Desc:   Definition of a simple vehicle that uses steering behavior
//
//------------------------------------------------------------------------

#include "2d/Vector2D.h"
#include "misc/Smoother.h"
#include "Vehicle.h"

#include <vector>
#include <list>
#include <string>

class GameWorld;
class SteeringBehavior;
class Vehicle;


class VehicleChaser : public Vehicle
{
private:

	//the target to chase
	Vehicle* m_target;

	//the offset
	Vector2D m_offset;

public:

		VehicleChaser(GameWorld* world,
					Vector2D position,
					double    rotation,
					Vector2D velocity,
					double    mass,
					double    max_force,
					double    max_speed,
					double    max_turn_rate,
					double    scale,
					Vehicle* target);

		~VehicleChaser();
};

#endif