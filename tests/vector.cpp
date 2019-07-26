#include <iostream>
#include <cassert>

#include <vector>

#include "../my/debug_allocator.h"
#include "../my/vector.h"


struct Snitch {
	int score;

	explicit Snitch(int score = 100) : score(score) {
		// std::cout << "+ Snitch {" << score << '}' << std::endl;
	}

	Snitch(const Snitch & other) : score(other.score) {
		// std::cout << "+ Snitch {" << score << "} (copy)" << std::endl;
	}

	Snitch(Snitch && other) : score(other.score) {
		// std::cout << "+ Snitch {" << score << "} (move)" << std::endl;
	}

	~Snitch() {
		// std::cout << "- Snitch {" << score << '}' << std::endl;
	}

	bool operator == (const Snitch & other) const {
		return score == other.score;
	}

	bool operator != (const Snitch & other) const {
		return score != other.score;
	}
};


template <typename Allocator, typename T>
void print_contents(const my::vector<Snitch, Allocator> & things, T && name) {
	std::cout << name << " [" << std::endl;

	for (auto it = things.cbegin(); it != things.cend(); it++) {
		std::cout << "  Snitch {" << (*it).score << '}' << std::endl;
	}

	std::cout << "]" << std::endl;
}


template <typename Allocator>
void assert_filler(
	const my::vector<Snitch, Allocator> & items,
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

	std::cout << "[+] Succeded assertion: " << count << " fillers" << std::endl;
}


template <typename Allocator, typename Iterable>
void assert_range(
	const my::vector<Snitch, Allocator> & items,
	const Iterable & protos
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

	std::cout << "[+] Succeded assertion: " << offset << " proto's" << std::endl;
}


void test_create_from_n_fillers() {
	std::cout << std::endl;
	std::cout << "> test_create_from_n_fillers" << std::endl;
	std::cout << "============================" << std::endl;

	Snitch filler {300};

	std::cout << "+ Vector 'balls' of 5 fillers" << std::endl;
	my::vector<Snitch> balls(5, filler);

	assert_filler(balls, 5, filler);

	assert(balls.size()     == 5    );
	assert(balls.capacity() == 5    );
	assert(balls.empty()    == false);

	assert(balls.front() == filler);
	assert(balls.back()  == filler);

	std::cout << std::endl;
}


void test_create_from_n_fillers_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_create_from_n_fillers_custom_allocator" << std::endl;
	std::cout << "=============================================" << std::endl;

	Snitch filler {300};

	std::cout << "+ Vector 'balls' of 5 fillers" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls(5, filler);

	assert_filler(balls, 5, filler);

	assert(balls.size()     == 5    );
	assert(balls.capacity() == 5    );
	assert(balls.empty()    == false);

	assert(balls.front() == filler);
	assert(balls.back()  == filler);

	std::cout << std::endl;
}


void test_create_n() {
	std::cout << std::endl;
	std::cout << "> test_create_n" << std::endl;
	std::cout << "===============" << std::endl;

	std::cout << "+ Vector 'balls' of 2" << std::endl;
	my::vector<Snitch> balls(2);

	assert_filler(balls, 2, Snitch());

	assert(balls.size()     == 2    );
	assert(balls.capacity() == 2    );
	assert(balls.empty()    == false);

	assert(balls.front() == Snitch());
	assert(balls.back()  == Snitch());

	std::cout << std::endl;
}


void test_create_n_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_create_n_custom_allocator" << std::endl;
	std::cout << "================================" << std::endl;

	std::cout << "+ Vector 'balls' of 2" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls(2, my::debug_allocator<Snitch>());

	assert_filler(balls, 2, Snitch());

	assert(balls.size()     == 2    );
	assert(balls.capacity() == 2    );
	assert(balls.empty()    == false);

	assert(balls.front() == Snitch());
	assert(balls.back()  == Snitch());

	std::cout << std::endl;
}


void test_create_default() {
	std::cout << std::endl;
	std::cout << "> test_create_default" << std::endl;
	std::cout << "=====================" << std::endl;

	std::cout << "+ Vector 'balls'" << std::endl;
	my::vector<Snitch> balls;

	assert_filler(balls, 0, Snitch());

	assert(balls.size()     == 0                      );
	assert(balls.capacity() == VECTOR_DEFAULT_CAPACITY);
	assert(balls.empty()    == true                   );

	std::cout << std::endl;
}


