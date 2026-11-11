#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 2
#define GROWTH_FACTOR 2

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} DynamicList;

// Initialize the list with dynamic memory
bool list_init(DynamicList *list, size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = INITIAL_CAPACITY;
    }
    list->data = (int *)malloc(initial_capacity * sizeof(int));
    if (list->data == NULL) {
        return false;
    }
    list->size = 0;
    list->capacity = initial_capacity;
    return true;
}

// Resize the array to new capacity using realloc
bool list_resize(DynamicList *list, size_t new_capacity) {
    if (new_capacity < list->size) {
        return false;
    }
    int *new_data = (int *)realloc(list->data, new_capacity * sizeof(int));
    if (new_data == NULL) {
        return false;
    }
    list->data = new_data;
    list->capacity = new_capacity;
    return true;
}

// Ensure capacity is available for at least one more element
static bool list_ensure_capacity(DynamicList *list) {
    if (list->size == list->capacity) {
        size_t old_cap = list->capacity;
        size_t new_cap = (old_cap == 0) ? INITIAL_CAPACITY : old_cap * GROWTH_FACTOR;
        if (!list_resize(list, new_cap)) {
            printf("[Error] Dynamic memory allocation failed during resize.\n");
            return false;
        }
        printf("[Info] Dynamic resize: capacity expanded from %zu to %zu.\n", old_cap, list->capacity);
    }
    return true;
}

// Add an element to the end of the list
bool list_add(DynamicList *list, int value) {
    if (!list_ensure_capacity(list)) {
        return false;
    }
    list->data[list->size++] = value;
    return true;
}

// Insert an element at a specific index (0 <= index <= size)
bool list_insert_at(DynamicList *list, size_t index, int value) {
    if (index > list->size) {
        return false;
    }
    if (!list_ensure_capacity(list)) {
        return false;
    }
    for (size_t i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }
    list->data[index] = value;
    list->size++;
    return true;
}

