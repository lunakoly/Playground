#include <gtest/gtest.h>
#include <iostream>

#include "compressed_pair.h"


struct Empty {};

struct NotEmpty {
    int score = 10;
};


TEST(compressed_pair_tests, empties) {
    ASSERT_EQ(sizeof(NotEmpty), sizeof(int));
    ASSERT_EQ(sizeof(Empty), 1);

    ASSERT_FALSE(my::is_empty<NotEmpty>::value);
    ASSERT_TRUE (my::is_empty<Empty>::value == true);
}


TEST(compressed_pair_tests, common) {
    my::compressed_pair<Empty, NotEmpty> pairA;
    ASSERT_EQ(sizeof(pairA), sizeof(int));

    my::compressed_pair<NotEmpty, Empty> pairB;
    ASSERT_EQ(sizeof(pairB), sizeof(int));

    my::compressed_pair<NotEmpty, NotEmpty> pairC;
    ASSERT_EQ(sizeof(pairC), 2 * sizeof(int));

    my::compressed_pair<Empty, Empty> pairD;
    ASSERT_EQ(sizeof(pairD), 2);
}


int main(int argc, char * argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}