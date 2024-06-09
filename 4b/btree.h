#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <readline/readline.h>

#define T 2 
#define  HAVENT_TREE -2
#define FAILED_KEY 3
#define FAILED_CHOICE 4
#define ERR_MEM NULL

typedef struct Node {
    int n;
    unsigned int keys[2 * T - 1];
    struct Node *children[2 * T]; 
    unsigned int *data[2 * T - 1]; 
    int leaf; 
} Node;

Node *createNode(int leaf);
void splitChild(Node *parent, int i, Node *child);

void insertH(Node *node, unsigned int key, unsigned int *value);


int insert(Node **root, unsigned int key, unsigned int *value);
int printTree(Node *node, int level);


Node *search(Node *node, unsigned int key);

void Next(Node *node, int indx);

void bPrev(Node *node, int indx) ;


void merge(Node *node, int indx);


void fill(Node *node, int indx);

void removeKey(Node **node, unsigned int key);
void Order(Node *node, unsigned int key);
Node *specfind(Node *node);
int D_Timing(Node **root);
int generateDotFile(Node *root, FILE *file);

void freeTree(Node *node);

void createDotFile(Node *root);
void loadTreeFromFile(Node **root, const char *filename);
void specialLoadTreeFromFile(Node **root, const char *filename);