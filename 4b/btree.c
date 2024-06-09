#include "btree.h"

Node *createNode(int leaf) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL){
	return ERR_MEM;
    }
    node->n = 0;
    node->leaf = leaf;
    return node;
}

void splitChild(Node *parent, int i, Node *child) {
    Node *newNode = createNode(child->leaf);
    newNode->n = T - 1;
    for (int j = 0; j < T - 1; j++) {
        newNode->keys[j] = child->keys[j + T];
        newNode->data[j] = child->data[j + T];
        newNode->children[j] = child->children[j + T];
    }
    newNode->children[T - 1] = child->children[2 * T - 1];
    child->n = T - 1;
    for (int j = parent->n; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[i + 1] = newNode;
    for (int j = parent->n - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
        parent->data[j + 1] = parent->data[j];
    }
    parent->keys[i] = child->keys[T - 1];
    parent->data[i] = child->data[T - 1];
    parent->n++;
}


void insertH(Node *node, unsigned int key, unsigned int *value) {
    int i = node->n - 1;

    if (node->leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            node->data[i + 1] = node->data[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->data[i + 1] = value;
        node->n++;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        if (node->children[i]->n == 2 * T - 1) {
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertH(node->children[i], key, value);
    }
}


int insert(Node **root, unsigned int key, unsigned int *value) {
    if (*root == NULL) {
        *root = createNode(1);
        (*root)->keys[0] = key;
        (*root)->data[0] = value;
        (*root)->n = 1;
    } else {
        if ((*root)->n == 2 * T - 1) {
            Node *newNode = createNode(0);
            newNode->children[0] = *root;
            splitChild(newNode, 0, *root);
            *root = newNode;
        }
        insertH(*root, key, value);
    }
    return 1;
}

int printTree(Node *node, int level) {
    if (node == NULL) return HAVENT_TREE;
    for (int i = 0; i < level; i++) printf("    "); 
    printf("[");
    for (int i = 0; i < node->n; i++) {
        printf("%d", node->keys[i]);
        if (i != node->n - 1) printf(", ");
    }
    printf("]");
    if (!node->leaf) {
        printf(" ->\n");
        for (int i = 0; i <= node->n; i++) {
            printTree(node->children[i], level + 1);
        }
    } else {
        printf("\n");
    }
    return 1;
}


Node *search(Node *node, unsigned int key) {
    int i = 0;
    while (i < node->n && key > node->keys[i]) {
        i++;
    }
    if (i < node->n && key == node->keys[i]) {
        return node;
    }
    if (node->leaf) {
        return NULL;
    }
    return search(node->children[i], key);
}

void Next(Node *node, int indx) {
    Node *child = node->children[indx];
    Node *s = node->children[indx + 1];
    child->keys[child->n] = node->keys[indx];
    child->data[child->n] = node->data[indx];
    child->n++;
    node->keys[indx] = s->keys[0];
    node->data[indx] = s->data[0];
    for (int i = 1; i < s->n; i++) {
        s->keys[i - 1] = s->keys[i];
        s->data[i - 1] = s->data[i];
        s->children[i - 1] = s->children[i];
    }
    s->children[s->n - 1] = s->children[s->n];
    s->n--;
}

void bPrev(Node *node, int indx) {
    Node *child = node->children[indx];
    Node *s = node->children[indx - 1];
    for (int i = child->n - 1; i >= 0; i--) {
        child->keys[i + 1] = child->keys[i];
        child->data[i + 1] = child->data[i];
        child->children[i + 1] = child->children[i];
    }
    child->children[child->n + 1] = child->children[child->n];
    child->keys[0] = node->keys[indx - 1];
    child->data[0] = node->data[indx - 1];
    child->n++;
    node->keys[indx - 1] = s->keys[s->n - 1];
    node->data[indx - 1] = s->data[s->n - 1];
    s->n--;
}


void merge(Node *node, int indx) {
    Node *child = node->children[indx];
    Node *s = node->children[indx + 1];
    child->keys[T - 1] = node->keys[indx];
    child->data[T - 1] = node->data[indx];
    for (int i = 0; i < s->n; i++) {
        child->keys[i + T] = s->keys[i];
        child->data[i + T] = s->data[i];
        child->children[i + T] = s->children[i];
    }
    child->children[2 * T - 1] = s->children[s->n];
    for (int i = indx + 1; i < node->n; i++) {
        node->data[i - 1] = node->data[i];
        node->keys[i - 1] = node->keys[i];
        node->children[i] = node->children[i + 1];
    }
    node->n--;
    free(s);
}


void fill(Node *node, int indx) {
    if (indx != node->n && node->children[indx + 1]->n >= T) {
        Next(node, indx);
    } 
    else if (indx != 0 && node->children[indx - 1]->n >= T) {
        bPrev(node, indx);
    } 
    else {
        if (indx != node->n) {
            merge(node, indx);
        } else {
            merge(node, indx - 1);
        }
    }
}

void removeKey(Node **node, unsigned int key) {
    int indx = 0;
    while (indx < (*node)->n && key > (*node)->keys[indx]) {
        indx++;
    }
    if (indx < (*node)->n && key == (*node)->keys[indx]) {
        if ((*node)->leaf) {
            for (int i = indx + 1; i < (*node)->n; i++) {
                (*node)->keys[i - 1] = (*node)->keys[i];
                (*node)->data[i - 1] = (*node)->data[i];
            }
            (*node)->n--;
        } else {
            Node *pred = (*node)->children[indx];
            while (!pred->leaf) {
                pred = pred->children[pred->n];
            }
            unsigned int predKey = pred->keys[pred->n - 1];
            unsigned int *predData = pred->data[pred->n - 1]; 
            (*node)->keys[indx] = predKey;
            (*node)->data[indx] = predData;
            removeKey(&(*node)->children[indx], predKey); 
        }
    } else {
        if ((*node)->leaf) {
            return;
        }
        if ((*node)->children[indx]->n < T) {
            fill(*node, indx);
        }
        if (indx > (*node)->n) {
            indx--;
        }
        removeKey(&(*node)->children[indx], key);
    }
    if ((*node)->n == 0) {
        Node *tmp = *node;
        if ((*node)->leaf) {
            *node = NULL;
        } else {
            *node = (*node)->children[0];
        }
        free(tmp);
    }
}
void Order(Node *node, unsigned int key) {
    if (node != NULL) {
        int i;
        Node *current = node;
        while (!current->leaf) {
            i = current->n - 1;
            while (i >= 0 && current->keys[i] > key) {
                i--;
            }
            if (i < 0) {
                current = current->children[0];
            } else {
                current = current->children[i + 1];
            }
        }
        while (current != node) {
            for (i = current->n - 1; i >= 0 && current->keys[i] <= key; i--) {
                printf("%u ", current->keys[i]);
            }
            Node *parent = node;
            while (true) {
                for (i = 0; i <= parent->n; ++i) {
                    if (parent->children[i] == current) {
                        break;
                    }
                }
                if (parent->children[i] == current) {
                    break; 
                }
                current = parent; 
                parent = node;    
            }
            i = i - 1; 
            while (i >= 0 && parent->keys[i] <= key) {
                if (!parent->leaf) {
                    current = parent->children[i + 1]; 
                    while (!current->leaf) { 
                        i = current->n - 1;
                        while (i >= 0 && current->keys[i] > key) {
                            i--;
                        }
                        if (i < 0) {
                            current = current->children[0];
                        } else {
                            current = current->children[i + 1];
                        }
                    }
                    Order(current, key); 
                }
                i--;
            }

            current = parent; 
        }
        for (i = current->n - 1; i >= 0 && current->keys[i] <= key; i--) {
            printf("%u ", current->keys[i]);
        }
    }
}


Node *specfind(Node *node) {
    if (node == NULL) {
        return NULL;
    }
    Node *current = node;
    while (!current->leaf) {
        current = current->children[0];
    }
    return current;
}

int D_Timing(Node **root) {
    int n = 10, cnt = 1000000, i, m;
    int key[10000]; 
    clock_t first, last;
    srand(time(NULL));
    unsigned int tt = 1;
    while (n-- > 0) {
        for (i = 0; i < 10000; ++i) {
            key[i] = rand() * rand(); 
        }
        
        for (i = 0; i < cnt; ) {
            int k = rand()*rand();
            if (insert(root, k, &tt)){
                ++i;
            }
        }
        m = 0;
        first = clock();
        for (i = 0; i < 10000; ++i) {
            if (search(*root, key[i])) {
                ++m;
            }
        }
        last = clock();
        printf("%d items were found\n", m);
        printf("Test #%d, number of nodes = %d, time = %f seconds\n", 10 - n, (10 - n) * cnt, (double)(last - first) / CLOCKS_PER_SEC);
    }
    return 1;
} 
int generateDotFile(Node *root, FILE *file) {
    if (root == NULL) {
        return HAVENT_TREE;
    }
    fprintf(file, "node%p [label=\"", root);
    for (int i = 0; i < root->n; i++) {
        fprintf(file, "%d | ", root->keys[i]);
    }
    fprintf(file, "\"];\n");
    if (!root->leaf) {
        for (int i = 0; i <= root->n; i++) {
            fprintf(file, "node%p -> node%p;\n", root, root->children[i]);
            generateDotFile(root->children[i], file);
        }
    }
    return 1;
}

void freeTree(Node *node) {
    if (node != NULL) {
        for (int i = 0; i < 4; i++) { 
        	if(node->children[i]!=NULL && node->leaf==0){
            	freeTree(node->children[i]);}
        }
        free(node);
    }
}

void createDotFile(Node *root) {
    FILE *dotFile = fopen("tree.dot", "w");
    if (dotFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(dotFile, "digraph BTree {\n");
    generateDotFile(root, dotFile);
    fprintf(dotFile, "}\n");

    fclose(dotFile);
    system("dot -Tpng tree.dot -o tree.png");
}
void loadTreeFromFile(Node **root, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return;
    }
    unsigned int key, value;
    while (fscanf(file, "%d\n%d\n", &key, &value) == 2) {
        insert(root, key, &value); 
    }

    fclose(file);
}
void specialLoadTreeFromFile(Node **root, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return;
    }
    unsigned int key;
    unsigned int c = 0;
    char line[100];
    while (fgets(line,sizeof(line),file)){
    	c++;
    	char* token = strtok(line,", ");
    	while (token!=NULL){
    		sscanf(token,"%u",&key);
    		unsigned int *n = (unsigned int *) calloc(1, sizeof(unsigned int));
    		*n = c;
        	insert(root, key, n); 
        	token = strtok(NULL,", ");
        }
    }
    fclose(file);
}