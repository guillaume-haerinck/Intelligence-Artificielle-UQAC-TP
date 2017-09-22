#include <SFML/Graphics.hpp>

#include "StateMachine.h"
#include "State.h"
#include "EntityManager.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "MinersWife.h"
#include "Miner.h"
#include "FSM/StateMachine.h"


namespace GUI
{
	void LoopSFML()
	{
		sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
		sf::CircleShape shape(100.f);
		shape.setFillColor(sf::Color::Green);

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear();
			window.draw(shape);
			window.display();
		}
	}

	/*void ShowWifeInfo(MinersWife* wife)
	{
		wife->ID();
		wife->Location();
		wife->GetFSM->CurrentState();
		wife->GetFSM->PreviousState();
		wife->GetFSM->GlobalState();
	}

	void ShowMinerInfo(Miner* miner)
	{
		miner->ID();
		miner->Location();
		miner->GetFSM->CurrentState();
		miner->GetFSM->PreviousState();
		miner->GetFSM->GlobalState();
	}*/

};
