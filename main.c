#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define M 3

typedef struct Node {
    int numberCases; /* count of cases in one node */
    char *keys[M - 1]; /*array of keys*/
    char *info[M - 1];
    struct Node *pointers[M]; /* (numberCases+1 pointers will be in use) */
    int release[M - 1];
} Node;

typedef struct Tree {
    Node *root;
} Tree;

typedef enum keyStatus {
    SearchFailure, Success, InsertIt, LessKeys
} keyStatus;

int countOfRelease(Tree *tree, Node *ptr, char *key);

char *create_str() {
    int len_str = 0;
    char *stdin_str = malloc(len_str + 2);
    char symbol = ' ';
    while (symbol != '\n') {
        stdin_str = realloc(stdin_str, len_str + 2);
        symbol = getchar();
        if (symbol == EOF) {
            printf("End of working.\n");
            free(stdin_str);
            return NULL;
        } else {
            stdin_str[len_str] = symbol;
            len_str++;
        }
    }
    stdin_str[len_str - 1] = '\0';
    return stdin_str;
}

int get_int(int *number) {
    int n, f = -10;
    char symbol;
    do {
        f = 0;
        n = scanf("%u", number);
        if (n < 0) {
            return 0;
        } else if (n == 0) {
            printf("Error! Repeat input, please:\n");
            scanf("%*[^\n]");
        } else if (*number <= 0) {
            printf("Error! Repeat input, please:\n");
            scanf("%*[^\n]");
        } else if (n == 1) {
            f = scanf("%c", &symbol);
            while (symbol == ' ' || symbol == '\t') {
                f = scanf("%c", &symbol);
            }
            if (symbol != '\n') {
                scanf("%*[^\n]");
                printf("Error! Repeat input, please:\n");
            }
        }
    } while (n == 0 || *number <= 0 || symbol != '\n');
    return 1;
}

int get_int2(int *number) {
    int n, f = -10;
    char symbol;
    do {
        f = 0;
        n = scanf("%u", number);
        if (n < 0) {
            return 0;
        } else if (n == 0) {
            printf("Error! Repeat input, please:\n");
            scanf("%*[^\n]");
        } else if (*number < 0) {
            printf("Error! Repeat input, please:\n");
            scanf("%*[^\n]");
        } else if (n == 1) {
            f = scanf("%c", &symbol);
            while (symbol == ' ' || symbol == '\t') {
                f = scanf("%c", &symbol);
            }
            if (symbol != '\n') {
                scanf("%*[^\n]");
                printf("Error! Repeat input, please:\n");
            }
        }
    } while (n == 0 || *number < 0 || symbol != '\n');
    return 1;
}

int searchPos(char *key, char **key_arr, int n) {
    int pos = 0;
    while (pos < n && strcmp(key, key_arr[pos]) > 0) //////////////////////// ВОЗМОЖНО НАДО ПОСТАВИТЬ >=
        pos++;
    return pos;
}


