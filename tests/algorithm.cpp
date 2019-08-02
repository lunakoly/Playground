#include <iostream>
#include <cassert>

// for random
#include <cstdlib>
// for time() seed
#include <ctime>

#include "../my/algorithm.h"
#include "../my/vector.h"


void test_max() {
	std::cout << "Max of ( 10,  20) = " << my::max( 10,  20) << std::endl;
	std::cout << "Max of ( 20,  10) = " << my::max( 20,  10) << std::endl;
	std::cout << "Max of ( 10,  10) = " << my::max( 10,  10) << std::endl;
	std::cout << "Max of (-10,  10) = " << my::max(-10,  10) << std::endl;
	std::cout << "Max of ( 10, -10) = " << my::max( 10, -10) << std::endl;
	std::cout << "Max of (-10, -20) = " << my::max(-10, -20) << std::endl;
	std::cout << "Max of (-20, -10) = " << my::max(-20, -10) << std::endl;
}

void test_bit_width() {
	std::cout << "Bits used by 16 = " << my::bit_width(16) << std::endl;
	std::cout << "Bits used by  8 = " << my::bit_width( 8) << std::endl;
	std::cout << "Bits used by  1 = " << my::bit_width( 1) << std::endl;
	std::cout << "Bits used by  0 = " << my::bit_width( 0) << std::endl;
	std::cout << "Bits used by 31 = " << my::bit_width(31) << std::endl;
}

void test_karatsuba() {
	for (size_t it = 0; it < 10; it++) {
		long long a = rand();
		long long b = rand();

		std::cout << "a = " << a << std::endl;
		std::cout << "b = " << b << std::endl;

		std::cout << "Expected = " << (a * b) << std::endl;
		std::cout << "Got      = " << my::karatsuba(a, b) << std::endl;
	}
}

void test_lower_bound() {
	my::vector<int> numbers = {4, 15, 125, 1125, 2451, 5012};
	std::cout << "4    -> " << *(my::lower_bound(numbers.begin(), numbers.end(), 4   )) << std::endl;
	std::cout << "15   -> " << *(my::lower_bound(numbers.begin(), numbers.end(), 15  )) << std::endl;
	std::cout << "2451 -> " << *(my::lower_bound(numbers.begin(), numbers.end(), 2451)) << std::endl;
	std::cout << "125  -> " << *(my::lower_bound(numbers.begin(), numbers.end(), 100 )) << std::endl;
	std::cout << "5012 -> " << *(my::lower_bound(numbers.begin(), numbers.end(), 4000)) << std::endl;
	std::cout << "4    -> " << *(my::lower_bound(numbers.begin(), numbers.end(), -10 )) << std::endl;
}

void test_upper_bound() {
	my::vector<int> numbers = {4, 15, 15, 15, 125, 1125, 1125, 2451, 5012};
	std::cout << "15   -> " << *(my::upper_bound(numbers.begin(), numbers.end(), 4   )) << std::endl;
	std::cout << "125  -> " << *(my::upper_bound(numbers.begin(), numbers.end(), 15  )) << std::endl;
	std::cout << "5012 -> " << *(my::upper_bound(numbers.begin(), numbers.end(), 2451)) << std::endl;
	std::cout << "125  -> " << *(my::upper_bound(numbers.begin(), numbers.end(), 100 )) << std::endl;
	std::cout << "5012 -> " << *(my::upper_bound(numbers.begin(), numbers.end(), 4000)) << std::endl;
	std::cout << "4    -> " << *(my::upper_bound(numbers.begin(), numbers.end(), -10 )) << std::endl;
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

	std::cout << "[+] Succeded assertion: " << offset << " proto's" << std::endl;
}

template <typename Iterable>
void print_contents(const Iterable & things) {
	std::cout << "[ ";

	for (auto it = things.cbegin(); it != things.cend(); it++) {
		std::cout << *it << ' ';
	}

	std::cout << ']' << std::endl;
}

void test_rotate() {
	my::vector<int> numbers = {0, 1, 1, 2, 3, 5, 8, 13, 21};

	auto result = my::rotate(numbers.begin(), numbers.begin() + 3, numbers.end(), std::swap);
	print_contents(numbers);
	assert_range(numbers, std::initializer_list {2, 3, 5, 8, 13, 21, 0, 1, 1});
	std::cout << "Result = " << *result << std::endl;


	result = my::rotate(numbers.begin(), numbers.begin() + 5, numbers.end(), my::fast_swap);
	print_contents(numbers);
	assert_range(numbers, std::initializer_list {21, 0, 1, 1, 2, 3, 5, 8, 13});
	std::cout << "Result = " << *result << std::endl;
}

void test_insertion_sort() {
	my::vector<int> numbers = {1, 14, 6, 12, 3, 167, 124, 5, 1};
	my::insertion_sort(numbers.begin(), numbers.end(), my::fast_swap);
	print_contents(numbers);
	assert_range(numbers, std::initializer_list {1, 1, 3, 5, 6, 12, 14, 124, 167});
}

void test_selection_sort() {
	my::vector<int> numbers = {1, 14, 6, 12, 3, 167, 124, 5, 1};
	my::selection_sort(numbers.begin(), numbers.end());
	print_contents(numbers);
	assert_range(numbers, std::initializer_list {1, 1, 3, 5, 6, 12, 14, 124, 167});
}

void test_merge_sort() {
	my::vector<int> numbers = {1, 14, 6, 12, 3, 167, 124, 5, 1};
	my::merge_sort(numbers.begin(), numbers.end());
	print_contents(numbers);
	assert_range(numbers, std::initializer_list {1, 1, 3, 5, 6, 12, 14, 124, 167});
}

void test_quick_sort() {
	my::vector<int> numbers = {1, 14, 6, 12, 3, 167, 124, 5, 1};
	my::quick_sort(numbers.begin(), numbers.end());
	print_contents(numbers);
	assert_range(numbers, std::initializer_list {1, 1, 3, 5, 6, 12, 14, 124, 167});
}

int main() {
	// set random seed
	srand(time(nullptr));

	// test_max();
	// test_bit_width();
	// test_karatsuba();
	// test_lower_bound();
	// test_upper_bound();
	// test_rotate();
	// test_insertion_sort();
	// test_selection_sort();
	// test_merge_sort();
	test_quick_sort();
}