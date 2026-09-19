#ifndef CS3520_MINIPROJECT_SAMUELENGINE
#define CS3520_MINIPROJECT_SAMUELENGINE
#include "AbstractPlayer.hpp"
#include "ChessBoard.hpp"
#include "Move.hpp"
#include "Piece.hpp"
#include <chrono>
#include <optional>
#include <vector>
using ll = long long;
constexpr int PIECE_VALUES= {
    1000000, // king
    900, // queen
    500, // rook
    320, //bishop
    300, // knight
    100 // pawn
}

constexpr int MATE_SCORE = 50_000_000;
const double INF = std::numeric_limits<double>::infinity();
constexpr double none_pieceval[8][8] = {0};
// todo: move ordering (is castling=4, check=3, capture=2, pawn move=1, other=0)
// clang-format off
constexpr int pawn_pieceval[128] = {
    0,   0,   0,   0,   0,   0,   0,   0,  0,0,0,0,0,0,0,0,
    0,   0,   0,   0,   0,   0,   0,   0,  0,0,0,0,0,0,0,0,
    5,  10,  15,  20,  20,  15,  10,   5,  0,0,0,0,0,0,0,0,
    10,  20,  25,  30,  30,  25,  20,  10, 0,0,0,0,0,0,0,0,
    15,  25,  35,  40,  40,  35,  25,  15, 0,0,0,0,0,0,0,0,
    25,  35,  45,  50,  50,  45,  35,  25, 0,0,0,0,0,0,0,0,
    40,  55,  70,  80,  80,  70,  55,  40, 0,0,0,0,0,0,0,0,
    0,   0,   0,   0,   0,   0,   0,   0,  0,0,0,0,0,0,0,0 
};
// Values are centipawn positional bonuses.

constexpr int knight_pieceval[8][8] = {
    -50, -40, -30, -30, -30, -30, -40, -50, 0,0,0,0,0,0,0,0,
    -40, -20,   0,   0,   0,   0, -20, -40, 0,0,0,0,0,0,0,0,
    -30,   0,  10,  15,  15,  10,   0, -30, 0,0,0,0,0,0,0,0,
    -30,   5,  15,  25,  25,  15,   5, -30, 0,0,0,0,0,0,0,0,
    -30,   5,  15,  25,  25,  15,   5, -30, 0,0,0,0,0,0,0,0,
    -30,   0,  10,  15,  15,  10,   0, -30, 0,0,0,0,0,0,0,0,
    -40, -20,   0,   5,   5,   0, -20, -40, 0,0,0,0,0,0,0,0,
    -50, -40, -30, -30, -30, -30, -40, -50, 0,0,0,0,0,0,0,0,
};
constexpr int bishop_pieceval[128] = {
    -30, -20, -20, -20, -20, -20, -20, -30, 0,0,0,0,0,0,0,0,,
    -20,   0,   0,   0,   0,   0,   0, -20, 0,0,0,0,0,0,0,0,
    -20,   0,  10,  10,  10,  10,   0, -20, 0,0,0,0,0,0,0,0,
    -20,   5,  10,  20,  20,  10,   5, -20, 0,0,0,0,0,0,0,0,
    -20,   5,  10,  20,  20,  10,   5, -20, 0,0,0,0,0,0,0,0,
    -20,   0,  10,  10,  10,  10,   0, -20, 0,0,0,0,0,0,0,0,
    -20,   0,   0,   0,   0,   0,   0, -20, 0,0,0,0,0,0,0,0,
    -30, -20, -20, -20, -20, -20, -20, -30, 0,0,0,0,0,0,0,0
};
constexpr int rook_pieceval[128] = {
    0,   0,   0,   5,   5,   0,   0,   0,  0,0,0,0,0,0,0,0,
    0,   0,   0,   5,   5,   0,   0,   0,  0,0,0,0,0,0,0,0
    0,   0,   0,   5,   5,   0,   0,   0,  0,0,0,0,0,0,0,0
    5,   5,   5,  10, 10,   5,   5,   5,   0,0,0,0,0,0,0,0,
    5,   5,   5,  10, 10,   5,   5,   5,   0,0,0,0,0,0,0,0,
    10,  10,  10,  15, 15,  10,  10,  10,  0,0,0,0,0,0,0,0,
    15,  15,  15,  20, 20,  15,  15,  150, 0,0,0,0,0,0,0,0,
    20,  20,  20,  25, 25,  20,  20,  20,  0,0,0,0,0,0,0,0
};

