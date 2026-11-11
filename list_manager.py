import sys

class DynamicIntList:
    def __init__(self, initial_capacity=2):
        self._capacity = max(1, initial_capacity)
        self._size = 0
        # Simulating raw fixed-size array in memory
        self._data = [None] * self._capacity

    def _resize(self, new_capacity):
        new_data = [None] * new_capacity
        for i in range(self._size):
            new_data[i] = self._data[i]
        self._data = new_data
        self._capacity = new_capacity

    def add(self, value):
        if self._size == self._capacity:
            old_cap = self._capacity
            self._resize(self._capacity * 2)
            print(f"[Info] Dynamic resize: capacity expanded from {old_cap} to {self._capacity}.")
        self._data[self._size] = value
        self._size += 1

    def delete_at(self, index):
        if index < 0 or index >= self._size:
            return False, None
        removed = self._data[index]
        for i in range(index, self._size - 1):
            self._data[i] = self._data[i + 1]
        self._data[self._size - 1] = None
        self._size -= 1

        # Shrink condition
        if self._size > 0 and self._size <= self._capacity // 4 and self._capacity // 2 >= 2:
            old_cap = self._capacity
            self._resize(self._capacity // 2)
            print(f"[Info] Dynamic resize: capacity shrunk from old {old_cap} to {self._capacity}.")

        return True, removed

    def delete_value(self, value):
        for i in range(self._size):
            if self._data[i] == value:
                success, _ = self.delete_at(i)
                return success
        return False

    def display(self):
        print("\n--- Current List ---")
        print(f"Size: {self._size} / Capacity: {self._capacity}")
        if self._size == 0:
            print("Elements: [ ] (List is empty)")
        else:
            elements = [str(self._data[i]) for i in range(self._size)]
            print(f"Elements: [ {', '.join(elements)} ]")
        print("--------------------")

    @property
    def size(self):
        return self._size


def main():
    list_mgr = DynamicIntList(initial_capacity=2)
    print("=========================================")
    print(" DYNAMIC INTEGER LIST MANAGER (Python)   ")
    print("=========================================")
    print("Initial capacity: 2")

    while True:
        print("\nMenu:")
        print("  1. Add an integer")
        print("  2. Delete by index")
        print("  3. Delete by value")
        print("  4. Display list")
        print("  5. Exit")

        try:
            choice = input("Enter your choice (1-5): ").strip()
        except (EOFError, KeyboardInterrupt):
            print("\nExiting...")
            break

        if choice == '1':
            try:
                val = int(input("Enter integer to add: ").strip())
                list_mgr.add(val)
                print(f"Successfully added {val}.")
            except ValueError:
                print("Invalid input! Please enter a valid integer.")
        elif choice == '2':
            if list_mgr.size == 0:
                print("List is empty! Nothing to delete.")
                continue
            try:
                idx = int(input(f"Enter index to delete (0 to {list_mgr.size - 1}): ").strip())
                success, removed = list_mgr.delete_at(idx)
                if success:
                    print(f"Successfully deleted element {removed} at index {idx}.")
                else:
                    print(f"Error: Index {idx} is out of bounds!")
            except ValueError:
                print("Invalid input! Please enter a valid integer index.")
        elif choice == '3':
            if list_mgr.size == 0:
                print("List is empty! Nothing to delete.")
                continue
            try:
                val = int(input("Enter value to delete: ").strip())
                if list_mgr.delete_value(val):
                    print(f"Successfully deleted first occurrence of {val}.")
                else:
                    print(f"Value {val} not found in the list.")
            except ValueError:
                print("Invalid input! Please enter a valid integer.")
        elif choice == '4':
            list_mgr.display()
        elif choice == '5':
            print("Exiting program. Goodbye!")
            break
        else:
            print("Invalid choice. Please select an option between 1 and 5.")


if __name__ == '__main__':
    main()

