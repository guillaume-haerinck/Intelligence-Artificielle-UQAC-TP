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

	if (pSwain->GetDead() == true)
	{
		cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": "
			<< "Oops ahm lively no mo-wr";

		pSwain->GetFSM()->ChangeState(Dead::Instance());
	}
	else
	{
		pSwain->GetFSM()->ChangeState(AtSwainHome::Instance());
	}
}


void CheatingAtMinerHome::Exit(Swain* pSwain)
{
	cout << "\n" << GetNameOfEntity(pSwain->ID()) << ": " << "Ah need t' flee, so her button won't sees me";
}


bool CheatingAtMinerHome::OnMessage(Swain* pSwain, const Telegram& msg)
{
	//send msg to global message handler
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
}

void Dead::Execute(Swain* pSwain)
{
	//if miner is not fatigued start to dig for nuggets again.

		pSwain->GetFSM()->ChangeState(Dead::Instance());
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