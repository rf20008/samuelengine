#ifndef CS3520_MINIPROJECT_SAMUELENGINE
#define CS3520_MINIPROJECT_SAMUELENGINE
#include "Piece.hpp"
#include "Move.hpp"
#include "AbstractPlayer.hpp"
#include "ChessBoard.hpp"
#include <vector>
#include <optional>
#include <chrono>
using ll = long long;

// to be done by Samuel
class SamuelEngine : public AbstractPlayer {
private:
    bool debug;
protected:
    ll numBoardsVisited;
    double default_tl;
    std::chrono::steady_clock::time_point deadline;

    std::vector<std::vector<double>> getPosVal(const PiecePtr ptr) const;
    double relative_value(const PiecePtr ptr) const;
    std::optional<double> returnStatusIfGameOver(const ChessBoard& board) const;
    double PieceValue(const PiecePtr ptr, const Square sq) const;
    double relative_value(const ChessBoard& board, const bool isWhite) const;
    double evaluate_chess_pos_without_depth(const ChessBoard& board) const;
    std::pair<double, Move> evaluate_chess_pos_with_depth(const ChessBoard& board, int depth, double alpha, double beta, bool maximizingPlayer) const;
    std::pair<double, Move> evaluate_chess_pos_with_tl(const ChessBoard& board, double time_limit = 3.0);
    inline bool shouldStop() const;
public:
    SamuelEngine(double tl, bool debug=false);
    virtual Move getMove(const ChessBoard&);
};
#endif