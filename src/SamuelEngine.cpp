// This is a copy of the engine I made to play chess in Python. I am translating it into C++
#include "SamuelEngine.hpp"
#include "Errors.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <optional>
#include <vector>

using namespace std;
const double INF = std::numeric_limits<double>::infinity();

// todo: move ordering (is castling=4, check=3, capture=2, pawn move=1, other=0)
constexpr double pawn_pieceval[8][8] = {
	{7.0, 8.0, 9.0, 10.0, 10.0, 9.0, 8.0, 7.0},			// 8th rank
	{3.0, 4.0, 5.0, 6.0, 6.0, 5.0, 4.0, 3.0},			// 7th rank
	{2.0, 2.5, 3.0, 3.5, 3.5, 3.0, 2.5, 2.0},			// 6th rank
	{0.7, 1.0, 1.3, 1.5, 1.5, 1.3, 1.0, 0.7},			// 5th rank
	{0.3, 0.7, 0.8, 0.9, 0.9, 0.8, 0.7, 0.3},			// 4th rank
	{0.15, 0.4, 0.5, 0.6, 0.6, 0.5, 0.4, 0.15},			// 3rd rank
	{-0.25, -0.2, -0.05, 0.0, 0.0, -0.05, -0.2, -0.25}, // 2nd rank
	{-1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0}	// 1st rank
};

constexpr double rook_pieceval[8][8] = {
	{5.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 5.0},		// 8th rank
	{4.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 4.0},		// 7th rank
	{3.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0},		// 6th rank
	{2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0},		// 5th rank
	{1.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, 1.0}, // 4th rank
	{1.0, -0.0, -0.0, -0.0, -0.0, -0.0, -0.0, 1.0}, // 3rd rank
	{0.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 0.0}, // 2nd rank
	{0.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 0.0}	// 1st rank
};
constexpr double king_pieceval[8][8] = {
	{-2.0, -1.5, -1.0, -1.0, -1.0, -1.0, -1.5, -2.0}, // 8th rank
	{-1.5, -1.0, -0.5, 0.0, 0.0, -0.5, -1.0, -1.5},	  // 7th rank
	{-1.0, -0.5, 0.0, 0.5, 0.5, 0.0, -0.5, -1.0},	  // 6th rank
	{-0.5, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -0.5},		  // 5th rank
	{-0.5, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -0.5},		  // 4th rank
	{-1.0, -0.5, 0.0, 0.5, 0.5, 0.0, -0.5, -1.0},	  // 3rd rank
	{-1.5, -1.0, -0.5, 0.0, 0.0, -0.5, -1.0, -1.5},	  // 2nd rank
	{-2.0, -1.5, -1.0, -1.0, -1.0, -1.0, -1.5, -2.0}  // 1st rank
};
constexpr double bishop_pieceval[8][8] = {
	{-4.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -4.0}, // 8th rank
	{-3.0, -2.0, -1.5, -1.0, -1.0, -1.5, -2.0, -3.0}, // 7th rank
	{-2.0, -1.0, 0.0, 0.5, 0.5, 0.0, -1.0, -2.0},	  // 6th rank
	{-1.0, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, -1.0},		  // 5th rank
	{-1.0, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -1.0},		  // 4th rank
	{-2.0, -0.5, 0.0, 0.5, 0.5, 0.0, -0.5, -2.0},	  // 3rd rank
	{-3.0, -1.5, -1.0, -1.0, -1.0, -1.0, -1.5, -3.0}, // 2nd rank
	{-4.0, -3.0, -3.0, -3.0, -3.0, -3.0, -3.0, -4.0}  // 1st rank
};
constexpr double knight_pieceval[8][8] = {
	{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}, // 8th rank
	{-4.0, -3.0, -2.0, -1.0, -1.0, -2.0, -3.0, -4.0}, // 7th rank
	{-3.0, -2.0, -1.0, 0.0, 0.0, -1.0, -2.0, -3.0},	  // 6th rank
	{-3.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, -3.0},	  // 5th rank
	{-3.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, -3.0},	  // 4th rank
	{-3.0, -2.0, -1.0, 0.0, 0.0, -1.0, -2.0, -3.0},	  // 3rd rank
	{-4.0, -3.0, -2.0, -1.0, -1.0, -2.0, -3.0, -4.0}, // 2nd rank
	{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}  // 1st rank
};
constexpr double queen_pieceval[8][8] = {
	{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}, // 8th rank
	{-1.0, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -1.0},		  // 7th rank
	{-1.0, 0.5, 1.0, 1.5, 1.5, 1.0, 0.5, -1.0},		  // 6th rank
	{-0.5, 1.0, 1.5, 2.0, 2.0, 1.5, 1.0, -0.5},		  // 5th rank
	{-0.5, 1.0, 1.5, 2.0, 2.0, 1.5, 1.0, -0.5},		  // 4th rank
	{-1.0, 0.5, 1.0, 1.5, 1.5, 1.0, 0.5, -1.0},		  // 3rd rank
	{-1.0, 0.0, 0.5, 1.0, 1.0, 0.5, 0.0, -1.0},		  // 2nd rank
	{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}  // 1st rank
};

