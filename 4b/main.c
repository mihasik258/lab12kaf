#include "btree.h"
int main() {
    Node *root = NULL;
    int choice;
    unsigned int key;
    unsigned int value;
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

    do {
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Error: Failed to read choice\n");
            return FAILED_CHOICE;
        }
        Node* ne;
        switch(choice) {
            case 1:
                printf("Enter key: ");
                if (scanf("%d", &key) != 1) {
                    fprintf(stderr, "Error: Failed to read key\n");
                    return FAILED_KEY;
                }
    
                printf("Enter value: ");
                if (scanf("%d", &value) != 1) {
                    fprintf(stderr, "Error: Failed to read value\n");
                    return FAILED_KEY;
                }
    
                insert(&root, key, &value);
                break;
           case 2: {
                printf("Enter key: ");
                if (scanf("%d", &key) != 1) {
                    fprintf(stderr, "Error: Failed to read key\n");
                    return FAILED_KEY;
                }
                Node *result = search(root, key);
                if (result != NULL) {
    
                    int i = 0;
                    while (i < result->n && key > result->keys[i]) {
                        i++;
                    }
                    printf("Key: %d, Value: %d\n", result->keys[i], *result->data[i]);
                } else {
                    printf("Key not found.\n");
                }
                break;
                }
            case 3:
                printf("Enter key: ");
                if (scanf("%d", &key) != 1) {
                    fprintf(stderr, "Error: Failed to read key\n");
                    return FAILED_KEY;
                }
                ne = specfind(root);
                printf("key - %d, value - %ls\n",ne->keys[0],ne->data[0]);
                break;
            case 4:
            	printf("Enter key: ");
                if (scanf("%d", &key) != 1) {
                    fprintf(stderr, "Error: Failed to read key\n");
                    return FAILED_KEY;
                }
            	removeKey(&root,key);
                break;
            case 5:
            	printf("Enter key: ");
                if (scanf("%d", &key) != 1) {
                    fprintf(stderr, "Error: Failed to read key\n");
                    return FAILED_KEY;
                }
            	Order(root,key);
                break;                
            case 6:
                D_Timing(&root);
                break;
            case 7:
            	char* filename;
            	filename = readline("filename: ");
            	loadTreeFromFile(&root,filename);
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
            case 11:
            	specialLoadTreeFromFile(&root,"dop.txt");
            	printf("Enter key: ");
                if (scanf("%d", &key) != 1) {
                    fprintf(stderr, "Error: Failed to read key\n");
                    return FAILED_KEY;
                }
                Node *result = search(root, key);
                int vector[100];
                int b = 0;
                while (result != NULL) {
    
                    int i = 0;
                    while (i < result->n && key > result->keys[i]) {
                        i++;
                    }
                    vector[b]= *(result->data[i]);
                    removeKey(&root,key);
                    result = search(root,key);
                    b++;
                }
                for (int i = 0;i<b;i++){
                	printf("%d ",vector[i]);
                }
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 11);
    return 0;
}