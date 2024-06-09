#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <limits.h>
#include <time.h>
 
#define MAX_VERTICES 100 
#define MAX_STRING_LENGTH 100 
 

typedef struct Vertex { 
    char name[MAX_STRING_LENGTH];
    int numEdges; 
    int edges[MAX_VERTICES]; 
    int lvl[MAX_VERTICES]; 
} Vertex; 
 

typedef struct Graph { 
    int numVertices; 
    Vertex humans[MAX_VERTICES]; 
} Graph; 

Graph *createGraph() { 
    Graph *graph = (Graph *)malloc(sizeof(Graph)); 
    if (graph == NULL) { 
        fprintf(stderr, "Ошибка выделения памяти для графа.\n"); 
        exit(1); 
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

void addVertex(Graph *graph, char *name) { 
    if (graph->numVertices >= MAX_VERTICES) { 
        fprintf(stderr, "Превышено максимальное количество вершин.\n"); 
        exit(1); 
    } 
    strcpy(graph->humans[graph->numVertices].name, name); 
    graph->humans[graph->numVertices].numEdges = 0; 
    graph->numVertices++; 
} 
 
void addEdge(Graph *graph, int from, int to, int weight) { 
    if (from < 0 || from >= graph->numVertices || to < 0 || to >= graph->numVertices) { 
        fprintf(stderr, "Неправильные индексы вершин.\n"); 
        exit(1); 
    } 
    graph->humans[from].edges[graph->humans[from].numEdges] = to; 
    graph->humans[from].lvl[graph->humans[from].numEdges] = weight; 
    graph->humans[from].numEdges++; 
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
int hasPositivePath(Graph *graph, int start, int end, int visited[]) {
    if (start == end) {
        return 1; 
    }

    visited[start] = 1;

    for (int i = 0; i < graph->humans[start].numEdges; i++) {
        int neighbor = graph->humans[start].edges[i];
        int weight = graph->humans[start].lvl[i];

        if (weight > 0 && !visited[neighbor]) {
            if (hasPositivePath(graph, neighbor, end, visited)) {
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
                // Сдвигаем элементы массива, чтобы удалить ребро
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
void printGraphvizToFile(Graph *graph, const char *filename) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        fprintf(stderr, "Ошибка открытия файла: %s\n", filename);
        return;
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
}
void splitGraph(Graph *graph) {
    int visited[MAX_VERTICES] = {0};
    for (int i = 0; i < graph->numVertices; i++) {
        if (!visited[i]) {
            for (int j = i + 1; j < graph->numVertices; j++) {
                if (!visited[j]) {
                    if (!hasPositivePath(graph, i, j, visited)) {
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
    char* randomString;
    int length = rand() % 50 + 1; 
    randomString = (char*)malloc((length + 1) * sizeof(char)); 

    for (int i = 0; i < length; i++) {
        randomString[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    randomString[length] = '\0'; 

    return randomString;
}
int generateNumber() {
    return rand() % 21 - 10; 
}
void generateGraphFile(const char *filename, int numVertices) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла.\n");
        return;
    }

    srand(time(NULL));

    for (int i = 0; i < numVertices; i++) {
        fprintf(file, "%s %d\n", generateStr(), generateNumber());
    }

    fclose(file);
}

void readGraphFromFile(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла для чтения.\n");
        return;
    }

    char buffer[MAX_STRING_LENGTH];
    int isVertexSection = 1; // Флаг для определения секции вершин и ребер
    int vertexIndex = 0;

    while (fgets(buffer, MAX_STRING_LENGTH, file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; // Удаление символа новой строки
        if (isVertexSection) {
            if (strcmp(buffer, "") == 0) {
                isVertexSection = 0; // Переход к секции ребер
            } else {
				addVertex(graph, buffer); 
                vertexIndex++;
            }
        } else {
            int from, to, length;
            if (sscanf(buffer, "%d %d %d", &from, &to, &length) == 3) {
				addEdge(graph, from, to, length); 
            }
        }
    }

    fclose(file);
}
void writeGraphToFile(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла для записи.\n");
        return;
    }

    // Запись вершин в файл
    for (int i = 0; i < graph->numVertices; i++) {
        fprintf(file, "%s\n", graph->humans[i].name);
    }

    // Запись ребер в файл
    for (int i = 0; i < graph->numVertices; i++) {
        for (int j = 0; j < graph->humans[i].numEdges; j++) {
            fprintf(file, "%d %d %d\n", i, graph->humans[i].edges[j], graph->humans[i].lvl[j]);
        }
    }

    fclose(file);
}

int main() { 
    Graph *graph = createGraph();
	readGraphFromFile(graph,"tt.txt");
	writeGraphToFile(graph,"ttt.txt");
     /*
    addVertex(graph, "A"); 
    addVertex(graph, "B"); 
    addVertex(graph, "C"); 
    addVertex(graph, "D"); 
    addEdge(graph, 0, 1, 5); 
    addEdge(graph, 1, 2, -2); 
    addEdge(graph, 2, 3, 7); 
    printGraph(graph); 
    int startVertex = findVertex(graph, "A");
    int visited[MAX_VERTICES] = {0};
    if (startVertex != -1) {
        printf("Обход в глубину из вершины %s:\n", graph->humans[startVertex].name);
        dfsPositiveEdges(graph, startVertex, visited);
        printf("\n");
    } else {
        printf("Вершина %s не найдена.\n", "A");
    }
    startVertex = findVertex(graph, "A");
    int endVertex = findVertex(graph, "D");

    if (startVertex != -1 && endVertex != -1) {
        int shortestDistance = bellmanFord(graph, startVertex, endVertex);
        if (shortestDistance != -1) {
            printf("Наименьшее расстояние от %s до %s: %d\n",
                   graph->humans[startVertex].name,
                   graph->humans[endVertex].name,
                   shortestDistance);
        } else {
            printf("В графе присутствует отрицательный цикл.\n");
        }
    } else {
        printf("Одна или обе вершины не найдены.\n");
    }
    splitGraph(graph);*/
    printGraph(graph); 
    return 0; 
}
