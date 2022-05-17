#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "graph.h"

#define INFINITY 999999.0f
#define UNDEFINED 0

/**
*To calculate the total distances of shortest path from given graph
*@param self //the whole graph
*@param start //the start vertex
*@return float //the total distances
*/
float* dijkstra(Graph *self, int start)
{
	int q = self->V;  //vertex set: total number of vertices 
	int *visited = malloc(self->V * sizeof *visited); // if a vertex is visited then visited = 1; else 0
	float *dist = malloc(self->V * sizeof *dist); // the shortest distance from start to a vertex

	//initialisation for each distance and visited
	for (int v = 0; v <self->V; v++) 
	{
		dist[v] = INFINITY;
		visited[v] = UNDEFINED;
 	}
	dist[start] = 0;  //distance of start vertex from itself is always 0

	while (q--)   //while the vertex set is not empty
	{
		float min = INFINITY; // the distance of the nearest vertex, initialise to infinity
		int m = start; // the nearest vertex
		
		//search the shortest path for all vertices
		for (int i = 0; i < self->V; i++)
		{
			if (!visited[i] && min > dist[i]) 
			{
				min = dist[i];
				m = i;
			}
		}

		visited[m] = 1; //the nearest vertex m is now visited

		// Update dist value of the adjacent vertices of the nearest vertex m 
		for (EdgeNodePtr current = self->edges[m].head; current != NULL; current = current->next) 
		{
			int v = current->edge.to_vertex; //the vertex that m connects to 

			if (!visited[v] && dist[m] != INFINITY && dist[v] > dist[m] + current->edge.weight)
				dist[v] = dist[m] + current->edge.weight;
		}
	}

	free(visited);
	return dist;
}

/**
*To search the graph's minimum possible total edge while 
*connecting all vertices by using prim's alogrithm 
*@param self //the whole graph
*@param start //the start vertex
*@return Graph //the prim's minimum spanning tree
*/
Graph prims_mst(Graph *self, int start) 
{
	int q = self->V;  //vertex set: total number of vertices 
	float *key = malloc(self->V * sizeof *key); // the cheapest cost of a connection to a vertex
	int *from = malloc(self->V * sizeof *from); //for keeping track vertex that gives cheapest cost
	int *visited = malloc(self->V * sizeof *visited); // if a vertex is visited then visited = 1; else 0
	
	Graph P;
	P.V = self->V; 
	P.edges = malloc(P.V * sizeof *P.edges);
	P.vertex_types = malloc(P.V * sizeof *P.vertex_types);

	//initialisation the graph with no edges
	for (int i = 0; i < P.V; i++) 
	{
		P.edges[i].head = NULL;
		P.vertex_types[i] = self->vertex_types[i];
	}
		
	//initialisation for key, from and visited
	for (int i = 0; i < P.V; i++)
	{
		key[i] = INFINITY;
		from[i] = UNDEFINED;
		visited[i] = UNDEFINED;
	}
	//initialise the start vertex
	key[start] = 0; //no cost for start vertex
	from[start] = -1; //no vertex is connecting to start vertex
	
	while (q--) //while the vertex set is not empty
	{
		float min = INFINITY; // the distance of the nearest vertex, initialise to infinity
		int m = start; // the nearest vertex

		// get the minimum cost 
		for (int i = 0; i < P.V; i++)
		{
			if (!visited[i] && min > key[i])
			{
				min = key[i];
				m = i;
			}
		}
		visited[m] = 1;  //the nearest vertex m is now visited

		//add into undirected Graph 
		int u = from[m];
		if (u != from[start]) 
		{
			//s->d
			EdgeNodePtr s = malloc(sizeof *s);
			s->edge.to_vertex = u;
			s->edge.weight = min;
			s->next = P.edges[m].head;
			P.edges[m].head = s;

			//d->s
			EdgeNodePtr d = malloc(sizeof *d);
			d->edge.to_vertex = m;
			d->edge.weight = min;
			d->next = P.edges[u].head;
			P.edges[u].head = d;
		}

		//update the cost of the adjacent vertices of the nearest vertex m 
		for (EdgeNodePtr current = self->edges[m].head; current != NULL; current = current->next)
		{
			int v = current->edge.to_vertex; //the vertex that m connects to 
			if (!visited[v] && key[v] > current->edge.weight)
			{
				key[v] =current->edge.weight;
				from[v] = m;
			}
		}
	}

	free(key);
	free(from);
	free(visited);

	return P; 
}

/**
*Connect each vertex to the different vertex types 
*and build a minimum spanning tree from that Graph
*@param self //the given graph
*@return Graph //MST
*/
Graph my_solution(Graph *self)
{
	Graph M;
	M.V = self->V;
	M.edges = malloc(M.V * sizeof *M.edges);
	M.vertex_types = malloc(M.V * sizeof *M.vertex_types);
 
	//initialisation the graph with no edges
	for (int i = 0; i < M.V; i++)
	{
		M.edges[i].head = NULL;
		M.vertex_types[i] = self->vertex_types[i];
	}

	//loop through each edge to check whether the to_vertex is different type, 
	//add into Graph M, if it is different
	for (int i = 0; i < M.V; i++) 
	{
		for (EdgeNodePtr current = self->edges[i].head; current != NULL; current = current->next) 
		{
			if (M.vertex_types[i] != self->vertex_types[current->edge.to_vertex]) 
			{
				EdgeNodePtr s = malloc(sizeof *s);
				s->edge.to_vertex = current->edge.to_vertex;
				s->edge.weight = current->edge.weight;
				s->next = M.edges[i].head;
				M.edges[i].head = s;
			}
		}
	}
	
	M = prims_mst(&M, 0);  //build mst 
	
	return M;
}

/**
*Free the memory of all edge nodes that have dynamically allocated
*@param self //the head of the edge
*/
void free_edge_node(EdgeNodePtr self)
{
	if (self->next)
		free_edge_node(self->next);

	free(self);
}

/**
*Free the memory of vertex_types,edge nodes and edges that
*have dynamically allocated of a given Graph
*@param self //the given Graph
*/
void free_graph(Graph *self) 
{
	free(self->vertex_types);

	for (int i = 0; i < self->V; i++)
		free_edge_node(self->edges[i].head);
	
	free(self->edges);
}