keyStatus
insertKey(Tree *tree, Node *root, char *key, char *info, char **upKey, char **upInfo, Node **newNode, int release,
          int *upRelease) {
    if (!root) {
        *newNode = NULL;
        *upKey = key;
        *upInfo = info;
        *upRelease = release;
        return InsertIt;
    }
    int numberCases = root->numberCases;
    int position = searchPos(key, root->keys, numberCases);
    char *newKey, *newInfo, *lastKey, *lastInfo;
    int newRelease, lastRelease;
    Node *newRoot, *lastRoot;

    keyStatus value = insertKey(tree, root->pointers[position], key, info, &newKey, &newInfo, &newRoot, release,
                                &newRelease);


    if (value != InsertIt) {
        return value;
    }
    if (numberCases < M - 1) {
        position = searchPos(newKey, root->keys, numberCases);
        for (int i = numberCases; i > position; --i) {
            root->keys[i] = root->keys[i - 1];
            root->info[i] = root->info[i - 1];
            root->release[i] = root->release[i - 1];
            root->pointers[i + 1] = root->pointers[i];
        }
        root->keys[position] = newKey;
        root->info[position] = newInfo;
        root->release[position] = newRelease;
        root->pointers[position + 1] = newRoot;
//        root->pointers[2] = NULL;
        ++(root->numberCases);
//        root->next = NULL;
        return Success;
    }
    if (position == M - 1) {
        lastKey = newKey;
        lastInfo = newInfo;
        lastRelease = newRelease;
        lastRoot = newRoot;
    } else {
        lastKey = root->keys[M - 2];
        lastInfo = root->info[M - 2];
        lastRelease = root->release[M - 2];
        lastRoot = root->pointers[M - 1];
        for (int i = M - 2; i > position; --i) {
            root->keys[i] = root->keys[i - 1];
            root->info[i] = root->info[i - 1];
            root->release[i] = root->release[i - 1];
            root->pointers[i + 1] = root->pointers[i];
        }
        root->keys[position] = newKey;
        root->info[position] = newInfo;
        root->release[position] = newRelease;
        root->pointers[position + 1] = newRoot;
    }
    int splitPos = (M - 1) / 2;
    (*upKey) = root->keys[splitPos];
    (*upInfo) = root->info[splitPos];
    (*upRelease) = root->release[splitPos];
    root->keys[splitPos] = NULL;
    root->info[splitPos] = NULL;
    root->release[splitPos] = -1;

    (*newNode) = malloc(sizeof(Node));/*Right node after split*/
    root->numberCases = splitPos; /*No. of keys for left split node*/
    (*newNode)->numberCases = M - 1 - splitPos;/*No. of keys for right split node*/
    for (int i = 0; i < (*newNode)->numberCases; i++) {
        (*newNode)->pointers[i] = root->pointers[i + splitPos + 1];
        root->pointers[i + splitPos + 1] = NULL;
        if (i < (*newNode)->numberCases - 1) {
            (*newNode)->keys[i] = root->keys[i + splitPos + 1];
            (*newNode)->info[i] = root->info[i + splitPos + 1];
            (*newNode)->release[i] = root->release[i + splitPos + 1];
        } else {
            (*newNode)->keys[i] = lastKey;
            (*newNode)->info[i] = lastInfo;
            (*newNode)->release[i] = lastRelease;
            (*newNode)->keys[1] = NULL;
            (*newNode)->info[1] = NULL;
            (*newNode)->release[1] = -1;
        }
    }
    (*newNode)->pointers[(*newNode)->numberCases] = lastRoot;
    (*newNode)->pointers[2] = NULL;
    return InsertIt;
}

void insert(Tree *tree, char *key, char *info) {
    Node *newNode;
    char *upKey, *upInfo;
    int release = countOfRelease(tree, tree->root, key) + 1;
    int upRelease;
    keyStatus value = insertKey(tree, tree->root, key, info, &upKey, &upInfo, &newNode, release, &upRelease);
    if (value == InsertIt) {
        Node *upRoot = tree->root;
        tree->root = malloc(sizeof(Node));
        tree->root->numberCases = 1;
        tree->root->keys[0] = upKey;
        tree->root->info[0] = upInfo;
        tree->root->release[0] = upRelease;
        tree->root->keys[1] = NULL;
        tree->root->info[1] = NULL;
        tree->root->release[1] = -1;
        tree->root->pointers[0] = upRoot;
        tree->root->pointers[1] = newNode;
        tree->root->pointers[2] = NULL;
    }
}

int dialogueInsert(Tree *tree) {
    printf("Enter the key:\n");
    char *key = create_str();
    if (!key) {
        free(key);
        return 2;
    }
    printf("Enter the information:\n");
    char *info = create_str();
    if (!info) {
        free(key);
        free(info);
        return 2;
    }
    insert(tree, key, info);
    return 0;
}

void display(Node *ptr, int blanks) {
    if (ptr) {
        int i;
        for (i = 1; i <= blanks; i++)
            printf(" ");
        for (i = 0; i < ptr->numberCases; i++)
            printf("%s ", ptr->keys[i]);
        printf("\n");
        for (i = 0; i <= ptr->numberCases; i++)
            display(ptr->pointers[i], blanks + 10);
    }/*End of if*/
}/*End of display()*/

int dialogueDisplay(Tree *tree) {
    display(tree->root, 0);
    return 0;
}

void inorder(Node *ptr) {
    if (ptr) {
        if (ptr->numberCases >= 1) {
            inorder(ptr->pointers[0]);
            printf("%s ", ptr->keys[0]);
            inorder(ptr->pointers[1]);
            if (ptr->numberCases == 2) {
                printf("%s ", ptr->keys[1]);
                inorder(ptr->pointers[2]);
            }
        }
    }
}

