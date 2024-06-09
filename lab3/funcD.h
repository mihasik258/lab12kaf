#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int KeyType;
typedef int RelType;
typedef struct InfoType {
    char data[50];
} InfoType;

typedef struct Node {
    RelType release;
    InfoType *info;
    struct Node *next;
} Node;

typedef struct KeySpace {
    KeyType key;
    Node *node;
    struct KeySpace *link;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
    int msize;
    int csize;
} Table;
typedef struct TableIterator {
  Node *current;
  Node *next;
} TableIterator;


void itInit(TableIterator *it, Table *t);
int IterChNext(TableIterator *it);
Node *IterNext(TableIterator *it);
Node *IterLast(TableIterator *it);
TableIterator AddElemIter(Table *t, KeyType k, Node *elem);
TableIterator DelElemIter(Table *t, KeyType k, TableIterator *it);
TableIterator SearchElemIter(Table *t, KeyType k, InfoType **info);
void insertElement(Table *t, KeyType key, char *data);
void deleteElement(Table *t, KeyType key, RelType release);
void deleteAllByKey(Table *t, KeyType key, RelType release);
void searchElement(Table *t, KeyType key, RelType release);
void printTable(Table *t) ;
void importFromFile(Table *t, char *filename) ;
void searchAllVersions(Table *t, KeyType key, RelType release);
void deleteTable(Table *tbl);
