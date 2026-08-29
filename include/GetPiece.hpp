#ifndef CS3520_MINI_PROJECT_GETPIECE
#define CS3520_MINI_PROJECT_GETPIECE
#include "Piece.hpp"
#include <memory>
PiecePtr getPiece(char c);
inline PiecePtr getPieceC(char c) { return getPiece(c); }
constexpr int pieceNum(char c) {
    char u = (c >= 'a' && c <= 'z') ? c - 32 : c;
    switch (u) {
        case 'K': return 5;
        case 'Q': return 4;
        case 'R': return 3;
        case 'B': return 2;
        case 'N': return 1;
        case 'P': return 0;
        default: 
            assert(false && "invalid piece char "); return 6;
    }
}
#endif