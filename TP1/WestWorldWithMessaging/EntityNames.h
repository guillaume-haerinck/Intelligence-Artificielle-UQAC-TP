#ifndef NAMES_H
#define NAMES_H

#include <string>

enum 
{
  ent_Miner_Bob,

  ent_Elsa,

  ent_Swain,

  ent_BarFly
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
  {
  case ent_Miner_Bob:

    return "Miner Bob";

  case ent_Elsa:

	  return "Elsa";

  case ent_Swain:

	  return "Swain";

  case ent_BarFly:

	  return "Bar Fly";

  default:

    return "UNKNOWN!";
  }
}

#endif