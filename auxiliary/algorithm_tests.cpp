#include <gtest/gtest.h>
#include <iostream>

// for random
#include <cstdlib>
// for time() seed
#include <ctime>

#include "algorithm.h"
#include "../fast_vector/fast_vector.h"


TEST(algorithm_tests, max) {
	ASSERT_EQ(my::max( 10,  20),  20);
	ASSERT_EQ(my::max( 20,  10),  20);
	ASSERT_EQ(my::max( 10,  10),  10);
	ASSERT_EQ(my::max(-10,  10),  10);
	ASSERT_EQ(my::max( 10, -10),  10);
	ASSERT_EQ(my::max(-10, -20), -10);
	ASSERT_EQ(my::max(-20, -10), -10);
}


TEST(algorithm_tests, bit_width) {
	ASSERT_EQ(my::bit_width(16), 5);
	ASSERT_EQ(my::bit_width( 8), 4);
	ASSERT_EQ(my::bit_width( 1), 1);
	ASSERT_EQ(my::bit_width( 0), 0);
	ASSERT_EQ(my::bit_width(31), 5);
}


TEST(algorithm_tests, karatsuba) {
	for (size_t it = 0; it < 10; it++) {
		long long a = rand();
		long long b = rand();
		ASSERT_EQ(my::karatsuba(a, b), a * b);
	}
}


TEST(algorithm_tests, lower_bound) {
	my::fast_vector<int> numbers = {4, 15, 125, 1125, 2451, 5012};
	ASSERT_EQ(*(my::lower_bound(numbers.begin(), numbers.end(), 4   )),    4);
	ASSERT_EQ(*(my::lower_bound(numbers.begin(), numbers.end(), 15  )),   15);
	ASSERT_EQ(*(my::lower_bound(numbers.begin(), numbers.end(), 2451)), 2451);
	ASSERT_EQ(*(my::lower_bound(numbers.begin(), numbers.end(), 100 )),  125);
	ASSERT_EQ(*(my::lower_bound(numbers.begin(), numbers.end(), 4000)), 5012);
	ASSERT_EQ(*(my::lower_bound(numbers.begin(), numbers.end(), -10 )),    4);
}


TEST(algorithm_tests, upper_bound) {
	my::fast_vector<int> numbers = {4, 15, 15, 15, 125, 1125, 1125, 2451, 5012};
	ASSERT_EQ(*(my::lower_bound(numbers.begin(), numbers.end(), 4   )),    4);
	ASSERT_EQ(*(my::lower_bound(numbers.begin(), numbers.end(), 15  )),   15);
	ASSERT_EQ(*(my::lower_bound(numbers.begin(), numbers.end(), 2451)), 2451);
	ASSERT_EQ(*(my::lower_bound(numbers.begin(), numbers.end(), 100 )),  125);
	ASSERT_EQ(*(my::lower_bound(numbers.begin(), numbers.end(), 4000)), 5012);
	ASSERT_EQ(*(my::lower_bound(numbers.begin(), numbers.end(), -10 )),    4);
}


template <typename FirstIterable, typename SecondIterable>
void assert_range(
	const FirstIterable & items,
	const SecondIterable & protos
) {
	size_t offset = 0;
	auto proto = protos.begin();

	for (auto it = items.cbegin(); it != items.cend(); it++) {
		assert(*proto == *it);
		offset++;
		proto++;
	}
}


template <typename Iterable>
void print_contents(const Iterable & things) {
	std::cout << "[ ";

	for (auto it = things.cbegin(); it != things.cend(); it++) {
		std::cout << *it << ' ';
	}

	std::cout << ']' << std::endl;
}


TEST(algorithm_tests, rotate) {
	my::fast_vector<int> numbers = {0, 1, 1, 2, 3, 5, 8, 13, 21};

	auto result = my::rotate(numbers.begin(), numbers.begin() + 3, numbers.end(), std::swap);
	assert_range(numbers, std::initializer_list {2, 3, 5, 8, 13, 21, 0, 1, 1});
	ASSERT_EQ(*result, 0);


	result = my::rotate(numbers.begin(), numbers.begin() + 5, numbers.end(), my::fast_swap);
	assert_range(numbers, std::initializer_list {21, 0, 1, 1, 2, 3, 5, 8, 13});
	ASSERT_EQ(*result, 2);
}


TEST(algorithm_tests, insertion_sort) {
	my::fast_vector<int> numbers = {1, 14, 6, 12, 3, 167, 124, 5, 1};
	my::insertion_sort(numbers.begin(), numbers.end(), my::fast_swap);
	assert_range(numbers, std::initializer_list {1, 1, 3, 5, 6, 12, 14, 124, 167});
}


TEST(algorithm_tests, selection_sort) {
	my::fast_vector<int> numbers = {1, 14, 6, 12, 3, 167, 124, 5, 1};
	my::selection_sort(numbers.begin(), numbers.end());
	assert_range(numbers, std::initializer_list {1, 1, 3, 5, 6, 12, 14, 124, 167});
}


TEST(algorithm_tests, merge_sort) {
	my::fast_vector<int> numbers = {1, 14, 6, 12, 3, 167, 124, 5, 1};
	my::merge_sort(numbers.begin(), numbers.end());
	assert_range(numbers, std::initializer_list {1, 1, 3, 5, 6, 12, 14, 124, 167});
}


TEST(algorithm_tests, quick_sort) {
	my::fast_vector<int> numbers = {1, 14, 6, 12, 3, 167, 124, 5, 1};
	my::quick_sort(numbers.begin(), numbers.end());
	assert_range(numbers, std::initializer_list {1, 1, 3, 5, 6, 12, 14, 124, 167});
}


TEST(algorithm_tests, select) {
	my::fast_vector<int> numbers = {1, 14, 6, 12, 3, 167, 124, 5, 1};
	auto found = my::select(numbers.begin(), numbers.end(), 3);
	ASSERT_EQ(*found, 5);
	found = my::select(numbers.begin(), numbers.end(), 0);
	ASSERT_EQ(*found, 1);
	found = my::select(numbers.begin(), numbers.end(), 8);
	ASSERT_EQ(*found, 167);
}


TEST(algorithm_tests, heap_sort) {
	my::fast_vector<int> numbers = {1, 14, 6, 12, 3, 167, 124, 5, 1};
	my::heap_sort(numbers.begin(), numbers.end());
	assert_range(numbers, std::initializer_list {1, 1, 3, 5, 6, 12, 14, 124, 167});
}


TEST(algorithm_tests, counting_sort) {
	my::fast_vector<int> numbers = {1, 14, 6, 12, 3, 167, 124, 5, 1};
	my::counting_sort(numbers.begin(), numbers.end(), 168);
	assert_range(numbers, std::initializer_list {1, 1, 3, 5, 6, 12, 14, 124, 167});
}


TEST(algorithm_tests, radix_sort) {
	my::fast_vector<int> numbers = {1, 14, 6, 12, 3, 167, 124, 5, 1};
	my::radix_sort(numbers.begin(), numbers.end(), 3);
	assert_range(numbers, std::initializer_list {1, 1, 3, 5, 6, 12, 14, 124, 167});
}


int main(int argc, char * argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}