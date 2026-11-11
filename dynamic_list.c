#include "dynamic_list.h"
#include <stdio.h>
#include <stdlib.h>

bool list_init(DynamicList *list, size_t initial_capacity) {
    if (list == NULL) return false;
    if (initial_capacity == 0) {
        initial_capacity = DEFAULT_INITIAL_CAPACITY;
    }
    list->data = (int *)malloc(initial_capacity * sizeof(int));
    if (list->data == NULL) {
        list->size = 0;
        list->capacity = 0;
        return false;
    }
    list->size = 0;
    list->capacity = initial_capacity;
    return true;
}

bool list_resize(DynamicList *list, size_t new_capacity) {
    if (list == NULL || new_capacity < list->size) {
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

static bool list_ensure_capacity(DynamicList *list) {
    if (list->size == list->capacity) {
        size_t old_cap = list->capacity;
        size_t new_cap = (old_cap == 0) ? DEFAULT_INITIAL_CAPACITY : old_cap * GROWTH_FACTOR;
        if (!list_resize(list, new_cap)) {
            return false;
        }
    }
    return true;
}

bool list_add(DynamicList *list, int value) {
    if (list == NULL || !list_ensure_capacity(list)) {
        return false;
    }
    list->data[list->size++] = value;
    return true;
}

bool list_insert_at(DynamicList *list, size_t index, int value) {
    if (list == NULL || index > list->size) {
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

bool list_delete_at(DynamicList *list, size_t index, int *removed_value) {
    if (list == NULL || index >= list->size) {
        return false;
    }
    if (removed_value != NULL) {
        *removed_value = list->data[index];
    }
    for (size_t i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->size--;

    // Dynamic shrink to avoid memory waste
    if (list->size > 0 && list->size <= list->capacity / 4 && list->capacity / 2 >= DEFAULT_INITIAL_CAPACITY) {
        list_resize(list, list->capacity / 2);
    }
    return true;
}

bool list_delete_value(DynamicList *list, int value) {
    if (list == NULL) return false;
    for (size_t i = 0; i < list->size; i++) {
        if (list->data[i] == value) {
            return list_delete_at(list, i, NULL);
        }
    }
    return false;
}

long long list_search(const DynamicList *list, int value) {
    if (list == NULL) return -1;
    for (size_t i = 0; i < list->size; i++) {
        if (list->data[i] == value) {
            return (long long)i;
        }
    }
    return -1;
}

bool list_get(const DynamicList *list, size_t index, int *out_value) {
    if (list == NULL || index >= list->size) {
        return false;
    }
    if (out_value != NULL) {
        *out_value = list->data[index];
    }
    return true;
}

bool list_set(DynamicList *list, size_t index, int new_value, int *old_value) {
    if (list == NULL || index >= list->size) {
        return false;
    }
    if (old_value != NULL) {
        *old_value = list->data[index];
    }
    list->data[index] = new_value;
    return true;
}

static int compare_int_asc(const void *a, const void *b) {
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

void list_sort(DynamicList *list) {
    if (list != NULL && list->size > 1) {
        qsort(list->data, list->size, sizeof(int), compare_int_asc);
    }
}

void list_reverse(DynamicList *list) {
    if (list == NULL || list->size < 2) return;
    size_t left = 0;
    size_t right = list->size - 1;
    while (left < right) {
        int temp = list->data[left];
        list->data[left] = list->data[right];
        list->data[right] = temp;
        left++;
        right--;
    }
}

void list_display(const DynamicList *list) {
    if (list == NULL) {
        printf("List is uninitialized (NULL).\n");
        return;
    }
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

void list_stats(const DynamicList *list) {
    if (list == NULL || list->size == 0) {
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

void list_free(DynamicList *list) {
    if (list != NULL) {
        if (list->data != NULL) {
            free(list->data);
            list->data = NULL;
        }
        list->size = 0;
        list->capacity = 0;
    }
}

