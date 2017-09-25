#include "BarFly.h"


bool BarFly::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void BarFly::HandleThread()
{
	for (int i = 0; i < 30; ++i)
	{
		mutex.lock();
		BarFly::Update();
		Sleep(800);
		mutex.unlock();
	}
}


void BarFly::Update()
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_GREEN);

	m_pStateMachine->Update();
}

void BarFly::AddToCoin(int val)
{
	m_iCoinInThePocket += val;
	if (m_iCoinInThePocket < 0) { m_iCoinInThePocket = 0; }
}

bool BarFly::NeedingAlcohol()const
{
	if (m_iThirst > ThirstLevelBF) { return true; }
	return false;
}

bool BarFly::HaveEnoughtMoney()const
{
	if (m_iCoinInThePocket >= MaxCoinFounded) { return true; }
	return false;
}

bool BarFly::Beaten()const
{
	//######## add a reception of the result of the fight here ########
	return false;
}

bool BarFly::CoinUnderSofa()const
{
	//########## To add random values later maybe #############
	return false;
}

bool BarFly::IsPoor()const
{
	if (m_iCoinInThePocket < 3) { return true; }
	return false;
}