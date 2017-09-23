#include "MinersWife.h"

bool MinersWife::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void MinersWife::HandleThread()
{
	for (int i = 0; i < 30; ++i)
	{
		protector.lock();
		MinersWife::Update();
		protector.unlock();
		Sleep(800);
	}
}


void MinersWife::Update()
{
  //set text color to green
  SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
 
  m_pStateMachine->Update();
}