void test_create_default_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_create_default_custom_allocator" << std::endl;
	std::cout << "======================================" << std::endl;

	std::cout << "+ Vector 'balls'" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls;

	assert_filler(balls, 0, Snitch());

	assert(balls.size()     == 0                      );
	assert(balls.capacity() == VECTOR_DEFAULT_CAPACITY);
	assert(balls.empty()    == true                   );

	std::cout << std::endl;
}


void test_create_from_range() {
	std::cout << std::endl;
	std::cout << "> test_create_from_range" << std::endl;
	std::cout << "========================" << std::endl;

	std::cout << "+ Vector (std) 'ufos' of {}" << std::endl;
	std::vector<Snitch> ufos = { Snitch {15}, Snitch {21}, Snitch {36} };

	std::cout << "+ Vector 'balls' of 'ufos' range" << std::endl;
	my::vector<Snitch> balls(ufos.cbegin(), ufos.cend());

	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {36} });

	assert(balls.size()     == 3    );
	assert(balls.capacity() == 3    );
	assert(balls.empty()    == false);

	assert(balls.front() == Snitch {15});
	assert(balls.back()  == Snitch {36});

	std::cout << std::endl;
}


void test_create_from_range_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_create_from_range_custom_allocator" << std::endl;
	std::cout << "=========================================" << std::endl;

	std::cout << "+ Vector (std) 'ufos' of {}" << std::endl;
	std::vector<Snitch, my::debug_allocator<Snitch>> ufos = { Snitch {15}, Snitch {21}, Snitch {36} };

	std::cout << "+ Vector 'balls' of 'ufos' range" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls(ufos.cbegin(), ufos.cend());

	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {36} });

	assert(balls.size()     == 3    );
	assert(balls.capacity() == 3    );
	assert(balls.empty()    == false);

	assert(balls.front() == Snitch {15});
	assert(balls.back()  == Snitch {36});

	std::cout << std::endl;
}


void test_create_from_list() {
	std::cout << std::endl;
	std::cout << "> test_create_from_list" << std::endl;
	std::cout << "=======================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls = { Snitch {15}, Snitch {36} };

	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(balls.size()     == 2    );
	assert(balls.capacity() == 2    );
	assert(balls.empty()    == false);

	assert(balls.front() == Snitch {15});
	assert(balls.back()  == Snitch {36});

	std::cout << std::endl;
}


void test_create_from_list_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_create_from_list_custom_allocator" << std::endl;
	std::cout << "========================================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {15}, Snitch {36} };

	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(balls.size()     == 2    );
	assert(balls.capacity() == 2    );
	assert(balls.empty()    == false);

	assert(balls.front() == Snitch {15});
	assert(balls.back()  == Snitch {36});

	std::cout << std::endl;
}


void test_create_copy() {
	std::cout << std::endl;
	std::cout << "> test_create_copy" << std::endl;
	std::cout << "==================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls { Snitch {15}, Snitch {36} };

	std::cout << "+ Vector 'ufos' of copy" << std::endl;
	my::vector<Snitch> ufos(balls);

	assert_range(ufos,  std::initializer_list { Snitch {15}, Snitch {36} });
	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(balls.size()     == ufos.size()    );
	assert(balls.capacity() == ufos.capacity());
	assert(balls.empty()    == ufos.empty()   );

	std::cout << std::endl;
}


void test_create_copy_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_create_copy_custom_allocator" << std::endl;
	std::cout << "===================================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls { Snitch {15}, Snitch {36} };

	std::cout << "+ Vector 'ufos' of copy" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> ufos(balls);

	assert_range(ufos,  std::initializer_list { Snitch {15}, Snitch {36} });
	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(balls.size()     == ufos.size()    );
	assert(balls.capacity() == ufos.capacity());
	assert(balls.empty()    == ufos.empty()   );

	std::cout << std::endl;
}


void test_create_move() {
	std::cout << std::endl;
	std::cout << "> test_create_move" << std::endl;
	std::cout << "==================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls { Snitch {15}, Snitch {36} };

	std::cout << "+ Vector 'ufos' of move" << std::endl;
	my::vector<Snitch> ufos(std::move(balls));

	assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(ufos.size()     == 2    );
	assert(ufos.capacity() == 2    );
	assert(ufos.empty()    == false);

	assert(balls.size() == 0);

	std::cout << std::endl;
}


void test_create_move_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_create_move_custom_allocator" << std::endl;
	std::cout << "===================================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls { Snitch {15}, Snitch {36} };

	std::cout << "+ Vector 'ufos' of move" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> ufos(std::move(balls));

	assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(ufos.size()     == 2    );
	assert(ufos.capacity() == 2    );
	assert(ufos.empty()    == false);

	assert(balls.size() == 0);

	std::cout << std::endl;
}


