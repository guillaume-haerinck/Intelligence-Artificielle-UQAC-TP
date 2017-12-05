#ifndef CAGE_WEAPON_H
#define CAGE_WEAPON_H
#pragma warning (disable:4786)



#include "Trigger.h"

#include <iosfwd>
#include <cassert>

template <class entity_type>
class Cage_Weapon : public Trigger<entity_type>
{
public:
	Cage_Weapon();
	~Cage_Weapon();
};

