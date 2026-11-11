#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H

#include <stddef.h>
#include <stdbool.h>

#define DEFAULT_INITIAL_CAPACITY 2
#define GROWTH_FACTOR 2

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} DynamicList;

/**
 * Initializes the dynamic list with the specified initial capacity.
 * If initial_capacity is 0, DEFAULT_INITIAL_CAPACITY (2) is used.
 * Returns true on success, false if memory allocation failed.
 */
bool list_init(DynamicList *list, size_t initial_capacity);

/**
 * Resizes the internal array to the specified new capacity.
 * Returns true on success, false on failure or if new_capacity < current size.
 */
bool list_resize(DynamicList *list, size_t new_capacity);

/**
 * Appends an integer value to the end of the list.
 * Automatically doubles capacity if the array is full.
 * Returns true on success, false on memory allocation failure.
 */
bool list_add(DynamicList *list, int value);

/**
 * Inserts an integer value at the specified 0-based index.
 * Valid index range: 0 <= index <= size.
 * Automatically doubles capacity if array is full.
 * Returns true on success, false if index is out of bounds or allocation fails.
 */
bool list_insert_at(DynamicList *list, size_t index, int value);

/**
 * Deletes the element at the specified 0-based index.
 * Shifts subsequent elements left.
 * If removed_value is non-null, stores the removed element in it.
 * Shrinks capacity by half if size <= capacity / 4 (down to DEFAULT_INITIAL_CAPACITY).
 * Returns true on success, false if index is out of bounds.
 */
bool list_delete_at(DynamicList *list, size_t index, int *removed_value);

/**
 * Deletes the first occurrence of the specified value.
 * Returns true if the value was found and deleted, false otherwise.
 */
bool list_delete_value(DynamicList *list, int value);

/**
 * Searches for the first occurrence of the specified value.
 * Returns the 0-based index if found, or -1 if not found.
 */
long long list_search(const DynamicList *list, int value);

/**
 * Gets the element at index.
 * Returns true on success, false if index is out of bounds.
 */
bool list_get(const DynamicList *list, size_t index, int *out_value);

/**
 * Updates the element at index with a new value.
 * If old_value is non-null, stores the previous value.
 * Returns true on success, false if index is out of bounds.
 */
bool list_set(DynamicList *list, size_t index, int new_value, int *old_value);

/**
 * Sorts all elements in ascending order in-place using quicksort.
 */
void list_sort(DynamicList *list);

/**
 * Reverses all elements in-place.
 */
void list_reverse(DynamicList *list);

/**
 * Displays the current size, capacity, and formatted elements of the list.
 */
void list_display(const DynamicList *list);

/**
 * Computes and prints summary statistics (count, min, max, sum, average).
 */
void list_stats(const DynamicList *list);

/**
 * Frees all heap memory associated with the dynamic list and resets fields.
 */
void list_free(DynamicList *list);

#endif // DYNAMIC_LIST_H