void test_swap() {
	std::cout << std::endl;
	std::cout << "> test_swap" << std::endl;
	std::cout << "===========" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls { Snitch {15}, Snitch {36} };

	std::cout << "+ Vector 'ufos' of 10" << std::endl;
	my::vector<Snitch> ufos(10);

	balls.swap(ufos);

	assert_filler(balls, 10, Snitch());
	assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(balls.size()     == 10   );
	assert(balls.capacity() == 10   );
	assert(balls.empty()    == false);

	assert(ufos.size()     == 2    );
	assert(ufos.capacity() == 2    );
	assert(ufos.empty()    == false);

	std::cout << std::endl;
}


void test_swap_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_swap_custom_allocator" << std::endl;
	std::cout << "============================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls { Snitch {15}, Snitch {36} };

	std::cout << "+ Vector 'ufos' of 10" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> ufos(10);

	balls.swap(ufos);

	assert_filler(balls, 10, Snitch());
	assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(balls.size()     == 10   );
	assert(balls.capacity() == 10   );
	assert(balls.empty()    == false);

	assert(ufos.size()     == 2    );
	assert(ufos.capacity() == 2    );
	assert(ufos.empty()    == false);

	std::cout << std::endl;
}


void test_operator_copy() {
	std::cout << std::endl;
	std::cout << "> test_operator_copy" << std::endl;
	std::cout << "====================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls { Snitch {15}, Snitch {36} };

	std::cout << "+ Vector 'ufos'" << std::endl;
	my::vector<Snitch> ufos;
	ufos = balls;

	assert_range(ufos,  std::initializer_list { Snitch {15}, Snitch {36} });
	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(balls.size()     == ufos.size()    );
	assert(balls.capacity() == ufos.capacity());
	assert(balls.empty()    == ufos.empty()   );

	std::cout << std::endl;
}


void test_operator_copy_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_operator_copy_custom_allocator" << std::endl;
	std::cout << "=====================================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls { Snitch {15}, Snitch {36} };

	std::cout << "+ Vector 'ufos'" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> ufos;
	ufos = balls;

	assert_range(ufos,  std::initializer_list { Snitch {15}, Snitch {36} });
	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(balls.size()     == ufos.size()    );
	assert(balls.capacity() == ufos.capacity());
	assert(balls.empty()    == ufos.empty()   );

	std::cout << std::endl;
}


void test_operator_move() {
	std::cout << std::endl;
	std::cout << "> test_operator_move" << std::endl;
	std::cout << "====================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls { Snitch {15}, Snitch {36} };

	std::cout << "+ Vector 'ufos'" << std::endl;
	my::vector<Snitch> ufos;
	ufos = std::move(balls);

	assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(ufos.size()     == 2    );
	assert(ufos.capacity() == 2    );
	assert(ufos.empty()    == false);

	std::cout << std::endl;
}


void test_operator_move_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_operator_move_custom_allocator" << std::endl;
	std::cout << "=====================================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls { Snitch {15}, Snitch {36} };

	std::cout << "+ Vector 'ufos'" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> ufos;
	ufos = std::move(balls);

	assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(ufos.size()     == 2    );
	assert(ufos.capacity() == 2    );
	assert(ufos.empty()    == false);

	std::cout << std::endl;
}


void test_operator_list() {
	std::cout << std::endl;
	std::cout << "> test_operator_list" << std::endl;
	std::cout << "====================" << std::endl;

	std::cout << "+ Vector 'ufos'" << std::endl;
	my::vector<Snitch> ufos;
	ufos = { Snitch {15}, Snitch {36} };

	assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(ufos.size()     == 2    );
	assert(ufos.capacity() == 2    );
	assert(ufos.empty()    == false);

	std::cout << std::endl;
}


void test_operator_list_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_operator_list_custom_allocator" << std::endl;
	std::cout << "=====================================" << std::endl;

	std::cout << "+ Vector 'ufos'" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> ufos;
	ufos = { Snitch {15}, Snitch {36} };

	assert_range(ufos, std::initializer_list { Snitch {15}, Snitch {36} });

	assert(ufos.size()     == 2    );
	assert(ufos.capacity() == 2    );
	assert(ufos.empty()    == false);

	std::cout << std::endl;
}


