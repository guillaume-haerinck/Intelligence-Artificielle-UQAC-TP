#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,
  Msg_HiBarFly,
  Msg_IBeatYou,
  Msg_YouBeatMe,
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_HiHoneyImHome:
    
    return "HiHoneyImHome"; 

  case Msg_StewReady:

	  return "StewReady";

  case Msg_HiBarFly:

	  return "HiBarFly";

  default:

    return "Not recognized!";
  }
}

#endif