void fixRelease2(Node *ptr, char *key, int release) {
    if (ptr) {
        if (ptr->numberCases >= 1) {
            fixRelease2(ptr->pointers[0], key, release);
            if (strcmp(ptr->keys[0], key) == 0 && ptr->release[0] > release) {
                ptr->release[0]--;
            }
            fixRelease2(ptr->pointers[1], key, release);
            if (ptr->numberCases == 2) {
                if (strcmp(ptr->keys[1], key) == 0 && ptr->release[1] > release) {
                    ptr->release[1]--;
                }
                fixRelease2(ptr->pointers[2], key, release);
            }
        }
    }
}

void fixRelease(Node *ptr, char *key, int release) {
    while (ptr) {
        int position = searchPos(key, ptr->keys, ptr->numberCases);
        if (ptr->keys[position] && strcmp(ptr->keys[position], key) == 0 && ptr->release[position] > release) {
            ptr->release[position]--;
            fixRelease(ptr->pointers[position + 1], key, release);
        }
        if (ptr->keys[position + 1] && strcmp(ptr->keys[position + 1], key) == 0 &&
            ptr->release[position + 1] > release) {
            ptr->release[position + 1]--;
            fixRelease(ptr->pointers[position + 2], key, release);
        }
        ptr = ptr->pointers[position];
    }
}

int countOfRelease(Tree *tree, Node *ptr, char *key) {
    int maxRelease = 0;
    while (ptr) {
        int position = searchPos(key, ptr->keys, ptr->numberCases);
        if (ptr->keys[position] && strcmp(ptr->keys[position], key) == 0 && ptr->release[position] > maxRelease) {
            maxRelease = ptr->release[position];
        }
        ptr = ptr->pointers[position];
    }
    return maxRelease;
}


void detour(Node *ptr, char *firstBound, char *secondBound) {
    if (ptr) {
        if (ptr->numberCases >= 1) {
            if (ptr->numberCases == 2) {
                detour(ptr->pointers[2], firstBound, secondBound);
                if (strcmp(ptr->keys[1], firstBound) < 0 || strcmp(ptr->keys[1], secondBound) > 0)
                    printf("%s ", ptr->keys[1]);
            }
            detour(ptr->pointers[1], firstBound, secondBound);
            if (strcmp(ptr->keys[0], firstBound) < 0 || strcmp(ptr->keys[0], secondBound) > 0)
                printf("%s ", ptr->keys[0]);
            detour(ptr->pointers[0], firstBound, secondBound);
        }
    }
}

int dialogueDetour(Tree *tree) {
    printf("Enter first bound:\n");
    char *firstBound = create_str();
    if (!firstBound) {
        free(firstBound);
        return 2;
    }
    printf("Enter second bound:\n");
    char *secondBound = create_str();
    if (!secondBound) {
        free(firstBound);
        free(secondBound);
        return 2;
    }
    detour(tree->root, firstBound, secondBound);
    printf("\n");
    free(firstBound);
    free(secondBound);
    return 0;
}


