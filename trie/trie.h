#ifndef TRIE_H
#define TRIE_H

#include <map>

using namespace std;

template <typename T>
class Node {
public:
    map<char, Node<T>*> children;
    T *data;

    Node(T value) : data(new T(value)), children() {}
};

template <typename T>
class Trie {
    Node<T> *root;
public:
    Trie() : root(new Node<T>(NULL)) {}

    void insert(const std::string &word, T value) {
        Node<T> *current = root;
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new Node<T>(NULL);
            }
            current = current->children[c];
        }
        current->data = new T(value);
    }

    bool search(const std::string &word) {
        Node<T> *current = root;
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children[c];
        }
        return current->data != NULL;
    }

    T* get(const std::string &word) {
        Node<T> *current = root;
        for (char c : word) {
            if (current->children.find(c) == current->children.end()) {
                return NULL;
            }
            current = current->children[c];
        }
        return current->data;
    }
};

#endif