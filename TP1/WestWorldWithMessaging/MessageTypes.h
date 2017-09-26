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
  Msg_IsThereSomeoneInTheCloset,
  Msg_YesThereIsSwain
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

  case Msg_IBeatYou:

	  return "IBeatYou";

  case Msg_YouBeatMe:

	  return "YouBeatMe";

  case Msg_IsThereSomeoneInTheCloset:

	  return "IsThereSomeoneInTheCloset";

  case Msg_YesThereIsSwain:

	  return "YesThereIsSwain";

  default:

    return "Not recognized!";
  }
}

#endif