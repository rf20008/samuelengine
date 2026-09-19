#ifndef CS3520_MINIPROJECT_SAMUELENGINE
#define CS3520_MINIPROJECT_SAMUELENGINE
#include "AbstractPlayer.hpp"
#include "ChessBoard.hpp"
#include "Move.hpp"
#include "Piece.hpp"
#include "TranspositionTable.hpp"
#include <chrono>
#include <optional>
#include <vector>
using ll = long long;
inline constexpr int PIECE_VALUES[6]= {
    1000000, // king
    900, // queen
    500, // rook
    320, //bishop
    300, // knight
    100 // pawn
};

inline constexpr int MATE_SCORE = 50'000'000;

// todo: move ordering (is castling=4, check=3, capture=2, pawn move=1, other=0)
// clang-format off
inline constexpr int pawn_pieceval[128] = {
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

inline constexpr int knight_pieceval[128] = {
    -50, -40, -30, -30, -30, -30, -40, -50, 0,0,0,0,0,0,0,0,
    -40, -20,   0,   0,   0,   0, -20, -40, 0,0,0,0,0,0,0,0,
    -30,   0,  10,  15,  15,  10,   0, -30, 0,0,0,0,0,0,0,0,
    -30,   5,  15,  25,  25,  15,   5, -30, 0,0,0,0,0,0,0,0,
    -30,   5,  15,  25,  25,  15,   5, -30, 0,0,0,0,0,0,0,0,
    -30,   0,  10,  15,  15,  10,   0, -30, 0,0,0,0,0,0,0,0,
    -40, -20,   0,   5,   5,   0, -20, -40, 0,0,0,0,0,0,0,0,
    -50, -40, -30, -30, -30, -30, -40, -50, 0,0,0,0,0,0,0,0,
};
inline constexpr int bishop_pieceval[128] = {
    -30, -20, -20, -20, -20, -20, -20, -30, 0,0,0,0,0,0,0,0,
    -20,   0,   0,   0,   0,   0,   0, -20, 0,0,0,0,0,0,0,0,
    -20,   0,  10,  10,  10,  10,   0, -20, 0,0,0,0,0,0,0,0,
    -20,   5,  10,  20,  20,  10,   5, -20, 0,0,0,0,0,0,0,0,
    -20,   5,  10,  20,  20,  10,   5, -20, 0,0,0,0,0,0,0,0,
    -20,   0,  10,  10,  10,  10,   0, -20, 0,0,0,0,0,0,0,0,
    -20,   0,   0,   0,   0,   0,   0, -20, 0,0,0,0,0,0,0,0,
    -30, -20, -20, -20, -20, -20, -20, -30, 0,0,0,0,0,0,0,0
};
inline constexpr int rook_pieceval[128] = {
    0,   0,   0,   5,   5,   0,   0,   0,  0,0,0,0,0,0,0,0,
    0,   0,   0,   5,   5,   0,   0,   0,  0,0,0,0,0,0,0,0,
    0,   0,   0,   5,   5,   0,   0,   0,  0,0,0,0,0,0,0,0,
    5,   5,   5,  10, 10,   5,   5,   5,   0,0,0,0,0,0,0,0,
    5,   5,   5,  10, 10,   5,   5,   5,   0,0,0,0,0,0,0,0,
    10,  10,  10,  15, 15,  10,  10,  10,  0,0,0,0,0,0,0,0,
    15,  15,  15,  20, 20,  15,  15,  150, 0,0,0,0,0,0,0,0,
    20,  20,  20,  25, 25,  20,  20,  20,  0,0,0,0,0,0,0,0
};

inline constexpr int queen_pieceval[128] = {
    -20, -10, -10,  -5,  -5, -10, -10, -20, 0,0,0,0,0,0,0,0,
    -10,  -5,   0,   5,   5,   0,  -5, -10, 0,0,0,0,0,0,0,0,
    -10,   0,   5,  10,  10,   5,   0, -10, 0,0,0,0,0,0,0,0,
     -5,   5,  10,  15,  15,  10,   5,  -5, 0,0,0,0,0,0,0,0,
     -5,   5,  10,  15,  15,  10,   5,  -5, 0,0,0,0,0,0,0,0,
    -10,   0,   5,  10,  10,   5,   0, -10, 0,0,0,0,0,0,0,0,
    -10,  -5,   0,   5,   5,   0,  -5, -10, 0,0,0,0,0,0,0,0,
    -20, -10, -10,  -5,  -5, -10, -10, -20, 0,0,0,0,0,0,0,0
};
inline constexpr int king_pieceval[128] = {
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
inline constexpr int const * const piece_PSTs[6] = {
    king_pieceval,
    queen_pieceval,
    rook_pieceval,
    bishop_pieceval,
    knight_pieceval,
    pawn_pieceval
};

struct ScoredMove {
    Move move;
    int score;
    bool operator<(const ScoredMove& other) const {
        return score > other.score;
    }
};
// to be done by Samuel
class SamuelEngine : public AbstractPlayer {
	private:
		bool debug;

	public:
		struct MoveOrderer {
			public:
				ChessBoard& m_board;
                const Move m_hashMove;

				MoveOrderer(ChessBoard &board, const Move& hashMove) : m_board(board), m_hashMove(hashMove) {}
				int score_move(const Move &mov);
				std::vector<Move> orderMoves();
		};
        TranspositionTable transpositionTable;
		ll numBoardsVisited;
		double default_tl;
		std::chrono::steady_clock::time_point deadline;

		std::optional<int> returnStatusIfGameOver(ChessBoard &board) const;
		int PieceValue(const Piece piece, const Square sq) const {
            int intrinsic_val = PIECE_VALUES[piece.pieceNum()];
            int Pidx = sq.idx;
            if (piece.color == Color::BLACK) {
                Pidx ^= 0x70; // flip for blac
                }
            int pos_val = piece_PSTs[piece.pieceNum()][Pidx];
            return intrinsic_val + pos_val;
        }
		int relative_value(const ChessBoard &board, Color c) const;
        
		int evaluate_chess_pos_without_depth(ChessBoard &board) const;
        int evaluate_chess_pos_without_depth_negating_if_necessary(ChessBoard& board) const {
            int score = evaluate_chess_pos_without_depth(board);
            return (board.get_whiteToMove() ? score : -score);
        }
		std::pair<int, Move> evaluate_chess_pos_with_depth(ChessBoard &board, int depth, int alpha, int beta);
		std::pair<int, Move> evaluate_chess_pos_with_tl(ChessBoard &board, double time_limit = 3.0);
		inline bool shouldStop() const;

	public:
		SamuelEngine(double tl, bool debug = false, size_t transposition_table_size = (2ULL<<20));
		virtual Move getMove(const ChessBoard &);
		virtual ~SamuelEngine() {}
};
#endif
