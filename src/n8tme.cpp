/*
  @file     n8time.cpp
  @author   Nathan Park
*/

#include "n8time.h"
#include <time.h> // clock

float GetTimeInSeconds()
{
  return clock() / (float) CLOCKS_PER_SEC;
}
