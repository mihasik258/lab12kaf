#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <limits.h>
#include <time.h>
 
#define MAX_VERTICES 500
#define MAX_STR 30
#define FAIL_CHOICE 1
#define ERR_FILE 2
#define ERR_VERTEX 3
#define ERR_IND 4
#define ERR_MEM NULL

typedef struct Vertex { 
    char name[MAX_STR];
    int numEdges; 
    int edges[MAX_VERTICES]; 
    int lvl[MAX_VERTICES]; 
} Vertex; 
 

typedef struct Graph { 
    int numVertices; 
    Vertex humans[MAX_VERTICES]; 
} Graph; 

Graph *createGraph();
void dfsPositiveEdges(Graph *graph, int start, int *visited);

int addVertex(Graph *graph, char *name);
 
int addEdge(Graph *graph, int from, int to, int weight);
 

int findVertex(Graph *graph, char *name);

void printGraph(Graph *graph);
int bellmanFord(Graph *graph, int start, int end);
int checkWay(Graph *graph, int start, int end, int visited[]);
void removeEdge(Graph *graph, char *fromLabel, char *toLabel);

void removeVertex(Graph *graph, char *name);
int printGraphvizToFile(Graph *graph, const char *filename);
void splitGraph(Graph *graph);
char* generateStr();
int generateNumber();
int generateGraphFile(const char *filename, int numVertices); 
int readGraphFromFile(Graph* graph, const char* filename); 
int writeGraphToFile(Graph* graph, const char* filename); 
int generateTextFile(const char *filename); 
