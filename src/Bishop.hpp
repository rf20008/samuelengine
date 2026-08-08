#ifndef CS3520_MINIPROJECT_BISHOP
#define CS3520_MINIPROJECT_BISHOP

#include <set>

#include "Piece.hpp"


class Bishop : public Piece {
    public:
        Piece(bool ownedbyWhite) : belongsToWhite(ownedbyWhite) {}
        virtual char symbol() const {
            return (belongsToWhite?'B':'b');
        }
};

#endif
