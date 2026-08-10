#ifndef CS3520_MINIPROJECT_QUEEN
#define CS3520_MINIPROJECT_QUEEN

#include <set>

#include "Piece.hpp"


class Queen : public Piece {
    public:
        Queen(bool ownedbyWhite) : Piece(ownedbyWhite) {}
        virtual char symbol() const {
            return (belongsToWhite?'Q':'q');
        }
	virtual ~Queen() {}
};

#endif
