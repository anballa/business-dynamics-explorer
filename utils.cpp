//
// Created by anany on 11/3/2025.
//

#include "utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <algorithm>

using namespace std::chrono;
using namespace std;

vector<string> stateList = {
    "Alabama","Alaska","Arizona","Arkansas","California","Colorado","Connecticut","Delaware","Florida","Georgia",
    "Hawaii","Idaho","Illinois","Indiana","Iowa","Kansas","Kentucky","Louisiana","Maine","Maryland",
    "Massachusetts","Michigan","Minnesota","Mississippi","Missouri","Montana","Nebraska","Nevada",
    "New Hampshire","New Jersey","New Mexico","New York","North Carolina","North Dakota","Ohio",
    "Oklahoma","Oregon","Pennsylvania","Rhode Island","South Carolina","South Dakota","Tennessee",
    "Texas","Utah","Vermont","Virginia","Washington","West Virginia","Wisconsin","Wyoming"
};

// Convert CSV line into Record
Record parseRecord(const string &line) {
    Record r;
    stringstream ss(line);
    string field;

    getline(ss, r.state, ',');              // State
    getline(ss, field, ','); r.year = stoi(field);
    getline(ss, field, ','); r.dhsDenominator = stoi(field);
    getline(ss, field, ','); r.numberOfFirms = stoi(field);
    getline(ss, field, ','); r.netJobCreation = stoi(field);
    getline(ss, field, ','); r.netJobCreationRate = stof(field);

    return r;
}

// Read data from CSV, insert into both HashMap and BTree
void loadDataFromCSV(const string &filename, HashMap &hashTable, BTree &bTree) {
    cout << "Reading CSV file: " << filename << endl;
    cout << "Current working directory: " << filesystem::current_path() << endl;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open file " << filename << endl;
        cout << "Generating random dataset instead..." << endl;
        generateRandomData(hashTable, bTree, 100000);
        return;
    }

    string line;
    getline(file, line); // Skip header
    int count = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;
        Record r = parseRecord(line);
        string key = r.state + "_" + to_string(r.year);
        hashTable.insert(key, r);
        bTree.insert(key, r);
        count++;
    }

    cout << "Loaded " << count << " records from CSV." << endl;

    if (count < 100000) {
        cout << "Generating " << (100000 - count) << " additional random records..." << endl;
        generateRandomData(hashTable, bTree, 100000 - count);
    }

    file.close();
    cout << "All data ready (" << 100000 << " total)." << endl;
}

// Generate synthetic records to fill up dataset
void generateRandomData(HashMap &hashTable, BTree &bTree, int count) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> yearDist(1978, 2020);
    uniform_int_distribution<> dhsDist(50000, 1000000);
    uniform_int_distribution<> firmDist(500, 100000);
    uniform_int_distribution<> jobDist(1000, 500000);
    uniform_real_distribution<> rateDist(0.1, 20.0);
    uniform_int_distribution<> stateDist(0, stateList.size() - 1);

    for (int i = 0; i < count; i++) {
        Record r;
        r.state = stateList[stateDist(gen)];
        r.year = yearDist(gen);
        r.dhsDenominator = dhsDist(gen);
        r.numberOfFirms = firmDist(gen);
        r.netJobCreation = jobDist(gen);
        r.netJobCreationRate = rateDist(gen);

        string key = r.state + "_" + to_string(r.year);
        hashTable.insert(key, r);
        bTree.insert(key, r);
    }
}

// Simple interactive menu
void mainMenu(HashMap &hashTable, BTree &bTree) {
    int choice;
    string state;
    int year;

    while (true) {
        cout << "\n--- Business Dynamics Data Explorer ---\n";
        cout << "[1] Search Record by State and Year\n";
        cout << "[2] Insert New Record\n";
        cout << "[3] Delete Record\n";
        cout << "[4] Display All Records\n";
        cout << "[5] Compare HashMap vs BTree Performance\n";
        cout << "[6] Exit\n";
        cout << "Enter choice: ";
        cin >> choice;


        if (choice == 1) {
            cout << "Enter State: ";
            cin >> ws;
            getline(cin, state);
            cout << "Enter Year: ";
            cin >> year;
            {
                string key = state + "_" + to_string(year);
                Record *rec = hashTable.search(key);
                if (rec)
                    cout << "Found: " << rec->state << " (" << rec->year << "), Net Job Creation Rate: "
                         << rec->netJobCreationRate << endl;
                else
                    cout << "Record not found." << endl;
            }
        }
        else if (choice == 2) {
            Record r;
            cout << "Enter State: "; cin >> ws; getline(cin, r.state);
            cout << "Enter Year: "; cin >> r.year;
            cout << "Enter DHS Denominator: "; cin >> r.dhsDenominator;
            cout << "Enter Number of Firms: "; cin >> r.numberOfFirms;
            cout << "Enter Net Job Creation: "; cin >> r.netJobCreation;
            cout << "Enter Net Job Creation Rate: "; cin >> r.netJobCreationRate;

            string key = r.state + "_" + to_string(r.year);
            hashTable.insert(key, r);
            bTree.insert(key, r);
            cout << "Record inserted successfully." << endl;
        }
        else if (choice == 3) {
            cout << "Enter State: "; cin >> ws; getline(cin, state);
            cout << "Enter Year: "; cin >> year;
            string key = state + "_" + to_string(year);
            hashTable.remove(key);
            bTree.remove(key);
            cout << "Record deleted successfully from both structures.\n";
        }
        else if (choice == 4) {
            hashTable.display();
        }
        else if (choice == 5) {
            comparePerformance(hashTable, bTree);
        }
        else if (choice == 6) {
            cout << "Exiting program..." << endl;
            break;
        }
    }
}


