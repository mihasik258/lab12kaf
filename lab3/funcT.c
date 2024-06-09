#include "func.h"

void insertElement(Table *tbl, KeyType key, char *data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed for newNode.\n");
        return;
    }

    newNode->release = 1;
    newNode->next = NULL; // Инициализируем указатель на следующий узел

    newNode->info = (InfoType *)malloc(sizeof(InfoType));
    if (newNode->info == NULL) {
        printf("Memory allocation failed for newNode->info.\n");
        free(newNode);
        return;
    }
    strncpy(newNode->info->data, data, sizeof(newNode->info->data)-1);
    newNode->info->data[sizeof(newNode->info->data)-1] = '\0'; 

    KeySpace *ks = tbl->ks;
    KeySpace *prev = NULL;
    
    while (ks != NULL) {
        if (ks->key == key) {
            Node *temp = ks->node;
            if (temp == NULL) {
                ks->node = newNode;
            } else {
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                newNode->release = temp->release + 1;
                temp->next = newNode;
            }
            tbl->csize++;
            return;
        }
        prev = ks;
        ks = ks->link;
    }
    KeySpace *newKs = (KeySpace *)malloc(sizeof(KeySpace));
    if (newKs == NULL) {
        printf("Memory allocation failed for newKs.\n");
        free(newNode->info);
        free(newNode);
        return;
    }

    newKs->key = key;
    newKs->node = newNode;
    newKs->link = NULL;

    if (prev == NULL) {
        tbl->ks = newKs;
    } else {
        prev->link = newKs;
    }

    tbl->msize++;
    tbl->csize++;
}



  

void deleteElement(Table *tbl, KeyType key, RelType release) {
    KeySpace *prev = NULL;
    KeySpace *ks = tbl->ks;
    while (ks != NULL) {
        if (ks->key == key) {
            Node *prevNode = NULL;
            Node *currentNode = ks->node;
            while (currentNode != NULL) {
                if (release == -1 || currentNode->release == release) {
                    if (prevNode == NULL) {
                        ks->node = currentNode->next;
                    } else {
                        prevNode->next = currentNode->next;
                    }
                    free(currentNode->info);
                    free(currentNode);
                    tbl->csize--;
                    if (ks->node == NULL) {
                        if (prev == NULL) {
                            tbl->ks = ks->link;
                        } else {
                            prev->link = ks->link;
                        }
                        free(ks);
                        tbl->msize--;
                    }
                    return;
                }
                prevNode = currentNode;
                currentNode = currentNode->next;
            }
        }
        prev = ks;
        ks = ks->link;
    }
}                    
void deleteAllByKey(Table *tbl, KeyType key, RelType release) {
    if (release == -1) {
        KeySpace *prev = NULL;
        KeySpace *ks = tbl->ks;

        while (ks != NULL) {
            if (ks->key == key) {
                Node *currentNode = ks->node;
                while (currentNode != NULL) {
                    Node *temp = currentNode;
                    currentNode = currentNode->next;
                    free(temp->info);
                    free(temp);
                    tbl->csize--;
                }
                ks->node = NULL;
                KeySpace *toDelete = ks;
                tbl->csize -= key; // Уменьшаем csize на значение ключа, а не на количество ключей
                if (prev == NULL) {
                    tbl->ks = ks->link;
                } else {
                    prev->link = ks->link;
                }
                free(toDelete);
                tbl->msize--;
                break; // Добавлено завершение цикла после удаления элемента
            }
            prev = ks;
            ks = ks->link;
        }
    } else {
        deleteElement(tbl, key, release);
    }
}                

void searchElement(Table *t, KeyType key, RelType release) {
    KeySpace *ks = t->ks;
    printf("Key     Release     Data\n");
    while (ks != NULL) {
        if (ks->key == key) {
            Node *node = ks->node;
            while (node != NULL) {
                if (release == -1 || node->release == release) {
                    printf("%d      %d      %s\n", ks->key, node->release, node->info->data);
                }
                node = node->next;
            }
        }
        ks = ks->link;
    }
}

void printTable(Table *t) {
    KeySpace *ks = t->ks;
    printf("Key     Release     Data\n");
    while (ks != NULL) {
        Node *node = ks->node;
        while (node != NULL) {
            printf("%d      %d      %s\n", ks->key, node->release, node->info->data);
            node = node->next;
        }
        ks = ks->link;
    }
}

void importFromFile(Table *t, char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error opening file.");
        return;
    }

    KeyType key;
    char data[50];

    while (fscanf(file, "%d %s", &key, data) != EOF) {
        insertElement(t, key, data);
    }

    fclose(file);
}

void searchAllVersions(Table *t, KeyType key, RelType release) {
    searchElement(t, key, release);
}
