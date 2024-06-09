#include "tree.h"

int main() {
    Node *root = NULL;
    int choice;
    char* key;
    char* value;
        printf("1. Insert a node\n");
        printf("2. Find a node\n");
        printf("3. Find a special node\n");
        printf("4. Delete node\n");
        printf("5. Tree traversal\n");
        printf("6. Perform timing test find\n");
        printf("7. File import\n");
        printf("8. Print tree\n");
        printf("9. Png tree\n");
        printf("10. Exit\n");
        printf("11. Perform timing test delete\n");

    do {
        printf("Enter your choice: ");
        scanf("%d", &choice);  
        Node* ne;
        Node* new;   
        switch(choice) {
            case 1:
            	key = readline("key: ");
            	value = readline("info: ");
            	insertNode(&root,key,value);
            	free(key);
            	free(value);
                break;
            case 2:
                key = readline("key: ");
                new = findNode(root,key,0);
                printf("key - %s, value - %s\n",new->key,new->value);
                free(key);
                //free(new);
                break;
            case 3:
                key = readline("key: ");
                ne = searchClosestKey(root,key);
                printf("key - %s, value - %s\n",ne->key,ne->value);
                free(key);
                //free(ne);
                break;
            case 4:
            	key = readline("key: ");
            	int t;
            	scanf("Version elemen: %d",&t);
            	deleteNode(&root,key,t);
            	free(key);
                break;
            case 5:
            	key = readline("key: ");
            	reverseOrderTraversal(root,key,NULL);
            	free(key);
                break;                
            case 6:
                D_Timing(&root);
                break;
            case 11:
                Del_Timing(&root);
                break;
            case 7:
            	char* filename;
            	filename = readline("filename: ");
            	loadTreeFromFile(&root,"t.txt");
            	free(filename);
            	break;
            case 8:
            	printTree(root,0);
            	break;
            case 9:
            	createDotFile(root);
            	break;
            case 10:
                freeTree(root);
                return 0;
            case 12:
            	FILE *fp = fopen("out.txt","w");
            	fclose(fp);
            	root = build("in.txt");
    			out(root);
    			break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 13);
    
    return 0;
}



