#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

// minimum key size is 3
#define KEYS_SIZE 15
// #define KEYS_SIZE 15
#define KEY_LEN 256
#define NULL 0
#define DATA_SIZE 3821

#define PAGE_SIZE 4096

#include <algorithm>
#include <cstring>
#include <utility>
#include <iostream>
#include <cassert>
#include <cstdio>

using namespace std;

FILE *db_file;

struct MetadataPage {
    int total_files;
    int next_free_page;

    int root_page;

    MetadataPage(): total_files(0), next_free_page(0), root_page(-1) {}
};

MetadataPage meta;

void read_page(int page_number, void *buffer) {
    long offset = static_cast<long>(page_number) * PAGE_SIZE;
    if (fseek(db_file, offset, SEEK_SET) != 0) {
        perror("read_page: fseek failed");
        assert(false && "Failed to seek to the requested page");
    }
    size_t bytesRead = fread(buffer, 1, PAGE_SIZE, db_file);
    if (bytesRead != PAGE_SIZE) {
        if (feof(db_file)) {
            // Fallback: zero-fill buffer if EOF reached (empty page)
            memset(buffer, 0, PAGE_SIZE);
            clearerr(db_file);
        } else if (ferror(db_file)) {
            perror("read_page: fread failed");
            assert(false && "Failed to read the full page");
        }
        // otherwise partial read but no error flag: still fallback
        if (bytesRead < PAGE_SIZE) {
            memset(static_cast<char*>(buffer) + bytesRead, 0, PAGE_SIZE - bytesRead);
        }
    }
}

void write_page(int page_number, const void *buffer) {
    long offset = static_cast<long>(page_number) * PAGE_SIZE;
    if (fseek(db_file, offset, SEEK_SET) != 0) {
        perror("write_page: fseek failed");
        assert(false);
    }
    if (fwrite(buffer, 1, PAGE_SIZE, db_file) != PAGE_SIZE) {
        perror("write_page: fwrite failed");
        assert(false);
    }
    fflush(db_file);
}

void init_db(const char *filename) {
    db_file = fopen(filename, "r+b");
    if (!db_file) {
        // Create new DB file
        db_file = fopen(filename, "w+b");
        memset(&meta, 0, sizeof(MetadataPage));
        meta.next_free_page = 1; // Page 0 is reserved for metadata
        write_page(0, &meta);
    } else {
        // Load existing metadata
        read_page(0, &meta);
    }
}

int new_page() {
    int allocated = meta.next_free_page++;
    write_page(0, &meta); // Persist updated metadata
    return allocated;
}

struct DataNode
{
    int page;
    int size;
    int pos;
    char fileName[KEY_LEN];
    
    unsigned char data[DATA_SIZE + 1];

    DataNode * nextDataNode;

    DataNode() : size(0), nextDataNode(NULL) {
        data[DATA_SIZE] = '\0';
        fileName[0] = '\0';
        page = new_page();
    }
};

struct Node
{
    int page;
    int keySize;
    bool isLeaf;
    char keys[KEYS_SIZE][KEY_LEN];
    int childrens[KEYS_SIZE + 1];
    char padding[180];

    Node() : keySize(0), isLeaf(true) {
        memset(childrens, 0, sizeof(childrens));
        page = new_page();
    }
};

class BPlusTree {
private:
    Node * root;
    void inOrderPrint(Node * node);

    Node * insert(DataNode * data, Node * root);
    int height(Node * node);
    DataNode * get(char key[], Node * node);
    bool deleteValue(char key[], Node * node);
public:
    BPlusTree();
    BPlusTree(int rootPage);
    ~BPlusTree();
    void clear();

    void insert(unsigned char data[], int size, char fileName[]);
    void inOrderPrint();
    int height();
    DataNode * get(char key[]);
    bool deleteValue(char key[]);
};


pair<Node *, Node *> mp(Node * a, Node * b) {
    return make_pair(a, b);
}

static int cstr_len(const char *s) {
  int i = 0;
  while (s[i]) i++;
  return i;
}

static void cp(const char *src, char *dst) {
  int n = cstr_len(src);
  std::copy(src, src + n, dst);
  dst[n] = '\0';
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


void mergeNodes(Node * a, Node * b, char key[]) {
    cp(key, a->keys[a->keySize++]);

    for (int i = 0; i < b->keySize; i++) {
        cp(b->keys[i], a->keys[a->keySize]);
        a->childrens[a->keySize++] = b->childrens[i];
    }
    a->childrens[a->keySize] = b->childrens[b->keySize];
}

void shiftLeft(int pos, Node * node) {
    for (int j = pos; j < node->keySize; j++) {
        node->childrens[j] = node->childrens[j + 1];
    }
    node->childrens[node->keySize] = NULL;

    bool isDeleted = false;
    for (int j = pos; j < node->keySize - 1; j++) {
        cp(node->keys[j + 1], node->keys[j]);
        isDeleted = true;
    }
    
    if (isDeleted) node->keySize--;
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
            write_page(curNode->page, curNode);

            curNode->nextDataNode = new DataNode();
            curNode = curNode->nextDataNode;

            curData += DATA_SIZE;
        }
    }

    curNode->data[size % (DATA_SIZE + 1)] = NULL;
    write_page(curNode->page, curNode);

    return node;
}