void comparePerformance(HashMap &hashTable, BTree &bTree) {
    cout << "\n--- Performance Comparison ---\n";

    vector<string> keys = hashTable.getAllKeys();
    if (keys.empty()) {
        cout << "No data available to test.\n";
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    shuffle(keys.begin(), keys.end(), gen);

    int testCount = min((int)keys.size(), 1000);
    cout << "Testing with " << testCount << " records...\n\n";

    // ===== SEARCH TEST =====
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < testCount; ++i) {
        hashTable.search(keys[i]);
    }
    auto end = chrono::high_resolution_clock::now();
    double hashSearch = chrono::duration_cast<chrono::microseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < testCount; ++i) {
        bTree.search(keys[i]);
    }
    end = chrono::high_resolution_clock::now();
    double btreeSearch = chrono::duration_cast<chrono::microseconds>(end - start).count();

    // ===== INSERT TEST =====
    vector<Record> inserts;
    uniform_int_distribution<> yearDist(1978, 2020);
    uniform_int_distribution<> dhsDist(50000, 1000000);
    uniform_int_distribution<> firmDist(500, 100000);
    uniform_int_distribution<> jobDist(1000, 500000);
    uniform_real_distribution<> rateDist(0.1, 20.0);
    vector<string> states = {
        "Alabama","Alaska","Arizona","Arkansas","California","Colorado","Connecticut","Delaware",
        "Florida","Georgia","Hawaii","Idaho","Illinois","Indiana","Iowa","Kansas","Kentucky","Louisiana",
        "Maine","Maryland","Massachusetts","Michigan","Minnesota","Mississippi","Missouri","Montana",
        "Nebraska","Nevada","New Hampshire","New Jersey","New Mexico","New York","North Carolina",
        "North Dakota","Ohio","Oklahoma","Oregon","Pennsylvania","Rhode Island","South Carolina",
        "South Dakota","Tennessee","Texas","Utah","Vermont","Virginia","Washington","West Virginia",
        "Wisconsin","Wyoming"
    };
    uniform_int_distribution<> stateDist(0, states.size() - 1);

    for (int i = 0; i < testCount; ++i) {
        Record r;
        r.state = states[stateDist(gen)];
        r.year = yearDist(gen);
        r.dhsDenominator = dhsDist(gen);
        r.numberOfFirms = firmDist(gen);
        r.netJobCreation = jobDist(gen);
        r.netJobCreationRate = rateDist(gen);
        inserts.push_back(r);
    }

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < testCount; ++i) {
        string key = inserts[i].state + "_" + to_string(inserts[i].year);
        hashTable.insert(key, inserts[i]);
    }
    end = chrono::high_resolution_clock::now();
    double hashInsert = chrono::duration_cast<chrono::microseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < testCount; ++i) {
        string key = inserts[i].state + "_" + to_string(inserts[i].year);
        bTree.insert(key, inserts[i]);
    }
    end = chrono::high_resolution_clock::now();
    double btreeInsert = chrono::duration_cast<chrono::microseconds>(end - start).count();

    // ===== DELETE TEST =====
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < testCount; ++i) {
        hashTable.remove(keys[i]);
    }
    end = chrono::high_resolution_clock::now();
    double hashDelete = chrono::duration_cast<chrono::microseconds>(end - start).count();

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < testCount; ++i) {
        bTree.remove(keys[i]);
    }
    end = chrono::high_resolution_clock::now();
    double btreeDelete = chrono::duration_cast<chrono::microseconds>(end - start).count();

    // ===== DISPLAY RESULTS =====
    cout << fixed << setprecision(3);
    cout << "Average Time per Operation (microseconds per op):\n";
    cout << left << setw(20) << "Operation"
         << setw(20) << "HashMap"
         << setw(20) << "BTree" << endl;
    cout << string(60, '-') << endl;
    cout << left << setw(20) << "Search"
         << setw(20) << (hashSearch / testCount)
         << setw(20) << (btreeSearch / testCount) << endl;
    cout << left << setw(20) << "Insert"
         << setw(20) << (hashInsert / testCount)
         << setw(20) << (btreeInsert / testCount) << endl;
    cout << left << setw(20) << "Delete"
         << setw(20) << (hashDelete / testCount)
         << setw(20) << (btreeDelete / testCount) << endl;
}

