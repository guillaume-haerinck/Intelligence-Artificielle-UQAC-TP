#ifndef SWAIN_OWNED_STATES_H
#define SWAIN_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   BarFlyOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Miner class.
//          Note that a global state has not been implemented.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "fsm/State.h"


class Swain;
struct Telegram;




//------------------------------------------------------------------------
//
//  In this state the miner will walk to a goldmine and pick up a nugget
//  of gold. If the miner already has a nugget of gold he'll change state
//  to VisitBankAndDepositGold. If he gets thirsty he'll change state
//  to QuenchThirst
//------------------------------------------------------------------------
class AtSwainHome : public State<Swain>
{
private:

	AtSwainHome() {}

	//copy ctor and assignment should be private
	AtSwainHome(const AtSwainHome&);
	AtSwainHome& operator=(const AtSwainHome&);

public:

	//this is a singleton
	static AtSwainHome* Instance();

	virtual void Enter(Swain* pSwain);

	virtual void Execute(Swain* pSwain);

	virtual void Exit(Swain* pSwain);

	virtual bool OnMessage(Swain* pSwain, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  In this state the miner will walk to a goldmine and pick up a nugget
//  of gold. If the miner already has a nugget of gold he'll change state
//  to VisitBankAndDepositGold. If he gets thirsty he'll change state
//  to QuenchThirst
//------------------------------------------------------------------------
class AtSwainHomeSleeping : public State<Swain>
{
private:

	AtSwainHomeSleeping() {}

	//copy ctor and assignment should be private
	AtSwainHomeSleeping(const AtSwainHomeSleeping&);
	AtSwainHomeSleeping& operator=(const AtSwainHomeSleeping&);

public:

	//this is a singleton
	static AtSwainHomeSleeping* Instance();

	virtual void Enter(Swain* pSwain);

	virtual void Execute(Swain* pSwain);

	virtual void Exit(Swain* pSwain);

	virtual bool OnMessage(Swain* pSwain, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  Entity will go to a bank and deposit any nuggets he is carrying. If the 
//  miner is subsequently wealthy enough he'll walk home, otherwise he'll
//  keep going to get more gold
//------------------------------------------------------------------------
class CheatingAtMinerHome : public State<Swain>
{
private:

	CheatingAtMinerHome() {}

	//copy ctor and assignment should be private
	CheatingAtMinerHome(const CheatingAtMinerHome&);
	CheatingAtMinerHome& operator=(const CheatingAtMinerHome&);

public:

	//this is a singleton
	static CheatingAtMinerHome* Instance();

	virtual void Enter(Swain* pSwain);

	virtual void Execute(Swain* pSwain);

	virtual void Exit(Swain* pSwain);

	virtual bool OnMessage(Swain* pSwain, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//  miner will go home and sleep until his fatigue is decreased
//  sufficiently
//------------------------------------------------------------------------
class Dead : public State<Swain>
{
private:

	Dead() {}

	//copy ctor and assignment should be private
	Dead(const Dead&);
	Dead& operator=(const Dead&);

public:

	//this is a singleton
	static Dead* Instance();

	virtual void Enter(Swain* pSwain);

	virtual void Execute(Swain* pSwain);

	virtual void Exit(Swain* pSwain);

	virtual bool OnMessage(Swain* pSwain , const Telegram& msg);
};




#endif