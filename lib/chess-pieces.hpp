#ifndef CHESS_PIECES_HPP
#define CHESS_PIECES_HPP

#include <string>

enum class PieceType {
  WPAWN,
  WKNIGHT,
  WBISHOP,
  WROOK,
  WQUEEN,
  WKING,
  // black pieces
  BPAWN,
  BKNIGHT,
  BBISHOP,
  BROOK,
  BQUEEN,
  BKING
};

inline std::string display_piece(PieceType pt) {
  switch (pt) {
  case PieceType::BPAWN:
    return "♟";
  case PieceType::BKNIGHT:
    return "♞";
  case PieceType::BBISHOP:
    return "♝";
  case PieceType::BROOK:
    return "♜";
  case PieceType::BQUEEN:
    return "♛";
  case PieceType::BKING:
    return "♚";
  case PieceType::WPAWN:
    return "♙";
  case PieceType::WKNIGHT:
    return "♘";
  case PieceType::WBISHOP:
    return "♗";
  case PieceType::WROOK:
    return "♖";
  case PieceType::WQUEEN:
    return "♕";
  case PieceType::WKING:
    return "♔";
  default:
    return "?";
  }
};

#endif
