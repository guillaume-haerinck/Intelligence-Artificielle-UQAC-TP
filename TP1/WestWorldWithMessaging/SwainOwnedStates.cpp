#include "SwainOwnedStated.h"
#include "fsm/State.h"
#include "Swain.h"
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


//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
AtSwainHome* AtSwainHome::Instance()
{
	static AtSwainHome instance;

	return &instance;
}


void AtSwainHome::Enter(Swain* pSwain)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	if (pSwain->Location() != swainHome)
	{
		cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": " << "Walkin' to the barFlyHome";

		pSwain->ChangeLocation(swainHome);
	}
}


void AtSwainHome::Execute(Swain* pSwain)
{
	//Now the miner is at the goldmine he digs for gold until he
	//is carrying in excess of MaxNuggets. If he gets thirsty during
	//his digging he packs up work for a while and changes state to
	//gp to the saloon for a whiskey.
	pSwain->HornyUp(1);

	pSwain->IncreaseFatigue();

	cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": " << "Pickin' up a nugget";

	//if enough gold mined, go and put it in the bank
	if (pSwain->HornyFull())
	{
		pSwain->GetFSM()->ChangeState(CheatingAtMinerHome::Instance());
	}

	if (pSwain->Fatigued())
	{
		pSwain->GetFSM()->ChangeState(AtSwainHomeSleeping::Instance());
	}
}


void AtSwainHome::Exit(Swain* pSwain)
{
	cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": "
		<< "Ah'm leavin' the goldm:ine with mah pockets full o' sweet gold";
}


bool AtSwainHome::OnMessage(Swain* pSwain, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

CheatingAtMinerHome* CheatingAtMinerHome::Instance()
{
	static CheatingAtMinerHome instance;

	return &instance;
}

void CheatingAtMinerHome::Enter(Swain* pSwain)
{
	//on entry the miner makes sure he is located at the bank
	if (pSwain->Location() != shack)
	{
		cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": " << "Goin' to the bank. Yes siree";

		pSwain->ChangeLocation(shack);
	}
}


void CheatingAtMinerHome::Execute(Swain* pSwain)
{
	pSwain->IncreaseFatigue();

	pSwain->SetHorny(0);

	cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": "
		<< "Depositing gold. Total savings now: ";

	//pSwain->Wealth() >= ComfortLevel)
	if (pSwain->GetDead() == true)
	{
		cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": "
			<< "WooHoo! Rich enough for now. Back home to mah li'lle lady";

		pSwain->GetFSM()->ChangeState(Dead::Instance());
	}
	//otherwise get more gold
	else
	{
		pSwain->GetFSM()->ChangeState(AtSwainHome::Instance());
	}
}


void CheatingAtMinerHome::Exit(Swain* pSwain)
{
	cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": " << "Leavin' the bank";
}


bool CheatingAtMinerHome::OnMessage(Swain* pSwain, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

Dead* Dead::Instance()
{
	static Dead instance;

	return &instance;
}

void Dead::Enter(Swain* pSwain)
{
	if (pSwain->Location() != shack)
	{
		cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": " << "Walkin' home";

		pSwain->ChangeLocation(shack);
	}
}

void Dead::Execute(Swain* pSwain)
{
	//if miner is not fatigued start to dig for nuggets again.
	
	cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": "
			<< "All mah fatigue has drained away. Time to find more gold!";

		pSwain->GetFSM()->ChangeState(Dead::Instance());
}

void Dead::Exit(Swain* pSwain)
{
}


bool Dead::OnMessage(Swain* pSwain, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_StewReady:

		cout << "\nMessage handled by " << GetNameOfEntity(pSwain->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pSwain->ID())
			<< ": Okay Hun, ahm a comin'!";

		pSwain->GetFSM()->ChangeState(Dead::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
}

//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

AtSwainHomeSleeping* AtSwainHomeSleeping::Instance()
{
	static AtSwainHomeSleeping instance;

	return &instance;
}

void AtSwainHomeSleeping::Enter(Swain* pSwain)
{
	if (pSwain->Location() != swainHome)
	{
		cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": " << "Walkin' home";

		pSwain->ChangeLocation(swainHome);
	}
}

void AtSwainHomeSleeping::Execute(Swain* pSwain)
{
	pSwain->DecreaseFatigue();
	//if miner is not fatigued start to dig for nuggets again.
	cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": "
			<< "All mah fatigue has drained away. Time to find more gold!";

	pSwain->GetFSM()->ChangeState(AtSwainHome::Instance());
}

void AtSwainHomeSleeping::Exit(Swain* pSwain)
{
}


bool AtSwainHomeSleeping::OnMessage(Swain* pSwain, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_StewReady:

		cout << "\nMessage handled by " << GetNameOfEntity(pSwain->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pSwain->ID())
			<< ": Okay Hun, ahm a comin'!";

		pSwain->GetFSM()->ChangeState(Dead::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
}