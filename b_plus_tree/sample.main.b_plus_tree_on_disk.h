#include <windows.h>
#include <iostream>
#include <chrono>

using namespace std;

#include "b_plus_tree/b_plus_tree_on_disk.h"

int main() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    std::cout << "Page size: " << sysInfo.dwPageSize << " bytes" << std::endl;

    // Node * node = new Node();
    // DataNode * dataNode = new DataNode();

    // cout << sizeof(node) << " : " << sizeof(*node) << "\n";
    // cout << sizeof(dataNode) << " : " << sizeof(*dataNode) << "\n";

    init_db("tree.db");

    // Simulate storing a record
    // int page = new_page();
    // char buffer[PAGE_SIZE] = {0};
    // strcpy(buffer, "test record");
    // write_page(page, buffer);

    // meta.total_files++;
    // write_page(0, &meta); // Save metadata

    // printf("Total files: %d\n", meta.total_files);

    BPlusTree * tree = new BPlusTree();

    // BPlusTree * tree = new BPlusTree(meta.root_page);

    char data[32];
    char filename[32];

    auto t0 = std::chrono::high_resolution_clock::now();

    for (int i = 1; i <= 200; i++) {
        sprintf(data, "%d\0", i);            // convert i to string
        sprintf(filename, "%d", i);    // build filename string

        tree->insert((unsigned char*)data, strlen(data) + 1, filename);
    }

    bool isDeleted = tree->deleteValue("9");

    tree->inOrderPrint();

    for (int i = 1; i <= 128; i++) {
        sprintf(filename, "%d", i);    // build filename string

        bool isDeleted = tree->deleteValue(filename);

        cout << filename << " is_deleted? " << isDeleted << "\n";
    }

    tree->inOrderPrint();

    for (int i = 129; i <= 200; i++) {
        sprintf(filename, "%d", i);    // build filename string

        bool isDeleted = tree->deleteValue(filename);

        cout << filename << " is_deleted? " << isDeleted << "\n";
    }
    
    isDeleted = tree->deleteValue("10");

    DataNode * searchResult = tree->get("5");

    fclose(db_file);

    return 0;
}
