#ifndef CS3520_MINIPROJECT_ROOK
#define CS3520_MINIPROJECT_ROOK

#include <set>

#include "Piece.hpp"


class Rook : public Piece {
    public:
        Piece(bool ownedbyWhite) : belongsToWhite(ownedbyWhite) {}
        virtual char symbol() const {
            return (belongsToWhite?'R':'r');
        }
};

#endif
