#ifndef CS3520_MINIPROJECT_BOARD
#define CS3520_MINIPROJECT_BOARD

// Pieces
#include "Piece.hpp"

// Moves
#include "GameStatus.hpp"
#include "Move.hpp"
#include "PlayerState.hpp"
#include "Square.hpp"

// Errors
#include "Errors.hpp"

// Zobrist
#include "Zobrist.hpp"

// Undo Move
#include "UndoMove.hpp"

// from STL
#include <memory>
#include <cstdint>
#include <cassert>
#include <optional>
#include <string>
#include <vector>

template <typename T> inline std::vector<T> &mergeSets(std::vector<T> &A, const std::vector<T> &B) {
	// return the set, consisting of adding all the items in B to A
    A.insert(A.end(), B.begin(), B.end());
	return A;
}

class ChessBoard {
	protected:
        uint64_t zobrist_hash;
		Piece pieces[128]{};
		bool whiteToMove;
		PlayerState whitePlayerState;
		PlayerState blackPlayerState;
		int halfmove_clock;
		int fullmove_clock;
		std::optional<Square> enPassant_targetSquare;
        std::vector<UndoMove> history;
        Square whiteKingPos;
        Square blackKingPos;
	public:
		// constructors

		ChessBoard();						// create a chess board with starting position
		ChessBoard(const std::string &fen); // create chess board from given FEN
		
		// rule of 5
		~ChessBoard() = default;
		ChessBoard(const ChessBoard &other) = default;
		ChessBoard(ChessBoard &&other) = default;
		ChessBoard &operator=(const ChessBoard &other) = default;
		ChessBoard &operator=(ChessBoard &&other) = default;

		// getters
		Piece getPiece(Square sq) const {
            // bounds check: an out-of-board square simply has no piece on it
            if (!sq.isValid()) {
                return EMPTY_SQUARE;
                //throw std::logic_error("Invalid square position (idx="+std::to_string(sq.idx)+")");
            }
            return pieces[sq.idx];
        }
        uint64_t getZobrist() const {return zobrist_hash;}
		int get_halfmove_clock() const { return halfmove_clock; }
		int get_fullmove_clock() const { return fullmove_clock; }
		bool get_whiteToMove() const { return whiteToMove; }
		PlayerState getWhitePlayerState() const { return whitePlayerState; }
		PlayerState getBlackPlayerState() const { return blackPlayerState; }
		std::optional<Square> getEnPassantTargetSquare() const { return enPassant_targetSquare; }
        std::vector<UndoMove> getHistory() const {return history;}

		// chess engine methods
		bool isMoveLegal(Move m); // return whether a move is legal
		void processMove(Move m);
		void processPsuedoLegalMove(Move m);
		void processEnPassantCapture(Move m, const Piece &start_ptr, const Piece &end_ptr);
		void processEnPassantUpdate(Move m, const Piece &start_ptr, const Piece &end_ptr);
		void processCastling(Move m, const Piece &start_ptr);
		bool isInCheck(bool player) const;
		bool isInCheckmate(); // interacts with move ordering!
		bool isInStalemate(); // also interacts with move ordering
        bool is_threefold_repetition() const;
		bool hasInsufficientMaterial() const;
        Square findKingSlow(Color expectedColor) const {
            for (int sq_idx = 0; sq_idx < 128; ++sq_idx) {
                Piece p = pieces[sq_idx];
                if (p.type == PieceType::KING && p.color == expectedColor) {
                    return Square(sq_idx);
                }
            }
            throw std::logic_error(std::string("findKing: no king found for ") + (colorName(expectedColor)) + " in the board with FEN " + this->debug_board());
        }
		Square findKing(Color expectedColor) const {
            assert(expectedColor != Color::NONE);
            Square kingPos = expectedColor == Color::WHITE ? whiteKingPos : blackKingPos;
            assert((getPiece(kingPos) == ((expectedColor == Color::WHITE) ? WHITE_KING : BLACK_KING))&&"king cache desync");
            return kingPos;
        }
		GameStatus getStatus(); // doesn't change board but interacts with move ordering

		// for engine use
		std::string fen() const;

		Piece getAndAssertPiece(const Square origin, const Piece expectedType) const {
            // get the piece at origin, and assert it is of tpe pieceTzype
            Piece ptr = getPiece(origin);
            assert(ptr == expectedType);
            return ptr;
        }
		bool hasPiece(const Square origin) const { return getPiece(origin) != EMPTY_SQUARE; }
		bool squareAttackedBy(Square target, bool attackerIsWhite) const;
		bool isSlidingAttacker(Square from, int dir, bool attackerIsWhite, char pieceLetterA, char pieceLetterB) const;

		std::vector<Move> whereKingCouldMove(const Square origin) const;
		std::vector<Move> wherePawnCouldMove(const Square origin) const;
		std::vector<Move> whereKnightCouldMove(const Square origin) const;
		std::vector<Move> isSlidingAttacker(const Square from, const int dir, bool attackerIsWhite) const;
		std::vector<Move> whereBishopCouldMove(const Square origin) const;
		std::vector<Move> whereRookCouldMove(const Square origin) const;
		std::vector<Move> whereQueenCouldMove(const Square origin) const;
		std::vector<Move> allPseudoLegalDestinations(const Square origin) const;

		std::vector<Move> allLegalMoves(const Square sq);
		std::vector<Move> allLegalMoves();

		friend std::ostream &operator<<(std::ostream &os, const ChessBoard &board);

		// move evaluation
		ChessBoard board_with_move(const Move &move) const; // return a copy of the board, with move move applied
		bool move_ends_game(const Move move);
		bool move_is_castling(const Move move) const;
		bool move_is_check(const Move move);
		bool move_is_capture(const Move m) const;
		bool move_is_zeroing(const Move m) const;


		int perftCopy(int depth, int divideThreshold = 2147483647);
        int perft(int depth, int divideThreshold = 2147483647);
		std::string debug_board() const;

        // zobrist
        constexpr int castlingBits() const {
            int bit = 0;
            bit |= (whitePlayerState.canKingsideCastle ? 1 : 0);
            bit |= (whitePlayerState.canQueensideCastle ? 2 : 0);
            bit |= (blackPlayerState.canKingsideCastle ? 4 : 0);
            bit |= (blackPlayerState.canQueensideCastle ? 8 : 0);
            return bit;
        }   
    
        uint64_t zobristFromScratch() const;
        void verifyZobrist() const {
            uint64_t scratch = this->zobristFromScratch(); 
            if (this->zobrist_hash != scratch) { 
                throw std::logic_error( 
                    "Zobrist mismatch: incremental=" + std::to_string(this->zobrist_hash) + 
                    " scratch=" + std::to_string(scratch) + 
                    " fen=" + this->fen()
                ); 
            } 
        }

        // undo move
        UndoMove buildUndo(const Move& m) const;
        void undoMove(const UndoMove& u);
        bool undoMove();


        // equality
        bool operator==(const ChessBoard& other) const {
            for(int i=0;i<128;i++) if(pieces[i]!=other.pieces[i]) return false;
            return (
                whitePlayerState == other.whitePlayerState &&
                blackPlayerState == other.blackPlayerState &&
                whiteToMove == other.whiteToMove &&
                halfmove_clock == other.halfmove_clock &&
                enPassant_targetSquare == other.enPassant_targetSquare
            );
        }
};
#endif
