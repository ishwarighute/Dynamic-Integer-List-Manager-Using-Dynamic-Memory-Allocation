#include "dynamic_list.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

static int total_tests = 0;
static int passed_tests = 0;

#define TEST_ASSERT(cond, msg) do { \
    total_tests++; \
    if (cond) { \
        passed_tests++; \
        printf("  [PASS] %s\n", msg); \
    } else { \
        printf("  [FAIL] %s (Line %d)\n", msg, __LINE__); \
    } \
} while(0)

void test_initialization() {
    printf("\n--- Test: Initialization ---\n");
    DynamicList list;
    bool ok = list_init(&list, 2);
    TEST_ASSERT(ok == true, "list_init succeeds");
    TEST_ASSERT(list.data != NULL, "Internal data pointer is not NULL");
    TEST_ASSERT(list.size == 0, "Initial size is 0");
    TEST_ASSERT(list.capacity == 2, "Initial capacity is 2");
    list_free(&list);
}

void test_add_and_dynamic_expansion() {
    printf("\n--- Test: Add and Dynamic Expansion ---\n");
    DynamicList list;
    list_init(&list, 2);

    TEST_ASSERT(list_add(&list, 10) == true, "Add 1st element (10)");
    TEST_ASSERT(list.size == 1 && list.capacity == 2, "Size is 1, Capacity is 2");

    TEST_ASSERT(list_add(&list, 20) == true, "Add 2nd element (20)");
    TEST_ASSERT(list.size == 2 && list.capacity == 2, "Size is 2, Capacity is 2");

    // This should trigger dynamic resize to 4
    TEST_ASSERT(list_add(&list, 30) == true, "Add 3rd element (30) - triggers resize");
    TEST_ASSERT(list.size == 3 && list.capacity == 4, "Size is 3, Capacity doubled to 4");

    TEST_ASSERT(list_add(&list, 40) == true, "Add 4th element (40)");
    TEST_ASSERT(list.size == 4 && list.capacity == 4, "Size is 4, Capacity is 4");

    // This should trigger dynamic resize to 8
    TEST_ASSERT(list_add(&list, 50) == true, "Add 5th element (50) - triggers resize");
    TEST_ASSERT(list.size == 5 && list.capacity == 8, "Size is 5, Capacity doubled to 8");

    // Verify all contents
    int v;
    list_get(&list, 0, &v); TEST_ASSERT(v == 10, "Element at index 0 is 10");
    list_get(&list, 1, &v); TEST_ASSERT(v == 20, "Element at index 1 is 20");
    list_get(&list, 2, &v); TEST_ASSERT(v == 30, "Element at index 2 is 30");
    list_get(&list, 3, &v); TEST_ASSERT(v == 40, "Element at index 3 is 40");
    list_get(&list, 4, &v); TEST_ASSERT(v == 50, "Element at index 4 is 50");

    list_free(&list);
}

void test_insert_at() {
    printf("\n--- Test: Insert at Arbitrary Index ---\n");
    DynamicList list;
    list_init(&list, 2);

    list_add(&list, 10);
    list_add(&list, 30);

    // Insert 20 in the middle (index 1)
    TEST_ASSERT(list_insert_at(&list, 1, 20) == true, "Insert 20 at index 1");
    TEST_ASSERT(list.size == 3, "Size is now 3");
    int v;
    list_get(&list, 1, &v);
    TEST_ASSERT(v == 20, "Element at index 1 is 20");
    list_get(&list, 2, &v);
    TEST_ASSERT(v == 30, "Element at index 2 is shifted to 30");

    // Insert at front (index 0)
    TEST_ASSERT(list_insert_at(&list, 0, 5) == true, "Insert 5 at head (index 0)");
    list_get(&list, 0, &v);
    TEST_ASSERT(v == 5, "Element at index 0 is 5");

    // Insert out of bounds
    TEST_ASSERT(list_insert_at(&list, 100, 99) == false, "Insert at out-of-bounds index fails gracefully");

    list_free(&list);
}

void test_delete_at() {
    printf("\n--- Test: Delete by Index ---\n");
    DynamicList list;
    list_init(&list, 4);

    list_add(&list, 100);
    list_add(&list, 200);
    list_add(&list, 300);
    list_add(&list, 400);

    // Delete middle element (index 1 -> 200)
    int removed = 0;
    TEST_ASSERT(list_delete_at(&list, 1, &removed) == true, "Delete at index 1 succeeds");
    TEST_ASSERT(removed == 200, "Removed value returned is 200");
    TEST_ASSERT(list.size == 3, "Size decreased to 3");
    int v;
    list_get(&list, 1, &v);
    TEST_ASSERT(v == 300, "Subsequent element shifted left (index 1 is now 300)");

    // Delete out of bounds
    TEST_ASSERT(list_delete_at(&list, 10, &removed) == false, "Delete at out-of-bounds index fails");

    list_free(&list);
}

