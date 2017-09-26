#include "Swain.h"

bool Swain::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

//Not working
/*
void Swain::HandleThread(sf::Mutex protector)
{
	for (int i = 0; i < 30; ++i)
	{
		protector.lock(); 
		Swain::Update();
		Sleep(800);
		protector.unlock();
	}
}
*/

void Swain::Update()
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	m_Horny += 1;

	m_pStateMachine->Update();
	Sleep(800);
}

void Swain::HornyUp(const int val)
{
	m_Horny += 1;

	if (m_Horny < 0) m_Horny = 0;
}


bool Swain::Fatigued()const
{
	if (m_iFatigue > TirednessLevel)
	{
		return true;
	}

	return false;
}
