#ifndef CS3520_MINIPROJECT_QUEEN
#define CS3520_MINIPROJECT_QUEEN

#include <set>

#include "Piece.hpp"


class Queen : public Piece {
    public:
        Piece(bool ownedbyWhite) : belongsToWhite(ownedbyWhite) {}
        virtual char symbol() const {
            return (belongsToWhite?'Q':'q');
        }
};

#endif