DataNode *getDataNode(int page) {
    void *buffer = malloc(PAGE_SIZE);
    if (!buffer) return NULL;

    read_page(page, buffer);

    DataNode *node = (DataNode *)buffer;

    return node;
}


Node *getNode(int page) {
    void *buffer = malloc(PAGE_SIZE);
    if (!buffer) return NULL;

    read_page(page, buffer);

    Node *node = (Node *)buffer;

    return node;
}

BPlusTree::BPlusTree() {
    root = new Node();
    meta.root_page = root->page;
    write_page(root->page, root);
    write_page(0, &meta);
}

BPlusTree::BPlusTree(int rootPage) {
    if (rootPage < 1) {
        BPlusTree();
        return;
    }

    root = getNode(rootPage);
}

BPlusTree::~BPlusTree() {
    // clear();
}

void BPlusTree::insert(unsigned char data[], int size, char fileName[]) {
    DataNode * dataNode = newDataNode(data, size, fileName);

    if (root == NULL) {
        root = new Node();
        write_page(root->page, root);
    }

    auto resp = insert(dataNode, root);
    if (resp) {
        root = resp;
        meta.root_page = root->page;
        write_page(root->page, root);
        write_page(0, &meta);
    }
}

Node * BPlusTree::insert(DataNode * data, Node * node) {
    int i;
    for (i = 0; i < node->keySize && strcmpb(data->fileName, node->keys[i]) > 0; i++);

    if (node->isLeaf) {
        if (node->childrens[i]) {
            if (strcmpb(data->fileName, getDataNode(node->childrens[i])->fileName) <= 0) {
                node->childrens[node->keySize + 1] = node->childrens[node->keySize];
                for (int j = node->keySize - 1; j >= i; j--) {
                    cp(node->keys[j], node->keys[j + 1]);
                    node->childrens[j + 1] = node->childrens[j];
                }
                cp(data->fileName, node->keys[i]); 
                node->childrens[i] = data->page;
            } else {
                node->childrens[node->keySize + 1] = node->childrens[node->keySize];
                for (int j = node->keySize - 1; j > i; j--) {
                    cp(node->keys[j], node->keys[j + 1]);
                    node->childrens[j + 1] = node->childrens[j];
                }
                cp(getDataNode(node->childrens[i])->fileName, node->keys[i]);
                node->childrens[i + 1] = data->page;
            }

            node->keySize++;
        } else {
            node->childrens[i] = data->page;
            if (node->keySize == 0) {
                cp(data->fileName, node->keys[0]);
                node->keySize++;
            }
        }
    } else {
        Node * newChildHead = insert(data, getNode(node->childrens[i]));

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
    write_page(node->page, node);

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
        newGrandParent->childrens[0] = node->page;
        newGrandParent->childrens[1] = secondNode->page;
        cp(node->keys[KEYS_SIZE / 2], newGrandParent->keys[0]);
        newGrandParent->keySize = 1;
        newGrandParent->isLeaf = false;

        node->keySize = KEYS_SIZE / 2;

        write_page(node->page, node);
        write_page(secondNode->page, secondNode);
        write_page(newGrandParent->page, newGrandParent);

        return newGrandParent;
    }

    return NULL;
}

DataNode * BPlusTree::get(char key[]) {
    return get(key, root);
}

DataNode * BPlusTree::get(char key[], Node * node) {
    if (node == NULL) return NULL;

    if (node->isLeaf) {
        for (int i = 0; i <= node->keySize; i++) {
            if (node->childrens[i]) {
                DataNode * dataNode = getDataNode(node->childrens[i]);
                if (strcmpb(key, dataNode->fileName) == 0) return dataNode;
            }
        }

        return NULL;
    }

    int i;
    for(i = 0; i < node->keySize && strcmpb(key, node->keys[i]) > 0; i++);

    return get(key, getNode(node->childrens[i]));
}

void BPlusTree::inOrderPrint() {
    inOrderPrint(root);
}

