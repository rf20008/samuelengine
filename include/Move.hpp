#ifndef CS3520_MINIPROJECT_MOVE
#define CS3520_MINIPROJECT_MOVE
#include "Square.hpp"
struct Move {
		Square startingSquare;
		Square endingSquare;
		char promotion = '\0';
		bool operator<(const Move &other) const;
		bool operator==(const Move &other) const;
		std::string operator()() const; 
};
#endif