int SamuelEngine::MoveOrderer::priorityOfMove(const Move &mov) {
	//if (m_board.move_ends_game(mov)) {return 1000;}
	if (m_board.move_is_castling(mov)) {
		return 4;
	} else if (m_board.move_is_check(mov)) {
		return 3;
	} else if (m_board.move_is_capture(mov)) {
		return 2;
	} else if (m_board.move_is_zeroing(mov)) {
		return 1;
	} else {
		return 0;
	}
}
bool SamuelEngine::MoveOrderer::operator()(const Move &m1, const Move &m2) { return priorityOfMove(m1) < priorityOfMove(m2); }

const double (*SamuelEngine::getPosVal(const PiecePtr ptr) const)[8] {
	switch (toupper(ptr->symbol())) {
	case 'K':
		return king_pieceval;
	case 'Q':
		return queen_pieceval;
	case 'R':
		return rook_pieceval;
	case 'B':
		return bishop_pieceval;
	case 'N':
		return knight_pieceval;
	case 'P':
		return pawn_pieceval;
	default:
		throw UnknownPiece("Unknown piece: " + std::string{ptr->symbol(), 1});
	}
}
double SamuelEngine::relative_value(const PiecePtr ptr) const {
	switch (toupper(ptr->symbol())) {
	case 'K':
		return 1000000;
	case 'Q':
		return 9;
	case 'R':
		return 5;
	case 'B':
		return 3;
	case 'N':
		return 3;
	case 'P':
		return 1;
	default:
		throw UnknownPiece("Unknown piece: " + std::string{ptr->symbol(), 1});
	}
}

std::optional<double> SamuelEngine::returnStatusIfGameOver(ChessBoard &board) const {
	GameStatus status = board.getStatus();
	if (isGameOver(status)) {
		switch (status) {
		case GameStatus::WHITE_WON:
			return std::optional<double>(INF);
		case GameStatus::BLACK_WON:
			return std::optional<double>(-INF);
		default:
			return std::optional<double>(0);
		}
	}
	return std::optional<double>();
}

double SamuelEngine::PieceValue(const PiecePtr ptr, const Square sq) const {
	double rel_intrinsic_val = relative_value(ptr);
	auto posValTable = getPosVal(ptr);
	double pos_val = posValTable[sq.rank()][sq.file()];
	return rel_intrinsic_val + pos_val;
}

double SamuelEngine::relative_value(const ChessBoard &board, const bool isWhite) const {
	double tot_val = 0;
	for (int rank = 0; rank < BOARD_SIZE; ++rank) {
		for (int file = 0; file < BOARD_SIZE; ++file) {
			Square sq{rank, file};
			PiecePtr piece = board.getPiece(sq);
			if (!piece)
				continue;
			if (piece->getBelongsToWhite() != isWhite)
				continue;
			tot_val += PieceValue(piece, sq);
		}
	}
	return tot_val;
}
double SamuelEngine::evaluate_chess_pos_without_depth(ChessBoard &board) const {
	std::optional<double> gameOverMaybe = returnStatusIfGameOver(board);
	if (gameOverMaybe)
		return *gameOverMaybe;

	return relative_value(board, true) - relative_value(board, false);
}

