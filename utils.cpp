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
#include <map>
#include <set>
#include <climits>
#include <unordered_map>

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
    getline(ss, field, ','); if (!field.empty()) r.reallocationRate = stof(field);
    getline(ss, field, ','); if (!field.empty()) r.establishmentsEntered = stoi(field);
    getline(ss, field, ','); if (!field.empty()) r.enteredRate = stof(field);
    getline(ss, field, ','); if (!field.empty()) r.establishmentsExited = stoi(field);
    getline(ss, field, ','); if (!field.empty()) r.exitedRate = stof(field);
    getline(ss, field, ','); if (!field.empty()) r.physicalLocations = stoi(field);
    getline(ss, field, ','); if (!field.empty()) r.firmExits = stoi(field);
    // Skip Data.Firm Exits.Establishment Exit and Data.Firm Exits.Employments
    getline(ss, field, ',');
    getline(ss, field, ',');
    getline(ss, field, ',');
    getline(ss, field, ','); if (!field.empty()) r.jobCreation = stoi(field);  // Data.Job Creation.Count
    getline(ss, field, ','); if (!field.empty()) r.jobCreationRate = stof(field);
    // Skip remaining fields or parse if needed
    getline(ss, field, ',');
    getline(ss, field, ',');
    getline(ss, field, ','); if (!field.empty()) r.jobDestruction = stoi(field);  // Data.Job Destruction.Count
    getline(ss, field, ','); if (!field.empty()) r.jobDestructionRate = stof(field);

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
    uniform_int_distribution<> enterExitDist(100, 10000);
    uniform_real_distribution<> reallocDist(10.0, 40.0);

    for (int i = 0; i < count; i++) {
        Record r;
        r.state = stateList[stateDist(gen)];
        r.year = yearDist(gen);
        r.dhsDenominator = dhsDist(gen);
        r.numberOfFirms = firmDist(gen);
        r.netJobCreation = jobDist(gen);
        r.netJobCreationRate = rateDist(gen);
        r.reallocationRate = reallocDist(gen);
        r.establishmentsEntered = enterExitDist(gen);
        r.enteredRate = rateDist(gen);
        r.establishmentsExited = enterExitDist(gen);
        r.exitedRate = rateDist(gen);
        r.physicalLocations = firmDist(gen);
        r.firmExits = enterExitDist(gen);
        r.jobCreation = jobDist(gen);
        r.jobCreationRate = rateDist(gen);
        r.jobDestruction = jobDist(gen);
        r.jobDestructionRate = rateDist(gen);

        string key = r.state + "_" + to_string(r.year);
        hashTable.insert(key, r);
        bTree.insert(key, r);
    }
}

