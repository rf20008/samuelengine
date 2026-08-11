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
    struct MoveOrderer {
        public:
            const ChessBoard m_board;

            MoveOrderer(const ChessBoard& board) : m_board(board) {}
            int priorityOfMove(const Move& mov) const;
            bool operator()(const Move& m1, const Move& m2) const;
    };
    ll numBoardsVisited;
    double default_tl;
    std::chrono::steady_clock::time_point deadline;

    const std::vector<std::vector<double>>& getPosVal(const PiecePtr ptr) const;
    double relative_value(const PiecePtr ptr) const;
    std::optional<double> returnStatusIfGameOver(const ChessBoard& board) const;
    double PieceValue(const PiecePtr ptr, const Square sq) const;
    double relative_value(const ChessBoard& board, const bool isWhite) const;
    std::vector<Move> orderMoves(const ChessBoard& board) const;
    double evaluate_chess_pos_without_depth(const ChessBoard& board) const;
    std::pair<double, Move> evaluate_chess_pos_with_depth(const ChessBoard& board, int depth, double alpha, double beta);
    std::pair<double, Move> evaluate_chess_pos_with_tl(const ChessBoard& board, double time_limit = 3.0);
    inline bool shouldStop() const;
public:
    SamuelEngine(double tl, bool debug=false);
    virtual Move getMove(const ChessBoard&);
};
#endif