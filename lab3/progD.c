#include "funcD.h"

int main() {
    Table table;
        table.ks = NULL;
    table.msize = 0;
    table.csize = 0;
    TableIterator iter;
	itInit(&iter, &table);
	KeyType key;
	char data[50];
	char filename[50];
	int choice;
        printf("Menu:\n");
        printf("1. Insert new element\n");
        printf("2. Delete element by key\n");
        printf("3. Delete all versions by key\n");
        printf("4. Search element by key\n");
        printf("5. Print table\n");
        printf("6. Import from file\n");
        printf("7. Search all versions by key\n");
        printf("0. Exit\n");
    do {
        printf("Enter your choice: ");  
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter key and data for the element: ");
                scanf("%d %s", &key, data);
				Node *node1 = (Node *)malloc(sizeof(Node));
    			node1->release = 1;
    			strcpy(node1->info->data, data);
    			AddElemIter(&table, key, node1);
                break;
            case 2:
                printf("Enter key to delete: ");
                scanf("%d", &key);
                deleteElement(&table, key, -1);
                break;
            case 3:
                printf("Enter the key and -1 to delete all versions, or enter the key and version: ");
                scanf("%d", &key);
                RelType rel;
                scanf("%d", &rel);                
                deleteAllByKey(&table, key, rel);
                break;
            case 4:
                printf("Enter key to search: ");
                scanf("%d", &key);
    InfoType *info;
    TableIterator iter = SearchElemIter(&table, key, &info);
    if (info != NULL) {
        printf("Found info: %s\n", (char*)info);
    } else {
        printf("Element not found.\n");
    }
                break;
            case 5:
                printTable(&table);
                break;
            case 6:
                printf("Enter filename to import from: ");
                scanf("%s", filename);
                importFromFile(&table, filename);
                break;
            case 7:
                printf("Enter the key and -1 to search all versions, or enter the key and version: ");
                scanf("%d", &key);
                RelType r;
                scanf("%d", &r);                
                searchAllVersions(&table, key, r);
                break;
            case 0:
            	deleteTable(&table);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}                 
