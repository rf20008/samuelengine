#ifndef CS3520_MINIPROJECT_KNIGHT
#define CS3520_MINIPROJECT_KNIGHT

#include <set>

#include "Piece.hpp"

class Knight : public Piece {
	public:
		Knight(bool ownedbyWhite) : Piece(ownedbyWhite) {}
		virtual char symbol() const {
			return (belongsToWhite ? 'N' : 'n'); // k is already used for king,
			// //so we adopt Forsyth-Edwards Notaton's convention of using N to denote knight
		}
		virtual ~Knight() {}
};

#endif
