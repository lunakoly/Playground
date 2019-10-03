#pragma once

#include <vector>
#include <iterator>


/**
 * Custom implementations
 */
namespace my {
	/**
	 * Returns the biggest amount of money
	 * that can be recieved based on individual prices
	 * of sub-sticks.
	 *
 	 *   Time Complexity: Θ(n^2), n = right - left
	 * Memory Complexity: Θ(n)
	 */
	template <typename Iterator>
	int stick(Iterator left, Iterator right, int length) {
		int best[length + 1] = { 0 };

		for (auto it = 1; it <= length; it++) {
			best[it] = 0;

			for (auto that = left; that < left + it; that++) {
				auto current = *that + best[it - (that - left + 1)];

				if (current > best[it])
					best[it] = current;
			}
		}

		return best[length];
	}

	/**
	 * Returns the min number of element multiplications
	 * required to calculate the multiplication of
	 * a series of matrices
	 *
  	 *   Time Complexity: Θ(n^3), n = right - left
	 * Memory Complexity: Θ(n^2)
	 */
	template <typename Iterator>
	int matrix_multiplication_order(
		Iterator left,
		Iterator right
	) {
		auto length = std::distance(left, right);
		int counts[length][length];

		for (auto it = 0; it < length; it++) {
			counts[it][it] = 0;
		}

		for (auto l = 1; l < length; l++) {
			for (auto i = 0; i < length - l; i++) {
				auto j = i + l;
				counts[i][j] = counts[i][i] + counts[i + 1][j] +
						(left + i)->rows * (left + j)->columns * (left + i)->columns;

				for (auto k = i + 1; k < j; k++) {
					auto count = counts[i][k] + counts[k + 1][j] +
						(left + i)->rows * (left + j)->columns * (left + k)->columns;

					if (count < counts[i][j])
						counts[i][j] = count;
				}
			}
		}

		return counts[0][length - 1];
	}

	/**
	 * Returns the length of the longest common
	 * subsequence of the two collections.
	 *
   	 *   Time Complexity: Θ(mn), m =  first_begin -  first_end
	 * Memory Complexity: Θ(mn), n = second_begin - second_end
	 */
	template <typename Iterator>
	std::vector<typename std::iterator_traits<Iterator>::value_type> longest_common_subsequence(
		Iterator first_begin,
		Iterator first_end,
		Iterator second_begin,
		Iterator second_end
	) {
		auto first_length  = std::distance( first_begin,  first_end);
		auto second_length = std::distance(second_begin, second_end);

		int counts[first_length][second_length];

		if (*first_begin == *second_begin) {
			counts[0][0] = 1;
		} else {
			counts[0][0] = 0;
		}

		for (auto i = 1; i < first_length; i++) {
			if (*(first_begin + i) == *second_begin) {
				counts[i][0] = 1;
			} else {
				counts[i][0] = counts[i - 1][0];
			}
		}

		for (auto j = 1; j < second_length; j++) {
			if (*(second_begin + j) == *first_begin) {
				counts[0][j] = 1;
			} else {
				counts[0][j] = 0 + counts[0][j - 1];
			}
		}

		for (auto i = 1; i < first_length; i++) {
			for (auto j = 1; j < second_length; j++) {
				if (*(first_begin + i) == *(second_begin + j)) {
					counts[i][j] = counts[i - 1][j - 1] + 1;
				} else {
					counts[i][j] = std::max(counts[i - 1][j], counts[i][j - 1]);
				}
			}
		}

		auto previous_i =  first_length - 1;
		auto previous_j = second_length - 1;
		std::vector<typename std::iterator_traits<Iterator>::value_type> result;

		while (
			previous_i >= 0 &&
			previous_j >= 0
		) {
			if (*(first_begin + previous_i) == *(second_begin + previous_j)) {
				result.push_back(*(first_begin + previous_i));
				previous_i--;
				previous_j--;
			}

			else if (
				previous_i > 0 &&
				counts[previous_i][previous_j] == counts[previous_i - 1][previous_j]
			) {
				previous_i--;
			}

			else {
				previous_j--;
			}
		}

		std::reverse(result.begin(), result.end());
		return result;
	}

	/**
	 * Fills in the last row of LCS table
	 *
	 *   Time Complexity: Θ(mn), m =  first_begin -  first_end
	 * Memory Complexity: Θ(n),  n = second_begin - second_end
	 */
	template <typename Iterator>
	void calculate_last_lcs_line(
		Iterator first_begin,
		Iterator first_end,
		Iterator second_begin,
		Iterator second_end,
		int * storage
	) {
		auto first_length  = std::distance( first_begin,  first_end);
		auto second_length = std::distance(second_begin, second_end);

		int intermediate[second_length];

		for (auto j = 0; j < second_length; j++) {
			if (*(second_begin + j) == *first_begin) {
				storage[j] = 1;
			} else {
				storage[j] = 0;
			}
		}

		for (auto i = 1; i < first_length; i++) {
			for (auto j = 0; j < second_length; j++) {
				intermediate[j] = storage[j];
			}

			if (*(first_begin + i) == *second_begin) {
				storage[0] = 1;
			} else {
				storage[0] = 0;
			}

			for (auto j = 1; j < second_length; j++) {
				if (*(first_begin + i) == *(second_begin + j)) {
					storage[j] = intermediate[j - 1] + 1;
				} else {
					storage[j] = std::max(intermediate[j], storage[j - 1]);
				}
			}
		}
	}

	/**
	 * LCS with linear memory usage
	 *
	 *   Time Complexity: Θ(mn), m =  first_begin -  first_end
	 * Memory Complexity: Θ(n),  n = second_begin - second_end
	 */
	template <typename Iterator>
	std::vector<typename std::iterator_traits<Iterator>::value_type> hirshberg(
		Iterator first_begin,
		Iterator first_end,
		Iterator second_begin,
		Iterator second_end
	) {
		using T = typename std::iterator_traits<Iterator>::value_type;

		auto first_length  = std::distance( first_begin,  first_end);
		auto second_length = std::distance(second_begin, second_end);

		int   forward_counts[second_length];
		int backwards_counts[second_length];

		std::vector<T>  first_reversed( first_length);
		std::vector<T> second_reversed(second_length);

		std::reverse_copy( first_begin,  first_end,  first_reversed.begin());
		std::reverse_copy(second_begin, second_end, second_reversed.begin());

		calculate_last_lcs_line(
			 first_begin, first_begin + first_length / 2,
			second_begin, second_end,
			forward_counts
		);

		calculate_last_lcs_line(
			 first_reversed.begin(), first_reversed.end() - first_length / 2,
			second_reversed.begin(), second_reversed.end(),
			backwards_counts
		);

		auto max = forward_counts[0] + backwards_counts[second_length - 1];
		auto max_k = 0;

		for (auto k = 1; k < second_length; k++) {
			auto current = forward_counts[k] + backwards_counts[second_length - 1 - k];

			if (current > max) {
				max = current;
				max_k = k;
			}
		}

		auto first_sub = longest_common_subsequence(
			first_begin, first_end - first_length / 2,
			second_begin, second_begin + max_k
		);

		auto second_sub = longest_common_subsequence(
			first_end - first_length / 2, first_end,
			second_begin + max_k, second_end
		);

		first_sub.insert(first_sub.end(), second_sub.begin(), second_sub.end());
		return first_sub;
	}
}