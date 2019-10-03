#include <gtest/gtest.h>

#include <iostream>
#include <cassert>

#include <vector>

#include "../debug_allocator/debug_allocator.h"
#include "fast_vector.h"


/*
 * A debug object. Define ALERT to see
 * log messages
 */
struct Snitch {
    int score;

    explicit Snitch(int score = 100) : score(score) {
#ifdef ALERT
        std::cout << "+ Snitch {" << score << '}' << std::endl;
#endif
    }

    Snitch(const Snitch & other) : score(other.score) {
#ifdef ALERT
        std::cout << "+ Snitch {" << score << "} (copy)" << std::endl;
#endif
    }

    Snitch(Snitch && other) : score(other.score) {
#ifdef ALERT
        std::cout << "+ Snitch {" << score << "} (move)" << std::endl;
#endif
    }

    ~Snitch() {
#ifdef ALERT
        std::cout << "- Snitch {" << score << '}' << std::endl;
#endif
    }

    bool operator == (const Snitch & other) const {
        return score == other.score;
    }

    bool operator != (const Snitch & other) const {
        return score != other.score;
    }
};


template <typename Allocator, typename T>
void print_contents(const my::fast_vector<Snitch, Allocator> & things, T && name) {
    std::cout << name << " [" << std::endl;

    for (auto it = things.cbegin(); it != things.cend(); it++) {
        std::cout << "  Snitch {" << (*it).score << '}' << std::endl;
    }

    std::cout << "]" << std::endl;
}


template <typename Iterable>
void assert_filler(
    const Iterable & items,
    size_t count,
    const Snitch & filler
) {
    size_t offset = 0;

    for (auto it = items.cbegin(); it != items.cend(); it++) {
        assert(items   [offset] == *it);
        assert(items.at(offset) == *it);
        assert(filler           == *it);
        offset++;
    }

    assert(offset == count);
}


template <typename FirstIterable, typename SecondIterable>
void assert_range(
    const FirstIterable & items,
    const SecondIterable & protos
) {
    size_t offset = 0;
    auto proto = protos.begin();

    for (auto it = items.cbegin(); it != items.cend(); it++) {
        assert(items   [offset] == *it);
        assert(items.at(offset) == *it);
        assert(*proto           == *it);
        offset++;
        proto++;
    }
}


TEST(vector_tests, create_from_n_fillers) {
    Snitch filler {300};

    my::fast_vector<Snitch> balls(5, filler);

    assert_filler(balls, 5, filler);

    assert(balls.size()     == 5    );
    assert(balls.capacity() == 5    );
    assert(balls.empty()    == false);

    assert(balls.front() == filler);
    assert(balls.back()  == filler);
}


TEST(vector_tests, create_from_n_fillers_custom_allocator) {
    Snitch filler {300};

    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls(5, filler);

    assert_filler(balls, 5, filler);

    assert(balls.size()     == 5    );
    assert(balls.capacity() == 5    );
    assert(balls.empty()    == false);

    assert(balls.front() == filler);
    assert(balls.back()  == filler);
}


TEST(vector_tests, create_n) {
    my::fast_vector<Snitch> balls(2);

    assert_filler(balls, 2, Snitch());

    assert(balls.size()     == 2    );
    assert(balls.capacity() == 2    );
    assert(balls.empty()    == false);

    assert(balls.front() == Snitch());
    assert(balls.back()  == Snitch());
}


TEST(vector_tests, create_n_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls(2, my::debug_allocator<Snitch>());

    assert_filler(balls, 2, Snitch());

    assert(balls.size()     == 2    );
    assert(balls.capacity() == 2    );
    assert(balls.empty()    == false);

    assert(balls.front() == Snitch());
    assert(balls.back()  == Snitch());
}


TEST(vector_tests, create_default) {
    my::fast_vector<Snitch> balls;

    assert_filler(balls, 0, Snitch());

    assert(balls.size()     == 0                      );
    assert(balls.capacity() == VECTOR_DEFAULT_CAPACITY);
    assert(balls.empty()    == true                   );
}