keyStatus delNode(Tree *tree, Node *root, char *key, int release) {
    int position, pivot, min;
    keyStatus value;
    Node *leftPtr, *rightPtr;

    if (!root)
        return SearchFailure;

    int numberCases = root->numberCases;
    char **arrayKeys = root->keys;
    char **arrayInfo = root->info;
    Node **pointers = root->pointers;
    min = (M - 1) / 2;
    position = searchPos(key, arrayKeys, numberCases);

    if (numberCases == 1 && position == 0 && strcmp(arrayKeys[position], key) == 0 &&
        root->release[position] > release ||
        numberCases == 2 && position == 0 && strcmp(arrayKeys[position], key) == 0 &&
        strcmp(arrayKeys[position + 1], key) == 0 && root->release[position] > release &&
        root->release[position + 1] < release ||
        numberCases == 2 && position == 1 && strcmp(arrayKeys[position], key) == 0 &&
        root->release[position] > release ||
        numberCases == 2 && position == 0 && strcmp(arrayKeys[position], key) == 0 &&
        strcmp(arrayKeys[position + 1], key) != 0 && root->release[position] > release ||
        numberCases == 2 && position == 0 && strcmp(arrayKeys[position], key) == 0 &&
        strcmp(arrayKeys[position + 1], key) == 0 && root->release[position + 1] >= release &&
        pointers[0] == NULL) {
        position++;
    } else if (numberCases == 2 && position == 0 && strcmp(arrayKeys[position], key) == 0 &&
               strcmp(arrayKeys[position + 1], key) == 0 && root->release[position + 1] > release) {
        position += 2;
    }

    if (pointers[0] == NULL) {
        if (position == numberCases || strcmp(key, arrayKeys[position]) < 0) {
            return SearchFailure;
        }
        for (int i = position + 1; i < numberCases; ++i) {
            free(arrayKeys[i - 1]);           ///вообще это случай, когда удаляем из двойной ноды в листе что-то
            free(arrayInfo[i - 1]);
            arrayKeys[i -
                      1] = arrayKeys[i]; ////// если я правильно понял, то тут сначала надо фришнуть данные (сделал)
            arrayInfo[i - 1] = arrayInfo[i];
            root->release[i - 1] = root->release[i];
            arrayKeys[i] = NULL;
            arrayInfo[i] = NULL;
            root->release[i] = -1;
            pointers[i] = pointers[i + 1]; //не понимаю, нахера это нужно, если тут null'ы везде по идее
        }
        if (position == 1) {
            free(arrayKeys[position]);
            free(arrayInfo[position]);
            arrayKeys[position] = NULL;
            arrayInfo[position] = NULL;
            root->release[position] = -1;
        }
        return --root->numberCases >= (root == tree->root ? 1 : min) ? Success : LessKeys;
    }
    if (position < numberCases && strcmp(key, arrayKeys[position]) == 0 && release == root->release[position]) {
        Node *qp = pointers[position], *qp1;
        int nkey;
        while (1) {
            nkey = qp->numberCases;
            qp1 = qp->pointers[nkey];
            if (qp1 == NULL)
                break;
            qp = qp1;
        }/*End of while*/
        char *strKey = arrayKeys[position];
        char *strInfo = arrayInfo[position];
        int helpRelease = root->release[position];
        arrayKeys[position] = qp->keys[nkey - 1];
        arrayInfo[position] = qp->info[nkey - 1];
        root->release[position] = qp->release[nkey - 1];
        qp->keys[nkey - 1] = strKey;
        qp->info[nkey - 1] = strInfo;
        qp->release[nkey - 1] = helpRelease;
//        *flag = 1;
    }/*End of if */
    value = delNode(tree, pointers[position], key, release);
    if (value != LessKeys)
        return value;

    if (position > 0 && pointers[position - 1]->numberCases > min) {
        pivot = position - 1; /*pivot for left and right node*/
        leftPtr = pointers[pivot];
        rightPtr = pointers[position];
        /*Assigns values for right node*/

        rightPtr->pointers[rightPtr->numberCases + 1] = rightPtr->pointers[rightPtr->numberCases];
        rightPtr->pointers[rightPtr->numberCases] = NULL;
        for (int i = rightPtr->numberCases; i > 0; i--) {
            rightPtr->keys[i] = strdup(rightPtr->keys[i - 1]);
            free(rightPtr->keys[i - 1]);
            rightPtr->keys[i - 1] = NULL;
            rightPtr->info[i] = strdup(rightPtr->info[i - 1]);
            free(rightPtr->info[i - 1]);
            rightPtr->info[i - 1] = NULL;
            rightPtr->release[i] = rightPtr->release[i - 1];
            rightPtr->release[i - 1] = -1;
            rightPtr->pointers[i] = rightPtr->pointers[i - 1];
            rightPtr->pointers[i - 1] = NULL;
        }
        rightPtr->numberCases++;
        if (rightPtr->keys[0]) {
            free(rightPtr->keys[0]);
            rightPtr->keys[0] = NULL;
        }
        if (rightPtr->info[0]) {
            free(rightPtr->info[0]);
            rightPtr->info[0] = NULL;
        }
        rightPtr->keys[0] = strdup(arrayKeys[pivot]);
        rightPtr->info[0] = strdup(arrayInfo[pivot]);
        rightPtr->release[0] = root->release[pivot];
        free(arrayKeys[pivot]);
        free(arrayInfo[pivot]);
        rightPtr->pointers[0] = leftPtr->pointers[leftPtr->numberCases];
        leftPtr->pointers[leftPtr->numberCases] = NULL;

        arrayKeys[pivot] = strdup(leftPtr->keys[--leftPtr->numberCases]);
        arrayInfo[pivot] = strdup(leftPtr->info[leftPtr->numberCases]);
        root->release[pivot] = leftPtr->release[leftPtr->numberCases];
        free(leftPtr->keys[leftPtr->numberCases]);
        free(leftPtr->info[leftPtr->numberCases]);
        leftPtr->keys[leftPtr->numberCases] = NULL;
        leftPtr->info[leftPtr->numberCases] = NULL;
        leftPtr->release[leftPtr->numberCases] = -1;
        return Success;
    }/*End of if */
//if (posn > min)
    if (position < numberCases && pointers[position + 1]->numberCases > min) {
        pivot = position; /*pivot for left and right node*/
        leftPtr = pointers[pivot];
        rightPtr = pointers[pivot + 1];

        /*Assigns values for left node*/
        free(leftPtr->keys[leftPtr->numberCases]);
        free(leftPtr->info[leftPtr->numberCases]);
        leftPtr->keys[leftPtr->numberCases] = strdup(arrayKeys[pivot]);
        leftPtr->info[leftPtr->numberCases] = strdup(arrayInfo[pivot]);
        leftPtr->release[leftPtr->numberCases] = root->release[pivot];
        leftPtr->pointers[leftPtr->numberCases + 1] = rightPtr->pointers[0];
        rightPtr->pointers[0] = NULL;

        free(arrayKeys[pivot]);
        free(arrayInfo[pivot]);
        arrayKeys[pivot] = strdup(rightPtr->keys[0]);
        arrayInfo[pivot] = strdup(rightPtr->info[0]);
        root->release[pivot] = rightPtr->release[0];

        leftPtr->numberCases++;
        rightPtr->numberCases--;
        for (int i = 0; i < rightPtr->numberCases; i++) {
            free(rightPtr->keys[i]);
            free(rightPtr->info[i]);
            rightPtr->keys[i] = strdup(rightPtr->keys[i + 1]);
            rightPtr->info[i] = strdup(rightPtr->info[i + 1]);
            rightPtr->release[i] = rightPtr->release[i + 1];
            free(rightPtr->keys[i + 1]);
            rightPtr->keys[i + 1] = NULL;
            free(rightPtr->info[i + 1]);
            rightPtr->info[i + 1] = NULL;
            rightPtr->release[i + 1] = -1;
            rightPtr->pointers[i] = rightPtr->pointers[i + 1];
            rightPtr->pointers[i + 1] = NULL;
        }/*End of for*/
        rightPtr->pointers[rightPtr->numberCases] = rightPtr->pointers[rightPtr->numberCases + 1];
        rightPtr->pointers[rightPtr->numberCases + 1] = NULL;
        return Success;
    }/*End of if */

    if (position == numberCases)
        pivot = position - 1;
    else
        pivot = position;

    leftPtr = pointers[pivot];
    rightPtr = pointers[pivot + 1];
    free(leftPtr->keys[leftPtr->numberCases]);
    free(leftPtr->info[leftPtr->numberCases]);
    leftPtr->keys[leftPtr->numberCases] = strdup(arrayKeys[pivot]);
    leftPtr->info[leftPtr->numberCases] = strdup(arrayInfo[pivot]);
    leftPtr->release[leftPtr->numberCases] = root->release[pivot];
    free(arrayKeys[pivot]);
    free(arrayInfo[pivot]);
    arrayKeys[pivot] = NULL;
    arrayInfo[pivot] = NULL;
    root->release[pivot] = -1;
    leftPtr->pointers[leftPtr->numberCases + 1] = rightPtr->pointers[0];
    rightPtr->pointers[0] = NULL;
    for (int i = 0; i < rightPtr->numberCases; i++) {
        free(leftPtr->keys[leftPtr->numberCases + 1 + i]);
        free(leftPtr->info[leftPtr->numberCases + 1 + i]);
        leftPtr->keys[leftPtr->numberCases + 1 + i] = strdup(rightPtr->keys[i]);
        leftPtr->info[leftPtr->numberCases + 1 + i] = strdup(rightPtr->info[i]);
        leftPtr->release[leftPtr->numberCases + 1 + i] = rightPtr->release[i];
        free(rightPtr->info[i]);
        free(rightPtr->keys[i]);
        rightPtr->info[i] = NULL;
        rightPtr->keys[i] = NULL;
        rightPtr->release[i] = -1;
        leftPtr->pointers[leftPtr->numberCases + 2 + i] = rightPtr->pointers[i + 1];
        rightPtr->pointers[i + 1] = NULL;
    }
    leftPtr->numberCases = leftPtr->numberCases + rightPtr->numberCases + 1;
    if (rightPtr->keys[0]) {
        free(rightPtr->keys[0]);
        free(rightPtr->info[0]);
        rightPtr->keys[0] = NULL;
        rightPtr->info[0] = NULL;
        rightPtr->release[0] = -1;
    }
    free(rightPtr); /*Remove right node*/
    pointers[pivot + 1] = NULL;
    for (int i = position + 1; i < numberCases; i++) {
        free(arrayKeys[i - 1]);
        free(arrayInfo[i - 1]);
        arrayKeys[i - 1] = strdup(arrayKeys[i]);
        arrayInfo[i - 1] = strdup(arrayInfo[i]);
        root->release[i - 1] = root->release[i];
        free(arrayKeys[i]);
        free(arrayInfo[i]);
        arrayKeys[i] = NULL;
        arrayInfo[i] = NULL;
        root->release[i] = -1;
        pointers[i] = pointers[i + 1];
        pointers[i + 1] = NULL;
    }

    return --root->numberCases >= (root == tree->root ? 1 : min) ? Success : LessKeys;
}