std::vector<Move> SamuelEngine::orderMoves(ChessBoard &board) const {
	std::vector<Move> movesVec = board.allLegalMoves();
	std::sort(begin(movesVec), end(movesVec), SamuelEngine::MoveOrderer(board));
	return movesVec;
}

std::pair<double, Move> SamuelEngine::evaluate_chess_pos_with_depth(ChessBoard &board, int depth, double alpha, double beta) {
	if ((numBoardsVisited & 127) == 0 && shouldStop()) {
		throw OutOfTime();
	}
	numBoardsVisited++;
	std::optional<double> gameOverMaybe = returnStatusIfGameOver(board);
	if (gameOverMaybe) {
		return {*gameOverMaybe, Move(Square("a1"), Square("a2"))};
	}
	if (depth == 0) {
		return {evaluate_chess_pos_without_depth(board), Move(Square("a1"), Square("a2"))};
	}
	std::vector<Move> moves = orderMoves(board);
	if (moves.empty()) {
		return {evaluate_chess_pos_without_depth(board), Move(Square("a1"), Square("a2"))};
	}

	Move bestMove = *moves.begin();
	if (board.get_whiteToMove()) {
		double value = -INF;

		for (Move move : moves) {
			ChessBoard newBoard = board;
			newBoard.processMove(move);
			auto [new_val, new_move] = evaluate_chess_pos_with_depth(newBoard, depth - 1, alpha, beta);
			if (new_val > value) {
				bestMove = move;
				value = new_val;
			}
			alpha = max(alpha, new_val);
			if (new_val >= beta) {
				break; // beta cutoff
			}
		}
		return {value, bestMove};
	} else {
		double value = INF;
		for (Move move : moves) {
			ChessBoard newBoard = board;
			newBoard.processMove(move);
			auto [new_val, new_move] = evaluate_chess_pos_with_depth(newBoard, depth - 1, alpha, beta);
			beta = min(beta, new_val);
			if (new_val < value) {
				value = new_val;
				bestMove = move;
			}
			if (new_val <= alpha) {
				break; //alpha cutoff
			}
		}
		return {value, bestMove};
	}
}
std::pair<double, Move> SamuelEngine::evaluate_chess_pos_with_tl(ChessBoard &board, double time_limit) {
	this->deadline = std::chrono::steady_clock::now() + std::chrono::nanoseconds(static_cast<long int>(time_limit * 1'000'000'000));
	double bestValue = 0;
	Move bestMove = *(board.allLegalMoves().begin());
	try {
		for (int depth = 1; !shouldStop(); ++depth) {
			auto [newValue, newMove] = evaluate_chess_pos_with_depth(board, depth, -INF, +INF);
			bestValue = newValue;
			bestMove = newMove;
		}
	} catch (OutOfTime &err) {
	}
	return {bestValue, bestMove};
}
inline bool SamuelEngine::shouldStop() const { return std::chrono::steady_clock::now() >= deadline; }
SamuelEngine::SamuelEngine(double tl, bool dbg) : debug(dbg), numBoardsVisited(0), default_tl(tl), deadline(std::chrono::steady_clock::now()) {}
Move SamuelEngine::getMove(const ChessBoard &board) {
    ChessBoard scratchBoard = board;
	this->numBoardsVisited = 0;
	if (debug) {
		cerr << "Beginning search" << endl;
	}
	auto [val, move] = evaluate_chess_pos_with_tl(scratchBoard, default_tl);
	if (debug) {
		cerr << "Finished search\n. Searched " << numBoardsVisited << " positions to find a value of " << val << endl;
	}
	return move;
}