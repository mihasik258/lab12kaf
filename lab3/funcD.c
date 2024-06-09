#include "funcD.h"

void insertElement(Table *t, KeyType key, char *data) {
    KeySpace *ks = t->ks;
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->release = 1;
    newNode->next = NULL;
    newNode->info = (InfoType *)malloc(sizeof(InfoType));
    strcpy(newNode->info->data, data);
    RelType maxRelease = 1;
    while (ks != NULL) {
        if (ks->key == key) {
            Node *temp = ks->node;
            while (temp != NULL) {
                if (temp->release >= maxRelease) {
                    maxRelease = temp->release + 1;
                }
                temp = temp->next;
            }
        }
        ks = ks->link;
    }
    ks = t->ks;
    KeySpace *prev = NULL;
    while (ks != NULL) {
        if (ks->key == key) {
            newNode->release = maxRelease;
            newNode->next = ks->node;
            ks->node = newNode;
            t->csize++;
            return;
        }
        prev = ks;
        ks = ks->link;
    }
    KeySpace *newKs = (KeySpace *)malloc(sizeof(KeySpace));
    newKs->key = key;
    newKs->node = newNode;
    newKs->link = NULL;
    if (prev == NULL) {
        t->ks = newKs; 
    } else {
        prev->link = newKs;
    }
    t->msize++;
    t->csize++;
}

void deleteElement(Table *t, KeyType key, RelType release) {
    KeySpace *prev = NULL;
    KeySpace *ks = t->ks;
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
                    t->csize--;
                    if (ks->node == NULL) {
                        if (prev == NULL) {
                            t->ks = ks->link;
                        } else {
                            prev->link = ks->link;
                        }
                        free(ks);
                        t->msize--;
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


void deleteAllByKey(Table *t, KeyType key, RelType release) {
    if (release == -1) {
        KeySpace *prev = NULL;
        KeySpace *ks = t->ks;
        while (ks != NULL) {
            if (ks->key == key) {
                Node *currentNode = ks->node;
                
                while (currentNode != NULL) {
                    Node *temp = currentNode;
                    currentNode = currentNode->next;

                    free(temp->info);
                    free(temp);
                    t->csize--;
                }

                ks->node = NULL;
                t->csize -= key; 
                KeySpace *toDelete = ks;               
                if (prev == NULL) {
                    t->ks = ks->link;
                } else {
                    prev->link = ks->link;
                }
                t->msize--;
                free(toDelete);
                return;
            }
            prev = ks;
            ks = ks->link;
        }
    } else {
        deleteElement(t, key, release);
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
void deleteTable(Table *t) {
    if (t == NULL) {
        return; 
    }
    KeySpace *ks = t->ks;
    while (ks != NULL) {
        KeySpace *nextKs = ks->link;
        Node *node = ks->node;
        while (node != NULL) {
            Node *temp = node;
            node = node->next;
            if (temp->info != NULL) {
                free(temp->info);
                temp->info = NULL;
            }
            if (temp != NULL) {
                free(temp);
                temp = NULL;
            }
        }
        free(ks);
        ks = nextKs;
    }
}
void itInit(TableIterator *it, Table *t) {
    if (t == NULL || t->ks == NULL) {
        it->current = NULL;
        it->next = NULL;
        return;
    }

    it->current = t->ks->node;
    it->next = t->ks->link != NULL ? t->ks->link->node : NULL;
}

int IterChNext(TableIterator *it) {
    return it->current != NULL;
}

Node *IterNext(TableIterator *it) {
    if (IterChNext(it)) {
        Node *current_node = it->current;
        it->current = it->current->next;
        return current_node;
    }
    return NULL;
}

Node *IterLast(TableIterator *it){
    while (IterChNext(it)) {
        it->current = IterNext(it);
    }
    return it->current;
}
TableIterator AddElemIter(Table *t, KeyType k, Node *elem)
{
    TableIterator iter;
    itInit(&iter, t);

    KeySpace *ks = NULL;
    Node *prevNode = NULL;

    while (IterChNext(&iter))
    {
        Node *current = IterNext(&iter);
        if (current->release > elem->release)
        {
            if (prevNode == NULL)  
            {
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->release = elem->release;
                newNode->info = elem->info;
                newNode->next = current;
                ks->node = newNode;
            }
            else
            {
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->release = elem->release;
                newNode->info = elem->info;
                newNode->next = current;
                prevNode->next = newNode;
            }
            return iter;
        }
        prevNode = current;
    }

    KeySpace *newKs = (KeySpace *)malloc(sizeof(KeySpace));
    newKs->key = k;
    newKs->node = elem;
    newKs->link = t->ks;
    t->ks = newKs;
    t->msize++;
    t->csize++;
    iter.current = newKs->node;
    iter.next = NULL;

    return iter;
}


TableIterator SearchElemIter(Table *t, KeyType k, InfoType **info)
{
    TableIterator iter;
    itInit(&iter, t);

    while (IterChNext(&iter))
    {
        Node *current = IterNext(&iter);
        if (current->release == iter.current->release)
        {
            *info = current->info;
            return iter;
        }
    }

    iter.current = NULL;
    iter.next = NULL;
    return iter;
}                            
TableIterator DelElemIter(Table *tabl, KeyType k, TableIterator *iter)
{
    TableIterator prevIter;
    prevIter.current = NULL;
    prevIter.next = tabl->ks != NULL ? tabl->ks->node : NULL;
    while (IterChNext(iter))
    {
        Node *current = IterNext(iter);
        if (current->release == iter->current->release)
        {
            if (prevIter.current == NULL)
            {
                tabl->ks->node = current->next;
            }
            else
            {
                prevIter.current->next = current->next;
            }
            free(current->info);
            free(current);
            tabl->csize--;
            if (tabl->ks->node == NULL)
            {
                if (prevIter.current == NULL)
                {
                    tabl->ks = tabl->ks->link;
                }
                else
                {
                    prevIter.next = tabl->ks != NULL ? tabl->ks->link != NULL ? tabl->ks->link->node : NULL : NULL;
                }
                free(iter->current);
                iter->current = prevIter.current; 
                iter->next = prevIter.next;

                tabl->msize--;
            }
            return prevIter;
        }
        prevIter.current = current;
    }

    return prevIter;
}