void deleteNode(Tree *tree, char *key, int release) {
    Node *upRoot;
//    int flag = 0;
    keyStatus value = delNode(tree, tree->root, key, release);
    switch (value) {
        case SearchFailure:
//            printf("Key %s isn't available. Try again:\n", key);
            break;
        case LessKeys:
            upRoot = tree->root;
            tree->root = tree->root->pointers[0];
            free(upRoot);
            break;
    }
}

int dialogueDelete(Tree *tree) {
    printf("Enter the key you would like to delete:\n");
    char *deleteKey = create_str();
    if (!deleteKey) {
        free(deleteKey);
        return 2;
    }
    printf("Enter the release:\n");
    int release;
    if (get_int(&release) != 1) {
        printf("Error! Restart the program, please.\n");
        free(deleteKey);
        return 2;
    }
    int maxRelease = countOfRelease(tree, tree->root, deleteKey);
    if (release > maxRelease) {
        printf("You entered too big release. Try again.\n");
        free(deleteKey);
        return 1;
    }
    deleteNode(tree, deleteKey, release);
    fixRelease(tree->root, deleteKey, release);
    free(deleteKey);
    return 0;
}

Node *specialFind(Node *ptr) {
    if (ptr) {
        while (ptr->pointers[0]) {
            ptr = ptr->pointers[0];
        }
        printf("Key: %s\tInfo: %s\n", ptr->keys[0], ptr->info[0]);
        return ptr;
    } else {
        return NULL;
    }
}

