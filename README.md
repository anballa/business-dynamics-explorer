# Business Dynamics Explorer

A C++ application for analyzing U.S. business dynamics data using optimized data structures (HashMap and B-Tree) for efficient storage, retrieval, and analysis of economic indicators across states and years.

---

## 📊 Project Overview

The **Business Dynamics Explorer** enables users to explore job creation, firm establishment, and economic activity trends across all 50 U.S. states from 1978-2020. The system compares performance between Hash Table and B-Tree implementations for operations like search, insert, and delete.

### Key Features

- **Dual Data Structure Implementation**: HashMap and B-Tree for performance comparison
- **Interactive Menu System**: Search, insert, delete, and analyze records with ease
- **CSV Data Loading**: Supports real-world business dynamics datasets
- **Synthetic Data Generation**: Automatically generates additional random records for testing
- **Advanced Analytics**: Top/bottom rankings, state-level aggregations, dataset statistics
- **Performance Benchmarking**: Compare operation times between data structures

---

## 🚀 Setup and Installation

### Prerequisites

- C++ compiler with C++17 support (g++, clang++, MSVC)
- Standard Template Library (STL)
- CSV dataset file: `bds_data.csv`

### Installation Steps

1. **Clone or download the project files**

```bash
# Ensure all source files are in the same directory:
# main.cpp, HashMap.h, HashMap.cpp, BTree.h, BTree.cpp
# Record.h, utils.h, utils.cpp, bds_data.csv
```

2. **Compile the project**

```bash
g++ -std=c++17 -o BusinessDynamicsExplorer main.cpp HashMap.cpp BTree.cpp utils.cpp
```

3. **Run the application**

```bash
./BusinessDynamicsExplorer
```

---

## 📁 Project Structure

```
BusinessDynamicsExplorer/
├── main.cpp              # Entry point, initializes data structures and menu
├── HashMap.h/cpp         # Hash table implementation with chaining
├── BTree.h/cpp           # B-Tree implementation for ordered data
├── Record.h              # Record structure definition
├── utils.h/cpp           # CSV parsing, data generation, menu functions
└── bds_data.csv          # Business dynamics dataset (optional)
```

---

## 💾 Data Structure

### Record Fields

Each record contains the following economic indicators:

| Field | Type | Description |
|-------|------|-------------|
| `state` | string | U.S. state name |
| `year` | int | Year of record (1978-2020) |
| `dhsDenominator` | int | DHS employment denominator |
| `numberOfFirms` | int | Total number of firms |
| `netJobCreation` | int | Net job creation count |
| `netJobCreationRate` | float | Net job creation rate (%) |
| `reallocationRate` | float | Job reallocation rate (%) |
| `establishmentsEntered` | int | Number of new establishments |
| `enteredRate` | float | Entry rate (%) |
| `establishmentsExited` | int | Number of closed establishments |
| `exitedRate` | float | Exit rate (%) |
| `physicalLocations` | int | Number of physical locations |
| `firmExits` | int | Number of firm exits |
| `jobCreation` | int | Total job creation |
| `jobCreationRate` | float | Job creation rate (%) |
| `jobDestruction` | int | Total job destruction |
| `jobDestructionRate` | float | Job destruction rate (%) |

---

## 🎯 Features & Usage

### Main Menu Options

```
[1] Insert New Record       - Add a new business dynamics record
[2] Search by State/Year    - Find specific record with performance metrics
[3] Delete Record           - Remove record from both data structures
[4] Show All Records        - Display all records for a specific state
[5] Top/Bottom 5 Rankings   - View top/bottom states by job creation
[6] Dataset Statistics      - View comprehensive dataset analytics
[7] Compare Data Structures - Benchmark HashMap vs B-Tree performance
[8] Exit                    - Quit the application
```

### Example Workflows

#### Search for a Record
```
Enter choice: 2
Enter State: California
Enter Year: 2015

--- Record Found ---
State: California
Year: 2015
Number of Firms: 987654
Net Job Creation: 450000
Net Job Creation Rate: 12.34%

Search Time (Hash Table): 0.003 ms
Search Time (BTree): 0.012 ms
```

#### View State History
```
Enter choice: 4
Enter State: Texas

All Records for Texas
Year  Firms    NetJob    JobRate  ...
1978  12345    5000      8.5%
1979  13000    5200      8.7%
...
```

#### Performance Comparison
```
Enter choice: 7

Average Time per Operation (microseconds per op):
Operation          HashMap              BTree
----------------------------------------------------------
Search             0.002                0.008
Insert             0.003                0.015
Delete             0.004                0.018
```

---

## 🧠 Implementation Details

### HashMap Implementation
- **Hash Function**: DJB2 algorithm for string keys
- **Collision Resolution**: Separate chaining with linked lists
- **Average Complexity**: O(1) search, insert, delete
- **Key Format**: `"State_Year"` (e.g., `"California_2015"`)

### B-Tree Implementation
- **Order (t)**: 3 (minimum degree)
- **Properties**: Self-balancing, maintains sorted order
- **Complexity**: O(log n) search, insert, delete
- **Use Case**: Range queries, prefix searches, ordered traversal

### Data Loading
1. Attempts to load `bds_data.csv`
2. If file missing/incomplete, generates synthetic data
3. Total dataset: 100,000 records
4. Inserts into both HashMap and B-Tree simultaneously

---

## 📈 Analytics Features

### Dataset Statistics
- Total records and unique states
- Year range coverage
- Aggregate metrics (total firms, job creation/destruction)
- Average rates and state-level summaries

### Top/Bottom Rankings
- Identifies best/worst performing states by job creation
- Prevents duplicate states (shows best year per state)
- Displays comprehensive economic indicators

### Performance Benchmarking
- Tests 1,000 random operations
- Measures average time per operation
- Compares HashMap vs B-Tree efficiency

---

## 🔧 Technical Requirements

- **C++ Standard**: C++17 or higher
- **Memory**: ~50MB for 100,000 records
- **Disk Space**: Minimal (CSV file ~10-20MB)
- **OS**: Cross-platform (Windows, macOS, Linux)

---

## 📝 Data Source

This application is designed to work with business dynamics datasets such as those from:
- U.S. Census Bureau Business Dynamics Statistics
- Bureau of Labor Statistics employment data
- State-level economic indicators

**CSV Format Expected:**
```csv
State,Year,DHS_Denominator,Number_of_Firms,Net_Job_Creation,...
Alabama,2015,500000,45000,1200,2.4,...
```

---

## 🛠️ Future Enhancements

- Multi-threaded data loading for large datasets
- Export results to CSV/JSON
- Visualization with charts and graphs
- Year-over-year trend analysis
- Multi-state comparison tools
- Database integration (SQLite/PostgreSQL)

---

## 🙏 Acknowledgements

- U.S. Census Bureau for business dynamics data
- Standard Template Library (STL) documentation
- C++ community for data structure best practices
