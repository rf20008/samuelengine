#include "Move.hpp"
#include "Errors.hpp"

bool Move::operator<(const Move &other) const {
	if (this->startingSquare != other.startingSquare) {
		return (this->startingSquare < other.startingSquare);
	}
	if (this->endingSquare != other.endingSquare) {
		return (this->endingSquare < other.endingSquare);
	}
	return (this->promotion < other.promotion);
}

bool Move::operator==(const Move &other) const { return (this->startingSquare == other.startingSquare) && (this->endingSquare == other.endingSquare) && (this->promotion == other.promotion); }
