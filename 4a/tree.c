#include "tree.h"

Node *createNode(char *key, char *value) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        exit(1);
    }
    newNode->key = strdup(key);
    newNode->value = strdup(value);
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->prev = NULL;
    newNode->infoField = NULL;
    
    return newNode;
}

int insertNode(Node **root, char *key, char *value) {
    if (*root == NULL) {
        *root = createNode(key, value);
    } else {
        int cmp = strcmp(key, (*root)->key);
        if (cmp == 0) {
            Node *infoNode = createNode(key, value);
            infoNode->infoField = (*root)->infoField;
            (*root)->infoField = infoNode;
        } else if (cmp < 0) {
            insertNode(&((*root)->left), key, value);
            (*root)->left->parent = *root; 
        } else {
            insertNode(&((*root)->right), key, value);
            (*root)->right->parent = *root; 
        }
    }
    return 1;
}
Node* findNode(Node *root, char *key, int index) {
    if (root == NULL) {
        return NULL;
    }
    if (strcmp(root->key, key) == 0) {
        return root;
    } else if (strcmp(root->key, key) > 0) {
        return findNode(root->left, key, index);
    } else {
        return findNode(root->right, key, index);
    }
}

void freeTree(Node *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        Node* prev;
        while(root->infoField != NULL){
            prev = root;
            root = root->infoField;
        	free(prev->key);
        	free(prev->value);
        	free(prev);           
        }
        free(root->key);
        free(root->value);
        free(root);
    }
}

