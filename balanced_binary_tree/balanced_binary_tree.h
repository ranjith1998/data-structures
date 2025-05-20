#ifndef BALANCED_BINARY_TREE_H
#define BALANCED_BINARY_TREE_H

template <typename T>
struct Node
{
    T value;
    Node * left;
    Node * right;
    int height;

    Node(T value) : value(value), left(NULL), right(NULL), height(1) {}
};

template <typename T>
class BalancedBinaryTree
{
private:
    Node<T> * root;
    bool (*compare) (T, T);
    void clear(Node<T> *);
    Node<T> * insert(T data, Node<T> * node);
    Node<T>* deleteValue(T value, Node<T>* node);
    void inorderTraversal(Node<T>* node);
    void preorderTraversal(Node<T>* node);
    void postorderTraversal(Node<T>* node);
public:
    BalancedBinaryTree(bool (*compare) (T, T));
    ~BalancedBinaryTree();
    void clear();

    void insert(T data);
    void deleteValue(T value);
    void inorderTraversal();
    void preorderTraversal();
    void postorderTraversal();
};

template <typename T>
BalancedBinaryTree<T>::BalancedBinaryTree(bool (*cmp) (T, T)) : root(NULL), compare(cmp) {}

template <typename T>
BalancedBinaryTree<T>::~BalancedBinaryTree()
{
    this->clear();
}

template <typename T>
void BalancedBinaryTree<T>::clear() {
    clear(root);
    root = NULL;
}

template <typename T>
void BalancedBinaryTree<T>::clear(Node<T> * node) {
    if (node == NULL) return;
    clear(node->left); clear(node->right);
    delete node;
}

template <typename T>
void BalancedBinaryTree<T>::insert(T data) {
    root = insert(data, root);
}


template <typename T>
Node<T> * BalancedBinaryTree<T>::insert(T data, Node<T> * node) {
    if (node == NULL) {
        return new Node<T>(data);
    }

    if (compare(data, node->value)) {
        node->left = insert(data, node->left);
    } else {
        node->right = insert(data, node->right);
    }

    updateHeight(node);

    return balanceHeight(node);
}

template <typename T>
Node<T> * balanceHeight(Node<T> * node) {
    if (node == NULL) return NULL;

    int leftChildHeight = 0, rightChildHeight = 0;
    if (node->left != NULL) leftChildHeight = node->left->height;
    if (node->right != NULL) rightChildHeight = node->right->height;
    
    if (leftChildHeight - rightChildHeight > 1) {
        Node<T> * temp = node;
        node = node->left;
        temp->left = node->right;
        node->right = temp;
        updateHeight(temp);
    } else if (rightChildHeight - leftChildHeight > 1) {
        Node<T> * temp = node;
        node = node->right;
        temp->right = node->left;
        node->left = temp;
        updateHeight(temp);
    }

    updateHeight(node);

    return node;
}

int max(int a, int b) {
    return a > b ? a : b;
}

template <typename T>
void updateHeight(Node<T> * node) {
    if (node != NULL)
        node->height = max(node->left == NULL ? 0 : node->left->height, node->right == NULL ? 0 : node->right->height) + 1;
}

template <typename T>
void BalancedBinaryTree<T>::deleteValue(T value) {
    deleteValue(value, root);
}

template <typename T>
Node<T>* BalancedBinaryTree<T>::deleteValue(T value, Node<T>* node) {
    if (node == nullptr) {
        return nullptr;
    }

    if (node->value == value) {
        if (node->left != NULL) {
            T maximumLeft = findMaximum(node->left);
            node->left = deleteValue(maximumLeft, node->left);
            node->value = maximumLeft;
        } else if (node->right != NULL) {
            T minimumRight = findMinimum(node->right);
            node->right = deleteValue(minimumRight, node->right);
            node->value = minimumRight;
        } else {
            delete node;
            node = NULL;
        }
    } else if (compare(value, node->value)) {
        node->left = deleteValue(value, node->left);
    } else {
        node->right = deleteValue(value, node->right);
    }

    updateHeight(node);

    return balanceHeight(node);
}

template <typename T>
void BalancedBinaryTree<T>::inorderTraversal(Node<T>* node) {
    if (node != nullptr) {
        inorderTraversal(node->left);
        std::cout << node->value << " ";
        inorderTraversal(node->right);
    }
}

template <typename T>
void BalancedBinaryTree<T>::preorderTraversal(Node<T>* node) {
    if (node != nullptr) {
        std::cout << node->value << " ";
        preorderTraversal(node->left);
        preorderTraversal(node->right);
    }
}

template <typename T>
void BalancedBinaryTree<T>::postorderTraversal(Node<T>* node) {
    if (node != nullptr) {
        postorderTraversal(node->left);
        postorderTraversal(node->right);
        std::cout << node->value << " ";
    }
}

template <typename T>
void BalancedBinaryTree<T>::inorderTraversal() {
    inorderTraversal(root);
}

template <typename T>
void BalancedBinaryTree<T>::preorderTraversal() {
    preorderTraversal(root);
}

template <typename T>
void BalancedBinaryTree<T>::postorderTraversal() {
    postorderTraversal(root);
}

template <typename T>
T findMaximum(Node<T>* node) {
    while(node->right != NULL) node = node->right;
    return node->value;
}

template <typename T>
T findMinimum(Node<T>* node) {
    while(node->left != NULL) node = node->left;
    return node->value;
}

#endif