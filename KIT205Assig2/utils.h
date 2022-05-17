#pragma once
#include "graph.h"

typedef struct stop 
{
	int type;
	int x;
	int y;
} Stop;

Stop* get_stops(int num_stops);

float get_score(Graph *self);

float distance(float x1, float y1, float x2, float y2);
