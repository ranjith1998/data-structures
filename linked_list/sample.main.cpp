#include <iostream>
#include "linked_list.h"

int main() {
    std::cout << "Linked List Implementation Demo" << std::endl;
    std::cout << "===============================" << std::endl;

    // Create a linked list of integers
    LinkedList<int> intList;
    
    // Insert some values
    std::cout << "\nInserting values at the end: 10, 20, 30" << std::endl;
    intList.insertAtEnd(10);
    intList.insertAtEnd(20);
    intList.insertAtEnd(30);
    intList.display();
    
    // Insert at the beginning
    std::cout << "\nInserting value at the beginning: 5" << std::endl;
    intList.insertAtBeginning(5);
    intList.display();
    
    // Insert at a specific position
    std::cout << "\nInserting value 15 at position 2" << std::endl;
    intList.insertAt(2, 15);
    intList.display();
    
    // Search for a value
    int searchValue = 15;
    int position = intList.search(searchValue);
    std::cout << "\nSearching for value " << searchValue << std::endl;
    if (position != -1) {
        std::cout << "Value " << searchValue << " found at position " << position << std::endl;
    } else {
        std::cout << "Value " << searchValue << " not found in the list" << std::endl;
    }
    
    // Delete from specific position
    std::cout << "\nDeleting element at position 1" << std::endl;
    intList.deleteAt(1);
    intList.display();
    
    // Delete from beginning
    std::cout << "\nDeleting element from the beginning" << std::endl;
    intList.deleteFromBeginning();
    intList.display();
    
    // Delete from end
    std::cout << "\nDeleting element from the end" << std::endl;
    intList.deleteFromEnd();
    intList.display();
    
    // Create a linked list of strings
    std::cout << "\n\nString Linked List Demo:" << std::endl;
    LinkedList<std::string> stringList;
    
    // Add some strings
    stringList.insertAtEnd("Apple");
    stringList.insertAtEnd("Banana");
    stringList.insertAtEnd("Cherry");
    stringList.display();
    
    // Insert at a specific position
    stringList.insertAt(1, "Blueberry");
    stringList.display();
    
    // Search for a string
    std::string searchStr = "Banana";
    position = stringList.search(searchStr);
    std::cout << "\nSearching for string '" << searchStr << "'" << std::endl;
    if (position != -1) {
        std::cout << "String '" << searchStr << "' found at position " << position << std::endl;
    } else {
        std::cout << "String '" << searchStr << "' not found in the list" << std::endl;
    }
    
    return 0;
}