int dialogueSpecialFind(Tree *tree) {
    if (!specialFind(tree->root)) {
        printf("Tree is empty.\n");
        return 2;
    }
    return 0;
}

Node *find(Node *root, char *key, int release) {
    while (root) {
        int position = searchPos(key, root->keys, root->numberCases);
        int numberCases = root->numberCases;
        char **arrayKeys = root->keys;
        char **arrayInfo = root->info;
        Node **pointers = root->pointers;
        if (numberCases == 1 && position == 0 && strcmp(arrayKeys[position], key) == 0 &&
            root->release[position] > release ||
            numberCases == 2 && position == 0 && strcmp(arrayKeys[position], key) == 0 &&
            strcmp(arrayKeys[position + 1], key) == 0 && root->release[position] > release &&
            root->release[position + 1] < release ||
            numberCases == 2 && position == 1 && strcmp(arrayKeys[position], key) == 0 &&
            root->release[position] > release ||
            numberCases == 2 && position == 0 && strcmp(arrayKeys[position], key) == 0 &&
            strcmp(arrayKeys[position + 1], key) != 0 && root->release[position] > release ||
            numberCases == 2 && position == 0 && strcmp(arrayKeys[position], key) == 0 &&
            strcmp(arrayKeys[position + 1], key) == 0 && root->release[position + 1] >= release &&
            pointers[0] == NULL) {
            position++;
            if (pointers[0] == NULL) {
                if (root->keys[position] && strcmp(root->keys[position], key) == 0 &&
                    root->release[position] == release) {
                    printf("Key: %s\tInfo: %s\tRelease: %d\n", root->keys[position], root->info[position],
                           root->release[position]);
                    return root;
                }
            }
        } else if (numberCases == 2 && position == 0 && strcmp(arrayKeys[position], key) == 0 &&
                   strcmp(arrayKeys[position + 1], key) == 0 && root->release[position + 1] > release) {
            position += 2;
        } else if (root->keys[position] && strcmp(root->keys[position], key) == 0 &&
                   root->release[position] == release) {
            printf("Key: %s\tInfo: %s\tRelease: %d\n", root->keys[position], root->info[position],
                   root->release[position]);
            return root;
        }
        root = root->pointers[position];
    }
    return NULL;
}

