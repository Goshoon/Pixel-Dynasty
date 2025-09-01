#include "gmath.h"

int Clamp(int variable, int minimum, int maximum)
{
  if (variable < minimum)
    return minimum;

  if (variable > maximum)
    return maximum;

  return variable;
}