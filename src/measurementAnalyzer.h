#pragma once
#include <pebble.h>

#define PARAMETER_INCREASE 0
#define PARAMETER_DECREASE 1
#define PARAMETER_STEADY 2
  
#define STEADY_DELTA  50
  
typedef struct Node 
{
  int x;
  int y;
  int z;
  struct Node *prev;
}node;

void initMeasurements();
void addMeasurement(int x, int y, int z);
void deinitMeasurements();