void test_delete_value() {
    printf("\n--- Test: Delete by Value ---\n");
    DynamicList list;
    list_init(&list, 4);

    list_add(&list, 7);
    list_add(&list, 14);
    list_add(&list, 21);

    TEST_ASSERT(list_delete_value(&list, 14) == true, "Delete existing value 14 succeeds");
    TEST_ASSERT(list.size == 2, "Size is now 2");
    TEST_ASSERT(list_search(&list, 14) == -1, "Search for 14 returns -1");

    TEST_ASSERT(list_delete_value(&list, 999) == false, "Delete non-existent value 999 returns false");

    list_free(&list);
}

void test_dynamic_shrinking() {
    printf("\n--- Test: Dynamic Memory Shrinking ---\n");
    DynamicList list;
    list_init(&list, 2);

    // Expand to capacity 8
    for (int i = 1; i <= 8; i++) {
        list_add(&list, i * 10);
    }
    TEST_ASSERT(list.capacity == 8, "List grew to capacity 8");

    // Delete elements until size becomes <= capacity / 4 (i.e. <= 2)
    while (list.size > 2) {
        list_delete_at(&list, list.size - 1, NULL);
    }
    // Now size is 2, which is <= 8/4, capacity should shrink to 4
    TEST_ASSERT(list.capacity <= 4, "Capacity shrunk automatically after mass deletions");

    list_free(&list);
}

void test_search_and_update() {
    printf("\n--- Test: Search and Update ---\n");
    DynamicList list;
    list_init(&list, 4);

    list_add(&list, 55);
    list_add(&list, 77);
    list_add(&list, 99);

    TEST_ASSERT(list_search(&list, 77) == 1, "Search for 77 returns index 1");
    TEST_ASSERT(list_search(&list, 123) == -1, "Search for 123 returns -1");

    int old_val = 0;
    TEST_ASSERT(list_set(&list, 1, 88, &old_val) == true, "Set index 1 to 88 succeeds");
    TEST_ASSERT(old_val == 77, "Old value was 77");
    int curr = 0;
    list_get(&list, 1, &curr);
    TEST_ASSERT(curr == 88, "Current value at index 1 is 88");

    list_free(&list);
}

void test_sort_and_reverse() {
    printf("\n--- Test: Sort and Reverse ---\n");
    DynamicList list;
    list_init(&list, 4);

    list_add(&list, 40);
    list_add(&list, 10);
    list_add(&list, 50);
    list_add(&list, 20);

    list_sort(&list);
    int a, b, c, d;
    list_get(&list, 0, &a);
    list_get(&list, 1, &b);
    list_get(&list, 2, &c);
    list_get(&list, 3, &d);
    TEST_ASSERT(a == 10 && b == 20 && c == 40 && d == 50, "List sorted in ascending order [10, 20, 40, 50]");

    list_reverse(&list);
    list_get(&list, 0, &a);
    list_get(&list, 1, &b);
    list_get(&list, 2, &c);
    list_get(&list, 3, &d);
    TEST_ASSERT(a == 50 && b == 40 && c == 20 && d == 10, "List reversed to [50, 40, 20, 10]");

    list_free(&list);
}

void test_memory_cleanup() {
    printf("\n--- Test: Memory Cleanup ---\n");
    DynamicList list;
    list_init(&list, 4);
    list_add(&list, 1);
    list_add(&list, 2);
    list_free(&list);

    TEST_ASSERT(list.data == NULL, "Data pointer is NULL after free");
    TEST_ASSERT(list.size == 0, "Size is 0 after free");
    TEST_ASSERT(list.capacity == 0, "Capacity is 0 after free");
}

int main(void) {
    printf("=========================================\n");
    printf("  DYNAMIC LIST MANAGER - TEST SUITE     \n");
    printf("=========================================\n");

    test_initialization();
    test_add_and_dynamic_expansion();
    test_insert_at();
    test_delete_at();
    test_delete_value();
    test_dynamic_shrinking();
    test_search_and_update();
    test_sort_and_reverse();
    test_memory_cleanup();

    printf("\n=========================================\n");
    printf("Test Results: %d / %d tests passed (%.1f%%)\n",
           passed_tests, total_tests, (float)passed_tests / total_tests * 100.0f);
    printf("=========================================\n");

    return (passed_tests == total_tests) ? 0 : 1;
}

