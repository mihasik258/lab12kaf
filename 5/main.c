#include"graph.h"

int main() { 
    Graph *graph = createGraph();
    int choice;
        printf("1. New human\n");
        printf("2. New acquaintance\n");
        printf("3. Delete human\n");
        printf("4. Delete acquaintance\n");
        printf("5. List of friends\n");
        printf("6. Smallest amount\n");
        printf("7. Split\n");
        printf("8. Print graph list\n");
        printf("9. Print graph graphiz\n");
        printf("10. Dop quest\n");
        printf("11. Exit\n");
 	do {
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Error: Failed to read choice\n");
            return FAIL_CHOICE;
        }
        switch(choice) {
        	case 1:{
        		char str[50];
        		scanf("%s",str);
        		if (addVertex(graph,str)!=0){
        			printf("Превышено максимальное количество вершин.\n"); 
        			choice = 11;
        		}		
        		break;}
        	case 2:{
        		int from,to,weight;
        		scanf("%d %d %d",&from,&to,&weight);
        		if (addEdge(graph,from,to,weight)!=0){
        			printf("Ошибка связи вершин.\n"); 
        			choice = 11;
        		}	
        		break;}
        	case 3:{
        		char str[50];
        		scanf("%s",str);
        		removeVertex(graph,str);
        		break;}
            case 4:{
        		char str1[50];
        		char str2[50];
        		scanf("%s %s",str1,str2);
        		removeEdge(graph,str1,str2);
        		break;}
        	case 5:{
        	    char str[50];
        		scanf("%s",str);
        		int startVertex = findVertex(graph, str);
			    int visited[MAX_VERTICES] = {0};
			    if (startVertex != -1) {
			        printf("Обход в глубину из вершины %s:\n", graph->humans[startVertex].name);
			        dfsPositiveEdges(graph, startVertex, visited);
			        printf("\n");
			    } else {
			        printf("Вершина %s не найдена.\n", str);
			    }
			    break;
        	}
        	case 6:{
        	    char str1[50];
        		char str2[50];
        		scanf("%s %s",str1,str2);
        		int startVertex = findVertex(graph, str1);
			    int endVertex = findVertex(graph, str2);
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
			    break;
        	}
        	case 7:{
        		splitGraph(graph);
        		break;
        	}
        	case 8:{
        		printGraph(graph);
        		break;
        	}
        	case 9:{
        		if(printGraphvizToFile(graph, "graph.dot")){
        			choice = 11;
        		}
        		break;
        	}
        	case 10:{
        		if(writeGraphToFile(graph,"dop2.txt")){
        			choice = 11;
        			break;
        		}
        		if(generateTextFile("dop.txt")){
        			choice = 11;
        			break;
        		}
        		if(readGraphFromFile(graph,"dop.txt")){
        			choice = 11;
        			break;
        		}
        		printGraph(graph);
        		break;
        	}
        	case 11:{
        		free(graph);
        		return 0;
        	}
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 11);
    return 0;
}
