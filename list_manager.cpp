#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <numeric>

class DynamicIntList {
private:
    int* data;
    size_t m_size;
    size_t m_capacity;

    void resize(size_t new_capacity) {
        if (new_capacity < m_size) return;
        
        int* new_data = new int[new_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        m_capacity = new_capacity;
    }

    void ensureCapacity() {
        if (m_size == m_capacity) {
            size_t old_cap = m_capacity;
            resize(m_capacity == 0 ? 2 : m_capacity * 2);
            std::cout << "[Info] Dynamic resize: capacity expanded from " << old_cap 
                      << " to " << m_capacity << ".\n";
        }
    }

public:
    DynamicIntList(size_t initial_capacity = 2) 
        : m_size(0), m_capacity(initial_capacity > 0 ? initial_capacity : 2) {
        data = new int[m_capacity];
    }

    ~DynamicIntList() {
        delete[] data;
        data = nullptr;
        m_size = 0;
        m_capacity = 0;
    }

    // Disable copy constructor and copy assignment
    DynamicIntList(const DynamicIntList&) = delete;
    DynamicIntList& operator=(const DynamicIntList&) = delete;

    void add(int value) {
        ensureCapacity();
        data[m_size++] = value;
    }

    bool insertAt(size_t index, int value) {
        if (index > m_size) return false;
        ensureCapacity();
        for (size_t i = m_size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        ++m_size;
        return true;
    }

    bool deleteAt(size_t index, int& removed_val) {
        if (index >= m_size) {
            return false;
        }
        removed_val = data[index];
        for (size_t i = index; i < m_size - 1; ++i) {
            data[i] = data[i + 1];
        }
        --m_size;

        // Shrink if size <= capacity / 4 (down to minimum capacity of 2)
        if (m_size > 0 && m_size <= m_capacity / 4 && m_capacity / 2 >= 2) {
            size_t old_cap = m_capacity;
            resize(m_capacity / 2);
            std::cout << "[Info] Dynamic resize: capacity shrunk from " << old_cap 
                      << " to " << m_capacity << ".\n";
        }
        return true;
    }

    bool deleteValue(int value) {
        for (size_t i = 0; i < m_size; ++i) {
            if (data[i] == value) {
                int dummy;
                return deleteAt(i, dummy);
            }
        }
        return false;
    }

    long long search(int value) const {
        for (size_t i = 0; i < m_size; ++i) {
            if (data[i] == value) {
                return static_cast<long long>(i);
            }
        }
        return -1;
    }

    bool set(size_t index, int new_value, int& old_value) {
        if (index >= m_size) return false;
        old_value = data[index];
        data[index] = new_value;
        return true;
    }

    void sortAscending() {
        if (m_size > 1) {
            std::sort(data, data + m_size);
        }
    }

    void reverseList() {
        if (m_size > 1) {
            std::reverse(data, data + m_size);
        }
    }

    void display() const {
        std::cout << "\n--- Current List ---\n";
        std::cout << "Size: " << m_size << " / Capacity: " << m_capacity << "\n";
        if (m_size == 0) {
            std::cout << "Elements: [ ] (List is empty)\n";
        } else {
            std::cout << "Elements: [ ";
            for (size_t i = 0; i < m_size; ++i) {
                std::cout << data[i] << (i + 1 < m_size ? ", " : " ");
            }
            std::cout << "]\n";
        }
        std::cout << "--------------------\n";
    }

    void showStats() const {
        if (m_size == 0) {
            std::cout << "List is empty! No statistics available.\n";
            return;
        }
        int min_val = *std::min_element(data, data + m_size);
        int max_val = *std::max_element(data, data + m_size);
        long long sum = std::accumulate(data, data + m_size, 0LL);
        double avg = static_cast<double>(sum) / m_size;

        std::cout << "\n--- List Statistics ---\n";
        std::cout << "Count:   " << m_size << "\n";
        std::cout << "Minimum: " << min_val << "\n";
        std::cout << "Maximum: " << max_val << "\n";
        std::cout << "Sum:     " << sum << "\n";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Average: " << avg << "\n";
        std::cout << "-----------------------\n";
    }

    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    bool isEmpty() const { return m_size == 0; }
};

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    DynamicIntList list(2);

    std::cout << "=========================================\n";
    std::cout << "  DYNAMIC INTEGER LIST MANAGER (C++)     \n";
    std::cout << "=========================================\n";
    std::cout << "Initial capacity: 2\n";

    int choice;
    while (true) {
        std::cout << "\n================ Operations Menu ================\n";
        std::cout << "  1. Add integer (Append)\n";
        std::cout << "  2. Insert integer at index\n";
        std::cout << "  3. Delete element by index\n";
        std::cout << "  4. Delete element by value\n";
        std::cout << "  5. Display list elements & capacity\n";
        std::cout << "  6. Search for an element\n";
        std::cout << "  7. Update element at index\n";
        std::cout << "  8. Sort list (ascending)\n";
        std::cout << "  9. Reverse list\n";
        std::cout << " 10. Show statistics (min, max, sum, avg)\n";
        std::cout << " 11. Exit\n";
        std::cout << "=================================================\n";
        std::cout << "Enter your choice (1-11): ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid input! Please enter a number.\n";
            clearInput();
            continue;
        }

        switch (choice) {
            case 1: {
                int val;
                std::cout << "Enter integer to add: ";
                if (!(std::cin >> val)) {
                    std::cout << "Invalid input! Please enter a valid integer.\n";
                    clearInput();
                    break;
                }
                list.add(val);
                std::cout << "Successfully added " << val << ".\n";
                break;
            }
            case 2: {
                size_t idx;
                int val;
                std::cout << "Enter index to insert at (0 to " << list.size() << "): ";
                if (!(std::cin >> idx)) {
                    std::cout << "Invalid index!\n";
                    clearInput();
                    break;
                }
                std::cout << "Enter integer to insert: ";
                if (!(std::cin >> val)) {
                    std::cout << "Invalid integer!\n";
                    clearInput();
                    break;
                }
                if (list.insertAt(idx, val)) {
                    std::cout << "Successfully inserted " << val << " at index " << idx << ".\n";
                } else {
                    std::cout << "Error: Index " << idx << " is out of bounds!\n";
                }
                break;
            }
            case 3: {
                if (list.isEmpty()) {
                    std::cout << "List is empty! Nothing to delete.\n";
                    break;
                }
                size_t idx;
                std::cout << "Enter index to delete (0 to " << list.size() - 1 << "): ";
                if (!(std::cin >> idx)) {
                    std::cout << "Invalid index format.\n";
                    clearInput();
                    break;
                }
                int removed;
                if (list.deleteAt(idx, removed)) {
                    std::cout << "Successfully deleted element " << removed << " at index " << idx << ".\n";
                } else {
                    std::cout << "Error: Index " << idx << " is out of bounds!\n";
                }
                break;
            }
            case 4: {
                if (list.isEmpty()) {
                    std::cout << "List is empty! Nothing to delete.\n";
                    break;
                }
                int val;
                std::cout << "Enter value to delete: ";
                if (!(std::cin >> val)) {
                    std::cout << "Invalid input!\n";
                    clearInput();
                    break;
                }
                if (list.deleteValue(val)) {
                    std::cout << "Successfully deleted first occurrence of " << val << ".\n";
                } else {
                    std::cout << "Value " << val << " not found in the list.\n";
                }
                break;
            }
            case 5:
                list.display();
                break;
            case 6: {
                if (list.isEmpty()) {
                    std::cout << "List is empty!\n";
                    break;
                }
                int val;
                std::cout << "Enter value to search for: ";
                if (!(std::cin >> val)) {
                    std::cout << "Invalid input!\n";
                    clearInput();
                    break;
                }
                long long idx = list.search(val);
                if (idx != -1) {
                    std::cout << "Element " << val << " found at index " << idx << ".\n";
                } else {
                    std::cout << "Element " << val << " was not found in the list.\n";
                }
                break;
            }
            case 7: {
                if (list.isEmpty()) {
                    std::cout << "List is empty!\n";
                    break;
                }
                size_t idx;
                int new_val, old_val;
                std::cout << "Enter index to update (0 to " << list.size() - 1 << "): ";
                if (!(std::cin >> idx)) {
                    std::cout << "Invalid index!\n";
                    clearInput();
                    break;
                }
                std::cout << "Enter new value: ";
                if (!(std::cin >> new_val)) {
                    std::cout << "Invalid integer!\n";
                    clearInput();
                    break;
                }
                if (list.set(idx, new_val, old_val)) {
                    std::cout << "Successfully updated index " << idx << ": changed from " << old_val << " to " << new_val << ".\n";
                } else {
                    std::cout << "Error: Index " << idx << " is out of bounds!\n";
                }
                break;
            }
            case 8:
                if (list.isEmpty()) {
                    std::cout << "List is empty!\n";
                } else {
                    list.sortAscending();
                    std::cout << "List sorted in ascending order.\n";
                    list.display();
                }
                break;
            case 9:
                if (list.isEmpty()) {
                    std::cout << "List is empty!\n";
                } else {
                    list.reverseList();
                    std::cout << "List reversed successfully.\n";
                    list.display();
                }
                break;
            case 10:
                list.showStats();
                break;
            case 11:
                std::cout << "Exiting program. Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice! Please select an option between 1 and 11.\n";
                break;
        }
    }

    return 0;
}
