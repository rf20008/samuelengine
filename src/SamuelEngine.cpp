// This is a copy of the engine I made to play chess in Python. I am translating it into C++
#include "SamuelEngine.hpp"
#include "Errors.hpp"

#include <limits>
#include <vector>

using namespace std;
const double INFINITY = std::numeric_limits<double>::infinity();
const double NEG_INF = -INFINITY;
const std::vector<std::vector<double>> PAWN_PIECEVAL = {
    {7.0, 8.0, 9.0, 10.0, 10.0, 9.0, 8.0, 7.0},  // 8th rank
    {3.0, 4.0, 5.0, 6.0, 6.0, 5.0, 4.0, 3.0},       // 7th rank
    {2.0, 2.5, 3.0, 3.5, 3.5, 3.0, 2.5, 2.0},       // 6th rank
    {0.7, 1.0, 1.3, 1.5, 1.5, 1.3, 1.0, 0.7},       // 5th rank
    {0.3, 0.7, 0.8, 0.9, 0.9, 0.8, 0.7, 0.3},       // 4th rank
    {0.15, 0.4, 0.5, 0.6, 0.6, 0.5, 0.4, 0.15},     // 3rd rank
    {-0.25, -0.2, -0.05, 0.0, 0.0, -0.05, -0.2, -0.25},  // 2nd rank
    {-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0}        // 1st rank
};

const std::vector<std::vector<double>> rook_pieceval = {
    {5.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 5.0},  // 8th rank
    {4.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 4.0},  // 7th rank
    {3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0},  // 6th rank
    {2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0},  // 5th rank
    {1.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, 1.0},  // 4th rank
    {1.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, 1.0},  // 3rd rank
    {0.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 0.0},  // 2nd rank
    {0.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 0.0}   // 1st rank
};
const std::vector<std::vector<double>> king_pieceval = {
    {-2.0, -1.5, -1.0, -1.0, -1.0, -1.0, -1.5, -2.0},  // 8th rank
    {-1.5, -1.0, -0.5,  0.0,  0.0, -0.5, -1.0, -1.5},  // 7th rank
    {-1.0, -0.5,  0.0,  0.5,  0.5,  0.0, -0.5, -1.0},  // 6th rank
    {-0.5,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -0.5},  // 5th rank
    {-0.5,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -0.5},  // 4th rank
    {-1.0, -0.5,  0.0,  0.5,  0.5,  0.0, -0.5, -1.0},  // 3rd rank
    {-1.5, -1.0, -0.5,  0.0,  0.0, -0.5, -1.0, -1.5},  // 2nd rank
    {-2.0, -1.5, -1.0, -1.0, -1.0, -1.0, -1.5, -2.0}   // 1st rank
};
const std::vector<std::vector<double>> bishop_pieceval = {
    {-4.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -4.0},  // 8th rank
    {-3.0, -2.0, -1.5, -1.0, -1.0, -1.5, -2.0, -3.0},  // 7th rank
    {-2.0, -1.0,  0.0,  0.5,  0.5,  0.0, -1.0, -2.0},  // 6th rank
    {-1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0},  // 5th rank
    {-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},  // 4th rank
    {-2.0, -0.5,  0.0,  0.5,  0.5,  0.0, -0.5, -2.0},  // 3rd rank
    {-3.0, -1.5, -1.0, -1.0, -1.0, -1.0, -1.5, -3.0},  // 2nd rank
    {-4.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -4.0}   // 1st rank
};
const std::vector<std::vector<double>> knight_pieceval = {
    {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},  // 8th rank
    {-4.0, -3.0, -2.0, -1.0, -1.0, -2.0, -3.0, -4.0},  // 7th rank
    {-3.0, -2.0, -1.0,  0.0,  0.0, -1.0, -2.0, -3.0},  // 6th rank
    {-3.0, -1.0,  0.0,  1.0,  1.0,  0.0, -1.0, -3.0},  // 5th rank
    {-3.0, -1.0,  0.0,  1.0,  1.0,  0.0, -1.0, -3.0},  // 4th rank
    {-3.0, -2.0, -1.0,  0.0,  0.0, -1.0, -2.0, -3.0},  // 3rd rank
    {-4.0, -3.0, -2.0, -1.0, -1.0, -2.0, -3.0, -4.0},  // 2nd rank
    {-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}   // 1st rank
};
const std::vector<std::vector<double>> queen_pieceval = {
    {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},  // 8th rank
    {-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},  // 7th rank
    {-1.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -1.0},  // 6th rank
    {-0.5,  1.0,  1.5,  2.0,  2.0,  1.5,  1.0, -0.5},  // 5th rank
    {-0.5,  1.0,  1.5,  2.0,  2.0,  1.5,  1.0, -0.5},  // 4th rank
    {-1.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -1.0},  // 3rd rank
    {-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},  // 2nd rank
    {-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}   // 1st rank
};
std::vector<std::vector<double>> getPosVal(const PiecePtr ptr) {
    switch (toupper(ptr->symbol())) {
        case 'K': return king_pieceval;
        case 'Q': return queen_pieceval;
        case 'R': return ROOK_PIECEVAL;
        case 'B': return bishop_pieceval;
        case 'N': return knight_pieceval;
        case 'P': return rook_pieceval;
        default: throw UnknownPiece("Unknown piece: " + std::string{c});
    }
}
double relative_value(const PiecePtr ptr) {
    switch (toupper(ptr->symbol())) {
        case 'K': return 1000000;
        case 'Q': return 9;
        case 'R': return 5;
        case 'B': return 3;
        case 'N': return 3;
        case 'P': return 1;
        default: throw UnknownPiece("Unknown piece: " + std::string{c});;
    }
}


std::optional<double> returnStatusIfGameOver(const ChessBoard& board) {
    GameStatus status = board.getStatus();
    if (isGameOver(status)) {
        switch (status) {
            case GameStatus::WHITE_WON: return std::optional<double>(INFINITY);
            case GameStatus::BLACK_WON: return std::optional<double>(-INFINITY);
            default: return std::optional<double>(0);
        }
    }
    return std::optional<double>();
}

double PieceValue(const PiecePtr ptr, const Square sq) {
    double rel_intrinsic_val = relative_value(ptr);
    auto posValTable = getPosVal(ptr);
    double pos_val = posValTable.at(sq.row-1).at(sq.col-1);
    return rel_intrinsic_val + pos_val;
}

double relative_value(const ChessBoard& board, const bool isWhite) {
    double tot_val = 0;
    for (int rank = 0; rank<BOARD_SIZE; ++rank) {
        for (int file = 0; file<BOARD_SIZE; ++file) {
            Square sq{rank, file};
            PiecePtr piece = board.getPiece(board);
            if (!piece) continue;
            if (piece->getBelongsToWhite() != isWhite) continue;
            tot_val += PieceValue(piece, sq);
        }
    }
    return tot_val;
}
double evaluate_chess_pos_without_depth(const ChessBoard& board) {
    std::optional<double> gameOverMaybe = returnStatusIfGameOver(board);
    if (!gameOverMaybe) return *gameOverMaybe;

    return relative_value(board, true) - relative_value(board, false);
}
SamuelEngine::SamuelEngine() {
    throw NotImplementedError("SamuelEngine::SamuelEngine() is not implemented yet");
}
Move SamuelEngine::getMove(const ChessBoard&) {
    throw NotImplementedError("virtual Move SamuelEngine::getMove(const ChessBoard&) is not implemented yet");
}