constexpr int queen_pieceval[128] = {
    -20, -10, -10,  -5,  -5, -10, -10, -20, 0,0,0,0,0,0,0,0,
    -10,  -5,   0,   5,   5,   0,  -5, -10, 0,0,0,0,0,0,0,0,
    -10,   0,   5,  10,  10,   5,   0, -10, 0,0,0,0,0,0,0,0,
     -5,   5,  10,  15,  15,  10,   5,  -5, 0,0,0,0,0,0,0,0,
     -5,   5,  10,  15,  15,  10,   5,  -5, 0,0,0,0,0,0,0,0,
    -10,   0,   5,  10,  10,   5,   0, -10, 0,0,0,0,0,0,0,0,
    -10,  -5,   0,   5,   5,   0,  -5, -10, 0,0,0,0,0,0,0,0,
    -20, -10, -10,  -5,  -5, -10, -10, -20, 0,0,0,0,0,0,0,0
};
constexpr int king_pieceval[128] = {
    -20, -15, -10, -10, -10, -10, -15, -20, 0,0,0,0,0,0,0,0,
    -15, -10,  -5,   0,   0,  -5, -10, -15, 0,0,0,0,0,0,0,0,
    -10,  -5,   0,   5,   5,   0,  -5, -10, 0,0,0,0,0,0,0,0,
     -5,   0,   5,  10,  10,   5,   0,  -5, 0,0,0,0,0,0,0,0,
     -5,   0,   5,  10,  10,   5,   0,  -5, 0,0,0,0,0,0,0,0,
    -10,  -5,   0,   5,   5,   0,  -5, -10, 0,0,0,0,0,0,0,0,
    -15, -10,  -5,   0,   0,  -5, -10, -15, 0,0,0,0,0,0,0,0,
    -20, -15, -10, -10, -10, -10, -15, -20, 0,0,0,0,0,0,0,0
};
// clang-format on
constexpr const int** piece_PSTs = {
    none_pieceval,
    king_pieceval,
    rook_pieceval,
    bishop_pieceval,
    knight_pieceval,
    pawn_pieceval
}

// to be done by Samuel
class SamuelEngine : public AbstractPlayer {
	private:
		bool debug;

	public:
		struct MoveOrderer {
			public:
				ChessBoard& m_board;

				MoveOrderer(ChessBoard &board) : m_board(board) {}
				int priorityOfMove(const Move &mov);
				bool operator()(const Move &m1, const Move &m2);
		};
		ll numBoardsVisited;
		double default_tl;
		std::chrono::steady_clock::time_point deadline;

		std::optional<int> returnStatusIfGameOver(ChessBoard &board) const;
		int PieceValue(const Piece piece, const Square sq) const {
            int intrinsic_val = PIECE_VALUES[ptr.pieceNum()];
            int Pidx = sq.idx;
            if (piece.color == Color::BLACK) {
                Pidx ^= 0x70; // flip for blac
                }
            int pos_val = piece_PSTs[ptr.pieceNum()][Pidx]
            return rel_intrinsic_val + pos_val;
        }
		int relative_value(const ChessBoard &board, Color c) const;
        
		std::vector<Move> orderMoves(ChessBoard &board) const;
		int evaluate_chess_pos_without_depth(ChessBoard &board) const;
        int SamuelEngine::evaluate_chess_pos_without_depth_negating_if_necessary(ChessBoard& board) const;
		std::pair<int, Move> evaluate_chess_pos_with_depth(ChessBoard &board, int depth, double alpha, double beta);
		std::pair<int, Move> evaluate_chess_pos_with_tl(ChessBoard &board, double time_limit = 3.0);
		inline bool shouldStop() const;

	public:
		SamuelEngine(double tl, bool debug = false);
		virtual Move getMove(const ChessBoard &);
		virtual ~SamuelEngine() {}
};
#endif