void test_assign_n_fillers() {
	std::cout << std::endl;
	std::cout << "> test_assign_n_fillers" << std::endl;
	std::cout << "=======================" << std::endl;

	std::cout << "+ Vector 'balls'" << std::endl;
	my::vector<Snitch> balls;

	Snitch filler {300};
	std::cout << "'balls' -> assign 5 fillers" << std::endl;
	balls.assign(5, filler);

	assert_filler(balls, 5, filler);

	assert(balls.size()     == 5    );
	assert(balls.capacity() == 5    );
	assert(balls.empty()    == false);

	assert(balls.front() == filler);
	assert(balls.back()  == filler);

	std::cout << std::endl;
}


void test_assign_n_fillers_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_assign_n_fillers_custom_allocator" << std::endl;
	std::cout << "========================================" << std::endl;

	std::cout << "+ Vector 'balls'" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls;

	Snitch filler {300};
	std::cout << "'balls' -> assign 5 fillers" << std::endl;
	balls.assign(5, filler);

	assert_filler(balls, 5, filler);

	assert(balls.size()     == 5    );
	assert(balls.capacity() == 5    );
	assert(balls.empty()    == false);

	assert(balls.front() == filler);
	assert(balls.back()  == filler);

	std::cout << std::endl;
}


void test_assign_list() {
	std::cout << std::endl;
	std::cout << "> test_assign_list" << std::endl;
	std::cout << "==================" << std::endl;

	std::cout << "+ Vector 'balls'" << std::endl;
	my::vector<Snitch> balls;

	Snitch filler {300};
	std::initializer_list list = { Snitch {15}, Snitch {36} };

	std::cout << "'balls' -> assign list" << std::endl;
	balls.assign(list.begin(), list.end());

	assert_range(balls, list);

	assert(balls.size()     == 2    );
	assert(balls.capacity() == 2    );
	assert(balls.empty()    == false);

	assert(balls.front() == Snitch {15});
	assert(balls.back()  == Snitch {36});

	std::cout << std::endl;
}


void test_assign_list_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_assign_list_custom_allocator" << std::endl;
	std::cout << "===================================" << std::endl;

	std::cout << "+ Vector 'balls'" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls;

	Snitch filler {300};
	std::initializer_list list = { Snitch {15}, Snitch {36} };

	std::cout << "'balls' -> assign list" << std::endl;
	balls.assign(list.begin(), list.end());

	assert_range(balls, list);

	assert(balls.size()     == 2    );
	assert(balls.capacity() == 2    );
	assert(balls.empty()    == false);

	assert(balls.front() == Snitch {15});
	assert(balls.back()  == Snitch {36});

	std::cout << std::endl;
}


void test_clear() {
	std::cout << std::endl;
	std::cout << "> test_clear" << std::endl;
	std::cout << "============" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls = { Snitch {15}, Snitch {36} };
	balls.clear();

	assert(balls.size()     == 0                      );
	assert(balls.capacity() == VECTOR_DEFAULT_CAPACITY);
	assert(balls.empty()    == true                   );

	std::cout << std::endl;
}


void test_clear_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_clear_custom_allocator" << std::endl;
	std::cout << "=============================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {15}, Snitch {36} };
	balls.clear();

	assert(balls.size()     == 0                      );
	assert(balls.capacity() == VECTOR_DEFAULT_CAPACITY);
	assert(balls.empty()    == true                   );

	std::cout << std::endl;
}


void test_reserve() {
	std::cout << std::endl;
	std::cout << "> test_reserve" << std::endl;
	std::cout << "==============" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls = { Snitch {1}, Snitch {2}, Snitch {3} };
	balls.reserve(30);

	assert(balls.size()     == 3    );
	assert(balls.capacity() == 30   );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_reserve_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_reserve_custom_allocator" << std::endl;
	std::cout << "===============================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {1}, Snitch {2}, Snitch {3} };
	balls.reserve(30);

	assert(balls.size()     == 3    );
	assert(balls.capacity() == 30   );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_shrink() {
	std::cout << std::endl;
	std::cout << "> test_shrink" << std::endl;
	std::cout << "=============" << std::endl;

	std::cout << "+ Vector 'balls'" << std::endl;
	my::vector<Snitch> balls = { Snitch {1}, Snitch {2}, Snitch {3} };

	balls.reserve(10);
	balls.shrink_to_fit();

	assert(balls.size()     == 3    );
	assert(balls.capacity() == 3    );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_shrink_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_shrink_custom_allocator" << std::endl;
	std::cout << "==============================" << std::endl;

	std::cout << "+ Vector 'balls'" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {1}, Snitch {2}, Snitch {3} };

	balls.reserve(10);
	balls.shrink_to_fit();

	assert(balls.size()     == 3    );
	assert(balls.capacity() == 3    );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_resize() {
	std::cout << std::endl;
	std::cout << "> test_resize" << std::endl;
	std::cout << "=============" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls = { Snitch {1}, Snitch {2}, Snitch {3} };

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

	std::cout << std::endl;
}


void test_resize_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_resize_custom_allocator" << std::endl;
	std::cout << "==============================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {1}, Snitch {2}, Snitch {3} };

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

	std::cout << std::endl;
}