TEST(vector_tests, create_default_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls;

    assert_filler(balls, 0, Snitch());

    assert(balls.size()     == 0                      );
    assert(balls.capacity() == VECTOR_DEFAULT_CAPACITY);
    assert(balls.empty()    == true                   );
}


TEST(vector_tests, create_from_range) {
    std::vector<Snitch> ufos = { Snitch {15}, Snitch {21}, Snitch {36} };

    my::fast_vector<Snitch> balls(ufos.cbegin(), ufos.cend());

    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {36} });

    assert(balls.size()     == 3    );
    assert(balls.capacity() == 3    );
    assert(balls.empty()    == false);

    assert(balls.front() == Snitch {15});
    assert(balls.back()  == Snitch {36});
}


TEST(vector_tests, create_from_range_custom_allocator) {
    std::vector<Snitch, my::debug_allocator<Snitch>> ufos = { Snitch {15}, Snitch {21}, Snitch {36} };

    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls(ufos.cbegin(), ufos.cend());

    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {36} });

    assert(balls.size()     == 3    );
    assert(balls.capacity() == 3    );
    assert(balls.empty()    == false);

    assert(balls.front() == Snitch {15});
    assert(balls.back()  == Snitch {36});
}


TEST(vector_tests, create_from_list) {
    my::fast_vector<Snitch> balls = { Snitch {15}, Snitch {36} };

    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(balls.size()     == 2    );
    assert(balls.capacity() == 2    );
    assert(balls.empty()    == false);

    assert(balls.front() == Snitch {15});
    assert(balls.back()  == Snitch {36});
}


TEST(vector_tests, create_from_list_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {15}, Snitch {36} };

    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(balls.size()     == 2    );
    assert(balls.capacity() == 2    );
    assert(balls.empty()    == false);

    assert(balls.front() == Snitch {15});
    assert(balls.back()  == Snitch {36});
}


TEST(vector_tests, create_copy) {
    my::fast_vector<Snitch> balls { Snitch {15}, Snitch {36} };

    my::fast_vector<Snitch> ufos(balls);

    assert_range(ufos,  std::initializer_list { Snitch {15}, Snitch {36} });
    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(balls.size()     == ufos.size()    );
    assert(balls.capacity() == ufos.capacity());
    assert(balls.empty()    == ufos.empty()   );
}


TEST(vector_tests, create_copy_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls { Snitch {15}, Snitch {36} };

    my::fast_vector<Snitch, my::debug_allocator<Snitch>> ufos(balls);

    assert_range(ufos,  std::initializer_list { Snitch {15}, Snitch {36} });
    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(balls.size()     == ufos.size()    );
    assert(balls.capacity() == ufos.capacity());
    assert(balls.empty()    == ufos.empty()   );
}


TEST(vector_tests, create_move) {
    my::fast_vector<Snitch> balls { Snitch {15}, Snitch {36} };

    my::fast_vector<Snitch> ufos(std::move(balls));

    assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(ufos.size()     == 2    );
    assert(ufos.capacity() == 2    );
    assert(ufos.empty()    == false);

    assert(balls.size() == 0);
}


TEST(vector_tests, create_move_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls { Snitch {15}, Snitch {36} };

    my::fast_vector<Snitch, my::debug_allocator<Snitch>> ufos(std::move(balls));

    assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(ufos.size()     == 2    );
    assert(ufos.capacity() == 2    );
    assert(ufos.empty()    == false);

    assert(balls.size() == 0);
}


TEST(vector_tests, swap) {
    my::fast_vector<Snitch> balls { Snitch {15}, Snitch {36} };

    my::fast_vector<Snitch> ufos(10);

    balls.swap(ufos);

    assert_filler(balls, 10, Snitch());
    assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(balls.size()     == 10   );
    assert(balls.capacity() == 10   );
    assert(balls.empty()    == false);

    assert(ufos.size()     == 2    );
    assert(ufos.capacity() == 2    );
    assert(ufos.empty()    == false);
}


TEST(vector_tests, swap_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls { Snitch {15}, Snitch {36} };

    my::fast_vector<Snitch, my::debug_allocator<Snitch>> ufos(10);

    balls.swap(ufos);

    assert_filler(balls, 10, Snitch());
    assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(balls.size()     == 10   );
    assert(balls.capacity() == 10   );
    assert(balls.empty()    == false);

    assert(ufos.size()     == 2    );
    assert(ufos.capacity() == 2    );
    assert(ufos.empty()    == false);
}


