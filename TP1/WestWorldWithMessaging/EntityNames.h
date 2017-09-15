#ifndef NAMES_H
#define NAMES_H

#include <string>

enum 
{
  ent_Miner_Bob,

  ent_Elsa,

  ent_BarFly,

  ent_Swain
};

inline std::string GetNameOfEntity(int n)
{
  switch(n)
  {
  case ent_Miner_Bob:

    return "Miner Bob";

  case ent_Elsa:

	  return "Elsa";

  case ent_BarFly:

	  return "Bar Fly";

  case ent_Swain:

	  return "Swain";

  default:

    return "UNKNOWN!";
  }
}

#endif