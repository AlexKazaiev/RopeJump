/* measurementAnalyzer.c -- analyzes the data provided by accelerometer
 *
 * Copyright (C) 2014 Alex Kazaiev
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#include "measurementAnalyzer.h"
#define MAX_NODES 5

static node* measurements;
static int prevx = 0;
static int prevy = 0;
static int prevz = 0;

int nodes = 1;

void initMeasurements()
{
  measurements = (node *)malloc(sizeof(node));
  measurements->x = 0;
  measurements->y = 0;
  measurements->z = 0;
  measurements->prev = NULL;
}

int compareParams(int prev, int curr)
{
  if(prev - curr > STEADY_DELTA) {
    return PARAMETER_DECREASE;
  }
  
  if(curr - prev > STEADY_DELTA) {
    return PARAMETER_INCREASE;
  }
  
  return PARAMETER_STEADY;
}

void addMeasurement(int x, int y, int z)
{
   node * newNode = (node *)malloc(sizeof(node));
   newNode-> x = compareParams(prevx, x);
   newNode-> y = compareParams(prevy, y);
   newNode-> z = compareParams(prevz, z);
  
   prevx = x;
   prevy = y;
   prevz = z;
  
   newNode-> prev = measurements;
   measurements = newNode;
   node dbgNode = *newNode;
   app_log(APP_LOG_LEVEL_DEBUG,"", 0, "added measurement: %d, %d, %d", dbgNode.x, dbgNode.y, dbgNode.z);
   nodes ++;
   if(nodes > MAX_NODES)
   {
     node * ptr = measurements;
     for(int k = 0; k < MAX_NODES; k++)
     {
         ptr = ptr->prev;
     }
     node * toDelete = ptr->prev;
     ptr->prev = NULL;
     free(toDelete);
     nodes --;
   }
}

void deinitMeasurements()
{
  while(measurements)
  {
    node *tmp = measurements->prev;
    measurements->prev = NULL;
    free(measurements);
    measurements = tmp;
  }
}