TEST(vector_tests, operator_copy) {
    my::fast_vector<Snitch> balls { Snitch {15}, Snitch {36} };

    my::fast_vector<Snitch> ufos;
    ufos = balls;

    assert_range(ufos,  std::initializer_list { Snitch {15}, Snitch {36} });
    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(balls.size()     == ufos.size()    );
    assert(balls.capacity() == ufos.capacity());
    assert(balls.empty()    == ufos.empty()   );
}


TEST(vector_tests, operator_copy_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls { Snitch {15}, Snitch {36} };

    my::fast_vector<Snitch, my::debug_allocator<Snitch>> ufos;
    ufos = balls;

    assert_range(ufos,  std::initializer_list { Snitch {15}, Snitch {36} });
    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(balls.size()     == ufos.size()    );
    assert(balls.capacity() == ufos.capacity());
    assert(balls.empty()    == ufos.empty()   );
}


TEST(vector_tests, operator_move) {
    my::fast_vector<Snitch> balls { Snitch {15}, Snitch {36} };

    my::fast_vector<Snitch> ufos;
    ufos = std::move(balls);

    assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(ufos.size()     == 2    );
    assert(ufos.capacity() == 2    );
    assert(ufos.empty()    == false);
}


TEST(vector_tests, operator_move_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls { Snitch {15}, Snitch {36} };

    my::fast_vector<Snitch, my::debug_allocator<Snitch>> ufos;
    ufos = std::move(balls);

    assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(ufos.size()     == 2    );
    assert(ufos.capacity() == 2    );
    assert(ufos.empty()    == false);
}


TEST(vector_tests, operator_list) {
    my::fast_vector<Snitch> ufos;
    ufos = { Snitch {15}, Snitch {36} };

    assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(ufos.size()     == 2    );
    assert(ufos.capacity() == 2    );
    assert(ufos.empty()    == false);
}


TEST(vector_tests, operator_list_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> ufos;
    ufos = { Snitch {15}, Snitch {36} };

    assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

    assert(ufos.size()     == 2    );
    assert(ufos.capacity() == 2    );
    assert(ufos.empty()    == false);
}


TEST(vector_tests, assign_n_fillers) {
    my::fast_vector<Snitch> balls;

    Snitch filler {300};
    balls.assign(5, filler);

    assert_filler(balls, 5, filler);

    assert(balls.size()     == 5    );
    assert(balls.capacity() == 5    );
    assert(balls.empty()    == false);

    assert(balls.front() == filler);
    assert(balls.back()  == filler);
}


TEST(vector_tests, assign_n_fillers_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls;

    Snitch filler {300};
    balls.assign(5, filler);

    assert_filler(balls, 5, filler);

    assert(balls.size()     == 5    );
    assert(balls.capacity() == 5    );
    assert(balls.empty()    == false);

    assert(balls.front() == filler);
    assert(balls.back()  == filler);
}


TEST(vector_tests, assign_list) {
    my::fast_vector<Snitch> balls;

    Snitch filler {300};
    std::initializer_list list = { Snitch {15}, Snitch {36} };

    balls.assign(list.begin(), list.end());

    assert_range(balls, list);

    assert(balls.size()     == 2    );
    assert(balls.capacity() == 2    );
    assert(balls.empty()    == false);

    assert(balls.front() == Snitch {15});
    assert(balls.back()  == Snitch {36});
}


TEST(vector_tests, assign_list_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls;

    Snitch filler {300};
    std::initializer_list list = { Snitch {15}, Snitch {36} };

    balls.assign(list.begin(), list.end());

    assert_range(balls, list);

    assert(balls.size()     == 2    );
    assert(balls.capacity() == 2    );
    assert(balls.empty()    == false);

    assert(balls.front() == Snitch {15});
    assert(balls.back()  == Snitch {36});
}


