#include "Swain.h"

bool Swain::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void Swain::HandleThread()
{
	for (int i = 0; i < 30; ++i)
	{
		mutex.lock();
		Swain::Update();
		Sleep(800);
		mutex.unlock();
	}
}

void Swain::Update()
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	m_Horny+= 1;

	m_pStateMachine->Update();
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
