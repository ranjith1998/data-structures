#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

// minimum key size is 3
#define KEYS_SIZE 15
// #define KEYS_SIZE 15
#define KEY_LEN 256
#define NULL 0
#define DATA_SIZE 3825

#include <algorithm>
#include <cstring>
#include <utility>
#include <iostream>

using namespace std;


struct DataNode
{
    int size;
    int pos;
    char fileName[KEY_LEN];
    
    unsigned char data[DATA_SIZE + 1];

    DataNode * nextDataNode;

    DataNode() : size(0), nextDataNode(NULL) {
        data[DATA_SIZE] = '\0';
        fileName[0] = '\0';
    }
};

struct Node
{
    int keySize;
    bool isLeaf;
    char keys[KEYS_SIZE][KEY_LEN];
    void * childrens[KEYS_SIZE + 1];
    Node ** nodeChildrens;
    DataNode ** dataChildrens;
    char padding[176];

    Node() : keySize(0), isLeaf(true) {
        memset(childrens, 0, sizeof(childrens));
        nodeChildrens = (Node **) childrens;
        dataChildrens = (DataNode **) childrens;
    }
};

class BPlusTree {
private:
    Node * root;
    void clear(Node * node);
    void clearData(DataNode * node);
    void inOrderPrint(Node * node);

    Node * insert(DataNode * data, Node * root);
    int height(Node * node);
public:
    BPlusTree();
    ~BPlusTree();
    void clear();

    void insert(unsigned char data[], int size, char fileName[]);
    void inOrderPrint();
    int height();
};


pair<Node *, Node *> mp(Node * a, Node * b) {
    return make_pair(a, b);
}

void cp(char src[], char dst[]) {
    copy(src, src + strlen(src), dst);
    dst[strlen(src)] = NULL;
}

int strcmpb(char a[], char b[]) {
    if (strlen(a) > strlen(b)) return 1;
    if (strlen(b) > strlen(a)) return -1;

    int i = 0;
    while (a[i] && a[i] == b[i]) i++;

    if (a[i] == b[i]) return 0;

    return a[i] > b[i] ? 1 : -1;
}

int strlen(char str[]) {
    int i = 0;
    while(str[i]) i++;
    return i;
}

char* gk(void * node, bool isLeaf) {
    if (isLeaf) return ((DataNode *) node)->fileName;
    
    return ((Node *) node)->keys[0];
}

DataNode * newDataNode(unsigned char data[], int size, char fileName[]) {
    DataNode * node = new DataNode();
    DataNode * curNode = node;
    unsigned char * curData = data;

    for (int i = 0; i < size; i += DATA_SIZE) {
        int incrementSize = DATA_SIZE;
        if (i + DATA_SIZE > size) incrementSize = size % DATA_SIZE;

        std::copy(curData, curData + incrementSize, curNode->data);
        
        cp(fileName, curNode->fileName);

        curNode->pos = i;
        curNode->size = incrementSize;

        if (i + DATA_SIZE < size) {
            curNode->nextDataNode = new DataNode();
            curNode = curNode->nextDataNode;

            curData += DATA_SIZE;
        }
    }

    curNode->data[size % (DATA_SIZE + 1)] = NULL;

    return node;
}

BPlusTree::BPlusTree() : root(NULL) {}

BPlusTree::~BPlusTree() {
    // clear();
}

void BPlusTree::clear() {
    clear(root);

    root = NULL;
}

void BPlusTree::clear(Node * node) {
    if (node == NULL) return;

    if (node->isLeaf)
        for (int i = 0; i <= node->keySize; i++)
            clearData((DataNode *) node->childrens[i]);
    else
        for (int i = 0; i <= node->keySize; i++)
            clear((Node *) node->childrens[i]);

    delete node;
}

void BPlusTree::clearData(DataNode * node) {
    if (node == NULL) return;

    if (node->nextDataNode != NULL) clearData(node->nextDataNode);

    delete node;
}

