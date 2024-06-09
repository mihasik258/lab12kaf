#include"graph.h"

Graph *createGraph() { 
    Graph *graph = (Graph *)malloc(sizeof(Graph)); 
    if (graph == NULL) { 
		return MEM_ERR; 
    } 
    graph->numVertices = 0; 
    return graph; 
} 
void dfsPositiveEdges(Graph *graph, int start, int *visited) {
    visited[start] = 1;
    printf("%s ", graph->humans[start].name);
    for (int i = 0; i < graph->humans[start].numEdges; i++) {
        int neighbor = graph->humans[start].edges[i];
        int weight = graph->humans[start].lvl[i];
        if (weight > 0 && !visited[neighbor]) {
            dfsPositiveEdges(graph, neighbor, visited);
        }
    }
}

int addVertex(Graph *graph, char *name) { 
    if (graph->numVertices >= MAX_VERTICES) { 
    	return ERR_VERTEX;
    } 
    strcpy(graph->humans[graph->numVertices].name, name); 
    graph->humans[graph->numVertices].numEdges = 0; 
    graph->numVertices++; 
    return 0;
} 
 
int addEdge(Graph *graph, int from, int to, int weight) { 
    if (from < 0 || from >= graph->numVertices || to < 0 || to >= graph->numVertices) { 
        return ERR_VERTEX;
    } 
    graph->humans[from].edges[graph->humans[from].numEdges] = to; 
    graph->humans[from].lvl[graph->humans[from].numEdges] = weight; 
    graph->humans[from].numEdges++; 
    return 0;
} 
 

int findVertex(Graph *graph, char *name) { 
    for (int i = 0; i < graph->numVertices; i++) { 
        if (strcmp(graph->humans[i].name, name) == 0) { 
            return i; 
        } 
    } 
    return -1; 
} 

void printGraph(Graph *graph) { 
    printf("Граф:\n"); 
    for (int i = 0; i < graph->numVertices; i++) { 
        printf("Вершина %s:\n", graph->humans[i].name); 
        for (int j = 0; j < graph->humans[i].numEdges; j++) { 
            printf("  -> %s (вес: %d)\n", graph->humans[graph->humans[i].edges[j]].name, graph->humans[i].lvl[j]); 
        } 
    } 
} 
int bellmanFord(Graph *graph, int start, int end) {
    int distances[MAX_VERTICES]; 
    int predecessors[MAX_VERTICES]; 
    for (int i = 0; i < graph->numVertices; i++) {
        distances[i] = INT_MAX; 
        predecessors[i] = -1;
    }
    distances[start] = 0;
    for (int i = 0; i < graph->numVertices - 1; i++) {
        for (int j = 0; j < graph->numVertices; j++) {
            for (int k = 0; k < graph->humans[j].numEdges; k++) {
                int neighbor = graph->humans[j].edges[k];
                int weight = graph->humans[j].lvl[k];
                if (distances[j] != INT_MAX && distances[j] + weight < distances[neighbor]) {
                    distances[neighbor] = distances[j] + weight;
                    predecessors[neighbor] = j;
                }
            }
        }
    }
    for (int j = 0; j < graph->numVertices; j++) {
        for (int k = 0; k < graph->humans[j].numEdges; k++) {
            int neighbor = graph->humans[j].edges[k];
            int weight = graph->humans[j].lvl[k];
            if (distances[j] != INT_MAX && distances[j] + weight < distances[neighbor]) {
                return -1; 
            }
        }
    }
    return distances[end];
}
int checkWay(Graph *graph, int start, int end, int visited[]) {
    if (start == end) {
        return 1; 
    }

    visited[start] = 1;

    for (int i = 0; i < graph->humans[start].numEdges; i++) {
        int neighbor = graph->humans[start].edges[i];
        int weight = graph->humans[start].lvl[i];

        if (weight > 0 && !visited[neighbor]) {
            if (checkWay(graph, neighbor, end, visited)) {
                return 1; 
            }
        }
    }

    visited[start] = 0; 

    return 0; 
}
void removeEdge(Graph *graph, char *fromLabel, char *toLabel) {
    int fromIndex = findVertex(graph, fromLabel);
    int toIndex = findVertex(graph, toLabel);
    if (fromIndex != -1 && toIndex != -1) {
        for (int i = 0; i < graph->humans[fromIndex].numEdges; i++) {
            if (graph->humans[fromIndex].edges[i] == toIndex) {
                for (int j = i; j < graph->humans[fromIndex].numEdges - 1; j++) {
                    graph->humans[fromIndex].edges[j] = graph->humans[fromIndex].edges[j + 1];
                    graph->humans[fromIndex].lvl[j] = graph->humans[fromIndex].lvl[j + 1];
                }
                graph->humans[fromIndex].numEdges--;
                return;
            }
        }
    }
}

