/* measurementAnalyzer.h -- analyzes the data provided by accelerometer
 *
 * Copyright (C) 2014 Alex Kazaiev
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
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