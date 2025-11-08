//
// Created by anany on 11/3/2025.
//

#ifndef HASHMAP_H
#define HASHMAP_H

#include "Record.h"
#include <vector>
#include <list>
#include <string>
using namespace std;

class HashMap {
private:
    vector<list<pair<string, Record>>> table;
    int capacity;
    int hashFunc(const string &key);

public:
    HashMap(int size);
    void insert(const string &key, const Record &record);
    Record* search(const string &key);
    void remove(const string &key);
    void display();
    std::vector<std::string> getAllKeys() const;
    std::vector<std::pair<std::string, Record>> searchPrefix(const std::string& prefix) const;

};

#endif