void reverseInOrderTraversal(Node *root) {
    if (root != NULL) {
        reverseInOrderTraversal(root->right);
        printf("Key: %s, Value: %s\n", root->key, root->value);
        Node *info = root->infoField;
        while (info != NULL) {
            printf("InfoField - Key: %s, Value: %s\n", info->key, info->value);
            info = info->infoField;
        }
        reverseInOrderTraversal(root->left);
    }
}
void printTree(Node *root, int level) {
    if (root == NULL) {
        return;
    }
    printTree(root->right, level + 1);
    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    printf("|-- %s\n", root->key);
    Node *info = root->infoField;
    while (info != NULL) {
        for (int i = 0; i < level; i++) {
            printf("    ");
        }
        printf("|-- %s\n", root->key);
        info = info->infoField;
    }
    printTree(root->left, level + 1);
}
int deleteNode(Node **root, char *key, int index) {
    Node *nodeToDelete = findNode(*root, key, index);
    if (nodeToDelete == NULL) {
        return NULL_ER;
    }
    Node *parent = nodeToDelete->parent;
    if (index != 0){
        int i = 0;
        Node *prev;
        for (i = 0; i<index;i++){
            prev = nodeToDelete;
            nodeToDelete = nodeToDelete->infoField;
        }
        if (i < index) return IND_ER;
        if (nodeToDelete->infoField!=NULL){
            prev->infoField = nodeToDelete->infoField;
        }
        else{
            prev->infoField = NULL;
        }
    }
    else{
        if (nodeToDelete->left == NULL && nodeToDelete->right == NULL) {
            if (parent == NULL) {
                *root = NULL;
            } else if (parent->left == nodeToDelete) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        } else if (nodeToDelete->left == NULL) {
            if (parent == NULL) {
                *root = nodeToDelete->right;
            } else if (parent->left == nodeToDelete) {
                parent->left = nodeToDelete->right;
            } else {
                parent->right = nodeToDelete->right;
            }
            if (nodeToDelete->right != NULL) {
                nodeToDelete->right->parent = parent;
            }
        } else if (nodeToDelete->right == NULL) {
            if (parent == NULL) {
                *root = nodeToDelete->left;
            } else if (parent->left == nodeToDelete) {
                parent->left = nodeToDelete->left;
            } else {
                parent->right = nodeToDelete->left;
            }
            if (nodeToDelete->left != NULL) {
                nodeToDelete->left->parent = parent;
            }
        } else {
            Node *successor = nodeToDelete->right;
            while (successor->left != NULL) {
                successor = successor->left;
            }
            Node *successorParent = successor->parent;
            if (successorParent != nodeToDelete) {
                successorParent->left = successor->right;
                if (successor->right != NULL) {
                    successor->right->parent = successorParent;
                }
                successor->right = nodeToDelete->right;
                if (nodeToDelete->right != NULL) {
                    nodeToDelete->right->parent = successor;
                }
            }
    
            if (parent == NULL) {
                *root = successor;
            } else if (parent->left == nodeToDelete) {
                parent->left = successor;
            } else {
                parent->right = successor;
            }
            successor->parent = parent;
            successor->left = nodeToDelete->left;
            if (nodeToDelete->left != NULL) {
                nodeToDelete->left->parent = successor;
            }
        }
    }
    free(nodeToDelete->key);
    free(nodeToDelete->value);
    free(nodeToDelete);
}
void reverseOrderTraversal(Node *root, char *substring, Node *prev) {
    if (root == NULL) {
        return;
    }

    reverseOrderTraversal(root->right, substring, root);

    if (strstr(root->key, substring) == root->key) {
        printf("%s\n", root->value);
    }

    root->prev = prev;

    reverseOrderTraversal(root->left, substring, root);
}
Node* searchClosestKey(Node *root, char *key) {
    if (root == NULL) {
        return NULL;
    }

    Node *closest = root;
    int maxDifference = abs(strcmp(root->key, key));
    Node *leftClosest = searchClosestKey(root->left, key);
    Node *rightClosest = searchClosestKey(root->right, key);

    if (leftClosest != NULL) {
        int leftDifference = abs(strcmp(leftClosest->key, key));
        if (leftDifference > maxDifference) {
            closest = leftClosest;
            maxDifference = leftDifference;
        }
    }

    if (rightClosest != NULL) {
        int rightDifference = abs(strcmp(rightClosest->key, key));
        if (rightDifference > maxDifference) {
            closest = rightClosest;
            maxDifference = rightDifference;
        }
    }
    return closest;
}
void generateDotFile(Node *root, FILE *dotFile) {
    if (root == NULL) {
        return;
    }
	fprintf(dotFile, "  %s [shape=ellipse,style=filled,fillcolor=\"#faaa1\",fontname=\"Arial\",fontsize=10,penwidth=2];\n", root->key);
    if (root->left != NULL) {
        fprintf(dotFile, "\"%s\" -> \"%s\"[color=\"#666666\"];\n", root->key, root->left->key);
        generateDotFile(root->left, dotFile);
    }

    if (root->right != NULL) {
        fprintf(dotFile, "\"%s\" -> \"%s\"[color=\"#666666\"];\n", root->key, root->right->key);
        generateDotFile(root->right, dotFile);
    }
}
void createDotFile(Node *root) {
    FILE *dotFile = fopen("tree.dot", "w");
    if (dotFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(dotFile, "digraph BinarySearchTree {\n");
    generateDotFile(root, dotFile);
    fprintf(dotFile, "}\n");

    fclose(dotFile);
    system("dot -Tpng tree.dot -o tree.png");
}
int D_Timing(Node **root) {
    int n = 10, cnt = 1000000, i, m;
    char key[10000][50]; 
    clock_t first, last;
    srand(time(NULL));
    
    while (n-- > 0) {
        for (i = 0; i < 10000; ++i) {
            sprintf(key[i], "key%d", rand()); 
        }
        
        for (i = 0; i < cnt; ) {
            int k = rand()*rand();
            if (insertNode(root, key[k % 10000], "value")){
                ++i;
            }
        }
        m = 0;
        first = clock();
        for (i = 0; i < 10000; ++i) {
            if (findNode(*root, key[i],0)) {
                ++m;
            }
        }
        last = clock();
        printf("%d items were found\n", m);
        printf("Test #%d, number of nodes = %d, time = %f seconds\n", 10 - n, (10 - n) * cnt, (double)(last - first) / CLOCKS_PER_SEC);
    }
    return 1;
}
int Del_Timing(Node **root) {
    int n = 10, cnt = 1000000, i, m;
    char key[10000][50]; 
    clock_t first, last;
    srand(time(NULL));
    
    while (n-- > 0) {
        for (i = 0; i < 10000; ++i) {
            sprintf(key[i], "key%d", rand()); 
        }
        
        for (i = 0; i < cnt; ) {
            int k = rand()*rand();
            if (insertNode(root, key[k % 10000], "value")) {
                ++i;
            }
        }
        m = 0;
        first = clock();
        for (i = 0; i < 10000; ++i) {
            if (deleteNode(root, key[i],0)){
                ++m;
            }
        }
        last = clock();
        printf("%d items were found\n", m);
        printf("Test #%d, number of nodes = %d, time = %f seconds\n", 10 - n, (10 - n) * cnt, (double)(last - first) / CLOCKS_PER_SEC);
    }
    return 1;
}
void loadTreeFromFile(struct Node** root, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    char line[100];
    char* key;
    char* info;
    int c = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
    	if (c % 2 == 0){
	        key = strtok(line, "\n");}
	    else{
	        info = strtok(line, "\n");
	        insertNode(root,key,info);}
	    c++;
    }
    fclose(file);
}
Node *build(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла %s\n", filename);
        return NULL;
    }

    Node *root = NULL;
    char word[128];
    int i = 0;

    while ((word[i] = fgetc(file)) != EOF) {
        if (isalnum(word[i])) { // Если символ буквенно-цифровой
            i++;
        } else if (i > 0) { // Если конец слова
            word[i] = '\0';
            for (int j = 0; word[j]; j++) {
                word[j] = tolower(word[j]);
            }
            insertNode(&root, word,"1"); 
            i = 0; 
        }
    }
    fclose(file);
    return root;
}

void writeS(const char *key, int number, const char *filename) {
    FILE *file = fopen(filename, "a"); // Открываем файл в режиме дозаписи ("a")
        fprintf(file, "%s — %d\n", key, number); 
        fclose(file);
}

void out(Node *root) {
    if (root != NULL) {
        out(root->right);
        Node *info = root->infoField;
        int c = 1;
        while (info != NULL) {
        	c++;
            info = info->infoField;
        }
        writeS(root->key,c,"out.txt");
        out(root->left);
    }
}