TEST(vector_tests, clear) {
    my::fast_vector<Snitch> balls = { Snitch {15}, Snitch {36} };
    balls.clear();

    assert(balls.size()     == 0                      );
    assert(balls.capacity() == VECTOR_DEFAULT_CAPACITY);
    assert(balls.empty()    == true                   );
}


TEST(vector_tests, clear_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {15}, Snitch {36} };
    balls.clear();

    assert(balls.size()     == 0                      );
    assert(balls.capacity() == VECTOR_DEFAULT_CAPACITY);
    assert(balls.empty()    == true                   );
}


TEST(vector_tests, reserve) {
    my::fast_vector<Snitch> balls = { Snitch {1}, Snitch {2}, Snitch {3} };
    balls.reserve(30);

    assert(balls.size()     == 3    );
    assert(balls.capacity() == 30   );
    assert(balls.empty()    == false);
}


TEST(vector_tests, reserve_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {1}, Snitch {2}, Snitch {3} };
    balls.reserve(30);

    assert(balls.size()     == 3    );
    assert(balls.capacity() == 30   );
    assert(balls.empty()    == false);
}


TEST(vector_tests, shrink) {
    my::fast_vector<Snitch> balls = { Snitch {1}, Snitch {2}, Snitch {3} };

    balls.reserve(10);
    balls.shrink_to_fit();

    assert(balls.size()     == 3    );
    assert(balls.capacity() == 3    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, shrink_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {1}, Snitch {2}, Snitch {3} };

    balls.reserve(10);
    balls.shrink_to_fit();

    assert(balls.size()     == 3    );
    assert(balls.capacity() == 3    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, resize) {
    my::fast_vector<Snitch> balls = { Snitch {1}, Snitch {2}, Snitch {3} };

    balls.resize(6, Snitch {10});

    assert_range(balls, std::initializer_list {
        Snitch {1}, Snitch {2}, Snitch {3}, Snitch {10}, Snitch {10}, Snitch {10}
    });

    assert(balls.size()     == 6    );
    assert(balls.empty()    == false);

    balls.resize(2);

    assert_range(balls, std::initializer_list { Snitch {1}, Snitch {2} });

    assert(balls.size()     == 2    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, resize_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {1}, Snitch {2}, Snitch {3} };

    balls.resize(6, Snitch {10});

    assert_range(balls, std::initializer_list {
        Snitch {1}, Snitch {2}, Snitch {3}, Snitch {10}, Snitch {10}, Snitch {10}
    });

    assert(balls.size()     == 6    );
    assert(balls.empty()    == false);

    balls.resize(2);

    assert_range(balls, std::initializer_list { Snitch {1}, Snitch {2} });

    assert(balls.size()     == 2    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, emplace_back) {
    my::fast_vector<Snitch> balls;
    balls.reserve(20);

    balls.emplace_back(15);
    balls.emplace_back(21);
    balls.emplace_back(36);

    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {36} });

    assert(balls.size()     == 3    );
    assert(balls.capacity() == 20   );
    assert(balls.empty()    == false);
}


TEST(vector_tests, emplace_back_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls;
    balls.reserve(20);

    balls.emplace_back(15);
    balls.emplace_back(21);
    balls.emplace_back(36);

    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {36} });

    assert(balls.size()     == 3    );
    assert(balls.capacity() == 20   );
    assert(balls.empty()    == false);
}


TEST(vector_tests, emplace) {
    my::fast_vector<Snitch> balls = { Snitch {15}, Snitch {21}, Snitch {36} };
    balls.reserve(4);

    balls.emplace(balls.begin() + 2, 25);

    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {25}, Snitch {36} });

    assert(balls.size()     == 4    );
    assert(balls.capacity() == 4    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, emplace_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {15}, Snitch {21}, Snitch {36} };
    balls.reserve(4);

    balls.emplace(balls.begin() + 2, 25);

    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {25}, Snitch {36} });

    assert(balls.size()     == 4    );
    assert(balls.capacity() == 4    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, push_back) {
    my::fast_vector<Snitch> balls;
    balls.reserve(20);

    balls.push_back(Snitch {15});
    balls.push_back(Snitch {21});
    balls.push_back(Snitch {36});

    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {36} });

    assert(balls.size()     == 3    );
    assert(balls.capacity() == 20   );
    assert(balls.empty()    == false);
}


