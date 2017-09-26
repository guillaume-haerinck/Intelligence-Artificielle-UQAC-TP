#include "Miner.h"

bool Miner::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}

//Not working
/*
void Miner::HandleThread(sf::Mutex protector)
{
	for (int i = 0; i < 30; ++i)
	{
		protector.lock();
		Miner::Update();
		Sleep(800);
		protector.unlock();
	}
}
*/


void Miner::Update()
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

	m_iThirst += 1;

	m_pStateMachine->Update();
	Sleep(800);
}



void Miner::AddToGoldCarried(const int val)
{
  m_iGoldCarried += val;

  if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

void Miner::AddToWealth(const int val)
{
  m_iMoneyInBank += val;

  if (m_iMoneyInBank < 0) m_iMoneyInBank = 0;
}

bool Miner::Thirsty()const
{
  if (m_iThirst >= ThirstLevel){return true;}

  return false;
}

bool Miner::Fatigued()const
{
  if (m_iFatigue > TirednessThreshold)
  {
    return true;
  }

  return false;
}
