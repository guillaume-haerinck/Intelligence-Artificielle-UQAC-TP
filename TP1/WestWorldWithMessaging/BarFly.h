#ifndef BARFLY_H
#define BARFLY_H

#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "BarFlyOwnedStates.h"
#include "FSM/StateMachine.h"
#include "misc/Utils.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

const int ThirstLevelBF = 3;
const int MaxCoinFounded = 9;

class BarFly : public BaseGameEntity
{
private:
	StateMachine<BarFly>*	m_pStateMachine;

	location_type			m_location;

	int						m_iCoinInThePocket;

	int						m_iThirst;

	bool					m_bBeaten;

public:
	BarFly(int id) :m_location(shack),
		m_iCoinInThePocket(8),
		m_iThirst(3),
		m_bBeaten(false),
		BaseGameEntity(id)
	{
		//set up state machine
		m_pStateMachine = new StateMachine<BarFly>(this);

		m_pStateMachine->SetCurrentState(RestAtBarFlyHome::Instance());
//		m_pStateMachine->SetCurrentState(RestAtBarFlyHome::Instance());
	}

	~BarFly() { delete m_pStateMachine; }

	void Update();

	virtual bool HandleMessage(const Telegram& msg);

	void HandleThread();

	StateMachine<BarFly>* GetFSM()const { return m_pStateMachine; }

	//############ Getter / Setters ###################
	location_type Location()const { return m_location; }
	void	ChangeLocation(location_type loc) { m_location = loc; }

	int		TotalCoinFound()const { return m_iCoinInThePocket; }
	void	SetCoinFound(int val) { m_iCoinInThePocket = val; }
	void	AddToCoin(int val);

	bool	NeedingAlcohol()const;
	void	BuyAndDrinkAbsinth() { m_iThirst -= 1; m_iCoinInThePocket -= 1; }
	bool	IsPoor()const; //Poor if less than 3 coin to buy absinth
	bool	HaveEnoughtMoney()const;
	bool	Beaten()const;
	bool	CoinUnderSofa()const;
};



#endif