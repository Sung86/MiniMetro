/**
* main.c
* Harness file of KIT205 Assignment 2
* @author Ann Sung Phang (481605)
* @version May 2019
*/

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "graph.h"

int main(int argc, char *argv[]) 
{
	Stop* stops = get_stops(20);

	Graph G;
	G.V = 20; 
	G.edges = malloc(G.V * sizeof *G.edges);
	G.vertex_types = malloc(G.V * sizeof *G.vertex_types);

	//Make undirected weighted graph
	//Complete graph - every vertex is connected to every other vertices
	for (int i = 0; i < G.V; i++)
	{
		G.edges[i].head = NULL;
		G.vertex_types[i] = stops[i].type;
	
		//loop through the edges
		for (int j = 0; j < G.V; j++) 
		{	
			//if stop is connected to different stop
			if (i != j) 
			{
				EdgeNodePtr edge_node = malloc(sizeof *edge_node);
				edge_node->edge.to_vertex = j;
				edge_node->edge.weight = distance((float)stops[i].x, (float)stops[i].y, (float)stops[j].x, (float)stops[j].y);
				edge_node->next = G.edges[i].head;
				G.edges[i].head = edge_node;
			}
		}
	}
	
	printf("base score: %f\n", get_score(&G));

	Graph P = prims_mst(&G, 0);
	printf("mst score: %f\n", get_score(&P));

	Graph M = my_solution(&G);
	printf("my score: %f\n", get_score(&M));
	
	//Free stops and graphs//
	free(stops); 
	free_graph(&G);
	free_graph(&P);
	free_graph(&M);

	printf("\npress enter to exit\n");
	getchar();
	return 0;
}