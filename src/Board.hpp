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

// Errors
#include "Errors.hpp"


// from STL
#include <vector>
#include <memory>
#include <string>
typedef std::shared_ptr<Piece> PiecePtr;
std::shared_ptr<Piece> getPiece(char c); 

class ChessBoard {
    protected:
        std::vector<std::vector<std::shared_ptr<Piece>>> pieces;
        bool whiteToMove;
        PlayerState whitePlayer;
        PlayerState blackPlayer;
        int halfmove_clock;
        int fullmove_clock;
        Square enPassant_targetSquare;
    public:
        // constructors

        ChessBoard(); // create a chess board with starting position
        ChessBoard(const std::string& fen); // create chess board from given FEN
        ChessBoard(
            const std::vector<std::vector<std::shared_ptr<Piece>>> pieces, 
            const bool& whiteToMove, 
            const PlayerState& whitePlayer,
            const PlayerState& blackPlayer,
            const int& halfmove_clock,
            const int& fullmove_clock,
            const Square& enPassant_targetSquare
        );

        // rule of 5
        ~ChessBoard();
        ChessBoard(const ChessBoard& other);
        ChessBoard(ChessBoard&& other) = delete;
        ChessBoard& operator=(const ChessBoard& other);
        ChessBoard& operator=(ChessBoard&& other) = delete;

        // getters and setters
        std::shared_ptr<Piece> getPiece(Square sq);
        int get_halfmove_clock();
        int get_fullmove_clock();
        bool get_whiteToMove();
        PlayerState getWhitePlayerState();
        PlayerState getBlackPlayerState();

        // chess engine methods
        bool isMoveLegal(Move m); // return whether a move is legal
        void processMove(Move m);
        bool isInCheck();
        bool isInCheckmate();
        bool isInStalemate();

        // for engine use
        std::string fen();
        std::set<Move> allLegalMoves();

        std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os);



};
#endif