#include "VehicleChaser.h"
#include "Vehicle.h"
#include "2d/C2DMatrix.h"
#include "2d/Geometry.h"
#include "SteeringBehaviors.h"
#include "2d/Transformations.h"
#include "GameWorld.h"
#include "misc/CellSpacePartition.h"
#include "misc/cgdi.h"

using std::vector;
using std::list;

VehicleChaser::VehicleChaser(GameWorld* world,
							Vector2D position,
							double    rotation,
							Vector2D velocity,
							double    mass,
							double    max_force,
							double    max_speed,
							double    max_turn_rate,
							double    scale,
							Vehicle* target):		Vehicle(world,
															position,
															rotation,
															velocity,
															mass,
															max_force,
															max_speed,
															max_turn_rate,
															scale),
													m_target(target)
													
{
	m_offset = Vector2D(2, 0);
	this->Steering()->OffsetPursuitOn(this->m_target, m_offset);
}


VehicleChaser::~VehicleChaser()
{
}