void test_emplace_back() {
	std::cout << std::endl;
	std::cout << "> test_emplace_back" << std::endl;
	std::cout << "===================" << std::endl;

	std::cout << "+ Vector 'balls'" << std::endl;
	my::vector<Snitch> balls;
	balls.reserve(20);

	balls.emplace_back(15);
	balls.emplace_back(21);
	balls.emplace_back(36);

	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {36} });

	assert(balls.size()     == 3    );
	assert(balls.capacity() == 20   );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_emplace_back_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_emplace_back_custom_allocator" << std::endl;
	std::cout << "====================================" << std::endl;

	std::cout << "+ Vector 'balls'" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls;
	balls.reserve(20);

	balls.emplace_back(15);
	balls.emplace_back(21);
	balls.emplace_back(36);

	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {36} });

	assert(balls.size()     == 3    );
	assert(balls.capacity() == 20   );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_emplace() {
	std::cout << std::endl;
	std::cout << "> test_emplace" << std::endl;
	std::cout << "==============" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls = { Snitch {15}, Snitch {21}, Snitch {36} };
	balls.reserve(4);

	balls.emplace(balls.begin() + 2, 25);

	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {25}, Snitch {36} });

	assert(balls.size()     == 4    );
	assert(balls.capacity() == 4    );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_emplace_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_emplace_custom_allocator" << std::endl;
	std::cout << "===============================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {15}, Snitch {21}, Snitch {36} };
	balls.reserve(4);

	balls.emplace(balls.begin() + 2, 25);

	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {25}, Snitch {36} });

	assert(balls.size()     == 4    );
	assert(balls.capacity() == 4    );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_push_back() {
	std::cout << std::endl;
	std::cout << "> test_push_back" << std::endl;
	std::cout << "================" << std::endl;

	std::cout << "+ Vector 'balls'" << std::endl;
	my::vector<Snitch> balls;
	balls.reserve(20);

	balls.push_back(Snitch {15});
	balls.push_back(Snitch {21});
	balls.push_back(Snitch {36});

	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {36} });

	assert(balls.size()     == 3    );
	assert(balls.capacity() == 20   );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_push_back_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_push_back_custom_allocator" << std::endl;
	std::cout << "=================================" << std::endl;

	std::cout << "+ Vector 'balls'" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls;
	balls.reserve(20);

	balls.push_back(Snitch {15});
	balls.push_back(Snitch {21});
	balls.push_back(Snitch {36});

	assert_range(balls, std::initializer_list { Snitch {15}, Snitch {21}, Snitch {36} });

	assert(balls.size()     == 3    );
	assert(balls.capacity() == 20   );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_pop_back() {
	std::cout << std::endl;
	std::cout << "> test_pop_back" << std::endl;
	std::cout << "===============" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls = { Snitch {15}, Snitch {21}, Snitch {36} };

	balls.pop_back();

	assert_range(balls, std::initializer_list {
		Snitch {15}, Snitch {21}
	});

	assert(balls.size()     == 2    );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_pop_back_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_pop_back_custom_allocator" << std::endl;
	std::cout << "================================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {15}, Snitch {21}, Snitch {36} };

	balls.pop_back();

	assert_range(balls, std::initializer_list {
		Snitch {15}, Snitch {21}
	});

	assert(balls.size()     == 2    );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_insert_one() {
	std::cout << std::endl;
	std::cout << "> test_insert_one" << std::endl;
	std::cout << "=================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls = { Snitch {15}, Snitch {21}, Snitch {36} };
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

	std::cout << std::endl;
}


void test_insert_one_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_insert_one_custom_allocator" << std::endl;
	std::cout << "==================================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {15}, Snitch {21}, Snitch {36} };
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

	std::cout << std::endl;
}


