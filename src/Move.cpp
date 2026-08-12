#include "Move.hpp"
#include "Errors.hpp"

bool Move::operator<(const Move &other) const {
	if (this->startingSquare != other.startingSquare) {
		return (this->startingSquare < other.startingSquare);
	}
	return (this->endingSquare < other.endingSquare);
}
