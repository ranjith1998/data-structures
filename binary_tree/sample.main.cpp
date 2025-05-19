#include <iostream>
#include "binary_tree/binary_tree.h"

// Comparison function for the binary tree
// Returns true if a should be placed to the left of b
bool compareInts(int a, int b) {
    return a < b;
}

int main() {
    // Create a binary tree with integer values
    BinaryTree<int> tree(compareInts);
    
    // Insert some values
    std::cout << "Inserting values: 50, 30, 70, 20, 40, 60, 80" << std::endl;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);
    
    // Display tree traversals
    std::cout << "\nInorder traversal: ";
    tree.inorderTraversal(tree.root);
    std::cout << std::endl;
    
    std::cout << "Preorder traversal: ";
    tree.preorderTraversal(tree.root);
    std::cout << std::endl;
    
    std::cout << "Postorder traversal: ";
    tree.postorderTraversal(tree.root);
    std::cout << std::endl;
    
    // Test search functionality
    int searchValue = 40;
    std::cout << "\nSearching for value " << searchValue << "... ";
    Node<int>* result = tree.search(searchValue);
    if (result != nullptr) {
        std::cout << "Found!" << std::endl;
    } else {
        std::cout << "Not found!" << std::endl;
    }
    
    searchValue = 100;
    std::cout << "Searching for value " << searchValue << "... ";
    result = tree.search(searchValue);
    if (result != nullptr) {
        std::cout << "Found!" << std::endl;
    } else {
        std::cout << "Not found!" << std::endl;
    }
    
    // Test tree height
    std::cout << "\nTree height: " << tree.height(tree.root) << std::endl;
    
    // Test delete functionality
    int deleteValue = 30;
    std::cout << "\nDeleting value " << deleteValue << std::endl;
    tree.deleteValue(deleteValue);
    
    std::cout << "Inorder traversal after deletion: ";
    tree.inorderTraversal(tree.root);
    std::cout << std::endl;
    
    // Insert more values to create a deeper tree
    std::cout << "\nInserting more values to create deeper tree: 10, 25, 35, 45, 55, 65, 75, 85" << std::endl;
    tree.insert(10);
    tree.insert(25);
    tree.insert(35);
    tree.insert(45);
    tree.insert(55);
    tree.insert(65);
    tree.insert(75);
    tree.insert(85);
    
    // Display updated tree
    std::cout << "\nInorder traversal of deeper tree: ";
    tree.inorderTraversal(tree.root);
    std::cout << std::endl;
    
    std::cout << "New tree height: " << tree.height(tree.root) << std::endl;
    
    // Test deletion of leaf node
    deleteValue = 10;
    std::cout << "\nDeleting leaf node " << deleteValue << std::endl;
    tree.deleteValue(deleteValue);
    std::cout << "Inorder traversal after leaf deletion: ";
    tree.inorderTraversal(tree.root);
    std::cout << std::endl;
    
    // Test deletion of node with one child
    deleteValue = 75;
    std::cout << "\nDeleting node with one child " << deleteValue << std::endl;
    tree.insert(73); // Add a child to 75 first
    std::cout << "After adding 73 as child, inorder traversal: ";
    tree.inorderTraversal(tree.root);
    std::cout << std::endl;
    
    tree.deleteValue(deleteValue);
    std::cout << "Inorder traversal after deleting node with one child: ";
    tree.inorderTraversal(tree.root);
    std::cout << std::endl;
    
    // Test deletion of node with two children
    deleteValue = 70;
    std::cout << "\nDeleting node with two children " << deleteValue << std::endl;
    tree.deleteValue(deleteValue);
    std::cout << "Inorder traversal after deleting node with two children: ";
    tree.inorderTraversal(tree.root);
    std::cout << std::endl;
    
    // Test deletion of root
    deleteValue = 50;
    std::cout << "\nDeleting root node " << deleteValue << std::endl;
    tree.deleteValue(deleteValue);
    std::cout << "Inorder traversal after deleting root: ";
    tree.inorderTraversal(tree.root);
    std::cout << std::endl;
    
    // Test multiple consecutive deletions
    std::cout << "\nPerforming multiple consecutive deletions: 55, 65, 85" << std::endl;
    tree.deleteValue(55);
    tree.deleteValue(65);
    tree.deleteValue(85);
    
    std::cout << "Inorder traversal after multiple deletions: ";
    tree.inorderTraversal(tree.root);
    std::cout << std::endl;
    
    // Insert more values after deletions
    std::cout << "\nInserting new values after deletions: 15, 33, 42, 67, 90" << std::endl;
    tree.insert(15);
    tree.insert(33);
    tree.insert(42);
    tree.insert(67);
    tree.insert(90);
    
    std::cout << "Final inorder traversal: ";
    tree.inorderTraversal(tree.root);
    std::cout << std::endl;
    
    std::cout << "Final tree height: " << tree.height(tree.root) << std::endl;
    
    return 0;
}
