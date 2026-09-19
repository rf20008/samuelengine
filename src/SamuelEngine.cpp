// This is a copy of the engine I made to play chess in Python. I am translating it into C++
#include "SamuelEngine.hpp"
#include "Errors.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <optional>
#include <vector>

using namespace std;

constexpr Move NULL_MOVE = Move(Square(0x88), Square(0x88));

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


std::optional<int> SamuelEngine::returnStatusIfGameOver(const ChessBoard& board) const {
    GameStatus status = board.getStatus();

    if (!isGameOver(status))
        return std::nullopt;

    switch (status) {
    case GameStatus::WHITE_WON:
        return MATE_SCORE - board.get_ply();

    case GameStatus::BLACK_WON:
        return -MATE_SCORE + board.get_ply();

    default: // draw
        return 0;
    }
}



int SamuelEngine::relative_value(const ChessBoard &board, const Color expectedColor) const {
	int tot_val = 0;
	for (int rank = 0; rank < BOARD_SIZE; ++rank) {
		for (int file = 0; file < BOARD_SIZE; ++file) {
			Square sq{rank, file};
			Piece piece = board.getPiece(sq);
			if (piece.color != expectedColor)
				continue;
			tot_val += PieceValue(piece, sq);
		}
	}
	return tot_val;
}
int SamuelEngine::evaluate_chess_pos_without_depth(ChessBoard &board) const {
	std::optional<int> gameOverMaybe = returnStatusIfGameOver(board);
	if (gameOverMaybe)
		return *gameOverMaybe;

	return relative_value(board, Color::WHITE) - relative_value(board, Color::BLACK);
}

int SamuelEngine::evaluate_chess_pos_without_depth_negating_if_necessary(ChessBoard& board) const {
    int score = evaluate_chess_pos_without_depth(board);
    return (board.get_whiteToMove() ? score : -score);
}

std::vector<Move> SamuelEngine::orderMoves(ChessBoard &board) const {
	std::vector<Move> movesVec = board.allLegalMoves();
	std::sort(begin(movesVec), end(movesVec), SamuelEngine::MoveOrderer(board));
	return movesVec;
}

std::pair<int, Move> SamuelEngine::evaluate_chess_pos_with_depth(ChessBoard &board, int depth, int alpha, int beta) {
	if ((numBoardsVisited & 127) == 0 && shouldStop()) {
		throw OutOfTime();
	}
	numBoardsVisited++;
	std::optional<int> gameOverMaybe = returnStatusIfGameOver(board);
	if (gameOverMaybe) {
		return {*gameOverMaybe, NULL_MOVE};
	}
	if (depth == 0) {
		return {evaluate_chess_pos_without_depth(board), NULL_MOVE};
	}
	std::vector<Move> moves = orderMoves(board);
	if (moves.empty()) {
		return {evaluate_chess_pos_without_depth(board), NULL_MOVE};
	}

	Move bestMove = *moves.begin();
	if (board.get_whiteToMove()) {
		double value = -MATE_SCORE;

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
		int value = MATE_SCORE;
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
std::pair<int, Move> SamuelEngine::evaluate_chess_pos_with_tl(ChessBoard &board, double time_limit) {
	this->deadline = std::chrono::steady_clock::now() + std::chrono::nanoseconds(static_cast<long int>(time_limit * 1'000'000'000));
	int bestValue = 0;
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