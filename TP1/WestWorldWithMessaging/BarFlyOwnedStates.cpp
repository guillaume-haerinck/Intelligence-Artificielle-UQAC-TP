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

bool RestAtBarFlyHome::OnMessage(BarFly* pBarFly, const Telegram& msg)
{
	return false;
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
	pBarFly->BuyAndDrinkAbsinth();

	if (pBarFly->IsPoor())
	{
		pBarFly->GetFSM()->ChangeState(RestAtBarFlyHome::Instance());
	}
}

void DrinkingAtBar::Exit(BarFly* pBarFly)
{
	if (pBarFly->IsPoor())
	{
		cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": Arh !!! will cam' back";
	}
	else
	{
		cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": Don't tou'ch my beer i panch ya";
	}
}

bool DrinkingAtBar::OnMessage(BarFly* pBarFly, const Telegram& msg)
{
	return false;
}

AggressiveDrunk* AggressiveDrunk::Instance()
{
	static AggressiveDrunk instance;

	return &instance;
}

void AggressiveDrunk::Enter(BarFly* pBarFly)
{
	cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": Goin' ta panch ya face in da face";

}

void AggressiveDrunk::Execute(BarFly* pBarFly)
{

	//Include a case where he is beaten by bob here
	if (pBarFly->Beaten())
	{
		pBarFly->GetFSM()->ChangeState(Hospital::Instance());
	}

}

void AggressiveDrunk::Exit(BarFly* pBarFly)
{
	if (pBarFly->Beaten())
	{
		cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": Goin' ta drank all maah ballast";
	}
}

bool AggressiveDrunk::OnMessage(BarFly* pBarFly, const Telegram& msg)
{
	return false;
}

Hospital* Hospital::Instance()
{
	static Hospital instance;

	return &instance;
}

void Hospital::Enter(BarFly* pBarFly)
{

}

void Hospital::Execute(BarFly* pBarFly)
{


}

void Hospital::Exit(BarFly* pBarFly)
{

}

bool Hospital::OnMessage(BarFly* pBarFly, const Telegram& msg)
{
	return false;
}
