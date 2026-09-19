// This is a copy of the engine I made to play chess in Python. I am translating it into C++
#include "SamuelEngine.hpp"
#include "Errors.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <optional>
#include <vector>

using namespace std;
const int INF = 1'000'000'000;
constexpr Move NULL_MOVE = Move(Square(0x88), Square(0x88));

int SamuelEngine::MoveOrderer::score_move(const Move &mov) {

    if (mov.promotion != '\0') {
        return 50'000;
    }

    if (m_board.move_is_capture(mov)) {
        Piece victim =
            m_board.getPiece(m_board.get_capturing_square(mov));

        Piece attacker =
            m_board.getPiece(mov.startingSquare);

        return 20'000
             + 100 * PIECE_VALUES[victim.pieceNum()]
             - PIECE_VALUES[attacker.pieceNum()];
    }

    if (m_board.move_is_check(mov)) {
        if (m_board.move_gives_checkmate(mov)) {
            return 100'000;
        }
        return 3;
    }

    if (m_board.move_draws_game(mov)) {
        return 2;
    }

    if (m_board.move_is_castling(mov)) {
        return 1;
    }

    return 0;
}
bool SamuelEngine::MoveOrderer::operator()(const Move &m1, const Move &m2) { return score_move(m1) > score_move(m2); }


std::optional<int> SamuelEngine::returnStatusIfGameOver(ChessBoard& board) const {
    GameStatus status = board.getStatus();

    if (!isGameOver(status))
        return std::nullopt;

    int score = 0;
    switch (status) {
    case GameStatus::WHITE_WON:
        score = MATE_SCORE - board.get_ply(); break;

    case GameStatus::BLACK_WON:
        score = -MATE_SCORE + board.get_ply(); break;

    default: // draw
        score = 0; break;
    }
    return score;
}

int negateScoreIfBlack(int score, Color color) {
    return isWhite(color) ? score : -score;
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



std::vector<Move> SamuelEngine::orderMoves(ChessBoard &board) const {
	std::vector<Move> movesVec = board.allLegalMoves();
	std::sort(begin(movesVec), end(movesVec), SamuelEngine::MoveOrderer(board));
	return movesVec;
}

std::pair<int, Move> SamuelEngine::evaluate_chess_pos_with_depth(ChessBoard &board, int depth, int alpha, int beta) {
	if ((numBoardsVisited & 255) == 0 && shouldStop()) {
		throw OutOfTime();
	}
	numBoardsVisited++;
	std::optional<int> gameOverMaybe = returnStatusIfGameOver(board);
	if (gameOverMaybe) {
		return {negateScoreIfBlack(*gameOverMaybe, board.getPlayerToMove()), NULL_MOVE};
	}
	if (depth == 0) {
		return {evaluate_chess_pos_without_depth_negating_if_necessary(board), NULL_MOVE};
	}
	std::vector<Move> moves = orderMoves(board);
	if (moves.empty()) {
		return {evaluate_chess_pos_without_depth_negating_if_necessary(board), NULL_MOVE};
	}

	Move bestMove = *moves.begin();
    // negamax!
    int value = -MATE_SCORE - 1000;
    for (Move move : moves) {
        assert(board.isMoveLegal(move));
        board.processPsuedoLegalMove(move); // i know the move is legal!
        int child_val = -(evaluate_chess_pos_with_depth(board, depth-1, -beta, -alpha).first);
        if (child_val > value) {
            value = child_val;
            bestMove = move;
        }
        alpha = max(alpha, value);
        board.undoMove();
        if (alpha >= beta) break; // cut-off
    }
    return {value, bestMove};
}
std::pair<int, Move> SamuelEngine::evaluate_chess_pos_with_tl(ChessBoard &board, double time_limit) {
	this->deadline = std::chrono::steady_clock::now() + std::chrono::nanoseconds(static_cast<long int>(time_limit * 1'000'000'000));
	int bestValue = 0;
	Move bestMove = *(board.allLegalMoves().begin());
    int depthReached = 0;
	try {
		for (int depth = 1; !shouldStop(); ++depth) {

			auto [newValue, newMove] = evaluate_chess_pos_with_depth(board, depth, -INF, +INF);
            depthReached = depth;
			bestValue = newValue;
			bestMove = newMove;
		}
	} catch (OutOfTime &err) {
	}
    if (debug) {
        cerr<<"Reached a depth of "<<depthReached<<"\n";
    }
    
	return {bestValue, bestMove};
}
inline bool SamuelEngine::shouldStop() const { return std::chrono::steady_clock::now() >= deadline; }
SamuelEngine::SamuelEngine(double tl, bool dbg) : debug(dbg), numBoardsVisited(0), default_tl(tl), deadline(std::chrono::steady_clock::now()) {}
Move SamuelEngine::getMove(const ChessBoard &board) {
    ChessBoard scratchBoard = board;
    // return forced move if only one legal move
    std::vector<Move> allMoves = scratchBoard.allLegalMoves();
    if (allMoves.size()==1) {
        return allMoves[0];
    }
	this->numBoardsVisited = 0;
	if (debug) {
		cerr << "Beginning search" << endl;
	}
	auto [val, move] = evaluate_chess_pos_with_tl(scratchBoard, default_tl);
	if (debug) {
		cerr << "Finished search\n. Searched " << numBoardsVisited << " positions to find a value of " << val << endl;
        cerr << "FEN: "<<board.fen()<<"\n";
	}
	return move;
}