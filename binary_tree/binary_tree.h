#ifndef BINARY_TREE_H
#define BINARY_TREE_H

template <typename T>
struct Node {
    T data;
    Node* left;
    Node* right;

    Node(T value) : data(value), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree {
public:
    Node<T>* root;
    // should return true if a should be inserted in left subtree
    // and false if it should be inserted in right subtree
    bool (*compare)(T, T);

    BinaryTree(bool (*cmp)(T, T)) : root(nullptr), compare(cmp) {}

    void insert(T value) {
        insertValue(value, root);
    }

    void insertValue(T value, Node<T>*& node) {
        if (node == nullptr) {
            node = new Node<T>(value);
        } else if (compare(value, node->data)) {
            insertValue(value, node->left);
        } else {
            insertValue(value, node->right);
        }
    }

    void inorderTraversal(Node<T>* node) {
        if (node != nullptr) {
            inorderTraversal(node->left);
            std::cout << node->data << " ";
            inorderTraversal(node->right);
        }
    }

    void preorderTraversal(Node<T>* node) {
        if (node != nullptr) {
            std::cout << node->data << " ";
            preorderTraversal(node->left);
            preorderTraversal(node->right);
        }
    }

    void postorderTraversal(Node<T>* node) {
        if (node != nullptr) {
            postorderTraversal(node->left);
            postorderTraversal(node->right);
            std::cout << node->data << " ";
        }
    }

    Node<T>* search(T value) {
        return searchValue(value, root);
    }

    Node<T>* searchValue(T value, Node<T>* node) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->data == value) {
            return node;
        } else if (compare(value, node->data)) {
            return searchValue(value, node->left);
        } else {
            return searchValue(value, node->right);
        }
    }

    void deleteValue(T value) {
        root = deleteNode(value, root);
    }

    Node<T>* deleteNode(T value, Node<T>* node) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->data == value) {
            if (node->left != NULL) {
                T maximumLeft = findMaximum(node->left);
                node->left = deleteNode(maximumLeft, node->left);
                node->data = maximumLeft;
            } else if (node->right != NULL) {
                T minimumRight = findMinimum(node->right);
                node->right = deleteNode(minimumRight, node->right);
                node->data = minimumRight;
            } else {
                delete node;
                node = NULL;
            }
        } else if (compare(value, node->data)) {
            node->left = deleteNode(value, node->left);
        } else {
            node->right = deleteNode(value, node->right);
        }
        return node;
    }

    T findMaximum(Node<T>* node) {
        while(node->right != NULL) node = node->right;
        return node->data;
    }

    T findMinimum(Node<T>* node) {
        while(node->left != NULL) node = node->left;
        return node->data;
    }

    int height(Node<T>* node) {
        if (node == nullptr) {
            return 0;
        }
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        return std::max(leftHeight, rightHeight) + 1;
    }
};

#endif