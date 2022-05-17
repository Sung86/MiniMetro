#pragma once

typedef struct edge 
{
	int to_vertex;
	float weight;
} Edge;

typedef struct edgeNode 
{
	Edge edge;
	struct edgeNode *next;
} *EdgeNodePtr;

typedef struct edgeList 
{
	EdgeNodePtr head;
} EdgeList;

typedef struct graph 
{
	int V; 
	int *vertex_types;
	EdgeList *edges;
} Graph;

float* dijkstra(Graph *self, int start);

Graph prims_mst(Graph *self, int start);

Graph my_solution(Graph *self);

void free_graph(Graph *self);