// Delete an element by its index (0-based)
bool list_delete_at(DynamicList *list, size_t index, int *removed_value) {
    if (index >= list->size) {
        return false;
    }
    if (removed_value != NULL) {
        *removed_value = list->data[index];
    }
    for (size_t i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->size--;

    // Reclaim memory when list is sparse (size <= capacity / 4)
    if (list->size > 0 && list->size <= list->capacity / 4 && list->capacity / 2 >= INITIAL_CAPACITY) {
        size_t old_cap = list->capacity;
        size_t new_cap = list->capacity / 2;
        if (list_resize(list, new_cap)) {
            printf("[Info] Dynamic resize: capacity shrunk from %zu to %zu.\n", old_cap, list->capacity);
        }
    }
    return true;
}

// Delete the first occurrence of a specific value
bool list_delete_value(DynamicList *list, int value) {
    for (size_t i = 0; i < list->size; i++) {
        if (list->data[i] == value) {
            return list_delete_at(list, i, NULL);
        }
    }
    return false;
}

// Search for a value, returns index if found, or -1 if not found
long long list_search(const DynamicList *list, int value) {
    for (size_t i = 0; i < list->size; i++) {
        if (list->data[i] == value) {
            return (long long)i;
        }
    }
    return -1;
}

// Update value at a specific index
bool list_set(DynamicList *list, size_t index, int new_value, int *old_value) {
    if (index >= list->size) {
        return false;
    }
    if (old_value != NULL) {
        *old_value = list->data[index];
    }
    list->data[index] = new_value;
    return true;
}

// In-place reverse
void list_reverse(DynamicList *list) {
    if (list->size < 2) return;
    size_t i = 0, j = list->size - 1;
    while (i < j) {
        int temp = list->data[i];
        list->data[i] = list->data[j];
        list->data[j] = temp;
        i++;
        j--;
    }
}

// Comparison function for qsort (ascending)
static int cmp_asc(const void *a, const void *b) {
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

// Sort elements in ascending order
void list_sort(DynamicList *list) {
    if (list->size > 1) {
        qsort(list->data, list->size, sizeof(int), cmp_asc);
    }
}

// Display all elements and current capacity
void list_display(const DynamicList *list) {
    printf("\n--- Current List ---\n");
    printf("Size: %zu / Capacity: %zu\n", list->size, list->capacity);
    if (list->size == 0) {
        printf("Elements: [ ] (List is empty)\n");
    } else {
        printf("Elements: [ ");
        for (size_t i = 0; i < list->size; i++) {
            printf("%d%s", list->data[i], (i + 1 < list->size) ? ", " : " ");
        }
        printf("]\n");
    }
    printf("--------------------\n");
}

// Display summary statistics
void list_stats(const DynamicList *list) {
    if (list->size == 0) {
        printf("List is empty! No statistics available.\n");
        return;
    }
    int min_val = list->data[0];
    int max_val = list->data[0];
    long long sum = 0;

    for (size_t i = 0; i < list->size; i++) {
        int v = list->data[i];
        if (v < min_val) min_val = v;
        if (v > max_val) max_val = v;
        sum += v;
    }

    double avg = (double)sum / (double)list->size;
    printf("\n--- List Statistics ---\n");
    printf("Count:   %zu\n", list->size);
    printf("Minimum: %d\n", min_val);
    printf("Maximum: %d\n", max_val);
    printf("Sum:     %lld\n", sum);
    printf("Average: %.2f\n", avg);
    printf("-----------------------\n");
}

// Free dynamically allocated memory
void list_free(DynamicList *list) {
    if (list->data != NULL) {
        free(list->data);
        list->data = NULL;
    }
    list->size = 0;
    list->capacity = 0;
}

// Helper to flush leftover characters from stdin
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main(void) {
    DynamicList list;
    if (!list_init(&list, INITIAL_CAPACITY)) {
        fprintf(stderr, "Fatal error: Initial dynamic memory allocation failed.\n");
        return 1;
    }

    printf("=========================================\n");
    printf("   DYNAMIC INTEGER LIST MANAGER (C)     \n");
    printf("=========================================\n");
    printf("Initial capacity: %d\n", INITIAL_CAPACITY);

    int choice;
    while (1) {
        printf("\n================ Operations Menu ================\n");
        printf("  1. Add integer (Append)\n");
        printf("  2. Insert integer at index\n");
        printf("  3. Delete element by index\n");
        printf("  4. Delete element by value\n");
        printf("  5. Display list elements & capacity\n");
        printf("  6. Search for an element\n");
        printf("  7. Update element at index\n");
        printf("  8. Sort list (ascending)\n");
        printf("  9. Reverse list\n");
        printf(" 10. Show statistics (min, max, sum, avg)\n");
        printf(" 11. Exit\n");
        printf("=================================================\n");
        printf("Enter your choice (1-11): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clear_input_buffer();
            continue;
        }

        switch (choice) {
            case 1: {
                int val;
                printf("Enter integer to add: ");
                if (scanf("%d", &val) != 1) {
                    printf("Invalid input! Please enter a valid integer.\n");
                    clear_input_buffer();
                    break;
                }
                if (list_add(&list, val)) {
                    printf("Successfully added %d.\n", val);
                }
                break;
            }
            case 2: {
                size_t idx;
                int val;
                printf("Enter index to insert at (0 to %zu): ", list.size);
                if (scanf("%zu", &idx) != 1) {
                    printf("Invalid index!\n");
                    clear_input_buffer();
                    break;
                }
                printf("Enter integer to insert: ");
                if (scanf("%d", &val) != 1) {
                    printf("Invalid integer!\n");
                    clear_input_buffer();
                    break;
                }
                if (list_insert_at(&list, idx, val)) {
                    printf("Successfully inserted %d at index %zu.\n", val, idx);
                } else {
                    printf("Error: Index %zu is out of bounds!\n", idx);
                }
                break;
            }
            case 3: {
                if (list.size == 0) {
                    printf("List is empty! Nothing to delete.\n");
                    break;
                }
                size_t idx;
                printf("Enter index to delete (0 to %zu): ", list.size - 1);
                if (scanf("%zu", &idx) != 1) {
                    printf("Invalid index format.\n");
                    clear_input_buffer();
                    break;
                }
                int removed;
                if (list_delete_at(&list, idx, &removed)) {
                    printf("Successfully deleted element %d at index %zu.\n", removed, idx);
                } else {
                    printf("Error: Index %zu is out of bounds!\n", idx);
                }
                break;
            }
            case 4: {
                if (list.size == 0) {
                    printf("List is empty! Nothing to delete.\n");
                    break;
                }
                int val;
                printf("Enter value to delete: ");
                if (scanf("%d", &val) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                if (list_delete_value(&list, val)) {
                    printf("Successfully deleted first occurrence of %d.\n", val);
                } else {
                    printf("Value %d not found in the list.\n", val);
                }
                break;
            }
            case 5:
                list_display(&list);
                break;
            case 6: {
                if (list.size == 0) {
                    printf("List is empty!\n");
                    break;
                }
                int val;
                printf("Enter value to search for: ");
                if (scanf("%d", &val) != 1) {
                    printf("Invalid input!\n");
                    clear_input_buffer();
                    break;
                }
                long long idx = list_search(&list, val);
                if (idx != -1) {
                    printf("Element %d found at index %lld.\n", val, idx);
                } else {
                    printf("Element %d was not found in the list.\n", val);
                }
                break;
            }
            case 7: {
                if (list.size == 0) {
                    printf("List is empty!\n");
                    break;
                }
                size_t idx;
                int new_val, old_val;
                printf("Enter index to update (0 to %zu): ", list.size - 1);
                if (scanf("%zu", &idx) != 1) {
                    printf("Invalid index!\n");
                    clear_input_buffer();
                    break;
                }
                printf("Enter new value: ");
                if (scanf("%d", &new_val) != 1) {
                    printf("Invalid integer!\n");
                    clear_input_buffer();
                    break;
                }
                if (list_set(&list, idx, new_val, &old_val)) {
                    printf("Successfully updated index %zu: changed from %d to %d.\n", idx, old_val, new_val);
                } else {
                    printf("Error: Index %zu is out of bounds!\n", idx);
                }
                break;
            }
            case 8:
                if (list.size == 0) {
                    printf("List is empty!\n");
                } else {
                    list_sort(&list);
                    printf("List sorted in ascending order.\n");
                    list_display(&list);
                }
                break;
            case 9:
                if (list.size == 0) {
                    printf("List is empty!\n");
                } else {
                    list_reverse(&list);
                    printf("List reversed successfully.\n");
                    list_display(&list);
                }
                break;
            case 10:
                list_stats(&list);
                break;
            case 11:
                printf("Freeing dynamically allocated memory and exiting...\n");
                list_free(&list);
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice! Please select an option between 1 and 11.\n");
                break;
        }
    }

    list_free(&list);
    return 0;
}
