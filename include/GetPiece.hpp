#ifndef CS3520_MINI_PROJECT_GETPIECE
#define CS3520_MINI_PROJECT_GETPIECE
#include "Piece.hpp"
#include <memory>
PiecePtr getPiece(char c);
inline PiecePtr getPieceC(char c) {return getPiece(c);}
#endif