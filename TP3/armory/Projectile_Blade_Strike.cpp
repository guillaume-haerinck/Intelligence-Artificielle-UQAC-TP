#include "Projectile_Blade_Strike.h"
#include "../lua/Raven_Scriptor.h"
#include "misc/cgdi.h"
#include "../Raven_Bot.h"
#include "../Raven_Game.h"
#include "game/EntityFunctionTemplates.h"
#include "2d/WallIntersectionTests.h"
#include "../Raven_Map.h"

#include "../Raven_Messages.h"
#include "Messaging/MessageDispatcher.h"

#include <list>


Blade_Strike::Blade_Strike(Raven_Bot* swordsman, Vector2D target) :
	Raven_Projectile(target,
		swordsman->GetWorld(),
		swordsman->ID(),
		swordsman->Pos(),
		swordsman->Facing(),
		script->GetInt("Blade_Damage"),
		script->GetDouble("Blade_Scale"),
		script->GetDouble("Blade_MaxSpeed"),
		script->GetDouble("Blade_Mass"),
		script->GetDouble("Blade_MaxForce")),
		swordsman(swordsman), 
		m_dBlastRadius(script->GetDouble("Blade_Range"))
{
	assert(target != Vector2D());
}

//------------------------------ Update ---------------------------------------


void Blade_Strike::Update()
{
	TestForImpact();
	m_bDead = true;
}

//----------------------------------- TestForImpact ---------------------------
void Blade_Strike::TestForImpact()
{
	InflictDamageOnBotsWithinBlastRadius();
}

//--------------- InflictDamageOnBotsWithinBlastRadius ------------------------
void Blade_Strike::InflictDamageOnBotsWithinBlastRadius()
{
	std::list<Raven_Bot*>::const_iterator curBot = m_pWorld->GetAllBots().begin();

	for (curBot; curBot != m_pWorld->GetAllBots().end(); ++curBot)
	{
		if ((*curBot) != swordsman) {
			if (Vec2DDistance(Pos(), (*curBot)->Pos()) < 1.5 * m_dBlastRadius + (*curBot)->BRadius())
			{
				//send a message to the bot to let it know it's been hit, and who the
				//shot came from
				Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
					m_iShooterID,
					(*curBot)->ID(),
					Msg_TakeThatMF,
					(void*)&m_iDamageInflicted);

			}
		}
	}
}

//-------------------------- Render -------------------------------------------
//-----------------------------------------------------------------------------
void Blade_Strike::Render()
{
	gdi->ThickRedPen();
	gdi->Cross(m_vTarget, m_dBlastRadius * Scale().x);
}