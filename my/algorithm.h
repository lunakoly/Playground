#pragma once

// for debug
// #include <iostream>

// for different math
#include <cmath>
// for malloc
#include <cstdlib>
// for memcpy
#include <cstring>
#include <iterator>


/**
 * Custom implementations
 */
namespace my {
	/**
	 * Return the max of x and y
	 * via bitwise arithmetics.
	 *
	 * Time   Complexity: O(1)
	 * Memory Complexity: O(n), n = sizeof(Number)
	 */
	template <typename Number>
	Number max(Number x, Number y) {
		Number difference = x - y;
		constexpr int shifting = sizeof(Number) * 8 - 1;
		return x - (difference & (difference >> shifting));
	}

	/**
	 * Returns the count of bits that
	 * number occupies. Negative numbers
	 * will always be claimed to occupy
	 * all bits.
	 *
	 * Time   Complexity: O(n), n = sizeof(Number)
	 * Memory Complexity: O(n), n = sizeof(Number)
	 */
	template <typename Number>
	int bit_width(Number x) {
		constexpr int shifting = sizeof(Number) * 8 - 1;
		Number mask = (Number) 1 << shifting;

		for (int it = shifting + 1; it > 0; it--) {
			if ((mask & x) != 0)
				return it;
			mask = mask >> 1;
		}

		return 0;
	}

	/**
	 * Fast multiplication by Karatsuba.
	 *
 	 * Time   Complexity: O(n ^ log_2(3)), n = sizeof(Number)
	 * Memory Complexity: O(n ^ log_2(3)), n = sizeof(Number)
	 */
	template <typename Number>
	Number karatsuba(Number x, Number y) {
		if (x < 100 && y < 100)
			return x * y;

		// there're 3 ways here:
		//
		// 1) get the base as ceil(sqrt(number)) (base^2 >= number).
		//    it's bad because we can only get the digits
		//    via / and %
		//
		// 2) get the half of digit count as ceil(log10(number) / 2.0)
		//    and the base as 10^(half of digit count).
		//    it also requires to do division
		//
		// 3) get the half of digit count as ceil(log2(number)) >> 1
		//    and the base as 1 << (half of digit count).
		//    this way it's easy to access the individual digits

		// bit_width is used instead of log2 & ceil

		Number max = my::max(x, y);
		Number half_digits = (bit_width(max)) >> 1;
		Number base = 1 << half_digits;

		Number a = x >> half_digits;
		Number b = x & (base - 1);
		Number c = y >> half_digits;
		Number d = y & (base - 1);

		Number ac = karatsuba(a, c);
		Number bd = karatsuba(b, d);
		Number middle = karatsuba((a + b), (c + d)) - ac - bd;

		return (ac * base + middle) * base + bd;
	}

	/**
	 * If we accidently meet *middle == value then we should
	 * ensure that the near left elements don't == value
	 * so we mark middle as right and go further.
	 */
	template<class Iterator, class T>
	Iterator lower_bound(Iterator left, Iterator right, T value) {
	    while (left != right) {
	        Iterator middle = left + std::distance(left, right) / 2;

	        if (value <= *middle) {
	            right = middle;
	        } else {
	            left = middle + 1;
	        }
	    }

	    return right;
	}

	/**
	 * If we accidently meet *middle == value then we should
	 * ensure that the near right elements don't == value
	 * so we mark middle as left and go further.
	 */
	template<class Iterator, class T>
	Iterator upper_bound(Iterator left, Iterator right, T value) {
	    while (left != right) {
	        Iterator middle = left + std::distance(left, right) / 2;

	        if (value >= *middle) {
	            left = middle + 1;
	        } else {
	            right = middle;
	        }
	    }

	    return left;
	}

	/**
	 * Uses memcpu to swap first's ans second's
	 * internals.
	 *
	 * Time   Complexity: O(n), n = sizeof(T)
	 * Memory Complexity: O(n), n = sizeof(T)
	 */
	template <typename T>
	void fast_swap(T & first, T & second) {
		char temp[sizeof(T)];
		std::memcpy(&temp, &first, sizeof(T));
		std::memcpy(&first, &second, sizeof(T));
		std::memcpy(&second, &temp, sizeof(T));
	}

	/**
	 * Describes a swap function that swaps
	 * T instances
	 */
	template <typename T>
	using swap_function = void (*) (T &, T &);

	/**
	 * Swap function for value_type of
	 * an Iterator
	 */
	template <typename Iterator>
	using swap_function_for = swap_function<typename std::iterator_traits<Iterator>::value_type>;

	/**
	 * Analogue of std::rotate
	 *
 	 * Time   Complexity: O(n), n = right - left
	 * Memory Complexity: O(1)
	 */
	template <typename Iterator>
	Iterator rotate(
		Iterator left,
		Iterator start,
		Iterator right,
		swap_function_for<Iterator> swap = std::swap
	) {
		Iterator result = right - (start - left);

		while (start != right) {
			if (std::distance(left, start) <= std::distance(start, right)) {
				auto that = start;

				while (left != that) {
					swap(*left, *start);
					start++;
					left++;
				}
			} else {
				auto that = start;

				while (right != that) {
					swap(*(right - 1), *(start - 1));
					start--;
					right--;
				}
			}
		}

		return result;
	}

	/**
	 * Swaps all elements
	 * of range [first1, last1) and
	 * [first2, first2 + last1 - first1)
	 *
 	 * Time   Complexity: O(n), n = last1 - first1
	 * Memory Complexity: O(1)
	 */
	template <typename Iterator>
	Iterator swap_ranges(
		Iterator first1,
		Iterator last1,
		Iterator first2,
		swap_function_for<Iterator> swap = std::swap
	) {
		while (first1 != last1) {
			swap(*first1, *first2);
			first1++;
			first2++;
		}

		return first2;
	}

	// template <typename Iterator>
	// void insertion_sort(Iterator left, Iterator right) {
	// 	auto it = left;

	// 	while (it != right) {
	// 		for (auto that = left; that < it; that++) {
	// 			if (*it < *that) {

	// 			}
	// 		}
	// 	}
	// }
}