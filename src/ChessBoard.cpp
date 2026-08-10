#include "ChessBoard.hpp"
#include "Errors.hpp"
#include "FEN.hpp"

#include <cctype>
#include <map>
#include <sstream>
#include <iostream>
#include <optional>
using namespace std;




ChessBoard::ChessBoard() : ChessBoard::ChessBoard("pppppppp/rnbqkbnr/8/8/8/8/RNBQKBNR/PPPPPPPP w KQkq - 0 1"){}

ChessBoard::ChessBoard(const std::string& fen) {
    (void) fen;
    std::istringstream fenSS(fen);
    std::string PiecePart;
    std::string PlayerPart;
    std::string CastlingPart;
    std::string EnPassantPart;
    int Halfmove_Part;
    int Fullmove_part;
    fenSS>>PiecePart>>PlayerPart>>CastlingPart>>EnPassantPart>>Halfmove_Part>>Fullmove_part;
    if (fenSS.fail()) {
        throw std::invalid_argument("Not enough arguments to construct fen (need 7)");
    }
    this->pieces = ParsePieces::parsePiecePart(PiecePart);
    this->whiteToMove = ParsePieces::parsePlayerPart(PlayerPart);
    auto [whiteState, blackState] = ParsePieces::parseCastlingPart(CastlingPart);
    this->whitePlayerState = whiteState;
    this->blackPlayerState = blackState;

    this->enPassant_targetSquare = ParsePieces::parseEnPassantPart(EnPassantPart);
    this->halfmove_clock = Halfmove_Part;
    if (halfmove_clock < 0) {
        throw std::invalid_argument("halfmove clock too low");
    }
    this->fullmove_clock = Fullmove_part;
    if (fullmove_clock < 1) {
        throw std::invalid_argument("fullmove clock too low");
    }
}

ChessBoard::ChessBoard(
    const std::vector<std::vector<std::shared_ptr<Piece>>>& pieces, 
    const bool& whiteToMove, 
    const PlayerState& whitePlayerState,
    const PlayerState& blackPlayerState,
    const int& halfmove_clock,
    const int& fullmove_clock,
    const Square& enPassant_targetSquare
):
	pieces(pieces),
	whiteToMove(whiteToMove),
	whitePlayerState(whitePlayerState),
	blackPlayerState(blackPlayerState),
	halfmove_clock(halfmove_clock),
	fullmove_clock(fullmove_clock),
	enPassant_targetSquare(enPassant_targetSquare)
	{}

std::shared_ptr<const Piece> ChessBoard::getPiece(Square sq) const {
    // bounds check: an out-of-board square simply has no piece on it
    if (!sq.isValid()) {
        return nullptr;
    }
    // Square rows/cols are 1-indexed (see Square::isValid), pieces is 0-indexed
    return pieces[sq.row - 1][sq.col - 1];
}

