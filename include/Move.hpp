#ifndef CS3520_MINIPROJECT_MOVE
#define CS3520_MINIPROJECT_MOVE
#include "Square.hpp"
struct Move {
    Square startingSquare;
    Square endingSquare;
    bool operator<(const Move& other) const;
};
#endif
