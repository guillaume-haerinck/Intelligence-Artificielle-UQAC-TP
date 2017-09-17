#include "BarFlyOwnedStates.h"
#include "fsm/State.h"
#include "BarFly.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

RestAtBarFlyHome* RestAtBarFlyHome::Instance()
{
	static RestAtBarFlyHome instance;

	return &instance;
}

void RestAtBarFlyHome::Enter(BarFly* pBarFly)
{
	cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": Need more alcohol !";
}

void RestAtBarFlyHome::Execute(BarFly* pBarFly)
{
	//############ seek for money

	if (pBarFly->HaveEnoughtMoney())
	{
		pBarFly->GetFSM()->ChangeState(DrinkingAtBar::Instance());
	}
}

void RestAtBarFlyHome::Exit(BarFly* pBarFly)
{
	cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": Gonna drink Them all";
}

DrinkingAtBar* DrinkingAtBar::Instance()
{
	static DrinkingAtBar instance;

	return &instance;
}

void DrinkingAtBar::Enter(BarFly* pBarFly)
{
	//If the BarFly is not drinking at the bar, 
	//he'll change his location
	if (pBarFly->Location() != saloon)
	{
		cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": Goin' ta drank all maah ballast";
		
		pBarFly->ChangeLocation(saloon);
	}
}

void DrinkingAtBar::Execute(BarFly* pBarFly)
{
	pBarFly->BuyAndDrinkAbsinth;

	//Include a case where he is beaten by bob here
	if(pBarFly->Beaten())
	{
		pBarFly->GetFSM()->ChangeState(Hospital::Instance());
	}
	else if (pBarFly->IsPoor())
	{
		pBarFly->GetFSM()->ChangeState(RestAtBarFlyHome::Instance());
	}
}

void DrinkingAtBar::Exit(BarFly* pBarFly)
{
	cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": Arh !!! will cam' back";
}
