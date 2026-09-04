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

		const double (*getPosVal(const Piece piece) const)[8];
		double relative_value(const Piece piece) const;
		std::optional<double> returnStatusIfGameOver(ChessBoard &board) const;
		double PieceValue(const Piece piece, const Square sq) const;
		double relative_value(const ChessBoard &board, Color c) const;
		std::vector<Move> orderMoves(ChessBoard &board) const;
		double evaluate_chess_pos_without_depth(ChessBoard &board) const;
		std::pair<double, Move> evaluate_chess_pos_with_depth(ChessBoard &board, int depth, double alpha, double beta);
		std::pair<double, Move> evaluate_chess_pos_with_tl(ChessBoard &board, double time_limit = 3.0);
		inline bool shouldStop() const;

	public:
		SamuelEngine(double tl, bool debug = false);
		virtual Move getMove(const ChessBoard &);
		virtual ~SamuelEngine() {}
};
#endif
