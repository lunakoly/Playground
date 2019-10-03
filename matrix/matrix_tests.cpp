#include <gtest/gtest.h>

#include "matrix.h"


TEST(matrix_tests, matrix_shape) {
    my::matrix_shape<size_t> shape { 10, 20 };

    ASSERT_EQ(shape.rows, 10);
    ASSERT_EQ(shape.columns, 20);
    ASSERT_EQ(shape.size(), 200);
}


int main(int argc, char * argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}