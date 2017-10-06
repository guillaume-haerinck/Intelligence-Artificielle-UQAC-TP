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

//---------------------------------------- methods for AtSwainHome
AtSwainHome* AtSwainHome::Instance()
{
	static AtSwainHome instance;

	return &instance;
}


void AtSwainHome::Enter(Swain* pSwain)
{
	if (pSwain->Location() != swainHome)
	{
		pSwain->ChangeLocation(swainHome);
	}
}


void AtSwainHome::Execute(Swain* pSwain)
{
	pSwain->HornyUp(1);

	pSwain->IncreaseFatigue();

	cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": " << "Gettin' some rest";

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
		<< "Taahm t' rin' a bell";
}


bool AtSwainHome::OnMessage(Swain* pSwain, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

//--------------------------- methods for CheatingAtMinerHome
CheatingAtMinerHome* CheatingAtMinerHome::Instance()
{
	static CheatingAtMinerHome instance;

	return &instance;
}

void CheatingAtMinerHome::Enter(Swain* pSwain)
{
	if (pSwain->Location() != shack)
	{
		cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": " << "Ya welcome me an ah will be welcomin' t' yae";

		pSwain->ChangeLocation(shack);
	}
}


void CheatingAtMinerHome::Execute(Swain* pSwain)
{
	pSwain->IncreaseFatigue();

	pSwain->SetHorny(0);

	pSwain->GetFSM()->ChangeState(AtSwainHome::Instance());
	cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": " << "Ah need t' flee, so her button won't sees me";
}


void CheatingAtMinerHome::Exit(Swain* pSwain)
{

}


bool CheatingAtMinerHome::OnMessage(Swain* pSwain, const Telegram& msg)
{
   SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_IsThereSomeoneInTheCloset:

		cout << "\nMessage handled by " << GetNameOfEntity(pSwain->ID())
			<< " at time: " << Clock->GetCurrentTime();

	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		cout << "\n" << GetNameOfEntity(pSwain->ID())
			<< ": They said to make love not war !";

		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pSwain->ID(),				//ID of sender
			ent_Miner_Bob,				//ID of recipient
			Msg_YesThereIsSwain,				//the message
			NO_ADDITIONAL_INFO);
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		pSwain->GetFSM()->ChangeState(Dead::Instance());

		return true;
	}



	return false;
}

//-------------------------------------- methods for Dead
Dead* Dead::Instance()
{
	static Dead instance;

	return &instance;
}

void Dead::Enter(Swain* pSwain)
{
	if (pSwain->Location() != shack)
	{
		pSwain->ChangeLocation(shack);
	}
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pSwain->ID())
		<< ": Look like am alive no-mwr";
}

void Dead::Execute(Swain* pSwain)
{
	//Loop as dead
}

void Dead::Exit(Swain* pSwain)
{
}


bool Dead::OnMessage(Swain* pSwain, const Telegram& msg)
{
	return false;
}

//-------------------------------------methods for AtSwainHomeSleeping

AtSwainHomeSleeping* AtSwainHomeSleeping::Instance()
{
	static AtSwainHomeSleeping instance;

	return &instance;
}

void AtSwainHomeSleeping::Enter(Swain* pSwain)
{
	if (pSwain->Location() != swainHome)
	{
		cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": " << "Gettin' balmy";

		pSwain->ChangeLocation(swainHome);
	}
}

void AtSwainHomeSleeping::Execute(Swain* pSwain)
{
	pSwain->DecreaseFatigue();
	pSwain->GetFSM()->ChangeState(AtSwainHome::Instance());
}

void AtSwainHomeSleeping::Exit(Swain* pSwain)
{
}


bool AtSwainHomeSleeping::OnMessage(Swain* pSwain, const Telegram& msg)
{
	return false; //send message to global message handler
}