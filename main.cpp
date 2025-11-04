#include <iostream>
#include "HashMap.h"
#include "BTree.h"
#include "Record.h"
#include "utils.h"
using namespace std;

int main() {
    cout << "Program started!" << endl;

    HashMap hashTable(10000);
    BTree bTree(3);

    string filename = "bds_data.csv";
    loadDataFromCSV(filename, hashTable, bTree);

    cout << "Data loaded successfully." << endl;
    mainMenu(hashTable, bTree);

    return 0;
}
