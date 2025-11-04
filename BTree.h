//
// Created by anany on 11/3/2025.
//

// BTree.h
#ifndef BTREE_H
#define BTREE_H

#include "Record.h"
#include <string>
#include <vector>
using namespace std;

class BTreeNode {
public:
    bool leaf;
    vector<string> keys;
    vector<Record> values;
    vector<BTreeNode*> children;
    int t;

    BTreeNode(int _t, bool _leaf);
    void insertNonFull(const string& key, const Record& value);
    void splitChild(int i, BTreeNode* y);
    BTreeNode* search(const string& key);
    void traverse();
    void remove(const string& key);
    int findKey(const string& key);
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    string getPredecessor(int idx);
    string getSuccessor(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);
};

class BTree {
public:
    BTreeNode* root;
    int t;
    BTree(int _t);
    void insert(const string& key, const Record& value);
    Record* search(const string& key);
    void traverse();
    void remove(const string& key);
};

#endif

