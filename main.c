#include "dynamic_list.h"
#include <stdio.h>
#include <stdlib.h>

static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main(void) {
    DynamicList list;
    if (!list_init(&list, DEFAULT_INITIAL_CAPACITY)) {
        fprintf(stderr, "Fatal error: Initial dynamic memory allocation failed.\n");
        return 1;
    }

    printf("=========================================\n");
    printf("   DYNAMIC INTEGER LIST MANAGER (C)     \n");
    printf("=========================================\n");
    printf("Initial capacity: %d\n", DEFAULT_INITIAL_CAPACITY);

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
                    printf("Successfully added %d. (Current size: %zu, capacity: %zu)\n", val, list.size, list.capacity);
                } else {
                    printf("Error: Failed to allocate memory to add %d.\n", val);
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
                    printf("Successfully deleted element %d at index %zu. (Remaining: %zu)\n", removed, idx, list.size);
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
                    printf("Successfully deleted first occurrence of %d. (Remaining: %zu)\n", val, list.size);
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

