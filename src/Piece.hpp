#ifndef CS3520_MINIPROJECT_PIECE
#define CS3520_MINIPROJECT_PIECE

#include <set>


class Piece {
    protected:
        bool belongsToWhite;
    public:
        Piece(bool ownedbyWhite) : belongsToWhite(ownedbyWhite) {}
        bool getBelongsToWhite() const {return belongsToWhite;}
        virtual char symbol() const = 0;
};

#endif