void removeVertex(Graph *graph, char *name) {
    int vertexIndex = findVertex(graph, name);
    if (vertexIndex != -1) {
        for (int i = 0; i < graph->numVertices; i++) {
            for (int j = 0; j < graph->humans[i].numEdges; j++) {
                if (graph->humans[i].edges[j] == vertexIndex) {
                    removeEdge(graph, graph->humans[i].name, name);
                    break;
                }
            }
        }
        for (int i = vertexIndex; i < graph->numVertices - 1; i++) {
            graph->humans[i] = graph->humans[i + 1];
        }
        graph->numVertices--;
    }
}
int printGraphvizToFile(Graph *graph, const char *filename) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        fprintf(stderr, "Ошибка открытия файла: %s\n", filename);
        return ERR_FILE;
    }

    fprintf(fp, "digraph G {\n");

    for (int i = 0; i < graph->numVertices; i++) {
        for (int j = 0; j < graph->humans[i].numEdges; j++) {
            int toIndex = graph->humans[i].edges[j];
            int weight = graph->humans[i].lvl[j];
            fprintf(fp, "\"%s\" -> \"%s\" [name=\"%d\"];\n", 
                    graph->humans[i].name, graph->humans[toIndex].name, weight);
        }
    }

    fprintf(fp, "}\n");
    fclose(fp);
    system("dot -Tpng graph.dot -o graph.png");
    return 0;
}
void splitGraph(Graph *graph) {
    int visited[MAX_VERTICES] = {0};
    for (int i = 0; i < graph->numVertices; i++) {
        if (!visited[i]) {
            for (int j = i + 1; j < graph->numVertices; j++) {
                if (!visited[j]) {
                    if (!checkWay(graph, i, j, visited)) {
                        removeEdge(graph, graph->humans[i].name, graph->humans[j].name);
                        removeEdge(graph, graph->humans[j].name, graph->humans[i].name);
                    }
                }
            }
            visited[i] = 1;
        }
    }
}
char* generateStr() {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    int length = rand() % 19 + 1; 
    char* str = (char*)malloc((length + 1) * sizeof(char)); 
	if (str == NULL){
		return NULL;
	}
    for (int i = 0; i < length; i++) {
        str[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    str[length] = '\0'; 

    return str;
}
int generateNumber() {
    return rand() % 21 - 10; 
}
int generateGraphFile(const char *filename, int numVertices) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return ERR_FILE;
    }

    srand(time(NULL));

    for (int i = 0; i < numVertices; i++) {
        fprintf(file, "%s %d\n", generateStr(), generateNumber());
    }

    fclose(file);
    return 0;
}

int readGraphFromFile(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return ERR_FILE;
    }

    char buffer[MAX_STR];
    int isVertexSection = 1; 
    int vertexIndex = 0;
    while (fgets(buffer, MAX_STR, file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; 
        if (isVertexSection) {
            if (strcmp(buffer, "") == 0) {
                isVertexSection = 0;
            } else {
				if (addVertex(graph,buffer)){
        			return ERR_VERTEX; 
        		}
                vertexIndex++;
            }
        } else {
            int from, to, length;
            if (sscanf(buffer, "%d %d %d", &from, &to, &length) == 3) {
				if (addEdge(graph, from, to, length)){
        			return ERR_VERTEX; 
        		}
            }
        }
    }
    fclose(file);
    return 0;
}
int writeGraphToFile(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return ERR_FILE;
    }
    for (int i = 0; i < graph->numVertices; i++) {
        fprintf(file, "%s\n", graph->humans[i].name);
    }
    for (int i = 0; i < graph->numVertices; i++) {
        for (int j = 0; j < graph->humans[i].numEdges; j++) {
            fprintf(file, "%d %d %d\n", i, graph->humans[i].edges[j], graph->humans[i].lvl[j]);
        }
    }
    fclose(file);
    return 0;
}
int generateTextFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return ERR_FILE;
    }
    srand(time(NULL));
    for (int i = 0; i < 500; i++) {
        int length = rand() % 19 + 1;
        for (int j = 0; j < length; j++) {
            fprintf(file, "%c", 'a' + rand() % 26);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
    for (int i = 0; i < 1000; i++) {
        int num1 = rand() % 500;
        int num2 = rand() % 500;
        int num3 = rand() % 21 - 10;
        fprintf(file, "%d %d %d\n", num1, num2, num3);
    }
    fclose(file);
    return 0;
}
