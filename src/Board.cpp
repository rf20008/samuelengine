#include "Board.hpp"
#include "Errors.hpp"

#include <map>
#include <iostream>
using namespace std;
const PiecePtr WhiteBishop = std::make_shared<Bishop>(true);
const PiecePtr BlackBishop = std::make_shared<Bishop>(false);

const PiecePtr WhiteKnight = std::make_shared<Knight>(true);
const PiecePtr BlackKnight = std::make_shared<Knight>(false);

const PiecePtr WhiteRook = std::make_shared<Rook>(true);
const PiecePtr BlackRook = std::make_shared<Rook>(false);

const PiecePtr WhiteKing = std::make_shared<King>(true);
const PiecePtr BlackKing = std::make_shared<King>(false);

const PiecePtr WhitePawn = std::make_shared<Pawn>(true);
const PiecePtr BlackPawn = std::make_shared<Pawn>(false);

const PiecePtr WhiteQueen = std::make_shared<Queen>(true);
const PiecePtr BlackQueen = std::make_shared<Queen>(false);

const std::map<char, PiecePtr> pieceMap{
    {'B', WhiteBishop},
    {'b', BlackBishop},
    {'N', WhiteKnight},
    {'n', BlackKnight},
    {'R', WhiteRook},
    {'r', BlackRook},
    {'K', WhiteKing},
    {'k', BlackKing},
    {'P', WhitePawn},
    {'p', BlackPawn},
    {'Q', WhiteQueen},
    {'q', BlackQueen}
};
std::shared_ptr<Piece> getPiece(char c){
    std::map<char, PiecePtr>::const_iterator it = pieceMap.find(c);
    if (it == pieceMap.end()) {throw UnknownPiece("Unknown piece: " + std::string{c});}
    return it->second;
}

ChessBoard::ChessBoard() {
    throw NotImplementedError("ChessBoard::ChessBoard is not yet implemented");
}

ChessBoard::ChessBoard(const std::string& fen) {
    (void) fen;
    throw NotImplementedError("ChessBoard::ChessBoard(std::string& fen) is not yet implemented");
}

ChessBoard::ChessBoard(
    const std::vector<std::vector<std::shared_ptr<Piece>>>& pieces, 
    const bool& whiteToMove, 
    const PlayerState& whitePlayerState,
    const PlayerState& blackPlayerState,
    const int& halfmove_clock,
    const int& fullmove_clock,
    const Square& enPassant_targetSquare
) {
    (void) pieces;
    (void) whiteToMove;
    (void) whitePlayerState;
    (void) blackPlayerState;
    (void) halfmove_clock;
    (void) fullmove_clock;
    (void) enPassant_targetSquare;
    throw NotImplementedError(
        "ChessBoard::ChessBoard(" 
        "const std::vector<std::vector<std::shared_ptr<Piece>>>& pieces, " 
        "const bool& whiteToMove, " 
        "const PlayerState& whitePlayer, " 
        "const PlayerState& blackPlayer, " 
        "const int& halfmove_clock, "
        "const int& fullmove_clock, "
        "const Square& enPassant_targetSquare"
        ") is not yet implemented"
    );
}

// Rule of Five

ChessBoard::~ChessBoard() {
    cerr<<"ERROR: ChessBoard::~ChessBoard() is not yet implemented"<<endl; // cannot throw because destructors are noexcept
}

ChessBoard::ChessBoard(const ChessBoard& other) {
    (void) other;
    throw NotImplementedError("ChessBoard::ChessBoard(const ChessBoard& other) is not yet implemented");   
}

ChessBoard& ChessBoard::operator=(const ChessBoard& other) {
    (void) other;
    throw NotImplementedError("ChessBoard& ChessBoard::operator=(const ChessBoard& other) is not yet implemented");
}

// move constructors are explicitly deleted!
PiecePtr getPiece(Square sq) {
    (void) sq;
    throw NotImplementedError("PiecePtr getPiece(Square sq) is not yet implemented");
}

bool ChessBoard::isMoveLegal(Move m){
    (void) m;
    throw NotImplementedError("bool ChessBoard::isMoveLegal(Move m) is not yet implemented");
} // return whether a move is legal

void ChessBoard::processMove(Move m) {
    (void) m;
    throw NotImplementedError("void ChessBoard::processMove(Move m) is not yet implemented");
}

bool ChessBoard::isInCheck() {
    throw NotImplementedError("bool ChessBoard::isInCheck() is not yet implemented");
}

bool ChessBoard::isInCheckmate() {
    throw NotImplementedError("bool ChessBoard::isInCheckmate() is not yet implemented");
}

bool ChessBoard::isInStalemate() {
    throw NotImplementedError("bool ChessBoard::isInStalemate() is not yet implemented");
}

// for engine use
std::string ChessBoard::fen() {
    throw NotImplementedError("std::string ChessBoard::fen() is not yet implemented");
}

std::set<Move> ChessBoard::allLegalMoves() {
    throw NotImplementedError("std::set<Move> ChessBoard::allLegalMoves() is not yet implemented");
}

std::basic_ostream<char>& ChessBoard::operator<<(std::basic_ostream<char>& os) {
    (void) os;
    throw NotImplementedError("std::basic_ostream<char>& ChessBoard::operator<<(std::basic_ostream<char>& os) is not yet implemented");
}