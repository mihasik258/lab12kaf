#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <time.h>

#define NULL_ER 1
#define IND_ER 2

typedef struct Node {
    char *key;
    char *value;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    struct Node *prev;
    struct Node *infoField;
} Node;

Node *createNode(char *key, char *value);
int insertNode(Node **root, char *key, char *value);
Node* findNode(Node *root, char *key, int index);
Node* searchClosestKey(Node *root, char *key);
void freeTree(Node *root);
void reverseInOrderTraversal(Node *root);
void printTree(Node *root, int level);
int deleteNode(Node **root, char *key, int index);
void reverseOrderTraversal(Node *root, char *substring, Node *prev);
void generateDotFile(Node *root, FILE *dotFile);
void createDotFile(Node *root);
int D_Timing(Node **root);
int Del_Timing(Node **root);
void loadTreeFromFile(struct Node** root, const char* filename);
Node* build(const char*);
void out(Node* );



