#ifndef BARFLY_OWNED_STATES_H
#define BARFLY_OWNED_STATES_H


#include "FSM/State.h"

class BarFly;
struct Telegram;

class DrinkingAtBar : public State<BarFly>
{
private:
	
	DrinkingAtBar(){}

	DrinkingAtBar(const DrinkingAtBar&);
	DrinkingAtBar& operator=(const DrinkingAtBar&);

public:

	//Singleton
	static DrinkingAtBar* Instance();

	virtual void Enter(BarFly* barfly);

	virtual void Execute(BarFly* barfly);

	virtual void Exit(BarFly* barfly);

	virtual bool OnMessage(BarFly* agent, const Telegram& msg);
};

class RestAtBarFlyHome : public State<BarFly>
{
private:

	RestAtBarFlyHome() {}

	RestAtBarFlyHome(const RestAtBarFlyHome&);
	RestAtBarFlyHome& operator=(const RestAtBarFlyHome&);

public:

	//Singleton
	static RestAtBarFlyHome* Instance();

	virtual void Enter(BarFly* barfly);

	virtual void Execute(BarFly* barfly);

	virtual void Exit(BarFly* barfly);

	virtual bool OnMessage(BarFly* agent, const Telegram& msg);
};

class AggressiveDrunk : public State<BarFly>
{
private:

	AggressiveDrunk() {}

	AggressiveDrunk(const AggressiveDrunk&);
	AggressiveDrunk& operator=(const AggressiveDrunk&);

public:

	//Singleton
	static AggressiveDrunk* Instance();

	virtual void Enter(BarFly* barfly);

	virtual void Execute(BarFly* barfly);

	virtual void Exit(BarFly* barfly);

	virtual bool OnMessage(BarFly* agent, const Telegram& msg);
};

class Hospital : public State<BarFly>
{
private:

	Hospital() {}

	Hospital(const Hospital&);
	Hospital& operator=(const Hospital&);

public:

	//Singleton
	static Hospital* Instance();

	virtual void Enter(BarFly* barfly);

	virtual void Execute(BarFly* barfly);

	virtual void Exit(BarFly* barfly);

	virtual bool OnMessage(BarFly* agent, const Telegram& msg);
};





#endif