#include "Goal_DodgeEdge.h"
#include "..\Raven_Bot.h"
#include "Raven_Goal_Types.h"
#include "..\Raven_SteeringBehaviors.h"
#include "time/CrudeTimer.h"
#include "..\constants.h"
#include "../navigation/Raven_PathPlanner.h"
#include "misc/cgdi.h"
#include "../lua/Raven_Scriptor.h"


#include "debug/DebugConsole.h"



//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
Goal_DodgeEdge::Goal_DodgeEdge(Raven_Bot* pBot,
                                     PathEdge   edge,
                                     bool       LastEdge,
									bool clockWise):

                                Goal<Raven_Bot>(pBot, goal_dodge_edge),
                                m_Edge(edge),
                                m_dTimeExpected(0.0),
                                m_bLastEdgeInPath(LastEdge),
								m_bEven(clockWise)
                                
{}

                            
                                             
//---------------------------- Activate -------------------------------------
//-----------------------------------------------------------------------------  
void Goal_DodgeEdge::Activate()
{
  m_iStatus = active;
  
  //the edge behavior flag may specify a type of movement that necessitates a 
  //change in the bot's max possible speed as it follows this edge
  switch(m_Edge.Behavior())
  {
    case NavGraphEdge::swim:
    {
      m_pOwner->SetMaxSpeed(script->GetDouble("Bot_MaxSwimmingSpeed"));
    }
   
    break;
   
    case NavGraphEdge::crawl:
    {
       m_pOwner->SetMaxSpeed(script->GetDouble("Bot_MaxCrawlingSpeed"));
    }
   
    break;
  }
  

  //record the time the bot starts this goal
  m_dStartTime = Clock->GetCurrentTime();   
  
  //calculate the expected time required to reach the this waypoint. This value
  //is used to determine if the bot becomes stuck 
  m_dTimeExpected = m_pOwner->CalculateTimeToReachPosition(m_Edge.Destination());
  
  //factor in a margin of error for any reactive behavior
  static const double MarginOfError = 2.0;

  m_dTimeExpected += MarginOfError;

  // Question E: Modify the target to left or right if possible
  if (m_bEven)
  {
	  if (m_pOwner->canStepRight(m_Edge.Destination()))
	  {
		  debug_con << "path modified to right" << "";
		  m_Edge.SetDestination(m_pOwner->getStepRight(m_Edge.Destination()));
		  m_pOwner->GetSteering()->SetTarget(m_Edge.Destination());
		  m_bEven = !m_bEven;
	  }
	  else
	  {
		  m_pOwner->GetSteering()->SetTarget(m_Edge.Destination());
		  m_bEven = !m_bEven;
	  }
  }

  else
  {
	  if (m_pOwner->canStepLeft(m_Edge.Destination()))
	  {
		  debug_con << "path modified to left" << "";
		  m_Edge.SetDestination(m_pOwner->getStepLeft(m_Edge.Destination()));
		  m_pOwner->GetSteering()->SetTarget(m_Edge.Destination());
		  m_bEven = !m_bEven;
	  }
	  else
	  {
		  m_pOwner->GetSteering()->SetTarget(m_Edge.Destination());
		  m_bEven = !m_bEven;
	  }
  }

  //Set the appropriate steering behavior. If this is the last edge in the path
  //the bot should arrive at the position it points to, else it should seek
  if (m_bLastEdgeInPath)
  {
     m_pOwner->GetSteering()->ArriveOn();
  }

  else
  {
    m_pOwner->GetSteering()->SeekOn();
  }
}



//------------------------------ Process --------------------------------------
//-----------------------------------------------------------------------------
int Goal_DodgeEdge::Process()
{
  //if status is inactive, call Activate()
  ActivateIfInactive();
  
  //if the bot has become stuck return failure
  if (isStuck())
  {
    m_iStatus = failed;
  }
  
  //if the bot has reached the end of the edge return completed
  else
  { 
    if (m_pOwner->isAtPosition(m_Edge.Destination()))
    {
      m_iStatus = completed;
    }
  }

  return m_iStatus;
}

//--------------------------- isBotStuck --------------------------------------
//
//  returns true if the bot has taken longer than expected to reach the 
//  currently active waypoint
//-----------------------------------------------------------------------------
bool Goal_DodgeEdge::isStuck()const
{  
  double TimeTaken = Clock->GetCurrentTime() - m_dStartTime;

  if (TimeTaken > m_dTimeExpected)
  {
    debug_con << "BOT " << m_pOwner->ID() << " IS STUCK!!" << "";

    return true;
  }

  return false;
}


//---------------------------- Terminate --------------------------------------
//-----------------------------------------------------------------------------
void Goal_DodgeEdge::Terminate()
{
  //turn off steering behaviors.
  m_pOwner->GetSteering()->SeekOff();
  m_pOwner->GetSteering()->ArriveOff();

  //return max speed back to normal
  m_pOwner->SetMaxSpeed(script->GetDouble("Bot_MaxSpeed"));
}

//----------------------------- Render ----------------------------------------
//-----------------------------------------------------------------------------
void Goal_DodgeEdge::Render()
{
  if (m_iStatus == active)
  {
    gdi->BluePen();
    gdi->Line(m_pOwner->Pos(), m_Edge.Destination());
    gdi->GreenBrush();
    gdi->BlackPen();
    gdi->Circle(m_Edge.Destination(), 3);
  }
}

