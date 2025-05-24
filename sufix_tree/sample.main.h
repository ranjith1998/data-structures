#include <iostream>
#include <fstream>
#include <random>
#include <string>

using namespace std;

#include "sufix_tree/sufix_tree.h"

void generateRandomFile(const string& filename, int size) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not create file " << filename << endl;
        return;
    }
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis('a', 'z'); // Generate random lowercase letters
    
    for (int i = 0; i < size; i++) {
        file << static_cast<char>(dis(gen));
    }
    
    file.close();
    cout << "Generated " << filename << " with " << size << " characters" << endl;
}

void generateAaaaaFile(const string& filename, int size) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not create file " << filename << endl;
        return;
    }
    
    for (int i = 0; i < size; i++) {
        file << 'a';
    }
    
    file.close();
    cout << "Generated " << filename << " with " << size << " characters" << endl;
}

string readFileContent(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return "";
    }
    
    string content;
    string line;
    while (getline(file, line)) {
        content += line;
    }
    
    file.close();
    return content;
}

int main() {
    // generateRandomFile("test_1000.txt", 1000);        // 10^3 characters
    // generateRandomFile("test_1000000.txt", 1000000);  // 10^6 characters
    generateAaaaaFile("test_1000000_a.txt", 1000000);

    // SuffixTree * tree = new SuffixTree("banana");

    // string fileContent = readFileContent("test_1000.txt");
    
    // cout << "Read " << fileContent.length() << " characters from test_1000.txt" << endl;
    
    // string fileContent = readFileContent("test_1000000.txt");

    // string fileContent = readFileContent("sample_data.txt");

    string fileContent = readFileContent("sample_data_2.txt");
    
    cout << "Read " << fileContent.length() << " characters from sample_data.txt" << endl;
        
    SuffixTree * tree = new SuffixTree(fileContent);

    vector<int> searchResult = tree->Search("ranjith");

    return 0;
}
