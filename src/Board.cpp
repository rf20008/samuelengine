#include "Board.hpp"
#include "Errors.hpp"

#include <cctype>
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

PiecePtr ChessBoard::getPiece(Square sq) {
    // bounds check: an out-of-board square simply has no piece on it
    if (!sq.isValid()) {
        return nullptr;
    }
    // Square rows/cols are 1-indexed (see Square::isValid), pieces is 0-indexed
    return pieces[sq.row - 1][sq.col - 1];
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

namespace {
    // Is there a piece belonging to `attackerIsWhite` on the far end of the
    // ray starting at `from` and stepping by `dir` (one step at a time) that
    // could capture along that ray -- i.e. a rook/queen on a rank/file ray,
    // or a bishop/queen on a diagonal ray? The ray stops at the first
    // occupied square either way (that piece blocks anything behind it).
    bool isSlidingAttacker(ChessBoard& board, Square from, Square dir, bool attackerIsWhite, char pieceLetterA, char pieceLetterB) {
        Square cur = from + dir;
        while (cur.isValid()) {
            PiecePtr p = board.getPiece(cur);
            if (p) {
                if (p->getBelongsToWhite() == attackerIsWhite) {
                    char sym = static_cast<char>(std::toupper(p->symbol()));
                    if (sym == pieceLetterA || sym == pieceLetterB) {
                        return true;
                    }
                }
                return false; // occupied, so the ray is blocked past here regardless
            }
            cur = cur + dir;
        }
        return false;
    }

    // Is `target` attacked by any piece belonging to `attackerIsWhite`?
    // This is a raw-attack check (used to detect check): it only asks
    // "could this piece capture on `target` right now", not whether doing
    // so would be a legal move for the attacker.
    bool squareAttackedBy(ChessBoard& board, Square target, bool attackerIsWhite) {
        static const Square knightOffsets[] = {
            {1, 2}, {2, 1}, {-1, 2}, {-2, 1}, {1, -2}, {2, -1}, {-1, -2}, {-2, -1}
        };
        for (const Square& off : knightOffsets) {
            Square sq = target + off;
            if (!sq.isValid()) continue;
            PiecePtr p = board.getPiece(sq);
            if (p && p->getBelongsToWhite() == attackerIsWhite && std::toupper(p->symbol()) == 'N') {
                return true;
            }
        }

        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                Square sq = target + Square(dr, dc);
                if (!sq.isValid()) continue;
                PiecePtr p = board.getPiece(sq);
                if (p && p->getBelongsToWhite() == attackerIsWhite && std::toupper(p->symbol()) == 'K') {
                    return true;
                }
            }
        }

        // A pawn attacks diagonally, one row "ahead" of where it sits (from
        // its own side's perspective): white pawns advance toward higher
        // rows, so an attacking white pawn sits one row *below* the target.
        int behind = attackerIsWhite ? -1 : 1;
        for (int dc : {-1, 1}) {
            Square sq = target + Square(behind, dc);
            if (!sq.isValid()) continue;
            PiecePtr p = board.getPiece(sq);
            if (p && p->getBelongsToWhite() == attackerIsWhite && std::toupper(p->symbol()) == 'P') {
                return true;
            }
        }

        static const Square rookDirs[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        for (const Square& dir : rookDirs) {
            if (isSlidingAttacker(board, target, dir, attackerIsWhite, 'R', 'Q')) {
                return true;
            }
        }

        static const Square bishopDirs[] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (const Square& dir : bishopDirs) {
            if (isSlidingAttacker(board, target, dir, attackerIsWhite, 'B', 'Q')) {
                return true;
            }
        }

        return false;
    }

    Square findKing(ChessBoard& board, bool belongsToWhite) {
        for (int r = 1; r <= BOARD_SIZE; ++r) {
            for (int c = 1; c <= BOARD_SIZE; ++c) {
                Square sq(r, c);
                PiecePtr p = board.getPiece(sq);
                if (p && p->getBelongsToWhite() == belongsToWhite && std::toupper(p->symbol()) == 'K') {
                    return sq;
                }
            }
        }
        throw std::logic_error("findKing: no king found for the requested player");
    }
}

bool ChessBoard::isInCheck(bool player) {
    // can the player whose turn it is, capture the king who is owned by Player?
    Square kingSquare = findKing(*this, player);
    return squareAttackedBy(*this, kingSquare, !player);
}

bool ChessBoard::isInCheckmate() {
    // this can be implemented by: are you in check right now
    // and for every legal move you make, would you still be in check? if so it's checkmate, otherwise no
    return isInCheck(whiteToMove) && allLegalMoves().empty();
}

bool ChessBoard::isInStalemate() {
    // is implemented by: if not in check now, but every legal move you make is in check
    return !isInCheck(whiteToMove) && allLegalMoves().empty();
}

GameStatus ChessBoard::getStatus() {
    // return the status of the game (whether white won, black won, it's a draw, or game is still going on)
    // keep in mind the 50 move rule, draws
    throw NotImplementedError("GameStatus ChessBoard::getStatus() is not yet implemented");
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