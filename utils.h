//
// Created by anany on 11/3/2025.
//

#ifndef UTILS_H
#define UTILS_H

#include "HashMap.h"
#include "BTree.h"
#include "Record.h"
#include <string>

void loadDataFromCSV(const std::string &filename, HashMap &hashTable, BTree &bTree);
void generateRandomData(HashMap &hashTable, BTree &bTree, int count);
void mainMenu(HashMap &hashTable, BTree &bTree);
void comparePerformance(HashMap &hashTable, BTree &bTree);
void showAllRecordsForState(HashMap &hashTable, BTree &bTree);
void showTopBottomJobCreation(HashMap &hashTable, BTree &bTree);
void showDatasetStatistics(HashMap &hashTable, BTree &bTree);

#endif