void BPlusTree::insert(unsigned char data[], int size, char fileName[]) {
    DataNode * dataNode = newDataNode(data, size, fileName);

    if (root == NULL) {
        root = new Node();
    }

    auto resp = insert(dataNode, root);
    if (resp) {
        root = resp;
    }
}

Node * BPlusTree::insert(DataNode * data, Node * node) {
    int i;
    for (i = 0; i < node->keySize && strcmpb(data->fileName, node->keys[i]) > 0; i++);

    if (node->isLeaf) {
        if (node->childrens[i]) {
            if (strcmpb(data->fileName, ((DataNode *) node->childrens[i])->fileName) <= 0) {
                for (int j = node->keySize - 1; j >= i; j--) {
                    cp(node->keys[j], node->keys[j + 1]);
                    node->childrens[j + 1] = node->childrens[j];
                }
                cp(data->fileName, node->keys[i]); 
                node->childrens[i] = data;
            } else {
                for (int j = node->keySize - 1; j > i; j--) {
                    cp(node->keys[j], node->keys[j + 1]);
                    node->childrens[j + 1] = node->childrens[j];
                }
                cp(((DataNode *) node->childrens[i])->fileName, node->keys[i]);
                node->childrens[i + 1] = data;
            }

            node->keySize++;
        } else {
            node->childrens[i] = data;
            if (node->keySize == 0) {
                cp(data->fileName, node->keys[0]);
                node->keySize++;
            }
        }
    } else {
        Node * newChildHead = insert(data, (Node *) node->childrens[i]);

        if (newChildHead) {
            for (int j = node->keySize - 1; j >= i; j++) {
                cp(node->keys[j], node->keys[j + 1]);
                node->childrens[j + 2] = node->childrens[j + 1];
            }

            cp(newChildHead->keys[0], node->keys[i]);
            node->childrens[i + 1] = newChildHead->childrens[1];
            node->keySize++;

            delete newChildHead;
        }
    }

    if (node->keySize == KEYS_SIZE) {
        Node * secondNode = new Node();
        secondNode->isLeaf = node->isLeaf;

        for (int j = KEYS_SIZE / 2 + 1; j < KEYS_SIZE; j++) {
            cp(node->keys[j], secondNode->keys[secondNode->keySize]);
            secondNode->childrens[secondNode->keySize++] = node->childrens[j];

            node->childrens[j] = NULL;
        }
        secondNode->childrens[secondNode->keySize] = node->childrens[KEYS_SIZE];
        node->childrens[KEYS_SIZE] = NULL;

        Node * newGrandParent = new Node();
        newGrandParent->childrens[0] = node;
        newGrandParent->childrens[1] = secondNode;
        cp(node->keys[KEYS_SIZE / 2], newGrandParent->keys[0]);
        newGrandParent->keySize = 1;
        newGrandParent->isLeaf = false;

        node->keySize = KEYS_SIZE / 2;

        return newGrandParent;
    }

    return NULL;
}

void BPlusTree::inOrderPrint() {
    inOrderPrint(root);
}

void BPlusTree::inOrderPrint(Node * node) {
    if (node == NULL) return;

    if (node->isLeaf) {
        for (int i = 0; i < node->keySize; i++) {
            cout << "data :: " << ((DataNode *) node->childrens[i])->fileName << "\n";
            cout << "key :: " << node->keys[i] << "\n";
        }
        if (node->childrens[node->keySize] != NULL) cout << "data :: " << ((DataNode *) node->childrens[node->keySize])->fileName << "\n";
    } else {
        for (int i = 0; i < node->keySize; i++) {
            inOrderPrint((Node *) node->childrens[i]);
            cout << "key :: " << node->keys[i] << "\n";
        }
        inOrderPrint((Node *) node->childrens[node->keySize]);
    }
}

int BPlusTree::height() {
    return height(root);
}

int BPlusTree::height(Node * node) {
    if (node == NULL) 0;

    if (node->isLeaf) {
        return 1;
    } 
    
    int maxDepth = 0;
    for (int i = 0; i <= node->keySize; i++) {
        maxDepth = max(maxDepth, height((Node *) node->childrens[i]));
    }

    return maxDepth + 1;
}

#endif
