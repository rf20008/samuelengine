#ifndef CS3520_MINIPROJECT_BOARD
#define CS3520_MINIPROJECT_BOARD
// Pieces
#include "Bishop.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Pawn.hpp"
#include "Piece.hpp"
#include "Queen.hpp"
#include "Rook.hpp"

// Moves
#include "GameStatus.hpp"
#include "Move.hpp"
#include "PlayerState.hpp"
#include "Square.hpp"

// Errors
#include "Errors.hpp"

// from STL
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

template <typename T> inline std::set<T> &mergeSets(std::set<T> &A, const std::set<T> &B) {
	// return the set, consisting of adding all the items in B to A
	for (const T &item : B) {
		A.insert(item);
	}
	return A;
}

class ChessBoard {
	protected:
		PiecePtr pieces[128];
		bool whiteToMove;
		PlayerState whitePlayerState;
		PlayerState blackPlayerState;
		int halfmove_clock;
		int fullmove_clock;
		std::optional<Square> enPassant_targetSquare;
		std::vector<Move> previousMoves;

	public:
		// constructors

		ChessBoard();						// create a chess board with starting position
		ChessBoard(const std::string &fen); // create chess board from given FEN
		ChessBoard(const std::vector<std::vector<PiecePtr>> &pieces, const bool &whiteToMove, const PlayerState &whitePlayerState, const PlayerState &blackPlayerState, const int &halfmove_clock, const int &fullmove_clock, const Square &enPassant_targetSquare, const std::vector<Move> &moves);

		ChessBoard(const std::vector<std::vector<PiecePtr>> &pieces, const bool &whiteToMove, const PlayerState &whitePlayerState, const PlayerState &blackPlayerState, const int &halfmove_clock, const int &fullmove_clock, const Square &enPassant_targetSquare);
		// rule of 5
		~ChessBoard() = default;
		ChessBoard(const ChessBoard &other) = default;
		ChessBoard(ChessBoard &&other) = default;
		ChessBoard &operator=(const ChessBoard &other) = default;
		ChessBoard &operator=(ChessBoard &&other) = default;

		// getters
		PiecePtr getPiece(Square sq) const;
		int get_halfmove_clock() const { return halfmove_clock; }
		int get_fullmove_clock() const { return fullmove_clock; }
		bool get_whiteToMove() const { return whiteToMove; }
		PlayerState getWhitePlayerState() const { return whitePlayerState; }
		PlayerState getBlackPlayerState() const { return blackPlayerState; }
		std::optional<Square> getEnPassantTargetSquare() const { return enPassant_targetSquare; }
		std::vector<Move> getPreviousMoves() const { return previousMoves; }

		// chess engine methods
		bool isMoveLegal(Move m) const; // return whether a move is legal
		void processMove(Move m);
		void processPsuedoLegalMove(Move m);
		void processEnPassantCapture(Move m, const PiecePtr &start_ptr, const PiecePtr &end_ptr);
		void processEnPassantUpdate(Move m, const PiecePtr &start_ptr, const PiecePtr &end_ptr);
		void processCastling(Move m, const PiecePtr &start_ptr);
		bool isInCheck(bool player) const;
		bool isInCheckmate() const;
		bool isInStalemate() const;
		bool hasInsufficientMaterial() const;
		Square findKing(bool belongsToWhite) const;
		GameStatus getStatus() const;

		// for engine use
		std::string fen() const;

		PiecePtr getAndAssertPiece(const Square origin, const char pieceType) const;
		bool hasPiece(const Square origin) const;
		bool squareAttackedBy(Square target, bool attackerIsWhite) const;
		bool isSlidingAttacker(Square from, Offset dir, bool attackerIsWhite, char pieceLetterA, char pieceLetterB) const;

		std::set<Move> whereKingCouldMove(const Square origin) const;
		std::set<Move> wherePawnCouldMove(const Square origin) const;
		std::set<Move> whereKnightCouldMove(const Square origin) const;
		std::set<Move> isSlidingAttacker(const Square from, const Offset dir, bool attackerIsWhite) const;
		std::set<Move> whereBishopCouldMove(const Square origin) const;
		std::set<Move> whereRookCouldMove(const Square origin) const;
		std::set<Move> whereQueenCouldMove(const Square origin) const;
		std::set<Move> allPseudoLegalDestinations(const Square origin) const;

		std::set<Move> allLegalMoves(const Square sq) const;
		std::set<Move> allLegalMoves() const;

		friend std::ostream &operator<<(std::ostream &os, const ChessBoard &board);

		// move evaluation
		ChessBoard board_with_move(const Move &move) const; // return a copy of the board, with move move applied
		bool move_ends_game(const Move move) const;
		bool move_is_castling(const Move move) const;
		bool move_is_check(const Move move) const;
		bool move_is_capture(const Move m) const;
		bool move_is_zeroing(const Move m) const;
		int perft(int depth, int divideThreshold = 2147483647) const;
		std::string debug_board() const;
};
#endif
