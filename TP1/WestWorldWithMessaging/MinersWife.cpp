#include "MinersWife.h"

bool MinersWife::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void MinersWife::HandleThread(sf::Mutex protector)
{
	for (int i = 0; i < 30; ++i)
	{
		protector.lock();
		MinersWife::Update();
		Sleep(800);
		protector.unlock();
	}
}


void MinersWife::Update()
{
	//set text color to green
	SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	m_pStateMachine->Update();
	Sleep(800);
}