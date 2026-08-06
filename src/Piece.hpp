#ifndef CS3520_MINIPROJECT_PIECE
#define CS3520_MINIPROJECT_PIECE

#include <set>

#include "Square.hpp"
#include "Move.hpp"


class Piece {
    protected:
        Square curSquare;
        bool belongsToWhite;
    public:
        Piece(const Square& square, bool ownedbyWhite) : curSquare(square), belongsToWhite(ownedbyWhite) {}
        bool belongsToWhite() const final {return belongsToWhite;}
        virtual char symbol() const = 0;
        Square getSquare() const {return this->curSquare;}
        virtual std::set<Square> possibleMoves() const = 0;
        void setSquare(Square newSquare) {
            curSquare=newSquare;
        }
}
#endif
