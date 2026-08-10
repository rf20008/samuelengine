#ifndef CS3520_MINIPROJECT_SAMUELENGINE
#define CS3520_MINIPROJECT_SAMUELENGINE
#include "Piece.hpp"
#include "Move.hpp"
#include "AbstractPlayer.hpp"
#include "ChessBoard.hpp"
#include <vector>
#include <optional>
using ll = long long;
// to be done by Samuel
class SamuelEngine : public AbstractPlayer {
protected:
    ll numBoardsVisited;

    std::vector<std::vector<double>> getPosVal(const PiecePtr ptr);
    double relative_value(const PiecePtr ptr);
    std::optional<double> returnStatusIfGameOver(const ChessBoard& board);
    double PieceValue(const PiecePtr ptr, const Square sq);
    double relative_value(const ChessBoard& board, const bool isWhite);
    double evaluate_chess_pos_without_depth(const ChessBoard& board);
    double evaluate_chess_pos_with_depth(const ChessBoard& board, int depth, double alpha, double beta, bool maximizingPlayer);
    double evaluate_chess_pos_with_tl(const ChessBoard& board, double time_limit = 3.0);
public:
    SamuelEngine();
    virtual Move getMove(const ChessBoard&);
};
#endif