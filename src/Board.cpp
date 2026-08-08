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
    // to be done by Samuel
    throw NotImplementedError("ChessBoard::ChessBoard() is not yet implemented");
}

ChessBoard::ChessBoard(const std::string& fen) {
    (void) fen;
    // to be done by Samuel
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
    // to be done by Joshua
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

PiecePtr getPiece(Square sq) {
    // to be done by Andrew
    // remember to bounds check!
    (void) sq;
    throw NotImplementedError("PiecePtr getPiece(Square sq) is not yet implemented");
}

bool ChessBoard::isMoveLegal(Move m){
    // check whether m is a legal move
    // (may want to use allLegalMoves)
    // to be done by Joshua
    (void) m;
    throw NotImplementedError("bool ChessBoard::isMoveLegal(Move m) is not yet implemented");
} // return whether a move is legal

void ChessBoard::processMove(Move m) {
    // perform the given move if and only if m is a legal move 
    // update the counters appropriately
    // to be done by Joshua
    (void) m;
    throw NotImplementedError("void ChessBoard::processMove(Move m) is not yet implemented");
}

bool ChessBoard::isInCheck(bool player) {
    // can the player whose turn it is, capture the king who is owned by Player?
    // to be done by Andrew
    throw NotImplementedError("bool ChessBoard::isInCheck() is not yet implemented");
}

bool ChessBoard::isInCheckmate() {
    // to be done by Andrew
    // this cna be implemented by: are you in check right now
    // and for every legal move you make, would you still be in check? if so it's checkmate, otherwise no
    throw NotImplementedError("bool ChessBoard::isInCheckmate() is not yet implemented");
}

bool ChessBoard::isInStalemate() {
    // to be done by Andrew
    // is implemented by: if not in check now, but every legal move you make is in check
    throw NotImplementedError("bool ChessBoard::isInStalemate() is not yet implemented");
}



// for engine use
std::string ChessBoard::fen() {
    // to be done by Samuel
    throw NotImplementedError("std::string ChessBoard::fen() is not yet implemented");
}

std::set<Move> ChessBoard::allLegalMoves(const Square sq) {
    // this method may need to be shared
    // let's share it
    // get all legal moves from the piece at the square indicated 
    // if there is no piece at that square, or if the piece at that square is owned by the opponent, return empty set
    throw NotImplementedError("std::set<Move> ChessBoard::allLegalMoves() is not yet implemented");
}
std::set<Move> ChessBoard::allLegalMoves() {
    // to be done by Joshua
    // return all legal moves from all pieces that the player owns
    // this function is necessary for the engine
    // it can call allLegalMoves for every piece it owns and splice them together into one set, then return that set

    throw NotImplementedError("std::set<Move> ChessBoard::allLegalMoves() is not yet implemented");
}

std::basic_ostream<char>& ChessBoard::operator<<(std::basic_ostream<char>& os) {
    // to be done by Joshua
    (void) os;
    throw NotImplementedError("std::basic_ostream<char>& ChessBoard::operator<<(std::basic_ostream<char>& os) is not yet implemented");
}