#include "VehicleLeader.h"
#include "2d/C2DMatrix.h"
#include "2d/Geometry.h"
#include "SteeringBehaviors.h"
#include "2d/Transformations.h"
#include "GameWorld.h"
#include "misc/CellSpacePartition.h"
#include "misc/cgdi.h"

using std::vector;
using std::list;

VehicleLeader::VehicleLeader(GameWorld* world,
							Vector2D position,
							double    rotation,
							Vector2D velocity,
							double    mass,
							double    max_force,
							double    max_speed,
							double    max_turn_rate,
							double    scale):		Vehicle(world,
															position,
															rotation,
															velocity,
															mass,
															max_force,
															max_speed,
															max_turn_rate,
															scale)
{
	m_isManuallyControlled = false;
	m_acceleration = 300;
	this->Steering()->WanderOn();
}

void VehicleLeader::MoveLeft()
{
	if (m_isManuallyControlled)
	{
		this->Steering()->WanderOff();
		this->m_vVelocity = Vector2D(-1, 0) * m_acceleration;
	}
}

void VehicleLeader::MoveUp()
{
	if (m_isManuallyControlled)
	{
		this->Steering()->WanderOff();
		this->m_vVelocity = Vector2D(0, -1) * m_acceleration;
	}
}

void VehicleLeader::MoveRight()
{
	if (m_isManuallyControlled)
	{
		this->Steering()->WanderOff();
		this->m_vVelocity = Vector2D(1, 0) * m_acceleration;
		
	}
}

void VehicleLeader::MoveDown()
{
	if (m_isManuallyControlled)
	{
		this->Steering()->WanderOff();
		
		this->m_vVelocity = Vector2D(0, 1) * m_acceleration;
	}
}

VehicleLeader::~VehicleLeader()
{
}
