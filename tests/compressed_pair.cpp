#include <iostream>

#include "../my/compressed_pair.h"


struct Empty {};

struct NotEmpty {
	int score = 10;
};


int main() {
	std::cout << "sizeof    NotEmpty = " << sizeof(NotEmpty) << std::endl;
	std::cout << "sizeof       Empty = " << sizeof(Empty)    << std::endl;
	std::cout << std::endl;

	std::cout << "is_empty  NotEmpty = " << my::is_empty<NotEmpty>::value << std::endl;
	std::cout << "is_empty     Empty = " << my::is_empty<Empty>::value    << std::endl;
	std::cout << std::endl;

	my::compressed_pair<Empty, NotEmpty> pairA;
	std::cout << "sizeof       pairA = " << sizeof(pairA) << std::endl;
	std::cout << std::endl;

	my::compressed_pair<NotEmpty, Empty> pairB;
	std::cout << "sizeof       pairB = " << sizeof(pairB) << std::endl;
	std::cout << std::endl;

	my::compressed_pair<NotEmpty, NotEmpty> pairC;
	std::cout << "sizeof       pairC = " << sizeof(pairC) << std::endl;
	std::cout << std::endl;

	my::compressed_pair<Empty, Empty> pairD;
	std::cout << "sizeof       pairD = " << sizeof(pairD) << std::endl;
	std::cout << std::endl;
}