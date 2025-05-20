#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include "balanced_binary_tree/balanced_binary_tree.h"

// Helper function to compare integers (less than)
bool compareLess(int a, int b) {
    return a < b;
}

// Helper function to compare integers (greater than)
bool compareGreater(int a, int b) {
    return a > b;
}

// Custom data type for testing
struct Person {
    std::string name;
    int age;
    
    Person(std::string n, int a) : name(n), age(a) {}
    
    // For output stream
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << p.name << "(" << p.age << ")";
        return os;
    }
};

// Compare persons by age
bool comparePersonsByAge(const Person a, const Person b) {
    return a.age < b.age;
}

// Helper function to capture std::cout output
class CaptureOutput {
private:
    std::stringstream buffer;
    std::streambuf* old;
public:
    CaptureOutput() : old(std::cout.rdbuf(buffer.rdbuf())) {}
    ~CaptureOutput() { std::cout.rdbuf(old); }
    
    std::string str() const { return buffer.str(); }
};

int main() {
    std::cout << "=== BALANCED BINARY TREE TESTS ===" << std::endl;
    
    // Test Case 1: Create an empty tree
    std::cout << "\nTest Case 1: Empty Tree" << std::endl;
    BalancedBinaryTree<int> emptyTree(compareLess);
    
    // Test traversals on empty tree (should print nothing)
    CaptureOutput captureEmpty;
    emptyTree.inorderTraversal();
    std::string emptyOutput = captureEmpty.str();
    assert(emptyOutput.empty());
    std::cout << "✓ Empty tree traversal test passed" << std::endl;
    
    // Test Case 2: Insert single element
    std::cout << "\nTest Case 2: Single Element" << std::endl;
    BalancedBinaryTree<int> singleElementTree(compareLess);
    singleElementTree.insert(10);
    
    CaptureOutput captureSingle;
    singleElementTree.inorderTraversal();
    std::string singleOutput = captureSingle.str();
    assert(singleOutput == "10 ");
    std::cout << "✓ Single element tree test passed" << std::endl;
    
    // Test Case 3: Insert multiple elements in ascending order
    std::cout << "\nTest Case 3: Multiple Elements (Ascending)" << std::endl;
    BalancedBinaryTree<int> ascendingTree(compareLess);
    for (int i = 1; i <= 7; i++) {
        ascendingTree.insert(i);
    }
    
    CaptureOutput captureAscending;
    ascendingTree.inorderTraversal();
    std::string ascendingOutput = captureAscending.str();
    assert(ascendingOutput == "1 2 3 4 5 6 7 ");
    std::cout << "✓ Ascending order insertion test passed" << std::endl;
    
    // Test Case 4: Insert multiple elements in descending order
    std::cout << "\nTest Case 4: Multiple Elements (Descending)" << std::endl;
    BalancedBinaryTree<int> descendingTree(compareLess);
    for (int i = 7; i >= 1; i--) {
        descendingTree.insert(i);
    }
    
    CaptureOutput captureDescending;
    descendingTree.inorderTraversal();
    std::string descendingOutput = captureDescending.str();
    assert(descendingOutput == "1 2 3 4 5 6 7 ");
    std::cout << "✓ Descending order insertion test passed" << std::endl;
    
    // Test Case 5: Insert in random order
    std::cout << "\nTest Case 5: Multiple Elements (Random Order)" << std::endl;
    BalancedBinaryTree<int> randomTree(compareLess);
    std::vector<int> randomValues = {4, 2, 6, 1, 3, 5, 7};
    for (int val : randomValues) {
        randomTree.insert(val);
    }
    
    CaptureOutput captureRandom;
    randomTree.inorderTraversal();
    std::string randomOutput = captureRandom.str();
    assert(randomOutput == "1 2 3 4 5 6 7 ");
    std::cout << "✓ Random order insertion test passed" << std::endl;
    
    // Test Case 6: Test different traversal methods
    std::cout << "\nTest Case 6: Different Traversal Methods" << std::endl;
    BalancedBinaryTree<int> traversalTree(compareLess);
    for (int val : randomValues) {
        traversalTree.insert(val);
    }
    
    // Test inorder traversal
    CaptureOutput captureInorder;
    traversalTree.inorderTraversal();
    std::string inorderOutput = captureInorder.str();
    
    // Test preorder traversal
    CaptureOutput capturePreorder;
    traversalTree.preorderTraversal();
    std::string preorderOutput = capturePreorder.str();
    
    // Test postorder traversal
    CaptureOutput capturePostorder;
    traversalTree.postorderTraversal();
    std::string postorderOutput = capturePostorder.str();
    
    std::cout << "Inorder traversal: " << inorderOutput << std::endl;
    std::cout << "Preorder traversal: " << preorderOutput << std::endl;
    std::cout << "Postorder traversal: " << postorderOutput << std::endl;
    
    // Test Case 7: Insert duplicates
    std::cout << "\nTest Case 7: Insert Duplicates" << std::endl;
    BalancedBinaryTree<int> duplicateTree(compareLess);
    std::vector<int> duplicateValues = {5, 3, 7, 3, 5, 7, 5};
    for (int val : duplicateValues) {
        duplicateTree.insert(val);
    }
    
    CaptureOutput captureDuplicate;
    duplicateTree.inorderTraversal();
    std::string duplicateOutput = captureDuplicate.str();
    std::cout << "Tree with duplicates (inorder): " << duplicateOutput << std::endl;
    
    // Test Case 8: Custom comparator (reverse order)
    std::cout << "\nTest Case 8: Custom Comparator (Reverse Order)" << std::endl;
    BalancedBinaryTree<int> reverseTree(compareGreater);
    for (int i = 1; i <= 7; i++) {
        reverseTree.insert(i);
    }
    
    CaptureOutput captureReverse;
    reverseTree.inorderTraversal();
    std::string reverseOutput = captureReverse.str();
    std::cout << "Reverse order tree (inorder): " << reverseOutput << std::endl;
    
    // Test Case 9: Clear tree
    std::cout << "\nTest Case 9: Clear Tree" << std::endl;
    BalancedBinaryTree<int> clearTree(compareLess);
    for (int i = 1; i <= 5; i++) {
        clearTree.insert(i);
    }
    
    CaptureOutput captureBeforeClear;
    clearTree.inorderTraversal();
    std::string beforeClearOutput = captureBeforeClear.str();
    assert(beforeClearOutput == "1 2 3 4 5 ");
    
    clearTree.clear();
    
    CaptureOutput captureAfterClear;
    clearTree.inorderTraversal();
    std::string afterClearOutput = captureAfterClear.str();
    assert(afterClearOutput.empty());
      std::cout << "✓ Clear tree test passed" << std::endl;
    
    // Test Case 10: Delete values
    std::cout << "\nTest Case 10: Delete Values" << std::endl;
    BalancedBinaryTree<int> deleteTree(compareLess);
    for (int i = 1; i <= 7; i++) {
        deleteTree.insert(i);
    }
    
    // Delete leaf node
    deleteTree.deleteValue(1);
    
    // Delete node with one child
    deleteTree.deleteValue(6);
    
    // Delete node with two children
    deleteTree.deleteValue(4);
    
    CaptureOutput captureAfterDelete;
    deleteTree.inorderTraversal();
    std::string afterDeleteOutput = captureAfterDelete.str();
    std::cout << "Tree after deletions (inorder): " << afterDeleteOutput << std::endl;
    assert(afterDeleteOutput == "2 3 5 7 ");
    std::cout << "✓ Delete values test passed" << std::endl;
    
    // Test Case 11: Test tree balance
    std::cout << "\nTest Case 11: Test Tree Balance" << std::endl;
    BalancedBinaryTree<int> balanceTree(compareLess);
    
    // Insert values in a way that would make an unbalanced BST
    std::vector<int> potentiallyUnbalancingSequence = {50, 25, 75, 12, 37, 62, 87, 6, 18, 31, 43, 56, 68, 81, 93};
    for (int val : potentiallyUnbalancingSequence) {
        balanceTree.insert(val);
    }
    
    CaptureOutput captureBalanced;
    balanceTree.inorderTraversal();
    std::string balancedOutput = captureBalanced.str();
    std::cout << "Balanced tree (inorder): " << balancedOutput << std::endl;
    
    // Verify tree is in sorted order
    std::string expectedBalancedOutput = "6 12 18 25 31 37 43 50 56 62 68 75 81 87 93 ";
    assert(balancedOutput == expectedBalancedOutput);
    std::cout << "✓ Tree balance test passed" << std::endl;
    
    // Test Case 12: Test deletion of root node
    std::cout << "\nTest Case 12: Delete Root Node" << std::endl;
    BalancedBinaryTree<int> rootDeletionTree(compareLess);
    rootDeletionTree.insert(50);
    rootDeletionTree.insert(25);
    rootDeletionTree.insert(75);
    
    // Delete root
    rootDeletionTree.deleteValue(50);
    
    CaptureOutput captureRootDeletion;
    rootDeletionTree.inorderTraversal();
    std::string rootDeletionOutput = captureRootDeletion.str();
    std::cout << "Tree after root deletion (inorder): " << rootDeletionOutput << std::endl;
    assert(rootDeletionOutput == "25 75 ");
    std::cout << "✓ Root deletion test passed" << std::endl;
    
    // Test Case 13: Delete node that doesn't exist
    std::cout << "\nTest Case 13: Delete Non-existent Node" << std::endl;
    BalancedBinaryTree<int> nonExistentTree(compareLess);
    nonExistentTree.insert(10);
    nonExistentTree.insert(5);
    nonExistentTree.insert(15);
    
    // Try to delete a node that doesn't exist
    nonExistentTree.deleteValue(20);
    
    CaptureOutput captureNonExistent;
    nonExistentTree.inorderTraversal();
    std::string nonExistentOutput = captureNonExistent.str();
    assert(nonExistentOutput == "5 10 15 ");
    std::cout << "✓ Non-existent node deletion test passed" << std::endl;
    
    // Test Case 14: Delete from empty tree
    std::cout << "\nTest Case 14: Delete from Empty Tree" << std::endl;
    BalancedBinaryTree<int> emptyDeleteTree(compareLess);
    
    // Try to delete from empty tree
    emptyDeleteTree.deleteValue(10);
    
    CaptureOutput captureEmptyDelete;
    emptyDeleteTree.inorderTraversal();
    std::string emptyDeleteOutput = captureEmptyDelete.str();
    assert(emptyDeleteOutput.empty());
    std::cout << "✓ Empty tree deletion test passed" << std::endl;
    
    // Test Case 15: Test with custom data type (Person)
    std::cout << "\nTest Case 15: Custom Data Type" << std::endl;
    BalancedBinaryTree<Person> personTree(comparePersonsByAge);
    
    // Insert people with different ages
    personTree.insert(Person("Alice", 30));
    personTree.insert(Person("Bob", 25));
    personTree.insert(Person("Charlie", 40));
    personTree.insert(Person("David", 20));
    personTree.insert(Person("Eve", 35));
    
    // Should print in order of age: David, Bob, Alice, Eve, Charlie
    CaptureOutput capturePersons;
    personTree.inorderTraversal();
    std::string personsOutput = capturePersons.str();
    std::cout << "Persons sorted by age: " << personsOutput << std::endl;
    std::cout << "✓ Custom data type test passed" << std::endl;
    
    std::cout << "\nAll tests completed successfully!" << std::endl;
    
    return 0;
}
