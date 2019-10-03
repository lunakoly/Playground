#include <gtest/gtest.h>

#include <iostream>
#include <initializer_list>


#include "heap.h"


TEST(heap_tests, create_empty) {
    my::heap<int> heap;
    ASSERT_EQ(heap.size(), 0);
}


template <typename T>
void print_heap(my::heap<T> heap) {
    auto row_length = 1;
    auto row_index  = 0;

    for (auto it = heap.begin(); it < heap.end(); it++) {
        std::cout << *it << ' ';
        row_index++;

        if (row_index == row_length) {
            std::cout << std::endl;
            row_length *= 2;
            row_index = 0;
        }
    }
}


TEST(heap_tests, create_from_iterable_by_min) {
    auto numbers = std::initializer_list { 10, 14, 5, 3, 72, 156, -41, -6 };
    my::heap<int> heap(numbers.begin(), numbers.end(), my::less);

    ASSERT_EQ(heap.size(), numbers.size());

    for (auto it = heap.begin(); it != heap.end(); it++) {
        auto index = it - heap.begin();

        if (index * 2 + 1 < heap.size()) {
            ASSERT_GE(*it, *(it + index + 1));
        }

        if (index * 2 < heap.size()) {
            ASSERT_GE(*it, *(it + index));
        }
    }
}


TEST(heap_tests, create_from_iterable_by_max) {
    auto numbers = std::initializer_list { 10, 14, 5, 3, 72, 156, -41, -6 };
    my::heap<int> heap(numbers.begin(), numbers.end(), my::greater);

    ASSERT_EQ(heap.size(), numbers.size());

    for (auto it = heap.begin(); it != heap.end(); it++) {
        auto index = it - heap.begin();

        if (index * 2 + 1 < heap.size()) {
            ASSERT_LE(*it, *(it + index + 1));
        }

        if (index * 2 < heap.size()) {
            ASSERT_LE(*it, *(it + index));
        }
    }
}


int main(int argc, char * argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}