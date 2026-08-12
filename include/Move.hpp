#ifndef CS3520_MINIPROJECT_MOVE
#define CS3520_MINIPROJECT_MOVE
#include "Square.hpp"
struct Move {
		Square startingSquare;
		Square endingSquare;
		char promotion = '\0';
        Move() : startingSquare("a1"), endingSquare("a1"), promotion('\0') {}
        Move(const Square start, const Square end, const char promo='\0') : startingSquare(start), endingSquare(end), promotion(promo) {}
        Move(const Move& other) = default;
        Move(Move&& other) = default;
        Move& operator=(const Move& other) = default;
        Move& operator=(Move&& other) = default;
        ~Move() = default;
		bool operator<(const Move &other) const;
		bool operator==(const Move &other) const;
		std::string operator()() const; 
};
#endif
