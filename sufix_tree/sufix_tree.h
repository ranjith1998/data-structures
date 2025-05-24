#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

#define SPECIAL '$'

#include <map>

using namespace std;

struct Node {
    map<char, Node *> childrens;
    int start;
    int end;

    Node(int start, int end): start(start), end(end) {}
};

class SuffixTree {
private:
    Node * root;
    string data;
    int last;

    void insertSpecial(Node * node) {
        node->childrens[SPECIAL] = new Node(data.size(), data.size());
    }

    Node * insert(int pos, Node * node) {
        int i;
        for (i = 0; node->start + i <= node->end && pos + i < data.size(); i++) {
            if (data[node->start + i] != data[pos + i]) {
                break;
            }
        }

        if (node->start + i > node->end && pos + i < data.size()) {
            char curChar = data[pos + i];
            if (node->childrens.find(curChar) != node->childrens.end())  {
                node->childrens[curChar] = insert(pos + i, node->childrens[curChar]);
            } else {
                node->childrens[curChar] = new Node(pos + i, last);
                insertSpecial(node->childrens[curChar]);
            }

            return node;
        } else if (node->start + i > node->end && pos + i == data.size()) {
            insertSpecial(node);
        } else {
            Node * newRoot = new Node(node->start, node->start + i - 1);

            newRoot->childrens[data[node->start + i]] = node;
            node->start += i;

            if (pos + i < data.size()) {
                char curChar = data[pos + i];
                newRoot->childrens[curChar] = new Node(pos + i, last);
                insertSpecial(newRoot->childrens[curChar]);
            } else {
                insertSpecial(newRoot);
            }

            return newRoot;
        }
    }

    void findAll(int magnitude, Node * node, vector<int> &result) {
        if (node == NULL) return;

        if (node->childrens.find(SPECIAL) != node->childrens.end())
            result.push_back(node->start + magnitude);

        int newMagnitude = magnitude - (node->end + 1 - node->start);

        for (auto child : node->childrens) {
            findAll(newMagnitude, child.second, result);
        }
    }
public:
    SuffixTree(string content) {
        root = new Node(0, -1);
        last = content.size() - 1;

        data = content;
        for (int i = 0; i < data.size(); i++) {
            insert(i, root);
        }
    }

    vector<int> Search(string content) {
        Node * current = root;

        int j;
        for (int i = 0; i < content.size();) {
            for (j = current->start; j <= current->end && i < content.size() && content[i] == data[j]; j++, i++);
            if (j < current->end) {
                if (i == content.size()) {
                    break;
                } else {
                    current = NULL;
                }
            } else {
                if (i == content.size()) {
                    break;
                } else {
                    if (current->childrens.find(content[i]) == current->childrens.end()) {
                        current = NULL;
                        break;
                    }

                    current = current->childrens[content[i]];
                }
            }
        }

        vector<int> result;
        findAll(j - current->start - content.size(), current, result);

        return result;
    }
};

#endif