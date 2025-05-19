#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>

// Node class for Linked List
template <typename T>
class Node {
public:
    T data;
    Node* next;

    // Constructor
    Node(T value) : data(value), next(nullptr) {}
};

// Linked List class
template <typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    // Constructor
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Destructor to free memory
    ~LinkedList() {
        clear();
    }

    // Insert at the beginning
    void insertAtBeginning(T value) {
        Node<T>* newNode = new Node<T>(value);
        
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
        size++;
    }

    // Insert at the end
    void insertAtEnd(T value) {
        Node<T>* newNode = new Node<T>(value);
        
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    // Insert at a specific position
    bool insertAt(int position, T value) {
        if (position < 0 || position > size) {
            return false;
        }

        if (position == 0) {
            insertAtBeginning(value);
            return true;
        }

        if (position == size) {
            insertAtEnd(value);
            return true;
        }

        Node<T>* current = head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }

        Node<T>* newNode = new Node<T>(value);
        newNode->next = current->next;
        current->next = newNode;
        size++;
        return true;
    }

    // Delete from the beginning
    bool deleteFromBeginning() {
        if (head == nullptr) {
            return false;
        }

        Node<T>* temp = head;
        head = head->next;
        delete temp;
        
        if (head == nullptr) {
            tail = nullptr;
        }
        
        size--;
        return true;
    }

    // Delete from the end
    bool deleteFromEnd() {
        if (head == nullptr) {
            return false;
        }

        if (head == tail) {
            delete head;
            head = tail = nullptr;
            size--;
            return true;
        }

        Node<T>* current = head;
        while (current->next != tail) {
            current = current->next;
        }

        delete tail;
        tail = current;
        tail->next = nullptr;
        size--;
        return true;
    }

    // Delete from a specific position
    bool deleteAt(int position) {
        if (position < 0 || position >= size || head == nullptr) {
            return false;
        }

        if (position == 0) {
            return deleteFromBeginning();
        }

        if (position == size - 1) {
            return deleteFromEnd();
        }

        Node<T>* current = head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }

        Node<T>* temp = current->next;
        current->next = temp->next;
        delete temp;
        size--;
        return true;
    }

    // Search for a value
    int search(T value) {
        Node<T>* current = head;
        int position = 0;

        while (current != nullptr) {
            if (current->data == value) {
                return position;
            }
            current = current->next;
            position++;
        }

        return -1;  // Not found
    }

    // Get the value at a specific position
    bool get(int position, T& value) {
        if (position < 0 || position >= size) {
            return false;
        }

        Node<T>* current = head;
        for (int i = 0; i < position; i++) {
            current = current->next;
        }

        value = current->data;
        return true;
    }

    // Get the size of the list
    int getSize() {
        return size;
    }

    // Check if the list is empty
    bool isEmpty() {
        return head == nullptr;
    }

    // Clear the list
    void clear() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }

    // Print the list
    void display() {
        Node<T>* current = head;
        
        std::cout << "List [" << size << "]: ";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << std::endl;
    }
};

#endif // LINKED_LIST_H
