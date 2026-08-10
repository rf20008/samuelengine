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

typedef std::shared_ptr<Piece> PiecePtr;
std::shared_ptr<Piece> getPiece(char c); 

class ChessBoard {
    protected:
        std::vector<std::vector<std::shared_ptr<Piece>>> pieces;
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
            const std::vector<std::vector<std::shared_ptr<Piece>>>& pieces, 
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
        std::shared_ptr<const Piece> getPiece(Square sq) const;
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
        std::set<Move> allLegalMoves(const Square sq) const;
        std::set<Move> allLegalMoves() const;

        std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os) const;



};
#endif