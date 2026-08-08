#ifndef CS3520_MINIPROJECT_PAWN
#define CS3520_MINIPROJECT_PAWN

#include <set>

#include "Piece.hpp"


class Pawn : public Piece {
    public:
        Pawn(bool ownedbyWhite) : Piece(ownedbyWhite) {}
        virtual char symbol() const {
            return (belongsToWhite?'P':'p');
        }
};

#endif
