#ifndef CS3520_MINIPROJECT_MOVE
#define CS3520_MINIPROJECT_MOVE
#include "Square.hpp"
struct Move {
		Square startingSquare;
		Square endingSquare;
		bool operator<(const Move &other) const;
		bool operator==(const Move &other) const { return startingSquare == other.startingSquare && endingSquare == other.endingSquare; }
		std::string operator()() const { return startingSquare.operator()() + endingSquare.operator()(); }
};
#endif
