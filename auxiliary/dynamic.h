#pragma once


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
}