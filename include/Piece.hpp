#ifndef CS3520_MINIPROJECT_PIECE
#define CS3520_MINIPROJECT_PIECE

#include <set>
#include <memory>



class Piece {
    protected:
        bool belongsToWhite;
    public:
        Piece(bool ownedbyWhite) : belongsToWhite(ownedbyWhite) {}
        bool getBelongsToWhite() const {return belongsToWhite;}
        virtual char symbol() const = 0;
        bool operator==(const Piece& other) const {return this->symbol() == other.symbol();}
};
using PiecePtr = std::shared_ptr<Piece>;
#endif
