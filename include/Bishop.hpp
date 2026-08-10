#ifndef CS3520_MINIPROJECT_BISHOP
#define CS3520_MINIPROJECT_BISHOP

#include <set>

#include "Piece.hpp"


class Bishop : public Piece {
    public:
        Bishop(bool ownedbyWhite) : Piece(ownedbyWhite) {}
        virtual char symbol() const {
            return (belongsToWhite?'B':'b');
        }
	virtual ~Bishop() {}
};

#endif
