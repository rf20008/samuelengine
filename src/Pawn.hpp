#ifndef CS3520_MINIPROJECT_PIECE
#define CS3520_MINIPROJECT_PIECE

#include <set>

#include "Piece.hpp"


class Pawn : public Piece {
    public:
        Piece(bool ownedbyWhite) : belongsToWhite(ownedbyWhite) {}
        virtual char symbol() const {
            return (belongsToWhite?'P':'p')
        }
};

#endif
