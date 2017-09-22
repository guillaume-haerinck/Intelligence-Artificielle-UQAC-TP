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
	if(pBarFly->Location() != barFlyHome)
	{
		cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": Need more alcohol !";
		pBarFly->ChangeLocation(barFlyHome);
	}
}

void RestAtBarFlyHome::Execute(BarFly* pBarFly)
{
	//############ seek for money
	pBarFly->AddToCoin(1);
	cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": Haa a coin ! gettin' close to my absinth ";
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
	cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": One more Absinth for me !!!";
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
}

bool DrinkingAtBar::OnMessage(BarFly* pBarFly, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	switch (msg.Msg)
	{
		case Msg_HiBarFly:
			cout << "\nMessage handled by " << GetNameOfEntity(pBarFly->ID()) << " at time: "
			<< Clock->GetCurrentTime();
			SetTextColor(FOREGROUND_RED | FOREGROUND_GREEN);
			cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": Bob !!! Told ya not to cam' back !!";
			pBarFly->GetFSM()->ChangeState(AggressiveDrunk::Instance());
	}
	

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
	if (RandFloat() > .5)
	{
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pBarFly->ID(),				//ID of sender
			ent_Miner_Bob,				//ID of recipient
			Msg_IBeatYou,				//the message
			NO_ADDITIONAL_INFO);
	}
	else
	{
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pBarFly->ID(),				 //ID of sender
			ent_Miner_Bob,				 //ID of recipient
			Msg_YouBeatMe,				 //the message
			NO_ADDITIONAL_INFO);
	}

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
		cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": Arh the alcool got me, not you ...";
	}
	else
	{
		cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": And don' cam' back !!!";
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
	if (pBarFly->Location() != hospital)
	{
		cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": I have no money for this, don't ya dare healin' me !";
		pBarFly->ChangeLocation(hospital);
		pBarFly->SetCoinFound(0);
	}
}

void Hospital::Execute(BarFly* pBarFly)
{
	pBarFly->GetFSM()->ChangeState(RestAtBarFlyHome::Instance());

}

void Hospital::Exit(BarFly* pBarFly)
{
	cout << "\n" << GetNameOfEntity(pBarFly->ID()) << ": I have no money for this, don't ya dare healin' me !";
}

bool Hospital::OnMessage(BarFly* pBarFly, const Telegram& msg)
{
	return false;
}