int dialogueFind(Tree *tree) {
    printf("Enter the key, would you like to find:\n");
    char *key = create_str();
    if (!key) {
        free(key);
        return 2;
    }
    printf("Enter the release:\n");
    int release;
    if (get_int(&release) != 1) {
        printf("Error! Restart the program, please.\n");
        free(key);
        return 2;
    }
    int maxRelease = countOfRelease(tree, tree->root, key);
    if (release > maxRelease) {
        printf("You entered too big release or tree does not contain this key. Try again.\n");
        free(key);
        return 1;
    }
    if (find(tree->root, key, release) == NULL) {
        printf("Tree does not contain this key.\n");
        free(key);
        return 1;
    }
    free(key);
    return 0;
}

void freeTree(Node *ptr) {
    if (ptr) {
        freeTree(ptr->pointers[0]);
        if (ptr->keys[0])
            free(ptr->keys[0]);
        if (ptr->keys[1])
            free(ptr->keys[1]);
        if (ptr->info[0])
            free(ptr->info[0]);
        if (ptr->info[1])
            free(ptr->info[1]);
        ptr->keys[0] = NULL;
        ptr->keys[1] = NULL;
        ptr->info[0] = NULL;
        ptr->info[1] = NULL;


        freeTree(ptr->pointers[1]);
        if (ptr->keys[0])
            free(ptr->keys[0]);
        if (ptr->keys[1])
            free(ptr->keys[1]);
        if (ptr->info[0])
            free(ptr->info[0]);
        if (ptr->info[1])
            free(ptr->info[1]);
        ptr->keys[0] = NULL;
        ptr->keys[1] = NULL;
        ptr->info[0] = NULL;
        ptr->info[1] = NULL;

        freeTree(ptr->pointers[2]);
        if (ptr->keys[0])
            free(ptr->keys[0]);
        if (ptr->keys[1])
            free(ptr->keys[1]);
        if (ptr->info[0])
            free(ptr->info[0]);
        if (ptr->info[1])
            free(ptr->info[1]);
        ptr->keys[0] = NULL;
        ptr->keys[1] = NULL;
        ptr->info[0] = NULL;
        ptr->info[1] = NULL;

        free(ptr);
        ptr = NULL;
    }
}

int dialogueFreeTree(Tree *tree) {
    freeTree(tree->root);
    return 0;
}

int readFile(Tree *tree, FILE *start, char *st) {
    char buffer[256];
    if (start) {
        int i = 0;
        char *key, *info;
        while ((fgets(buffer, 256, start)) != NULL) {
            if (i % 2 == 0) {
                key = strdup(buffer);
                key[strlen(key) - 1] = '\0';
            } else {
                info = strdup(buffer);
                info[strlen(info) - 1] = '\0';
                insert(tree, key, info);
            }
            i++;
        }
        fclose(start);
    }
    free(st);
    return 0;
}

int dialogueReadFile(Tree *tree) {
    FILE *start;
    printf("Enter file name:\n");
    char *st = create_str();
    start = fopen(st, "r");
    if (start == NULL) {
        printf("No such file exists.\n");
        free(st);
        return 1;
    }
    readFile(tree, start, st);
    return 0;
}