TEST(vector_tests, push_back_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls;
    balls.reserve(20);

    balls.push_back(Snitch {15});
    balls.push_back(Snitch {21});
    balls.push_back(Snitch {36});

    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {36} });

    assert(balls.size()     == 3    );
    assert(balls.capacity() == 20   );
    assert(balls.empty()    == false);
}


TEST(vector_tests, pop_back) {
    my::fast_vector<Snitch> balls = { Snitch {15}, Snitch {21}, Snitch {36} };

    balls.pop_back();

    assert_range(balls, std::initializer_list {
        Snitch {15}, Snitch {21}
    });

    assert(balls.size()     == 2    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, pop_back_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {15}, Snitch {21}, Snitch {36} };

    balls.pop_back();

    assert_range(balls, std::initializer_list {
        Snitch {15}, Snitch {21}
    });

    assert(balls.size()     == 2    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, insert_one) {
    my::fast_vector<Snitch> balls = { Snitch {15}, Snitch {21}, Snitch {36} };
    balls.reserve(4);

    balls.insert(balls.begin() + 2, Snitch {25});

    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {25}, Snitch {36} });

    Snitch additional {17};
    balls.insert(balls.begin() + 1, additional);

    assert_range(balls, std::initializer_list {
        Snitch {15}, Snitch {17}, Snitch {21}, Snitch {25}, Snitch {36}
    });

    assert(balls.size()     == 5    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, insert_one_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {15}, Snitch {21}, Snitch {36} };
    balls.reserve(4);

    balls.insert(balls.begin() + 2, Snitch {25});

    assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {25}, Snitch {36} });

    Snitch additional {17};
    balls.insert(balls.begin() + 1, additional);

    assert_range(balls, std::initializer_list {
        Snitch {15}, Snitch {17}, Snitch {21}, Snitch {25}, Snitch {36}
    });

    assert(balls.size()     == 5    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, insert_n) {
    my::fast_vector<Snitch> balls = { Snitch {15}, Snitch {21}, Snitch {36} };
    std::initializer_list list = { Snitch {16}, Snitch {17}, Snitch {20} };

    balls.insert(balls.begin() + 1, list.begin(), list.end());

    assert_range(balls, std::initializer_list {
        Snitch {15}, Snitch {16}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
    });

    assert(balls.size()     == 6    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, insert_n_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {15}, Snitch {21}, Snitch {36} };
    std::initializer_list list = { Snitch {16}, Snitch {17}, Snitch {20} };

    balls.insert(balls.begin() + 1, list.begin(), list.end());

    assert_range(balls, std::initializer_list {
        Snitch {15}, Snitch {16}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
    });

    assert(balls.size()     == 6    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, erase_one) {
    my::fast_vector<Snitch> balls = {
        Snitch {15}, Snitch {16}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
    };

    balls.erase(balls.cbegin() + 1);

    assert_range(balls, std::initializer_list {
        Snitch {15}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
    });

    assert(balls.size()     == 5    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, erase_one_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls = {
        Snitch {15}, Snitch {16}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
    };

    balls.erase(balls.cbegin() + 1);

    assert_range(balls, std::initializer_list {
        Snitch {15}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
    });

    assert(balls.size()     == 5    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, erase_n) {
    my::fast_vector<Snitch> balls = {
        Snitch {15}, Snitch {16}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
    };

    balls.erase(balls.cbegin() + 1, balls.cbegin() + 5);

    assert_range(balls, std::initializer_list {
        Snitch {15}, Snitch {36}
    });

    assert(balls.size()     == 2    );
    assert(balls.empty()    == false);
}


TEST(vector_tests, erase_n_custom_allocator) {
    my::fast_vector<Snitch, my::debug_allocator<Snitch>> balls = {
        Snitch {15}, Snitch {16}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
    };

    balls.erase(balls.cbegin() + 1, balls.cbegin() + 5);

    assert_range(balls, std::initializer_list {
        Snitch {15}, Snitch {36}
    });

    assert(balls.size()     == 2    );
    assert(balls.empty()    == false);
}


int main(int argc, char * argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}