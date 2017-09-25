#include <fstream>
#include <time.h>

#include "Locations.h"
#include "Miner.h"
#include "MinersWife.h"
#include "Swain.h"
#include "BarFly.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"

#include <boost/thread.hpp>

std::ofstream os;

int main()
{

//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  //seed random number generator
  srand((unsigned) time(NULL));

  //create a miner
  Miner* Bob = new Miner(ent_Miner_Bob);

  //create his wife
  MinersWife* Elsa = new MinersWife(ent_Elsa);

  //create a swain
  Swain* JeanErnestain = new Swain(ent_Swain);

  //create a BarFly
  BarFly* Bernard = new BarFly(ent_BarFly);

  //register them with the entity manager
  EntityMgr->RegisterEntity(Bob);
  EntityMgr->RegisterEntity(Elsa);
  EntityMgr->RegisterEntity(JeanErnestain);
  EntityMgr->RegisterEntity(Bernard);


  
  //Create and launch one thread for each entity
  boost::thread BobThread(boost::bind(&Miner::HandleThread, Bob));
  boost::thread ElsaThread(boost::bind(&MinersWife::HandleThread, Elsa));
  boost::thread JeanThread(boost::bind(&Swain::HandleThread, JeanErnestain));
  boost::thread BernardThread(boost::bind(&BarFly::HandleThread, Bernard));
  

  
  //Take care of the messages
  for (int i=0; i<30; ++i)
  { 
    Dispatch->DispatchDelayedMessages();
    Sleep(800);
  }

  //Wait for all of the threads to finish
  BobThread.join();
  ElsaThread.join();
  JeanThread.join();
  BernardThread.join();

  //tidy up
  delete Bob;
  delete Elsa;
  delete JeanErnestain;
  delete Bernard;

  //wait for a keypress before exiting
  PressAnyKeyToContinue();
 
  return 0;
}