bool ChessBoard::isMoveLegal(Move m) const{
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
    bool isSlidingAttacker(const ChessBoard& board, Square from, Square dir, bool attackerIsWhite, char pieceLetterA, char pieceLetterB) {
        Square cur = from + dir;
        while (cur.isValid()) {
            std::shared_ptr<const Piece> p = board.getPiece(cur);
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
    bool squareAttackedBy(const ChessBoard& board, Square target, bool attackerIsWhite) {
        static const Square knightOffsets[] = {
            {1, 2}, {2, 1}, {-1, 2}, {-2, 1}, {1, -2}, {2, -1}, {-1, -2}, {-2, -1}
        };
        for (const Square& off : knightOffsets) {
            Square sq = target + off;
            if (!sq.isValid()) continue;
            std::shared_ptr<const Piece> p = board.getPiece(sq);
            if (p && p->getBelongsToWhite() == attackerIsWhite && std::toupper(p->symbol()) == 'N') {
                return true;
            }
        }

        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                Square sq = target + Square(dr, dc);
                if (!sq.isValid()) continue;
                std::shared_ptr<const Piece> p = board.getPiece(sq);
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
            std::shared_ptr<const Piece> p = board.getPiece(sq);
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

    
}
Square ChessBoard::findKing(bool belongsToWhite) const {
    for (int r = 1; r <= BOARD_SIZE; ++r) {
        for (int c = 1; c <= BOARD_SIZE; ++c) {
            Square sq(r, c);
            std::shared_ptr<const Piece> p = this->getPiece(sq);
            if (p && p->getBelongsToWhite() == belongsToWhite && std::toupper(p->symbol()) == 'K') {
                return sq;
            }
        }
    }
    throw std::logic_error("findKing: no king found for the requested player");
}

bool ChessBoard::isInCheck(bool player) const  {
    // can the player whose turn it is, capture the king who is owned by Player?
    Square kingSquare = this->findKing(player);
    return squareAttackedBy(*this, kingSquare, !player);
}

bool ChessBoard::isInCheckmate() const {
    // this can be implemented by: are you in check right now
    // and for every legal move you make, would you still be in check? if so it's checkmate, otherwise no
    return isInCheck(whiteToMove) && allLegalMoves().empty();
}

bool ChessBoard::isInStalemate() const {
    // is implemented by: if not in check now, but every legal move you make is in check
    return !isInCheck(whiteToMove) && allLegalMoves().empty();
}
bool ChessBoard::hasInsufficientMaterial() const {
    // is there anything other than a king, bishop, or knight?
    int numBishops = 0;
    int numKnights = 0;
    for (size_t row = 0; row<BOARD_SIZE; ++row) {
        for (size_t col = 0; col<BOARD_SIZE; ++col) {
            const PiecePtr piece = pieces[row][col];
            if (!piece) continue;
            char pieceTypeT = toupper(piece->symbol());
            switch (pieceTypeT) {
                case 'K': continue;
                case 'B': ++numBishops;
                case 'N': ++numKnights;
                default: return false;
            }
        }
    }
    return (numBishops <= 1) && (numKnights <= 1);
}
GameStatus ChessBoard::getStatus() const {
    // return the status of the game (whether white won, black won, it's a draw, or game is still going on)
    if (isInCheckmate()) { // the game is over, checkmate
        // is it black's turn? then white won
        if (whiteToMove) {
            return GameStatus::BLACK_WON;
        } else {
            return GameStatus::WHITE_WON;
        }
    } else if (isInStalemate()) {
        return GameStatus::DRAW;
    } else if (halfmove_clock >= 100) {
        return GameStatus::DRAW;
    } else if (this->hasInsufficientMaterial()) {
        return GameStatus::DRAW;
    } else {
        if (whiteToMove) {
            return GameStatus::STILL_GOING_WHITE_TURN;
        } else {
            return GameStatus::STILL_GOING_BLACK_TURN;
        }
    }
}

// for engine use
std::string ChessBoard::fen() const {
    // to be done by Samuel
    throw NotImplementedError("std::string ChessBoard::fen() is not yet implemented");
}

std::set<Move> ChessBoard::allLegalMoves(const Square sq) const {
    // this method may need to be shared
    // to be done by Andrew
    // get all legal moves from the piece at the square indicated 
    // if there is no piece at that square, or if the piece at that square is owned by the opponent, return empty set
    throw NotImplementedError("std::set<Move> ChessBoard::allLegalMoves() is not yet implemented");
}
std::set<Move> ChessBoard::allLegalMoves() const {
    // done by Samuel
    // return all legal moves from all pieces that the player owns
    // this function is necessary for the engine
    // it can call allLegalMoves for every piece it owns and splice them together into one set, then return that set
    std::set<Move> legalMoves;
    for (size_t row = 0; row < BOARD_SIZE; ++row) {
        for (size_t col = 0; col<BOARD_SIZE; ++col) {
            std::set<Move> movesFromSquare = allLegalMoves(Square(row+1, col+1));
            for (Move move : movesFromSquare) {
                legalMoves.insert(move);
            }
        }
    }
    return legalMoves;
}

std::ostream& operator<<(std::ostream& os, const ChessBoard& board) {
	for (const std::vector<std::shared_ptr<Piece>>& pieces_row: board.pieces){
		for (const std::shared_ptr<Piece>& piece_ptr: pieces_row){
			if (piece_ptr == nullptr){
				os << ' ';
			}
			else{
				os << piece_ptr->symbol();
			}
		}
		os << endl;
	}
	return os;
}
