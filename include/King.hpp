#ifndef CS3520_MINIPROJECT_KING
#define CS3520_MINIPROJECT_KING

#include <set>

#include "Piece.hpp"

class King : public Piece {
	public:
		King(bool ownedbyWhite) : Piece(ownedbyWhite) {}
		virtual char symbol() const { return (belongsToWhite ? 'K' : 'k'); }
		virtual ~King() {}
};

#endif
