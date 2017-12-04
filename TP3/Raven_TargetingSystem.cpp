#include "Raven_TargetingSystem.h"
#include "Raven_Bot.h"
#include "Raven_SensoryMemory.h"
#include "Raven_Game.h"
#include "debug/DebugConsole.h"

#include "Messaging/Telegram.h"
#include "Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"



//-------------------------------- ctor ---------------------------------------
//-----------------------------------------------------------------------------
Raven_TargetingSystem::Raven_TargetingSystem(Raven_Bot* owner):m_pOwner(owner),
                                                               m_pCurrentTarget(0)
{}



//----------------------------- Update ----------------------------------------

//-----------------------------------------------------------------------------
void Raven_TargetingSystem::Update()
{
  double ClosestDistSoFar = MaxDouble;
  m_pCurrentTarget       = 0;

  //grab a list of all the opponents the owner can sense
  std::list<Raven_Bot*> SensedBots;
  SensedBots = m_pOwner->GetSensoryMem()->GetListOfRecentlySensedOpponents();
  
  std::list<Raven_Bot*>::const_iterator curBot = SensedBots.begin();
  for (curBot; curBot != SensedBots.end(); ++curBot)
  {
    //make sure the bot is alive and that it is not the owner
    if ((*curBot)->isAlive() && (*curBot != m_pOwner) )
    {
		double dist = Vec2DDistanceSq((*curBot)->Pos(), m_pOwner->Pos());

		// Question F
		// if the cible is the one designated by the leader of the team, target it
		if (m_pOwner->GetWorld()->isTeamMode())
		{
			if (*curBot == m_pOwner->GetTeamTarget())
			{
				debug_con << "Le bot à pris pour cible le TeamTarget" << "";
				m_pCurrentTarget = *curBot;
				break;
			}
		}

		if (dist < ClosestDistSoFar)
		{
			ClosestDistSoFar = dist;
			m_pCurrentTarget = *curBot;

			/*
			// Question F leader send the cible to the team
			if (m_pOwner->GetWorld()->isTeamMode() && m_pOwner->isLeader())
			{
				debug_con << "Leader demande a son équipe d'attaquer sa cible !" << "";
				m_pCurrentTarget = *curBot;

				// Send to each agent on by one
				Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
					m_pOwner->ID(),
					1,
					Msg_TeamTarget,
					m_pCurrentTarget);
			}
			*/
		}
    }
  }
}




bool Raven_TargetingSystem::isTargetWithinFOV()const
{
  return m_pOwner->GetSensoryMem()->isOpponentWithinFOV(m_pCurrentTarget);
}

bool Raven_TargetingSystem::isTargetShootable()const
{
  return m_pOwner->GetSensoryMem()->isOpponentShootable(m_pCurrentTarget);
}

Vector2D Raven_TargetingSystem::GetLastRecordedPosition()const
{
  return m_pOwner->GetSensoryMem()->GetLastRecordedPositionOfOpponent(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenVisible()const
{
  return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenVisible(m_pCurrentTarget);
}

double Raven_TargetingSystem::GetTimeTargetHasBeenOutOfView()const
{
  return m_pOwner->GetSensoryMem()->GetTimeOpponentHasBeenOutOfView(m_pCurrentTarget);
}
