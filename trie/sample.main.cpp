#include <iostream>
#include "trie/trie.h"
#include <string>

int main() {
    // Create a trie to store string-integer pairs
    Trie<int> wordFrequency;
    
    // Insert some words with associated values
    wordFrequency.insert("apple", 5);
    wordFrequency.insert("app", 3);
    wordFrequency.insert("application", 10);
    wordFrequency.insert("banana", 7);
    wordFrequency.insert("bat", 2);
    
    // Search for words
    std::cout << "Testing search functionality:" << std::endl;
    std::cout << "Is 'apple' in trie? " << (wordFrequency.search("apple") ? "Yes" : "No") << std::endl;
    std::cout << "Is 'app' in trie? " << (wordFrequency.search("app") ? "Yes" : "No") << std::endl;
    std::cout << "Is 'apricot' in trie? " << (wordFrequency.search("apricot") ? "Yes" : "No") << std::endl;
    std::cout << "Is 'bat' in trie? " << (wordFrequency.search("bat") ? "Yes" : "No") << std::endl;
    
    // Get values associated with words
    std::cout << "\nTesting get functionality:" << std::endl;
    int* appleValue = wordFrequency.get("apple");
    if (appleValue) {
        std::cout << "Value for 'apple': " << *appleValue << std::endl;
    }
    
    int* appValue = wordFrequency.get("app");
    if (appValue) {
        std::cout << "Value for 'app': " << *appValue << std::endl;
    }
    
    int* bananaValue = wordFrequency.get("banana");
    if (bananaValue) {
        std::cout << "Value for 'banana': " << *bananaValue << std::endl;
    }
    
    int* apricotValue = wordFrequency.get("apricot");
    std::cout << "Value for 'apricot': " << (apricotValue ? std::to_string(*apricotValue) : "Not found") << std::endl;
    
    std::cout << "\nTrie testing completed successfully!" << std::endl;
    
    return 0;
}
