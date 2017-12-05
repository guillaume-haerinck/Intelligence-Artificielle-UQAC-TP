#pragma once

#include "Goal_Evaluator.h"

class GetWeaponCacheGoal_Evaluator : public Goal_Evaluator
{
public:
	GetWeaponCacheGoal_Evaluator(double bias) : Goal_Evaluator(bias)
	{}

	double CalculateDesirability(Raven_Bot* pBot);

	void  SetGoal(Raven_Bot* pEnt);

	void  RenderInfo(Vector2D Position, Raven_Bot* pBot);
};

