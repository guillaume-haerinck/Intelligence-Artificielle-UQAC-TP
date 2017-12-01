#include "Raven_Weapon.h"



Vector2D Raven_Weapon::getVectorPrecision(double precision) {
	return Vector2D(RandInRange(-5*precision/100, 5*precision/100), RandInRange(-5*precision/100, 5*precision/100)); 
}
