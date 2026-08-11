#include "ChessBoard.hpp"
#include "Errors.hpp"
#include "FEN.hpp"

#include <cctype>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include <optional>
using namespace std;


vector<Square> getPawnDirs(int direction) {
    vector<Square> pawnDirs {Square(direction, -1), Square(direction, 1)};
    return pawnDirs;
}

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
    const std::vector<std::vector<std::shared_ptr<const Piece>>>& pieces, 
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
	if (this->isMoveLegal(m)){
		// update fullmove_clock
		this->fullmove_clock += (!this->whiteToMove);
        
		// TODO: update halfmove_clock
		shared_ptr<const Piece>& start_ptr = this->pieces.at(m.startingSquare.row).at(m.endingSquare.col);
		shared_ptr<const Piece>& end_ptr = this->pieces.at(m.endingSquare.row).at(m.endingSquare.col);
		bool isCapture = (end_ptr != nullptr);
		bool isPawnMove = (end_ptr != nullptr) && ((end_ptr->symbol() == 'p') || (end_ptr->symbol() == 'P'));
		this->halfmove_clock += (isCapture || isPawnMove);

		// TODO: modify `Move` type to include castling as an option
		// TODO: update whitePlayerState, blackPlayerState
		// TODO: update enPassant_targetSquare

		// move the piece!
		end_ptr = std::move(start_ptr);
	}
}


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

const Square kingOffsets[] = {
    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1,-1}, {1,0}, {1, 1}
};

PiecePtr ChessBoard::getAndAssertPiece(const Square origin, const char pieceType) const {
    // get the piece at origin, and assert it is of tpe pieceTzype
    PiecePtr ptr = getPiece(origin);
    if (!ptr || toupper(ptr->symbol()) != toupper(pieceType)) throw WrongPieceType("Wrong piece type at origin");
    return ptr;
}
std::set<Square> ChessBoard::whereKingCouldMove(const Square origin) const {
    // get al neighboring places
    PiecePtr king = getAndAssertPiece(origin, 'K');
    bool attackerIsWhite = king->getBelongsToWhite();

    std::set<Square> places;
    for (const Square& off : kingOffsets) {
        Square target = origin + off;
        if (!target.isValid()) continue;
        // if there is a piece of a different color, then it's okay
        // else not
        PiecePtr p = getPiece(target);
        if ((!p) || (p && p->getBelongsToWhite() != attackerIsWhite)) places.insert(target);
    }
    return places;
}

std::set<Square> ChessBoard::wherePawnCouldMove(const Square origin) const {
    PiecePtr pawn = getAndAssertPiece(origin, 'K');
    bool attackerIsWhite = pawn->getBelongsToWhite();
    int direction = attackerIsWhite ? 1 : -1; // pawns move up if white, down if black
    size_t canMoveTwoSpaces = ((attackerIsWhite) ? ((origin.row == 2) ? 1 : 0) : ((origin.row==7) ? 1 : 0));
    // check 2 capturing pieces
    std::set<Square> places;
    for (const Square& off : getPawnDirs(direction)) {
        Square target = origin + off;
        if (!target.isValid()) continue;
        // if there is a piece of a different color, then it's okay
        // else not
        PiecePtr p = getPiece(target);
        if ((p && p->getBelongsToWhite() != attackerIsWhite)) places.insert(target);
    }
    // consider the spaces it can move
    for (size_t i = 1; i<=(1+canMoveTwoSpaces); ++i) {
        Square target = origin + Square(i*direction, 0);
        if (!target.isValid()) continue;
        PiecePtr p = getPiece(target);
        if (p) break;
        else {places.insert(target);}
    }
    return places;
}
static const Square knightOffsets[] = {
    {1, 2}, {2, 1}, {-1, 2}, {-2, 1}, {1, -2}, {2, -1}, {-1, -2}, {-2, -1}
};

std::set<Square> ChessBoard::whereKnightCouldMove(const Square origin) const {
    PiecePtr knight = getAndAssertPiece(origin, 'N');
    bool attackerIsWhite = knight->getBelongsToWhite();

    std::set<Square> places;
    for (const Square& off : knightOffsets) {
        Square target = origin + off;
        if (!target.isValid()) continue;
        // if there is a piece of a different color, then it's okay
        // else not
        PiecePtr p = getPiece(target);
        if ((!p) || (p && p->getBelongsToWhite() != attackerIsWhite)) places.insert(target);
    }
    return places;
}
// get all pieces an attacker with the ray (dir) starting at from, who his of color belongsToWhite, could reach legally (without considering check)
std::set<Square> ChessBoard::isSlidingAttacker(const Square from, const Square dir, bool attackerIsWhite) const {
    Square cur = from + dir;
    std::set<Square> places;
    while (cur.isValid()) {
        PiecePtr p = getPiece(cur);
        if (p) {
            if (p->getBelongsToWhite() != attackerIsWhite) {
                // not of same color, so can capture!
                places.insert(cur);
            }
            break; // occupied, so the ray is blocked past here regardless
        }
        places.insert(cur);
        cur = cur + dir;
        
    }
    return places;
}


