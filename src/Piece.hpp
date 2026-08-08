#ifndef CS3520_MINIPROJECT_PIECE
#define CS3520_MINIPROJECT_PIECE

#include <set>

#include "Square.hpp"
#include "Move.hpp"


class Piece {
    protected:
        bool belongsToWhite;
    public:
        Piece(bool ownedbyWhite) : belongsToWhite(ownedbyWhite) {}
        bool belongsToWhite() const final {return belongsToWhite;}
        virtual char symbol() const = 0;
};

#endif