void test_insert_n() {
	std::cout << std::endl;
	std::cout << "> test_insert_n" << std::endl;
	std::cout << "===============" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls = { Snitch {15}, Snitch {21}, Snitch {36} };
	std::initializer_list list = { Snitch {16}, Snitch {17}, Snitch {20} };

	balls.insert(balls.begin() + 1, list.begin(), list.end());

	assert_range(balls, std::initializer_list {
		Snitch {15}, Snitch {16}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
	});

	assert(balls.size()     == 6    );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_insert_n_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_insert_n_custom_allocator" << std::endl;
	std::cout << "================================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls = { Snitch {15}, Snitch {21}, Snitch {36} };
	std::initializer_list list = { Snitch {16}, Snitch {17}, Snitch {20} };

	balls.insert(balls.begin() + 1, list.begin(), list.end());

	assert_range(balls, std::initializer_list {
		Snitch {15}, Snitch {16}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
	});

	assert(balls.size()     == 6    );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_erase_one() {
	std::cout << std::endl;
	std::cout << "> test_erase_one" << std::endl;
	std::cout << "================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls = {
		Snitch {15}, Snitch {16}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
	};

	balls.erase(balls.cbegin() + 1);

	assert_range(balls, std::initializer_list {
		Snitch {15}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
	});

	assert(balls.size()     == 5    );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_erase_one_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_erase_one" << std::endl;
	std::cout << "================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls = {
		Snitch {15}, Snitch {16}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
	};

	balls.erase(balls.cbegin() + 1);

	assert_range(balls, std::initializer_list {
		Snitch {15}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
	});

	assert(balls.size()     == 5    );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_erase_n() {
	std::cout << std::endl;
	std::cout << "> test_erase_n" << std::endl;
	std::cout << "==============" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch> balls = {
		Snitch {15}, Snitch {16}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
	};

	balls.erase(balls.cbegin() + 1, balls.cbegin() + 5);

	assert_range(balls, std::initializer_list {
		Snitch {15}, Snitch {36}
	});

	assert(balls.size()     == 2    );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


void test_erase_n_custom_allocator() {
	std::cout << std::endl;
	std::cout << "> test_erase_n_custom_allocator" << std::endl;
	std::cout << "===============================" << std::endl;

	std::cout << "+ Vector 'balls' of {}" << std::endl;
	my::vector<Snitch, my::debug_allocator<Snitch>> balls = {
		Snitch {15}, Snitch {16}, Snitch {17}, Snitch {20}, Snitch {21}, Snitch {36}
	};

	balls.erase(balls.cbegin() + 1, balls.cbegin() + 5);

	assert_range(balls, std::initializer_list {
		Snitch {15}, Snitch {36}
	});

	assert(balls.size()     == 2    );
	assert(balls.empty()    == false);

	std::cout << std::endl;
}


int main(int argc, const char * argv[]) {
	std::cout << "START" << std::endl;
	std::cout << "=====" << std::endl;
	std::cout << std::endl;

	{
		test_create_from_n_fillers();
		test_create_from_n_fillers_custom_allocator();
		test_create_n();
		test_create_n_custom_allocator();
		test_create_default();
		test_create_default_custom_allocator();
		test_create_from_range();
		test_create_from_range_custom_allocator();
		test_create_from_list();
		test_create_from_list_custom_allocator();
		test_create_copy();
		test_create_copy_custom_allocator();
		test_create_move();
		test_create_move_custom_allocator();
		test_swap();
		test_swap_custom_allocator();
		test_operator_copy();
		test_operator_copy_custom_allocator();
		test_operator_move();
		test_operator_move_custom_allocator();
		test_operator_list();
		test_operator_list_custom_allocator();
		test_assign_n_fillers();
		test_assign_n_fillers_custom_allocator();
		test_assign_list();
		test_assign_list_custom_allocator();
		test_clear();
		test_clear_custom_allocator();
		test_reserve();
		test_reserve_custom_allocator();
		test_shrink();
		test_shrink_custom_allocator();
		test_resize();
		test_resize_custom_allocator();
		test_emplace_back();
		test_emplace_back_custom_allocator();
		test_emplace();
		test_emplace_custom_allocator();
		test_push_back();
		test_push_back_custom_allocator();
		test_insert_one();
		test_insert_one_custom_allocator();
		test_insert_n();
		test_insert_n_custom_allocator();
		test_erase_one();
		test_erase_one_custom_allocator();
		test_erase_n();
		test_erase_n_custom_allocator();
	}

	std::cout << std::endl;
	std::cout << "===" << std::endl;
	std::cout << "END" << std::endl;
	return 0;
}