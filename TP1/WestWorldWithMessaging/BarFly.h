#ifndef MINER_H
#define MINER_H

#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "MinerOwnedStates.h"
#include "FSM/StateMachine.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

const int ThirstLevel		= 3;
const int MaxCoinFounded	= 9;

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
		m_iCoinInThePocket(0),
		m_iThirst(0),
		m_bBeaten(false),
		BaseGameEntity(id)
	{
		m_pStateMachine = new StateMachine<BarFly>(this);

		m_pStateMachine -> SetCurrentState(RestAtBarFlyHome::Instance());
	}
	
	~BarFly() { delete m_pStateMachine; }

	void Update();

	virtual bool HandleMessage(const Telegram& msg);

	StateMachine<BarFly>* GetFSM()const {return m_pStateMachine;}
	
	//############ Getter / Setters ###################
	location_type Location()const {return m_location;}
	void	ChangeLocation(location_type loc) {m_location = loc;}

	int		CoinFound()const { return m_iCoinInThePocket; }
	void	SetCoinFound(int val) { m_iCoinInThePocket; }
	void	AddToCoin(int val);

	bool	NeedingAlcohol()const;
	void	BuyAndDrinkAbsinth() { m_iThirst -= 1; m_iCoinInThePocket -= 3; }
	bool	IsPoor()const; //Poor if less than 3 coin to buy absinth
	bool	HaveEnoughtMoney()const;
	bool	HaveMaxMoney()const;
	bool	Beaten()const;
	bool	CoinUnderSofa()const;
};



#endif