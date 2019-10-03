#include <gtest/gtest.h>

#include <initializer_list>

#include "dynamic.h"

#include "../matrix/matrix.h"


TEST(dynamic_tests, stick) {
	auto prices = std::initializer_list { 0, 2, 7, 10 };
	auto max = my::stick(prices.begin(), prices.end(), 6);
	ASSERT_EQ(max, 14);
}


TEST(dynamic_tests, matrix_multiplication_order) {
	auto data = std::initializer_list {
		my::matrix_shape<size_t> { 10,  20 },
		my::matrix_shape<size_t> { 20,  50 },
		my::matrix_shape<size_t> { 50,   1 },
		my::matrix_shape<size_t> {  1, 100 }
	};

	auto result = my::matrix_multiplication_order(data.begin(), data.end());
	ASSERT_EQ(result, 2200);
}


TEST(dynamic_tests, longest_common_subsequence) {
	std::string first = "abadsfaf";
	std::string second = "ajfkfjj";

	auto out = my::longest_common_subsequence(
		 first.begin(),  first.end(),
		second.begin(), second.end()
	);

	std::string result(out.begin(), out.end());
	ASSERT_STREQ("aff", result.c_str());
}


TEST(dynamic_tests, hirshberg) {
	std::string first = "abadsfaf";
	std::string second = "ajfkfjj";

	auto out = my::hirshberg(
		 first.begin(),  first.end(),
		second.begin(), second.end()
	);

	std::string result(out.begin(), out.end());
	ASSERT_STREQ("aff", result.c_str());
}


int main(int argc, char * argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}