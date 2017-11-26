#ifndef RAVEN_HUMAN_BOT_H
#define RAVEN_HUMAN_BOT_H

#include "Raven_Bot.h"

#pragma once
class Raven_Human_Bot : public Raven_Bot
{
public:
	Raven_Human_Bot(Raven_Game* world, Vector2D pos);

	~Raven_Human_Bot();
};

#endif