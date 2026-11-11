# Task 3: Dynamic Integer List Manager

A complete, production-grade **Integer List Manager** built using dynamically resized arrays and explicit dynamic memory management.

---

## 📌 Project Overview

This project implements a dynamic array (vector-like container) from scratch, satisfying all requirements of **Task 3**:
- **Add**: Appends integers with automatic geometric capacity doubling ($O(1)$ amortized).
- **Delete**: Supports deletion by 0-based index or by value with left-shifting ($O(n)$) and memory shrinking.
- **Display**: Visualizes current logical size, allocated physical capacity, and contents.
- **Extended Operations**: Arbitrary index insertion, in-place search, element update, ascending sorting (`qsort` / `std::sort`), in-place reversal, and summary statistics (min, max, sum, average).
- **Memory Safety**: No leaks — guaranteed via clean initialization, safe reallocation, and destruction (`free` / `delete[]`).

---

## 📁 Repository Structure

```
c:\arrays1\
├── dynamic_list.h         # C API header with complete function prototypes and documentation
├── dynamic_list.c         # C implementation of the dynamic array engine
├── main.c                 # C interactive CLI application
├── test_runner.c          # Automated unit test suite (47 assertions, 100% pass)
├── list_manager.c         # Self-contained single-file C solution (for single-file course submissions)
├── list_manager.cpp       # Object-oriented C++ implementation with RAII
├── list_manager.py        # Python reference implementation
├── Makefile               # MinGW makefile for compilation and automated testing
├── build.bat              # One-click Windows build and test script
└── README.md              # Project documentation and specifications
```

---

## 🧠 Memory Management Architecture

### 1. Geometric Growth (Doubling)
```
Initial:      [  _  ,  _  ]          (Size: 0, Capacity: 2)
Add 10, 20:   [ 10  , 20  ]          (Size: 2, Capacity: 2)  <- FULL
Add 30:       [ 10  , 20  , 30  ,  _  ] (Size: 3, Capacity: 4)  <- DOUBLED
```
- **Amortized Analysis**: Copying $n$ elements costs $O(n)$, but happens only once every $n$ insertions. The amortized time per insertion is:
  $$\frac{1}{n} \sum_{i=1}^{\log_2 n} 2^i = O(1)$$

### 2. Element Deletion & Left Shift
When deleting element at index $k$:
$$\text{data}[i] \leftarrow \text{data}[i+1] \quad \text{for } k \le i < \text{size} - 1$$
$$\text{size} \leftarrow \text{size} - 1$$

### 3. Graceful Memory Reclamation (Halving)
To avoid memory thrashing (repeatedly doubling and halving around a single boundary), the array only halves its capacity when:
$$\text{size} \le \frac{\text{capacity}}{4} \quad \text{and} \quad \frac{\text{capacity}}{2} \ge \text{INITIAL\_CAPACITY}$$

---

## ⏱️ Complexity Summary

| Operation | Function | Time Complexity | Space Complexity |
|---|---|:---:|:---:|
| Append | `list_add` | $O(1)$ amortized ($O(n)$ worst case) | $O(1)$ |
| Insert at Index | `list_insert_at` | $O(n)$ | $O(1)$ |
| Delete by Index | `list_delete_at` | $O(n)$ | $O(1)$ |
| Delete by Value | `list_delete_value` | $O(n)$ | $O(1)$ |
| Search | `list_search` | $O(n)$ | $O(1)$ |
| Access / Update | `list_get` / `list_set` | $O(1)$ | $O(1)$ |
| Sort | `list_sort` | $O(n \log n)$ | $O(\log n)$ |
| Reverse | `list_reverse` | $O(n)$ | $O(1)$ |
| Display | `list_display` | $O(n)$ | $O(1)$ |
| Statistics | `list_stats` | $O(n)$ | $O(1)$ |

---

## 🛠️ Build and Execution Guide

### Option A: Using Makefile (MinGW)
```powershell
# Build everything and run test suite
mingw32-make all
mingw32-make test

# Run interactive C app
.\list_manager_c.exe

# Run interactive C++ app
.\list_manager_cpp.exe
```

### Option B: Using Windows Batch Script
```powershell
.\build.bat
```

### Option C: Manual GCC Compilation
```powershell
# Compile modular C application
gcc -Wall -Wextra -std=c11 dynamic_list.c main.c -o list_manager_c.exe
.\list_manager_c.exe

# Compile and run automated unit tests
gcc -Wall -Wextra -std=c11 dynamic_list.c test_runner.c -o test_runner.exe
.\test_runner.exe

# Compile C++ application
g++ -Wall -Wextra -std=c++17 list_manager.cpp -o list_manager_cpp.exe
.\list_manager_cpp.exe
```

---

## 🧪 Test Suite Results

The automated test runner (`test_runner.c`) validates **47 assertions** across 9 test suites:
- ✅ **Initialization**: Correct size (0), initial capacity (2), valid pointer allocation.
- ✅ **Dynamic Expansion**: Consecutive insertions triggering capacity doubling ($2 \to 4 \to 8$).
- ✅ **Insert at Index**: Head, middle, and tail insertions with element shifting.
- ✅ **Delete by Index**: Proper left-shift, size reduction, bounds checking.
- ✅ **Delete by Value**: Removal of matching elements, handling non-existent values.
- ✅ **Dynamic Shrinking**: Memory reclamation when list occupancy falls below 25%.
- ✅ **Search & Update**: Index lookup and in-place replacement.
- ✅ **Sort & Reverse**: QuickSort ordering and two-pointer reversal.
- ✅ **Memory Cleanup**: Zero leaks after `list_free()`.
