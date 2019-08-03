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

	/**
	 * Analogue of std::rotate
	 *
 	 * Time   Complexity: O(n), n = right - left
	 * Memory Complexity: O(1)
	 */
	template <typename Iterator>
	Iterator rotate_old(
		Iterator left,
		Iterator start,
		Iterator right,
		swap_function_for<Iterator> swap = std::swap
	) {
		Iterator result = right - std::distance(left, start);

		while (left < start && start < right) {
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
		Iterator result = right - std::distance(left, start);
		Iterator anchor = start;

		while (left != anchor) {
			swap(*left, *anchor);
			anchor++;
			left++;

			if (anchor == right) {
				anchor = start;
			} else if (left == start) {
				start = anchor;
			}
		}

		return result;
	}

	/**
	 * Just the insertion sort
	 *
	 * Time   Complexity: O(nn), n = right - left
	 * Memory Complexity: O(1)
	 */
	template <typename Iterator>
	void insertion_sort(
		Iterator left,
		Iterator right,
		swap_function_for<Iterator> swap = std::swap
	) {
		for (auto it = left + 1; it != right; it++) {
			auto place = upper_bound(left, it, *it);
			rotate(place, it, it + 1, swap);
		}
	}

	/**
	 * Just the selection sort
	 *
	 * Time   Complexity: O(nn), n = right - left
	 * Memory Complexity: O(1)
	 */
	template <typename Iterator>
	void selection_sort(
		Iterator left,
		Iterator right,
		swap_function_for<Iterator> swap = std::swap
	) {
		for (auto it = left; it < right - 1; it++) {
			auto smallest = it;

			for (auto that = it + 1; that != right; that++) {
				if (*that < *smallest) {
					smallest = that;
				}
			}

			swap(*it, *smallest);
		}
	}

	/**
	 * Just the merge sort
	 *
	 * Time   Complexity: O(nlogn), n = right - left
	 * Memory Complexity: O(n),     n = right - left
	 */
	template <typename Iterator>
	void merge_sort(
		Iterator left,
		Iterator right,
		swap_function_for<Iterator> swap = std::swap
	) {
		size_t size = std::distance(left, right);

		if (size <= 1)
			return;

		Iterator middle = left + size / 2;

		merge_sort(left, middle);
		merge_sort(middle, right);

		char temp[size * sizeof(typename std::iterator_traits<Iterator>::value_type)];

		Iterator a = left;
		Iterator b = middle;
		Iterator c = (Iterator) temp;

		while (a < middle || b < right) {
			if (a >= middle) {
				swap(*c, *b);
				b++;
				c++;
			} else if (b >= right) {
				swap(*c, *a);
				a++;
				c++;
			} else if (*a <= *b) {
				swap(*c, *a);
				a++;
				c++;
			} else {swap(*c, *b);
				b++;
				c++;
			}
		}

		my::swap_ranges(left, right, (Iterator) temp, swap);
	}

	/**
	 * Sorts all elements in such way
	 * that every element to the left
	 * of pivot are <= pivot and
	 * every element to the right
	 * are >= than it
	 *
	 * Time   Complexity: O(n), n = right - left
	 * Memory Complexity: O(1)
	 */
	template <typename Iterator>
	Iterator partition(
		Iterator left,
		Iterator right,
		Iterator pivot,
		swap_function_for<Iterator> swap = std::swap
	) {
		if (pivot != right - 1) {
			swap(*(pivot), *(right - 1));
			pivot = right - 1;
		}

		Iterator a = left;
		Iterator b = right - 2;

		while (a < b) {
			if (*a <= *pivot) {
				a++;
			} else if (*b > *pivot) {
				b--;
			} else {
				swap(*a, *b);
			}
		}

		// exceptional case where pivot
		// is the greatest item and we
		// shouldn't swap it with anything
		if (*b >= *pivot)
			swap(*b, *pivot);

		return b;
	}

	/**
	 * Just the quick sort
	 *
	 * Time   Complexity: O(nlogn), n = right - left
	 * Memory Complexity: O(1)
	 */
	template <typename Iterator>
	void quick_sort(
		Iterator left,
		Iterator right,
		swap_function_for<Iterator> swap = std::swap
	) {
		size_t size = std::distance(left, right);

		if (size <= 1)
			return;

		Iterator middle = left + size / 2;

		if (*middle < *left) {
			swap(*left, *middle);
		}

		if (*(right - 1) < *left) {
			swap(*left, *(right - 1));
		}

		if (*middle < *(right - 1)) {
			swap(*middle, *(right - 1));
		}

		auto separator = partition(left, right, right - 1, swap);
		quick_sort(left, separator, swap);
		quick_sort(separator + 1, right, swap);
	}

	/**
	 * Returns an element that would be located
	 * at a specific position if the container
	 * was sorted
	 *
	 * Time   Complexity: O(n), n = right - left
	 * Memory Complexity: O(1)
	 */
	template <typename Iterator>
	Iterator select(
		Iterator left,
		Iterator right,
		size_t index,
		swap_function_for<Iterator> swap = std::swap
	) {
		constexpr size_t type_size = sizeof(typename std::iterator_traits<Iterator>::value_type);
		size_t size = std::distance(left, right);

		if (size <= 1)
			return left;

		// array of medians of portions of 5 items
		size_t sub_size = ceil(size / 5);
		char temp[type_size * sub_size];

		Iterator sub_begin = left;
		Iterator sub_end = left + 5;
		Iterator sub = (Iterator) temp;

		// fill all but the last portion (<= 5)
		while (sub_begin < right - 5) {
			quick_sort(sub_begin, sub_end, swap);
			Iterator sub_median = sub_begin + std::distance(sub_begin, sub_end) / 2;
			std::memcpy(sub, sub_median, type_size);
			sub_begin += 5;
			sub_end += 5;
			sub++;
		}

		// process the last portion
		quick_sort(sub_begin, right, swap);
		Iterator sub_median = sub_begin + std::distance(sub_begin, right) / 2;
		std::memcpy(sub, sub_median, type_size);

		// median of medians
		Iterator temp_median = select((Iterator) temp, (Iterator) temp + sub_size, size / 2, swap);
		// find median element in the source container
		Iterator median = lower_bound(left, right, *temp_median);

		Iterator separator = partition(left, right, median, swap);
		size_t separator_index = separator - left;

		if (separator_index == index)
			return separator;

		if (separator_index < index)
			return select(separator + 1, right, index - separator_index - 1);

		return select(left, separator, index);
	}
}