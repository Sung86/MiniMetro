#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include "float.h"
#include "utils.h"

const float EDGE_COST = 1.0f;
const float TIME_COST = 200.0f;
const int ITERATIONS =  1000;

//Generates an array of stops
Stop* get_stops(int num_stops)
{
	srand(time(0));  // may replace with a fixed seed for testing

	Stop* stops;
	stops = malloc(num_stops * sizeof *stops);

	int range = num_stops / 2;

	for (int i = 0; i < num_stops; i++) 
	{
		stops[i].type = 0;
		while ((float)rand()/(float)RAND_MAX > 0.3 && stops[i].type < 6)
			stops[i].type++;
		
		stops[i].x = rand()%range;
		stops[i].y = rand()%range;
	}
	
	return stops;
}

//Calculates the cost of a given graph
float get_score(Graph *self) 
{
	srand(time(0)); // may replace with a fixed seed for testing
	float score = 0;

	// first calculate score for laying the track
	for (int v = 0; v < self->V; v++) 
	{
		EdgeNodePtr current = self->edges[v].head;
		while (current != NULL) 
		{
			score += EDGE_COST * current->edge.weight;
			current = current->next;
		}
	}

	// now simulate ITERATIONS journeys to get an average cost per trip
	for (int i = 0; i < ITERATIONS; i++) 
	{
		int start = rand()%self->V;
		int end = rand() % self->V;		
		int target_type = self->vertex_types[end];
		
		// it would be better to use an all-pairs shortest path algorithm
		// but this will give your dijkstra a good workout ;-)
		float* distances = dijkstra(self, start);
		float best = FLT_MAX;
		for (int v = 0; v < self->V; v++) 
		{
			if (self->vertex_types[v] == target_type && distances[v] < best) 
			{
				best = distances[v];
			}
		}
		free(distances);
		score += TIME_COST * best/ITERATIONS;
	}
	return score;
}

//Calculates the distance between two stops by using coordinates
float distance(float x1, float y1, float x2, float y2) 
{
	return sqrtf((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}