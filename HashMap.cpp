//
// Created by anany on 11/3/2025.
//

#include "HashMap.h"
#include <iostream>
#include <iomanip>
using namespace std;

HashMap::HashMap(int size) : capacity(size) {
    table.resize(size);
}

int HashMap::hashFunc(const string &key) {
    unsigned long hash = 5381;
    for (char c : key) hash = ((hash << 5) + hash) + c;
    return hash % capacity;
}

void HashMap::insert(const string &key, const Record &record) {
    int index = hashFunc(key);
    table[index].push_back({key, record});
}

Record* HashMap::search(const string &key) {
    int index = hashFunc(key);
    for (auto &p : table[index]) {
        if (p.first == key)
            return &p.second;
    }
    return nullptr;
}

void HashMap::remove(const string &key) {
    int index = hashFunc(key);
    for (auto it = table[index].begin(); it != table[index].end(); ++it) {
        if (it->first == key) {
            table[index].erase(it);
            return;
        }
    }
}


void HashMap::display() {
    cout << "\n============================================================================================================================\n";
    cout << "                                                   All Records\n";
    cout << "============================================================================================================================\n";

    cout << left
         << setw(12) << "State"
         << setw(6)  << "Year"
         << setw(10) << "Firms"
         << setw(12) << "NetJob"
         << setw(10) << "JobRate"
         << setw(12) << "Realloc"
         << setw(12) << "Entered"
         << setw(10) << "EntRate"
         << setw(10) << "Exited"
         << setw(10) << "ExRate"
         << setw(10) << "PhysLoc"
         << setw(10) << "FirmEx"
         << setw(12) << "JobCreate"
         << setw(10) << "CreateR"
         << setw(12) << "JobDestr"
         << setw(10) << "DestrR"
         << endl;

    cout << string(160, '-') << endl;

    for (int i = 0; i < capacity; ++i) {
        for (auto &p : table[i]) {
            const Record &r = p.second;
            cout << left << setw(12) << r.state
                 << setw(6)  << r.year
                 << setw(10) << r.numberOfFirms
                 << setw(12) << r.netJobCreation
                 << setw(10) << fixed << setprecision(2) << r.netJobCreationRate
                 << setw(12) << fixed << setprecision(2) << r.reallocationRate
                 << setw(12) << r.establishmentsEntered
                 << setw(10) << fixed << setprecision(2) << r.enteredRate
                 << setw(10) << r.establishmentsExited
                 << setw(10) << fixed << setprecision(2) << r.exitedRate
                 << setw(10) << r.physicalLocations
                 << setw(10) << r.firmExits
                 << setw(12) << r.jobCreation
                 << setw(10) << fixed << setprecision(2) << r.jobCreationRate
                 << setw(12) << r.jobDestruction
                 << setw(10) << fixed << setprecision(2) << r.jobDestructionRate
                 << endl;
        }
    }

    cout << "============================================================================================================================\n";
}

std::vector<std::string> HashMap::getAllKeys() const {
    std::vector<std::string> keys;
    for (const auto &bucket : table) {
        for (const auto &entry : bucket) {
            keys.push_back(entry.first);
        }
    }
    return keys;
}

std::vector<std::pair<std::string, Record>> HashMap::searchPrefix(const std::string& prefix) const {
    std::vector<std::pair<std::string, Record>> results;
    for (const auto &bucket : table) {
        for (const auto &entry : bucket) {
            if (entry.first.substr(0, prefix.length()) == prefix) {
                results.push_back(entry);
            }
        }
    }
    return results;
}