std::set<Square> ChessBoard::whereBishopCouldMove(const Square origin) const {
    PiecePtr bishop = getAndAssertPiece(origin, 'B');
    bool attackerIsWhite = bishop->getBelongsToWhite();
    std::set<Square> places;
    for (Square dir : {Square(-1, -1), Square(-1, 1), Square(1, -1), Square(1,1)}) {
        mergeSets(places, isSlidingAttacker(origin, dir, attackerIsWhite));
    }
    return places;
}
std::set<Square> ChessBoard::whereRookCouldMove(const Square origin) const {
    PiecePtr rook = getAndAssertPiece(origin, 'R');
    bool attackerIsWhite = rook->getBelongsToWhite();
    std::set<Square> places;
    for (Square dir : {Square(-1, 0), Square(1, 0), Square(0, -1), Square(0,1)}) {
        mergeSets(places, isSlidingAttacker(origin, dir, attackerIsWhite));
    }
    return places;
}
std::set<Square> ChessBoard::whereQueenCouldMove(const Square origin) const {
    PiecePtr queen = getAndAssertPiece(origin, 'R');
    bool attackerIsWhite = queen->getBelongsToWhite();
    std::set<Square> places;
    for (Square dir : {Square(-1, 0), Square(1, 0), Square(0, -1), Square(0,1), Square(-1, -1), Square(-1, 1), Square(1, -1), Square(1,1)}) {
        mergeSets(places, isSlidingAttacker(origin, dir, attackerIsWhite));
    }
    return places;
}


std::set<Square> ChessBoard::allPseudoLegalDestinations(const Square origin) const {
    PiecePtr piece = getPiece(origin);
    std::set<Square> moves;
    if (!piece) return moves; // no piece there
    if (piece->getBelongsToWhite() != whiteToMove) return moves; // can't move the piece, is of wrong color
    switch (toupper(piece->symbol())) {
        case 'R': return whereRookCouldMove(origin);
        case 'B': return whereBishopCouldMove(origin);
        case 'Q': return whereQueenCouldMove(origin);
        case 'N': return whereKnightCouldMove(origin);
        case 'P': return wherePawnCouldMove(origin);
        case 'K': return whereKingCouldMove(origin);
        default: throw UnknownPiece("unknown piece");
    }

}
// Is `target` attacked by any piece belonging to `attackerIsWhite`?
// This is a raw-attack check (used to detect check): it only asks
// "could this piece capture on `target` right now", not whether doing
// so would be a legal move for the attacker.
bool squareAttackedBy(const ChessBoard& board, Square target, bool attackerIsWhite) {
    // get all the psuedo-legal moves
    // and check if they would end on the target

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
    std::string PiecePart = ParsePieces::getPiecePart(pieces);
    std::string PlayerPart = ParsePieces::getPlayerPart(whiteToMove);
    std::string CastlingPart = ParsePieces::getCastlingPart(whitePlayerState, blackPlayerState);
    std::string enPassantPart = ParsePieces::getEnPassantPart(enPassant_targetSquare);
    std::string halfMovePart = std::to_string(halfmove_clock);
    std::string fullMovePart = std::to_string(fullmove_clock);
    return PiecePart + " " + PlayerPart + " " + CastlingPart + " " + enPassantPart + " " + halfMovePart + " " + fullMovePart;
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
	for (const std::vector<std::shared_ptr<const Piece>>& pieces_row: board.pieces){
		for (const std::shared_ptr<const Piece>& piece_ptr: pieces_row){
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

ChessBoard ChessBoard::board_with_move(const Move& move) const {
    ChessBoard newBoard = (*this);
    newBoard.processMove(move);
    return newBoard;
}
bool ChessBoard::move_ends_game(const Move move) const {
    ChessBoard newBoard = this->board_with_move(move);
    return isGameOver(newBoard.getStatus());
}
bool ChessBoard::move_is_castling(const Move move) const {
    PiecePtr pieceAtBeginning = this->getPiece(move.startingSquare);
    if (!pieceAtBeginning || toupper(pieceAtBeginning->symbol()) != 'K') return false;
    Square diff = move.endingSquare - move.startingSquare;
    return max(abs(diff.row), abs(diff.col)) > 1;
}
bool ChessBoard::move_is_check(const Move move) const {
    ChessBoard newBoard = this->board_with_move(move);
    return newBoard.isInCheck(true) || newBoard.isInCheck(false);
}
bool ChessBoard::move_is_capture(const Move move) const {
    return static_cast<bool>(this->getPiece(move.endingSquare));
}
bool ChessBoard::move_is_zeroing(const Move move) const {
    if (this->move_is_capture(move)) return true;
    PiecePtr pieceAtBeginning = this->getPiece(move.startingSquare);
    return (static_cast<bool>(pieceAtBeginning) && toupper(pieceAtBeginning->symbol()) == 'P');
}
