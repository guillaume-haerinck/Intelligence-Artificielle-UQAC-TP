#ifndef SWAIN_H
#define SWAIN_H
//------------------------------------------------------------------------
//
//  Name:   BarFly.h
//
//  Desc:   A class defining a goldminer.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "SwainOwnedStated.h"
#include "fsm/StateMachine.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

//the amount of gold a Swain must have before he feels he can go home
const int MaxHornyLevel = 3;
//above this value a Swain is sleepy
const int TirednessLevel = 5;



class Swain : public BaseGameEntity
{
private:

	//an instance of the state machine class
	StateMachine<Swain>*  m_pStateMachine;

	location_type         m_Location;

	//how many nuggets the miner has in his pockets
	int                   m_Horny;

	//the higher the value, the more tired the miner
	int                   m_iFatigue;

	bool			      m_isDead;

public:

	Swain(int id):m_Location(swainHome),
		m_Horny(0),
		m_iFatigue(0),
		m_isDead(false),
		BaseGameEntity(id)

	{
		//set up state machine
		m_pStateMachine = new StateMachine<Swain>(this);

		m_pStateMachine->SetCurrentState(AtSwainHome::Instance());

		/* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE MINER */
	}

	~Swain() { delete m_pStateMachine; }

	//this must be implemented
	void Update();

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);

	void HandleThread(sf::Mutex protector);

	StateMachine<Swain>* GetFSM()const { return m_pStateMachine; }



	//-------------------------------------------------------------accessors
	location_type Location()const { return m_Location; }
	void          ChangeLocation(location_type loc) { m_Location = loc; }

	int           GetHorny()const { return m_Horny; }
	void          SetHorny(int val) { m_Horny = val; }
	void          HornyUp(int val);
	bool          HornyFull()const{ return m_Horny >= m_Horny; }

	bool          Fatigued()const;
	void          DecreaseFatigue() { m_iFatigue = 0; }
	void          IncreaseFatigue() { m_iFatigue += 1; }


	bool          GetDead()const { return m_isDead; }
	void          isDead() { m_isDead = true; }

};



#endif