void BPlusTree::inOrderPrint(Node * node) {
    if (node == NULL) return;

    if (node->isLeaf) {
        for (int i = 0; i < node->keySize; i++) {
            cout << "data :: " << getDataNode(node->childrens[i])->fileName << "\n";
            cout << "key :: " << node->keys[i] << "\n";
        }
        if (node->childrens[node->keySize] != NULL) cout << "data :: " << getDataNode(node->childrens[node->keySize])->fileName << "\n";
    } else {
        for (int i = 0; i < node->keySize; i++) {
            inOrderPrint(getNode(node->childrens[i]));
            cout << "key :: " << node->keys[i] << "\n";
        }
        inOrderPrint(getNode(node->childrens[node->keySize]));
    }
}

int BPlusTree::height() {
    return height(root);
}

int BPlusTree::height(Node * node) {
    if (node == NULL) return 0;

    if (node->isLeaf) {
        return 1;
    } 
    
    int maxDepth = 0;
    for (int i = 0; i <= node->keySize; i++) {
        maxDepth = max(maxDepth, height(getNode(node->childrens[i])));
    }

    return maxDepth + 1;
}

bool BPlusTree::deleteValue(char key[]) {
    bool isDeleted = deleteValue(key, root);

    if (root->childrens[1] == NULL && !root->isLeaf) {
        root = getNode(root->childrens[0]);
    }

    return isDeleted;
}


void balance(int lowKey, Node * rootNode) {
    Node * node = getNode(rootNode->childrens[lowKey]);

    if (rootNode->keySize == 1 & rootNode->childrens[1] == NULL) {
        return;
    };

    if (lowKey > 0) {
        Node * leftNode = getNode(rootNode->childrens[lowKey - 1]);
        if (leftNode->keySize > KEYS_SIZE / 2) {
            node->childrens[node->keySize + 1] = node->childrens[node->keySize];
            for (int i = node->keySize; i > 0; i--) {
                cp(node->keys[i - 1], node->keys[i]);
                node->childrens[i] = node->childrens[i - 1];
            }

            cp(leftNode->keys[leftNode->keySize - 1], node->keys[0]);
            node->childrens[0] = leftNode->childrens[leftNode->keySize];

            leftNode->keySize--;
            node->keySize++;

            write_page(leftNode->page, leftNode);
            write_page(node->page, node);
        } else {
            mergeNodes(leftNode, node, rootNode->keys[lowKey - 1]);
            
            write_page(leftNode->page, leftNode);

            rootNode->childrens[lowKey] = leftNode->page;

            shiftLeft(lowKey - 1, rootNode);
            write_page(rootNode->page, rootNode);
        }
    } else {
        Node * rightNode = getNode(rootNode->childrens[lowKey + 1]);
        if (rightNode->keySize > KEYS_SIZE / 2) {
            node->childrens[node->keySize + 1] = rightNode->childrens[0];
            cp(rightNode->keys[0], node->keys[node->keySize]);
            node->keySize++;

            shiftLeft(0, rightNode);

            write_page(rightNode->page, rightNode);
            write_page(node->page, node);
        } else {
            mergeNodes(node, rightNode, rootNode->keys[lowKey]);
            write_page(node->page, node);

            rootNode->childrens[lowKey + 1] = node->page;

            shiftLeft(lowKey, rootNode);
            write_page(rootNode->page, rootNode);
        }
    }
}

bool BPlusTree::deleteValue(char key[], Node * node) {
    if (node->isLeaf) {
        for (int i = 0; i <= node->keySize; i++) {
            if (node->childrens[i]) {
                DataNode * dataNode = getDataNode(node->childrens[i]);
                if (strcmpb(key, dataNode->fileName) == 0) {
                    for (int j = i; j < node->keySize; j++) {
                        node->childrens[j] = node->childrens[j + 1];
                    }
                    node->childrens[node->keySize] = NULL;

                    if (node->keySize > 1) {
                        for (int j = i; j < node->keySize - 1; j++) {
                            cp(node->keys[j + 1], node->keys[j]);
                        }
                        node->keySize--;
                    } else {
                        if (i == 1) {
                            int dummy = 0; // nothing needs to be done.
                        } else {
                            if (node->childrens[1] == NULL) {
                                node->keySize = 0;
                            } else {
                                DataNode * specialChild = (DataNode *) node->childrens[0];
                                cp(specialChild->fileName, node->keys[0]);
                            }
                        }
                    }

                    write_page(node->page, node);

                    return true;
                }
            }
        }

        return false;
    }

    int i;
    for (i = 0; i < node->keySize && (strcmpb(key, node->keys[i]) > 0); i++);

    Node * child = getNode(node->childrens[i]);
    if (deleteValue(key, child)) {
        if (child->keySize < KEYS_SIZE / 2) {
            balance(i, node);
        }

        return true;
    }

    return false;
}

#endif