// Main interactive menu
void mainMenu(HashMap &hashTable, BTree &bTree) {
    int choice;
    string state;
    int year;

    while (true) {
        cout << "\n========================================================================================================================\n";
        cout << "                                    BUSINESS DYNAMICS DATA EXPLORER\n";
        cout << "========================================================================================================================\n";
        cout << "Choose an option below:\n";
        cout << "[1] Insert New Record\n";
        cout << "[2] Search by State and Year\n";
        cout << "[3] Delete Record\n";
        cout << "[4] Show All Records for State\n";
        cout << "[5] Top/Bottom 5 by Job Creation\n";
        cout << "[6] Dataset Statistics\n";
        cout << "[7] Compare Data Structures\n";
        cout << "[8] Exit\n";
        cout << "========================================================================================================================\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            Record r;
            cout << "\n--- Insert New Record ---\n";
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
        else if (choice == 2) {
            cout << "\n--- Search Record by State and Year ---\n";
            cout << "Enter State: ";
            cin >> ws;
            getline(cin, state);
            cout << "Enter Year: ";
            cin >> year;
            
            string key = state + "_" + to_string(year);
            
            // Search in Hash Table
            auto start = high_resolution_clock::now();
            Record *recHash = hashTable.search(key);
            auto end = high_resolution_clock::now();
            double hashTime = duration_cast<microseconds>(end - start).count() / 1000.0;
            
            // Search in BTree
            start = high_resolution_clock::now();
            Record *recBTree = bTree.search(key);
            end = high_resolution_clock::now();
            double btreeTime = duration_cast<microseconds>(end - start).count() / 1000.0;
            
            if (recHash) {
                cout << "\n--- Record Found ---\n";
                cout << "State: " << recHash->state << "\n";
                cout << "Year: " << recHash->year << "\n";
                cout << "Number of Firms: " << recHash->numberOfFirms << "\n";
                cout << "Net Job Creation: " << recHash->netJobCreation << "\n";
                cout << "Net Job Creation Rate: " << fixed << setprecision(2) << recHash->netJobCreationRate << "%\n";
                cout << "\nSearch Time (Hash Table): " << fixed << setprecision(3) << hashTime << " ms\n";
                cout << "Search Time (BTree): " << fixed << setprecision(3) << btreeTime << " ms\n";
            } else {
                cout << "Record not found." << endl;
            }
        }
        else if (choice == 3) {
            cout << "\n--- Delete Record ---\n";
            cout << "Enter State: "; cin >> ws; getline(cin, state);
            cout << "Enter Year: "; cin >> year;
            string key = state + "_" + to_string(year);
            hashTable.remove(key);
            bTree.remove(key);
            cout << "Record deleted successfully from both structures.\n";
        }
        else if (choice == 4) {
            showAllRecordsForState(hashTable, bTree);
        }
        else if (choice == 5) {
            showTopBottomJobCreation(hashTable, bTree);
        }
        else if (choice == 6) {
            showDatasetStatistics(hashTable, bTree);
        }
        else if (choice == 7) {
            comparePerformance(hashTable, bTree);
        }
        else if (choice == 8) {
            cout << "Exiting program..." << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
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

void showAllRecordsForState(HashMap &hashTable, BTree &bTree) {
    string state;
    cout << "\n--- Show All Records for State ---\n";
    cout << "Enter State: ";
    cin >> ws;
    getline(cin, state);
    
    string prefix = state + "_";
    
    // Search in Hash Table
    auto start = high_resolution_clock::now();
    vector<pair<string, Record>> hashResults = hashTable.searchPrefix(prefix);
    auto end = high_resolution_clock::now();
    double hashTime = duration_cast<microseconds>(end - start).count() / 1000.0;
    
    // Search in BTree
    start = high_resolution_clock::now();
    vector<pair<string, Record>> btreeResults = bTree.searchPrefix(prefix);
    end = high_resolution_clock::now();
    double btreeTime = duration_cast<microseconds>(end - start).count() / 1000.0;
    
    // Sort results by year
    sort(hashResults.begin(), hashResults.end(), 
         [](const pair<string, Record>& a, const pair<string, Record>& b) {
             return a.second.year < b.second.year;
         });
    
    if (hashResults.empty()) {
        cout << "No records found for state: " << state << endl;
        return;
    }
    
    cout << "\n========================================================================================================================\n";
    cout << "                                    All Records for " << state << "\n";
    cout << "========================================================================================================================\n";
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
    
    for (const auto& entry : hashResults) {
        const Record &r = entry.second;
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
    
    cout << "========================================================================================================================\n";
    cout << "Total records found: " << hashResults.size() << "\n";
    cout << "Search Time (Hash Table): " << fixed << setprecision(3) << hashTime << " ms\n";
    cout << "Search Time (BTree): " << fixed << setprecision(3) << btreeTime << " ms\n";
}

void showTopBottomJobCreation(HashMap &hashTable, BTree &bTree) {
    cout << "\n--- Top/Bottom 5 by Job Creation ---\n";
    
    vector<string> keys = hashTable.getAllKeys();
    if (keys.empty()) {
        cout << "No data available.\n";
        return;
    }
    
    vector<pair<string, Record>> allRecords;
    for (const string& key : keys) {
        Record* rec = hashTable.search(key);
        if (rec) {
            allRecords.push_back({key, *rec});
        }
    }

    // Prevent duplicates
    unordered_map<string, Record> bestPerState;
    for (const auto& entry : allRecords) {
        const string& state = entry.second.state;
        // Keep the record with the highest jobCreation for each state
        if (!bestPerState.count(state) || entry.second.jobCreation > bestPerState[state].jobCreation) {
            bestPerState[state] = entry.second;
        }
    }

    // Rebuild allRecords with only one best record per state
    allRecords.clear();
    for (const auto& best : bestPerState) {
        allRecords.push_back({best.first, best.second});
    }
    
    // Sort by job creation (descending for top, ascending for bottom)
    sort(allRecords.begin(), allRecords.end(),
         [](const pair<string, Record>& a, const pair<string, Record>& b) {
             return a.second.jobCreation > b.second.jobCreation;
         });
    
    cout << "\n========================================================================================================================\n";
    cout << "                                    TOP 5 BY JOB CREATION\n";
    cout << "========================================================================================================================\n";
    cout << left
         << setw(15) << "State"
         << setw(8)  << "Year"
         << setw(15) << "Job Creation"
         << setw(20) << "Job Creation Rate"
         << setw(20) << "Net Job Creation "
         << setw(20) << "Number of Firms"
         << endl;
    cout << string(93, '-') << endl;
    
    int count = min(5, (int)allRecords.size());
    for (int i = 0; i < count; i++) {
        const Record &r = allRecords[i].second;
        cout << left << setw(15) << r.state
             << setw(8)  << r.year
             << setw(15) << r.jobCreation
             << setw(20) << fixed << setprecision(2) << r.jobCreationRate
             << setw(20) << r.netJobCreation
             << setw(20) << r.numberOfFirms
             << endl;
    }
    
    // Sort by job creation (ascending for bottom)
    sort(allRecords.begin(), allRecords.end(),
         [](const pair<string, Record>& a, const pair<string, Record>& b) {
             return a.second.jobCreation < b.second.jobCreation;
         });
    
    cout << "\n========================================================================================================================\n";
    cout << "                                    BOTTOM 5 BY JOB CREATION\n";
    cout << "========================================================================================================================\n";
    cout << left
         << setw(15) << "State"
         << setw(8)  << "Year"
         << setw(15) << "Job Creation"
         << setw(20) << "Job Creation Rate"
         << setw(20) << "Net Job Creation"
         << setw(20) << "Number of Firms"
         << endl;
    cout << string(93, '-') << endl;
    
    for (int i = 0; i < count; i++) {
        const Record &r = allRecords[i].second;
        cout << left << setw(15) << r.state
             << setw(8)  << r.year
             << setw(15) << r.jobCreation
             << setw(20) << fixed << setprecision(2) << r.jobCreationRate
             << setw(20) << r.netJobCreation
             << setw(20) << r.numberOfFirms
             << endl;
    }
    cout << "========================================================================================================================\n";
}

void showDatasetStatistics(HashMap &hashTable, BTree &bTree) {
    cout << "\n--- Dataset Statistics ---\n";
    
    vector<string> keys = hashTable.getAllKeys();
    if (keys.empty()) {
        cout << "No data available.\n";
        return;
    }
    
    vector<Record> allRecords;
    for (const string& key : keys) {
        Record* rec = hashTable.search(key);
        if (rec) {
            allRecords.push_back(*rec);
        }
    }
    
    if (allRecords.empty()) {
        cout << "No records found.\n";
        return;
    }
    
    // Calculate statistics
    int totalRecords = allRecords.size();
    int totalFirms = 0;
    long long totalJobCreation = 0;
    long long totalJobDestruction = 0;
    long long totalNetJobCreation = 0;
    double totalJobCreationRate = 0.0;
    set<string> uniqueStates;
    set<int> uniqueYears;
    int minYear = INT_MAX, maxYear = INT_MIN;
    
    for (const Record& r : allRecords) {
        totalFirms += r.numberOfFirms;
        totalJobCreation += r.jobCreation;
        totalJobDestruction += r.jobDestruction;
        totalNetJobCreation += r.netJobCreation;
        totalJobCreationRate += r.jobCreationRate;
        uniqueStates.insert(r.state);
        uniqueYears.insert(r.year);
        if (r.year < minYear) minYear = r.year;
        if (r.year > maxYear) maxYear = r.year;
    }
    
    double avgJobCreationRate = totalJobCreationRate / totalRecords;
    
    // Find state with most records
    map<string, int> stateCounts;
    for (const Record& r : allRecords) {
        stateCounts[r.state]++;
    }
    string mostRecordsState = "";
    int maxStateCount = 0;
    for (const auto& pair : stateCounts) {
        if (pair.second > maxStateCount) {
            maxStateCount = pair.second;
            mostRecordsState = pair.first;
        }
    }
    
    cout << "\n========================================================================================================================\n";
    cout << "                                    DATASET STATISTICS\n";
    cout << "========================================================================================================================\n";
    cout << left << setw(40) << "Total Records:" << right << setw(20) << totalRecords << "\n";
    cout << left << setw(40) << "Unique States:" << right << setw(20) << uniqueStates.size() << "\n";
    cout << left << setw(40) << "Year Range:" << right << setw(20) << (to_string(minYear) + " - " + to_string(maxYear)) << "\n";
    cout << left << setw(40) << "Total Number of Firms:" << right << setw(20) << totalFirms << "\n";
    cout << left << setw(40) << "Total Job Creation:" << right << setw(20) << totalJobCreation << "\n";
    cout << left << setw(40) << "Total Job Destruction:" << right << setw(20) << totalJobDestruction << "\n";
    cout << left << setw(40) << "Total Net Job Creation:" << right << setw(20) << totalNetJobCreation << "\n";
    cout << left << setw(40) << fixed << setprecision(2) << "Average Job Creation Rate:" << right << setw(20) << avgJobCreationRate << "%\n";
    cout << left << setw(40) << "State with Most Records:" << right << setw(20) << (mostRecordsState + " (" + to_string(maxStateCount) + " records)") << "\n";
    cout << "========================================================================================================================\n";
}

