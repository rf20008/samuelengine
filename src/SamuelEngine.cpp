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
    if (mov == m_hashMove) return 2'000'000;
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
std::vector<Move> SamuelEngine::MoveOrderer::orderMoves() {
    std::vector<ScoredMove> scored;

    for (const Move& move : m_board.allLegalMoves()) {
        scored.push_back({move, score_move(move)});
    }

    sort(scored.begin(), scored.end());

    std::vector<Move> result;
    result.reserve(scored.size());

    for (const ScoredMove& sm : scored) {
        result.push_back(sm.move);
    }

    return result;
}

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
    std::vector<Move> movesVec = board.allLegalMoves();
	if (movesVec.empty()) {
		return {evaluate_chess_pos_without_depth_negating_if_necessary(board), NULL_MOVE};
	}
    TTEntry entry = transpositionTable.probe(board.getZobrist());
    const int originalAlpha = alpha;
    Move hashMove = entry.bestMove;
    if (!board.isMoveLegal(hashMove)) hashMove = NULL_MOVE;
    const int originalBeta = beta;
    if (entry && entry.depth >= depth) {
        if (entry.flag == TTFlag::EXACT) {
            return {entry.value, entry.bestMove};
        }

        if (entry.flag == TTFlag::LOWER_BOUND) {
            alpha = std::max(alpha, entry.value);
        }
        else if (entry.flag == TTFlag::UPPER_BOUND) {
            beta = std::min(beta, entry.value);
        }

        if (alpha >= beta) {
            return {entry.value, entry.bestMove};
        }
    }
    MoveOrderer orderer(board, hashMove);
    movesVec = orderer.orderMoves();

    
    assert(movesVec.size() > 0);
	Move bestMove = movesVec[0];
    // negamax!
    int value = -MATE_SCORE - 1000;
    for (size_t moveNum = 0; moveNum < movesVec.size(); ++moveNum) {
        Move curMove = movesVec[moveNum];
        assert(board.isMoveLegal(curMove));
        board.processPsuedoLegalMove(curMove);
        int score = 0;
        if (moveNum == 0) { // principal valuation!
            score = -(evaluate_chess_pos_with_depth(board, depth-1, -beta, -alpha).first);
        } else {
            score = -(evaluate_chess_pos_with_depth(board, depth-1, -alpha-1, -alpha).first);
            if (alpha < score && score < beta) { // it failed high!
                score = -(evaluate_chess_pos_with_depth(board, depth-1, -beta, -alpha).first);
            }
        }
        board.undoMove();
        if (score > value) {
            value = score;
            bestMove = curMove;
        }
    
        alpha = std::max(alpha, score);
        if (alpha>=beta) break;
    }
    TTFlag flag;

    if (value <= originalAlpha) {
        flag = TTFlag::UPPER_BOUND;
    }
    else if (value >= originalBeta) {
        flag = TTFlag::LOWER_BOUND;
    }
    else {
        flag = TTFlag::EXACT;
    }

    // ---- TT STORE ----
    transpositionTable.store(TTEntry{
        board.getZobrist(),
        depth,
        value,
        flag,
        bestMove
    });
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
SamuelEngine::SamuelEngine(double tl, bool dbg, size_t transposition_table_size) : debug(dbg), transpositionTable(transposition_table_size), numBoardsVisited(0), default_tl(tl), deadline(std::chrono::steady_clock::now()) {}
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