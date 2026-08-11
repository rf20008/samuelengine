#ifndef CS3520_MINIPROJECT_BOARD
#define CS3520_MINIPROJECT_BOARD
// Pieces
#include "Bishop.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Pawn.hpp"
#include "Queen.hpp"
#include "Rook.hpp"
#include "Piece.hpp"

// Moves
#include "Move.hpp"
#include "Square.hpp"
#include "PlayerState.hpp"
#include "GameStatus.hpp"

// Errors
#include "Errors.hpp"


// from STL
#include <vector>
#include <memory>
#include <string>
#include <optional>
#include <set>

template<typename T>
inline std::set<T>& mergeSets(std::set<T>& A, const std::set<T>& B) {
    // return the set, consisting of adding all the items in B to A
    for (const T& item : B) {A.insert(item);}
    return A;
}

class ChessBoard {
    protected:
        std::vector<std::vector<PiecePtr>> pieces;
        bool whiteToMove;
        PlayerState whitePlayerState;
        PlayerState blackPlayerState;
        int halfmove_clock;
        int fullmove_clock;
        std::optional<Square> enPassant_targetSquare;
    public:
        // constructors

        ChessBoard(); // create a chess board with starting position
        ChessBoard(const std::string& fen); // create chess board from given FEN
        ChessBoard(
            const std::vector<std::vector<PiecePtr>>& pieces, 
            const bool& whiteToMove, 
            const PlayerState& whitePlayerState,
            const PlayerState& blackPlayerState,
            const int& halfmove_clock,
            const int& fullmove_clock,
            const Square& enPassant_targetSquare
        );

        // rule of 5
        ~ChessBoard() = default;
        ChessBoard(const ChessBoard& other) = default;
        ChessBoard(ChessBoard&& other) = default;
        ChessBoard& operator=(const ChessBoard& other) = default;
        ChessBoard& operator=(ChessBoard&& other) = default;

        // getters 
        PiecePtr getPiece(Square sq) const;
        int get_halfmove_clock() const {return halfmove_clock;}
        int get_fullmove_clock() const {return fullmove_clock;}
        bool get_whiteToMove() const {return whiteToMove;}
        PlayerState getWhitePlayerState() const {return whitePlayerState;}
        PlayerState getBlackPlayerState() const {return blackPlayerState;}

        
        // chess engine methods
        bool isMoveLegal(Move m) const; // return whether a move is legal
        void processMove(Move m);
        bool isInCheck(bool player) const;
        bool isInCheckmate() const;
        bool isInStalemate() const;
        bool hasInsufficientMaterial() const;
        Square findKing(bool belongsToWhite) const;
        GameStatus getStatus() const;

        // for engine use
        std::string fen() const;

        PiecePtr getAndAssertPiece(const Square origin, const char pieceType) const;
        
        std::set<Square> whereKingCouldMove(const Square origin) const;
        std::set<Square> wherePawnCouldMove(const Square origin) const;
        std::set<Square> whereKnightCouldMove(const Square origin) const;
        bool isSlidingAttacker(const ChessBoard& board, Square from, Square dir, bool attackerIsWhite, char pieceLetterA, char pieceLetterB);
        std::set<Square> whereBishopCouldMove(const Square origin) const;
        std::set<Square> whereRookCouldMove(const Square origin) const;
        std::set<Square> whereQueenCouldMove(const Square origin) const;
        std::set<Square> allPseudoLegalDestinations(const Square origin) const;

        std::set<Move> allLegalMoves(const Square sq) const;
        std::set<Move> allLegalMoves() const;

        friend std::ostream& operator<<(std::ostream& os, const ChessBoard& board);

        // move evaluation
        ChessBoard board_with_move(const Move& move) const; // return a copy of the board, with move move applied
        bool move_ends_game(const Move move) const;
        bool move_is_castling(const Move move) const;
        bool move_is_check(const Move move) const;
        bool move_is_capture(const Move m) const;
        bool move_is_zeroing(const Move m) const;


};
#endif
