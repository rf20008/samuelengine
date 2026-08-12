#include "ChessBoard.hpp"
#include "Errors.hpp"
#include "FEN.hpp"

#include <cctype>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <sstream>
using namespace std;

vector<Square> getPawnDirs(int direction) {
	// Square.row is the file, Square.col is the rank, and pawns capture
	// diagonally: one file over (row), one rank forward (col, by direction).
	vector<Square> pawnDirs{Square(-1, direction), Square(1, direction)};
	return pawnDirs;
}

ChessBoard::ChessBoard() : ChessBoard::ChessBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}

ChessBoard::ChessBoard(const std::string &fen) {
	(void)fen;
	std::istringstream fenSS(fen);
	std::string PiecePart;
	std::string PlayerPart;
	std::string CastlingPart;
	std::string EnPassantPart;
	int Halfmove_Part;
	int Fullmove_part;
	fenSS >> PiecePart >> PlayerPart >> CastlingPart >> EnPassantPart >> Halfmove_Part >> Fullmove_part;
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

ChessBoard::ChessBoard(const std::vector<std::vector<std::shared_ptr<const Piece>>> &pieces, const bool &whiteToMove, const PlayerState &whitePlayerState, const PlayerState &blackPlayerState, const int &halfmove_clock, const int &fullmove_clock, const Square &enPassant_targetSquare) : pieces(pieces), whiteToMove(whiteToMove), whitePlayerState(whitePlayerState), blackPlayerState(blackPlayerState), halfmove_clock(halfmove_clock), fullmove_clock(fullmove_clock), enPassant_targetSquare(enPassant_targetSquare) {}

std::shared_ptr<const Piece> ChessBoard::getPiece(Square sq) const {
	// bounds check: an out-of-board square simply has no piece on it
	if (!sq.isValid()) {
		return nullptr;
	}
	// Square rows/cols are 1-indexed (see Square::isValid), pieces is 0-indexed.
	// pieces is stored rank-major (see ParsePieces::parsePiecePart), while
	// Square.row is the file and Square.col is the rank -- so the rank
	// (col) is the outer index and the file (row) is the inner one.
	return pieces[sq.col - 1][sq.row - 1];
}

bool ChessBoard::isMoveLegal(Move m) const {
	// A move is legal exactly when its destination shows up among the legal
	// destinations for whichever piece sits on the move's starting square --
	// allLegalMoves(sq) does the real work of applying piece movement rules
	// and rejecting anything that would leave our own king in check.
	std::set<Move> legalMovesFromStart = allLegalMoves(m.startingSquare);
	return legalMovesFromStart.count(m) > 0;
} // return whether a move is legal

void ChessBoard::processEnPassant(Move m, const PiecePtr& start_ptr, const PiecePtr& end_ptr) {
    if (m.endingSquare == enPassant_targetSquare && toupper(start_ptr->symbol()) == 'P' && !end_ptr) {
        Square squareCaptured = m.endingSquare + ((start_ptr->symbol() == 'P') ? Square(0, -1) : Square(0, 1));
        //cout<<"removing piece at"<<(squareCaptured.toString())<<endl;
        this->pieces.at(squareCaptured.col - 1).at(squareCaptured.row - 1) = PiecePtr();
    }
    // update enPassantTargetSquare
    //cout<<"isPawnMove: "<<isPawnMove<<endl;
    //if (enPassant_targetSquare) cout<<"enPassant target Square: "<< (enPassant_targetSquare->toString())<<endl;
    if (maxNorm(m.endingSquare - m.startingSquare) > 1) {
        //cout<<"a Pawn moved 2 squares\n";
        enPassant_targetSquare = std::optional<Square>(m.startingSquare + ((start_ptr->symbol() == 'P') ? Square(0, 1) : Square(0, -1)));
    } else {
        enPassant_targetSquare = std::optional<Square>();
    }
}
void ChessBoard::processCastling(Move m, const PiecePtr& start_ptr) {
    // check it's a king move, else do nothing
    if (!start_ptr || toupper(start_ptr->symbol()) != 'K') return;
    // need to move rook to appropriate location
    if (maxNorm(m.endingSquare-m.startingSquare)<=1) return; // this was a normal king move, not castling
    Square oldRookPos;
    Square newRookPos;
    if (m == Move("e1", "g1")) {oldRookPos="h1"; newRookPos="f1";} // white kingside castling
    if (m == Move("e1", "c1")) {oldRookPos="a1"; newRookPos="d1";} // white queenside castling
    if (m == Move("e8", "g8")) {oldRookPos="h8"; newRookPos="f8";} // black kingside castling
    if (m == Move("e8", "c8")) {oldRookPos="a8"; newRookPos="d8";} // black queenside castling
    // sanity check
    // assert a rook at oldRookPos, and newRookPOs is empty
    PiecePtr& oldRook = pieces.at(oldRookPos.col-1).at(oldRookPos.row-1);
    PiecePtr& newRook = pieces.at(newRookPos.col-1).at(newRookPos.row-1);
    if (!oldRook || toupper(oldRook->symbol()) != 'R') {
        throw WrongPieceType("Expected a rook at " + oldRookPos.toString() + " that was not found");
    } 
    if (newRook) {
        throw WrongPieceType("Expected newRookPos to be empty, but found " + std::string(1, newRook->symbol()) + " instead");
    }
    newRook = std::move(oldRook);
    return;

}
void ChessBoard::processMove(Move m) {
	if (this->isMoveLegal(m)) {
		// update fullmove_clock (increments once Black's move completes a full move pair)
		this->fullmove_clock += (!this->whiteToMove);

		// Square rows/cols are 1-indexed, pieces is 0-indexed and rank-major (see getPiece)
		shared_ptr<const Piece> &start_ptr = this->pieces.at(m.startingSquare.col - 1).at(m.startingSquare.row - 1);
		shared_ptr<const Piece> &end_ptr = this->pieces.at(m.endingSquare.col - 1).at(m.endingSquare.row - 1);
		bool isCapture = (end_ptr != nullptr);
		bool isPawnMove = (start_ptr != nullptr) && ((start_ptr->symbol() == 'p') || (start_ptr->symbol() == 'P'));
		// the halfmove clock counts moves since the last capture/pawn move (for the 50-move rule), so it resets on either
		this->halfmove_clock = (isCapture || isPawnMove) ? 0 : (this->halfmove_clock + 1);

		// TODO: modify `Move` type to include castling as an option

		processCastling(m, start_ptr);

		// TODO: update whitePlayerState, blackPlayerState

        PlayerState cur_state = whiteToMove?whitePlayerState:blackPlayerState;
        
        // if king moved, both are gone
        if (toupper(start_ptr -> symbol()) == 'K') cur_state = PlayerState(false, false);
        
        // if rook on a file moved, queenside is gone
        if (toupper(start_ptr->symbol()) == 'R' && m.startingSquare==Square(whiteToMove ? 1 : 8, 1)) cur_state.canQueensideCastle=false;
        // if rook on h file moved, kingside is gone
        if (toupper(start_ptr->symbol()) == 'R' && m.startingSquare==Square(whiteToMove ? 1 : 8, 8)) cur_state.canKingsideCastle=false;

        if (whiteToMove) {
            whitePlayerState=cur_state;
        } else {
            blackPlayerState=cur_state;
        }
        //if was an enpassant capture, must remove the pawn it en-passanted
		
        if (isPawnMove) processEnPassant(m, start_ptr, end_ptr);
		end_ptr = std::move(start_ptr);
		this->whiteToMove = !this->whiteToMove;
	} else {
        throw IllegalMoveError("You have attempted an illegal move from " + m.startingSquare.toString() + " to " + m.endingSquare.toString() + ".");
    }
}

// Is there a piece belonging to `attackerIsWhite` on the far end of the
// ray starting at `from` and stepping by `dir` (one step at a time) that
// could capture along that ray -- i.e. a rook/queen on a rank/file ray,
// or a bishop/queen on a diagonal ray? The ray stops at the first
// occupied square either way (that piece blocks anything behind it).
bool ChessBoard::isSlidingAttacker(Square from, Square dir, bool attackerIsWhite, char pieceLetterA, char pieceLetterB) const {
	Square cur = from + dir;
	while (cur.isValid()) {
		std::shared_ptr<const Piece> p = getPiece(cur);
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

const Square kingOffsets[] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

PiecePtr ChessBoard::getAndAssertPiece(const Square origin, const char pieceType) const {
	// get the piece at origin, and assert it is of tpe pieceTzype
	PiecePtr ptr = getPiece(origin);
	if (!ptr || toupper(ptr->symbol()) != toupper(pieceType))
		throw WrongPieceType("Wrong piece type at origin");
	return ptr;
}
bool ChessBoard::hasPiece(const Square origin) const {
    return !getPiece(origin);

}
std::set<Square> ChessBoard::whereKingCouldMove(const Square origin) const {
	// get al neighboring places
	PiecePtr king = getAndAssertPiece(origin, 'K');
	bool attackerIsWhite = king->getBelongsToWhite();

	std::set<Square> places;
	for (const Square &off : kingOffsets) {
		Square target = origin + off;
		if (!target.isValid())
			continue;
		// if there is a piece of a different color, then it's okay
		// else not
		PiecePtr p = getPiece(target);
		if ((!p) || (p && p->getBelongsToWhite() != attackerIsWhite))
			places.insert(target);
	}
    // check for castling
    if (whiteToMove) {
        // if white can kingside castle, and f1 and g1 are open
        if (whitePlayerState.canKingsideCastle && !hasPiece("f1") && !hasPiece("g1")) {
            if (!squareAttackedBy("f1", false)) places.insert(Square("g1"));
        } 
        // check queenside castling
        else if (whitePlayerState.canQueensideCastle && !hasPiece("c1") && !hasPiece("d1")) {
            if (!squareAttackedBy("g1", false)) places.insert(Square("c1"));
        }
    }
    else { // can black castle
        // check kingside castling
        if (blackPlayerState.canKingsideCastle && !hasPiece("f8") && !hasPiece("g8")) {
            if (!squareAttackedBy("f8", true)) places.insert(Square("g8"));
        }
        // check queenside castling
        if (blackPlayerState.canKingsideCastle && !hasPiece("c8") && !hasPiece("d8")) {
            if (!squareAttackedBy("d8", true)) places.insert(Square("c8"));
        }
    }
	return places;
}

std::set<Square> ChessBoard::wherePawnCouldMove(const Square origin) const {
	PiecePtr pawn = getAndAssertPiece(origin, 'P');
	bool attackerIsWhite = pawn->getBelongsToWhite();
	int direction = attackerIsWhite ? 1 : -1; // pawns move up if white, down if black
	// origin.col is the rank: white's pawns start on rank 2, black's on rank 7.
	size_t canMoveTwoSpaces = ((attackerIsWhite) ? ((origin.col == 2) ? 1 : 0) : ((origin.col == 7) ? 1 : 0));
	// check 2 capturing pieces
	std::set<Square> places;
	for (const Square &off : getPawnDirs(direction)) {
		Square target = origin + off;
		if (!target.isValid())
			continue;
		// if there is a piece of a different color, then it's okay
		// else not
		PiecePtr p = getPiece(target);
		if ((p && p->getBelongsToWhite() != attackerIsWhite) || (enPassant_targetSquare && target == *enPassant_targetSquare))
			places.insert(target); // it's enPassant!!
	}
	// consider the spaces it can move
	for (size_t i = 1; i <= (1 + canMoveTwoSpaces); ++i) {
		Square target = origin + Square(0, i * direction);
		if (!target.isValid())
			continue;
		PiecePtr p = getPiece(target);
		if (p)
			break;
		else {
			places.insert(target);
		}
	}
	return places;
}
static const Square knightOffsets[] = {{1, 2}, {2, 1}, {-1, 2}, {-2, 1}, {1, -2}, {2, -1}, {-1, -2}, {-2, -1}};

std::set<Square> ChessBoard::whereKnightCouldMove(const Square origin) const {
	PiecePtr knight = getAndAssertPiece(origin, 'N');
	bool attackerIsWhite = knight->getBelongsToWhite();

	std::set<Square> places;
	for (const Square &off : knightOffsets) {
		Square target = origin + off;
		if (!target.isValid())
			continue;
		// if there is a piece of a different color, then it's okay
		// else not
		PiecePtr p = getPiece(target);
		if ((!p) || (p && p->getBelongsToWhite() != attackerIsWhite))
			places.insert(target);
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
	for (Square dir : {Square(-1, -1), Square(-1, 1), Square(1, -1), Square(1, 1)}) {
		mergeSets(places, isSlidingAttacker(origin, dir, attackerIsWhite));
	}
	return places;
}
std::set<Square> ChessBoard::whereRookCouldMove(const Square origin) const {
	PiecePtr rook = getAndAssertPiece(origin, 'R');
	bool attackerIsWhite = rook->getBelongsToWhite();
	std::set<Square> places;
	for (Square dir : {Square(-1, 0), Square(1, 0), Square(0, -1), Square(0, 1)}) {
		mergeSets(places, isSlidingAttacker(origin, dir, attackerIsWhite));
	}
	return places;
}
std::set<Square> ChessBoard::whereQueenCouldMove(const Square origin) const {
	PiecePtr queen = getAndAssertPiece(origin, 'Q');
	bool attackerIsWhite = queen->getBelongsToWhite();
	std::set<Square> places;
	for (Square dir : {Square(-1, 0), Square(1, 0), Square(0, -1), Square(0, 1), Square(-1, -1), Square(-1, 1), Square(1, -1), Square(1, 1)}) {
		mergeSets(places, isSlidingAttacker(origin, dir, attackerIsWhite));
	}
	return places;
}

std::set<Square> ChessBoard::allPseudoLegalDestinations(const Square origin) const {
	PiecePtr piece = getPiece(origin);
	std::set<Square> moves;
	if (!piece)
		return moves; // no piece there
	if (piece->getBelongsToWhite() != whiteToMove)
		return moves; // can't move the piece, is of wrong color
	switch (toupper(piece->symbol())) {
	case 'R':
		return whereRookCouldMove(origin);
	case 'B':
		return whereBishopCouldMove(origin);
	case 'Q':
		return whereQueenCouldMove(origin);
	case 'N':
		return whereKnightCouldMove(origin);
	case 'P':
		return wherePawnCouldMove(origin);
	case 'K':
		return whereKingCouldMove(origin);
	default:
		throw UnknownPiece("unknown piece");
	}
}

// Is `target` attacked by any piece belonging to `attackerIsWhite`?
// This is a raw-attack check (used to detect check): it only asks
// "could this piece capture on `target` right now", not whether doing
// so would be a legal move for the attacker.
bool ChessBoard::squareAttackedBy(Square target, bool attackerIsWhite) const {
	// get all the psuedo-legal moves
	// and check if they would end on the target

	static const Square knightOffsets[] = {{1, 2}, {2, 1}, {-1, 2}, {-2, 1}, {1, -2}, {2, -1}, {-1, -2}, {-2, -1}};
	for (const Square &off : knightOffsets) {
		Square sq = target + off;
		if (!sq.isValid())
			continue;
		std::shared_ptr<const Piece> p = getPiece(sq);
		if (p && p->getBelongsToWhite() == attackerIsWhite && std::toupper(p->symbol()) == 'N') {
			return true;
		}
	}

	for (int dr = -1; dr <= 1; ++dr) {
		for (int dc = -1; dc <= 1; ++dc) {
			if (dr == 0 && dc == 0)
				continue;
			Square sq = target + Square(dr, dc);
			if (!sq.isValid())
				continue;
			std::shared_ptr<const Piece> p = getPiece(sq);
			if (p && p->getBelongsToWhite() == attackerIsWhite && std::toupper(p->symbol()) == 'K') {
				return true;
			}
		}
	}

	// A pawn attacks diagonally, one rank "ahead" of where it sits (from
	// its own side's perspective): white pawns advance toward higher
	// ranks, so an attacking white pawn sits one rank *below* the target.
	// Square.row is the file, Square.col is the rank.
	int behind = attackerIsWhite ? -1 : 1;
	for (int dc : {-1, 1}) {
		Square sq = target + Square(dc, behind);
		if (!sq.isValid())
			continue;
		std::shared_ptr<const Piece> p = getPiece(sq);
		if (p && p->getBelongsToWhite() == attackerIsWhite && std::toupper(p->symbol()) == 'P') {
			return true;
		}
	}

	static const Square rookDirs[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
	for (const Square &dir : rookDirs) {
		if (isSlidingAttacker(target, dir, attackerIsWhite, 'R', 'Q')) {
			return true;
		}
	}

	static const Square bishopDirs[] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
	for (const Square &dir : bishopDirs) {
		if (isSlidingAttacker(target, dir, attackerIsWhite, 'B', 'Q')) {
			return true;
		}
	}

	return false;
}

Square ChessBoard::findKing(bool belongsToWhite) const {
	for (int r = 0; r < BOARD_SIZE; ++r) {
		for (int c = 0; c < BOARD_SIZE; ++c) {
			std::shared_ptr<const Piece> p = pieces[c][r];
			if (p && p->getBelongsToWhite() == belongsToWhite && std::toupper(p->symbol()) == 'K') {
				return {r + 1, c + 1};
			}
		}
	}
	throw std::logic_error("findKing: no king found for the requested player");
}

bool ChessBoard::isInCheck(bool player) const {
	// can the player whose turn it is, capture the king who is owned by Player?
	Square kingSquare = this->findKing(player);
	return this->squareAttackedBy(kingSquare, !player);
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
	for (size_t row = 0; row < BOARD_SIZE; ++row) {
		for (size_t col = 0; col < BOARD_SIZE; ++col) {
			const PiecePtr piece = pieces[row][col];
			if (!piece)
				continue;
			char pieceTypeT = toupper(piece->symbol());
			switch (pieceTypeT) {
			case 'K':
				continue;
			case 'B':
				++numBishops;
			case 'N':
				++numKnights;
			default:
				return false;
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
	// get all legal moves from the piece at the square indicated
	// if there is no piece at that square, or if the piece at that square is owned by the opponent, return empty set
	std::set<Move> legalMoves;
	std::set<Square> pseudoLegalDestinations = allPseudoLegalDestinations(sq);
	if (pseudoLegalDestinations.empty()) {
		return legalMoves; // no piece there, or it belongs to the opponent
	}

	bool moverIsWhite = getPiece(sq)->getBelongsToWhite();

	for (const Square &dest : pseudoLegalDestinations) {
		Move candidate{sq, dest};

		// Simulate the move on a scratch copy of the board -- deliberately
		// *not* going through processMove/isMoveLegal here, since that
		// would call right back into allLegalMoves and recurse forever --
		// then reject the move if it would leave our own king in check.
		std::vector<std::vector<PiecePtr>> hypothetical = pieces;
		hypothetical[dest.col - 1][dest.row - 1] = hypothetical[sq.col - 1][sq.row - 1];
		hypothetical[sq.col - 1][sq.row - 1] = nullptr;

		ChessBoard hypotheticalBoard(hypothetical, whiteToMove, whitePlayerState, blackPlayerState, halfmove_clock, fullmove_clock, enPassant_targetSquare.value_or(Square()));
		if (!hypotheticalBoard.isInCheck(moverIsWhite)) {
			legalMoves.insert(candidate);
		}
	}
	return legalMoves;
}
std::set<Move> ChessBoard::allLegalMoves() const {
	// done by Samuel
	// return all legal moves from all pieces that the player owns
	// this function is necessary for the engine
	// it can call allLegalMoves for every piece it owns and splice them together into one set, then return that set
	std::set<Move> legalMoves;
	for (size_t row = 0; row < BOARD_SIZE; ++row) {
		for (size_t col = 0; col < BOARD_SIZE; ++col) {
			std::set<Move> movesFromSquare = allLegalMoves(Square(row + 1, col + 1));
			for (Move move : movesFromSquare) {
				legalMoves.insert(move);
			}
		}
	}
	return legalMoves;
}

std::ostream &operator<<(std::ostream &os, const ChessBoard &board) {
	for (const std::vector<std::shared_ptr<const Piece>> &pieces_row : board.pieces) {
		for (const std::shared_ptr<const Piece> &piece_ptr : pieces_row) {
			if (piece_ptr == nullptr) {
				os << ' ';
			} else {
				os << piece_ptr->symbol();
			}
		}
		os << endl;
	}
	return os;
}

ChessBoard ChessBoard::board_with_move(const Move &move) const {
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
	if (!pieceAtBeginning || toupper(pieceAtBeginning->symbol()) != 'K')
		return false;
	Square diff = move.endingSquare - move.startingSquare;
	return max(abs(diff.row), abs(diff.col)) > 1;
}
bool ChessBoard::move_is_check(const Move move) const {
	ChessBoard newBoard = this->board_with_move(move);
	return newBoard.isInCheck(!whiteToMove);
}
bool ChessBoard::move_is_capture(const Move move) const { return static_cast<bool>(this->getPiece(move.endingSquare)); }
bool ChessBoard::move_is_zeroing(const Move move) const {
	if (this->move_is_capture(move))
		return true;
	PiecePtr pieceAtBeginning = this->getPiece(move.startingSquare);
	return (static_cast<bool>(pieceAtBeginning) && toupper(pieceAtBeginning->symbol()) == 'P');
}

// this function exists for testing purposes
int ChessBoard::perft(int depth) const {
    if (depth==0) return 1;
    int perft_res = 0;
    std::set<Move> moves = this->allLegalMoves();
    if (depth==1) return moves.size();
    for (Move m : moves) {
        perft_res += this->board_with_move(m).perft(depth-1);
    }
    return perft_res;
}