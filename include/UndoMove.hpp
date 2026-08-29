#pragma once
#ifndef SAMUELENGINE_UNDOMOVE_STRUCT_HPP
#define SAMUELENGINE_UNDOMOVE_STRUCT_HPP
#include "Piece.hpp"
#include "PlayerState.hpp"
#include "Square.hpp"
#include "Move.hpp"

#include <optional>
#include <cstdint>

struct UndoMove {
    Move move;
    PiecePtr capturedPiece {};
    PiecePtr originalPiece{};
    Square rookFrom = Square(-1);
    Square rookTo = Square(-1);
    Square capturedSquare = Square(-1);
    PlayerState whiteState;
    PlayerState blackState;
    std::optional<Square> epTarget;
    int halfmove = 0;
    int fullmove = 0;
    uint64_t zobrist = 0;
};
#endif