int dialogueTiming(Tree *) {
    int count = 10000;
    double sr = 0;
    for (int i = 0; i < 15; ++i) {
//        for (int j = 0; j < 5; ++j) {
        sr = 0;
        char **key = malloc(sizeof(char *) * count);
        char **info = malloc(sizeof(char *) * count);
        clock_t start, end;
        srand(time(NULL));
        Tree tree;
        tree.root = NULL;
        for (int k = 0; k < count; ++k) {
            key[k] = malloc(sizeof(char) * 10);
            for (int l = 0; l < 9; ++l) {
                key[k][l] = rand() % 26 + 'A';
            }
            key[k][9] = '\0';
            info[k] = malloc(sizeof(char) * 10);
            for (int l = 0; l < 9; ++l) {
                info[k][l] = rand() % 26 + 'A';
            }
            info[k][9] = '\0';
            insert(&tree, key[k], info[k]);
        }

        int release[5000];
        for (int j = 0; j < 5000; ++j) {
            release[j] = rand() * rand() % 3;
        }

        clock_t sum = 0;

        char **deleteKey = malloc(sizeof(char *) * 5000);
//        char **deleteInfo = malloc(sizeof(char *) * 5000);
        for (int j = 0; j < 5000; ++j) {
            deleteKey[j] = malloc(sizeof(char) * 10);
//            deleteInfo[j] = malloc(sizeof(char) * 10);
            for (int l = 0; l < 9; ++l) {
                deleteKey[j][l] = rand() % 26 + 'A';
//                deleteInfo[j][l] = rand()%26 + 'A';
            }
            deleteKey[j][9] = '\0';
//            deleteInfo[j][9] = '\0';
        }

        start = clock();
        for (int k = 0; k < 5000; ++k) {
            find(tree.root, deleteKey[k], release[k]);
        }
        end = clock();

        sum = end - start;
        sr = (double) sum / CLOCKS_PER_SEC;
        for (int j = 0; j < 5000; ++j) {
            free(deleteKey[j]);
        }
        free(deleteKey);
//        free(deleteInfo);
        free(key);
        free(info);
        freeTree(tree.root);
//        }
        printf("%d\tsrednee:%lf\n", count, 1000000 * (sr));
        count += 10000;
    }
    return 0;
}


int dialog(const char *msgs[], int len_msg) {
    int rc;
    for (int i = 0; i < len_msg; ++i) {
        puts(msgs[i]);
    }
    do {
        if (get_int2(&rc) != 1) {
            printf("Error! Restart program, please.\n");
            return -1;
        }
    } while (rc < 0 || rc > len_msg - 1);
    printf("%d\n", rc);
    return rc;
}

//void createDotFile(Node *ptr, FILE *file) {
//    if (ptr) {
//        if (ptr->numberCases >= 1) {
//            createDotFile(ptr->pointers[0], file);
//            if (ptr->pointers[0])
//                fprintf(file, "    %s -> %s [color=\"red\", arrowhead=\"vee\"];\n", ptr->keys[0], ptr->pointers[0]->keys[0]);
//            createDotFile(ptr->pointers[1], file);
//            if (ptr->numberCases == 2) {
//                if (ptr->pointers[1])
//                    fprintf(file, "    %s -> %s [color=\"red\", arrowhead=\"vee\"];\n", ptr->keys[1], ptr->pointers[1]->keys[0]);
//                createDotFile(ptr->pointers[2], file);
//            }
//        }
//    }
//}
//
//int generateDotFile(Node *root, const char *filename) {
//    FILE *file = fopen(filename, "w");
//    if (file == NULL) {
//        printf("Ошибка при открытии файла.\n");
//        return 1;
//    }
//    fprintf(file, "digraph BinaryTree {\n");
//    fprintf(file, "    node [shape=circle, style=filled, color=lightblue, fontcolor=black];\n");
//    createDotFile(root, file);
//    fprintf(file, "}\n");
//    fclose(file);
//    printf("File done.\n");
//    return 0;
//}

int main() {
    const char *msg[] = {"0. Quit",
                         "1. Add element in the tree",
                         "2. Remove element in the tree",
                         "3. Show element by a key in the tree",
                         "4. Show tree like a tree",
                         "5. Do detour",
                         "6. Do special find",
                         "7. Read by a file",
                         "8. Do timing"};
    const int len_msg = sizeof(msg) / sizeof(msg[0]);
    Tree tree;
    tree.root = NULL;
    int (*fptr[])(Tree *) = {NULL, dialogueInsert, dialogueDelete, dialogueFind, dialogueDisplay, dialogueDetour,
                             dialogueSpecialFind, dialogueReadFile, dialogueTiming};
    int rc = 142412;
    while (rc != 0) {
        if ((rc = dialog(msg, len_msg)) == -1) {
            break;
        }
        if (rc == 0) {
            break;
        }
        if (fptr[rc](&tree) == 2) {
            break;
        }
    }
//    generateDotFile(tree.root, "image.gv");
    printf("That's all. Bye!\n");
    dialogueFreeTree(&tree);
    return 0;
}
