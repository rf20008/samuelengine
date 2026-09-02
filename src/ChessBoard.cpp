#include "ChessBoard.hpp"
#include "Errors.hpp"
#include "FEN.hpp"
#include "Ensure.hpp"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <sstream>
using namespace std;

constexpr int NORTH = 16;
constexpr int SOUTH = -16;
constexpr int EAST = 1;
constexpr int WEST = -1;
constexpr int NE = 17;
constexpr int NW = 15;
constexpr int SE = -15;
constexpr int SW = -17;
constexpr int rPr = pieceNum('R');
inline constexpr int PAWN_CAPTURE_DIRS[2][2] = {
	{NW, NE}, // White: rank+1, file+-1
	{SW, SE}  // Black: rank-1, file+-1
};

constexpr int kingOffsets[] = {NORTH, SOUTH, EAST, WEST, NE, NW, SE, SW};
constexpr int rookOffsets[] = {NORTH, SOUTH, EAST, WEST};
constexpr int bishopOffsets[] = {NE, NW, SE, SW};
constexpr int queenOffsets[] = {NORTH, SOUTH, EAST, WEST, NE, NW, SE, SW};
constexpr std::string knownPieceTypeChars = "KQRBN";
constexpr int knightOffsets[] = {33, 31, 18, 14, -14, -18, -31, -33}; // 2N+E etc

ChessBoard::ChessBoard() : ChessBoard::ChessBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}

ChessBoard::ChessBoard(const std::string &fen) {
	(void)fen;
    this->zobrist_hash = 0;
	std::istringstream fenSS(fen);
	std::string PiecePart;
	std::string PlayerPart;
	std::string CastlingPart;
	std::string EnPassantPart;
	for (int i = 0; i < 128; i++)
        pieces[i] = EMPTY_SQUARE;
	int Halfmove_Part;
	int Fullmove_part;
	fenSS >> PiecePart >> PlayerPart >> CastlingPart >> EnPassantPart >> Halfmove_Part >> Fullmove_part;
	if (fenSS.fail()) {
		throw std::invalid_argument("Not enough arguments to construct fen (need 7)");
	}
	ParsePieces::parsePiecePart(PiecePart, this->pieces);
	this->playerToMove = ParsePieces::parsePlayerPart(PlayerPart);
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
    this->zobrist_hash = this->zobristFromScratch();

    // find Kings and validate them
    int numWhiteKings = 0;
    int numBlackKings = 0;
    for (int sq64 = 0; sq64<64; ++sq64) {
        Square sq = Square::from64(sq64);  
        Piece piece = this->pieces[sq.idx];
        if (piece.type == PieceType::KING) { // it's a king
            if (piece.color == Color::WHITE) {
                ++numWhiteKings; whiteKingPos = sq;
            } else {
                ++numBlackKings; blackKingPos = sq;
            }
        }
    }
    if (numWhiteKings != 1 || numBlackKings != 1) {
        throw std::logic_error("Invalid chess position. Expected 1 king of each color but found " + std::to_string(numWhiteKings) + " white kings and " + std::to_string(numBlackKings) + " black kings. FEN: " + fen);
    }
}


std::vector<Move> ChessBoard::getAllMovesFromPieceEndingAt(PieceType expectedType, Square expectedEndingSquare) {
    std::vector<Move> candidateMoves;
    for (int sqnum = 0; sqnum<64; ++sqnum) {
        // does the piece belong to the current player
        // and is it the rightt ype
        Square beginningSquare = Square::from64(sqnum);
        Piece p = getPiece(Square::from64(sqnum));
        if (p.color != playerToMove) continue; // wrong color
        if (p.type != expectedType) continue;

        std::vector<Move> candidateMovesFromSquare = allLegalMoves(beginningSquare);
        for (const Move & candidateMove : candidateMovesFromSquare) {
            if (candidateMove.endingSquare == expectedEndingSquare) 
                candidateMoves.push_back(candidateMove);
        }
        // and filter out al moves to see if it ends at the desired spot
    }
    return candidateMoves;
}


std::vector<Move> ChessBoard::getSANRegular(PieceType expectedType, Square expectedEndingSquare, const std::string ambiguators) {

    return ambiguateMove(getAllMovesFromPieceEndingAt(expectedType, expectedEndingSquare), expectedEndingSquare, ambiguators);

}
std::vector<Move> ChessBoard::ambiguateMove(std::vector<Move> candidateMoves, Square expectedEndingSquare, const std::string ambiguators) const {
    for (char ambiguatorChar : ambiguators) {
        // is it a file or a rank ambiguator
        bool isFileAmbiguator = (ambiguatorChar >= 'a') && (ambiguatorChar <= 'h');

        bool isRankAmbiguator = (ambiguatorChar >= '1') && (ambiguatorChar <= '8');
        // exception: x = capture on the square
        bool isCaptureAmbiguator = (ambiguatorChar == 'x');
        if (isCaptureAmbiguator) {
            if (getPiece(expectedEndingSquare).isEmpty()) {
                throw InvalidSAN("Expected capture, but empty square found");
            } else {
                continue;
            }
        } 
        if (!isFileAmbiguator && !isRankAmbiguator) {
            throw InvalidSAN("Invalid ambiguator: " + std::string(1, ambiguatorChar));
        }
        assert(!(isFileAmbiguator && isRankAmbiguator));
        int ambiguator = (isFileAmbiguator ? ambiguatorChar - '1' : ambiguatorChar - 'a');
        std::vector<Move> newCandidates;
        for (Move candidateMove : candidateMoves) {
            // if it's file, must match file
            if (isFileAmbiguator && candidateMove.startingSquare.file() == ambiguator) {
                newCandidates.push_back(candidateMove);
            } else if (isRankAmbiguator && candidateMove.startingSquare.rank() == ambiguator) {
                newCandidates.push_back(candidateMove);
            }
        }
        candidateMoves = newCandidates;
    }

    // make sure is unambiguous
    /*if (candidateMoves.empty()) {
        throw InvalidSAN("No SAN move found");
    } else if (candidateMoves.size() > 1) {
        throw InvalidSAN("Error: Ambiguous SAN move. Perhaps put an ambiguator");
    }*/
    //assert(candidateMoves.size() == 1);
    return candidateMoves;
}

Move ChessBoard::getMove(const std::string& moveNotation) const {
    ChessBoard boardCopy = *this;
    // get all moves made by the specified type
    // is there at least one character

    // special case: O-O or O-O-O
    // or promotion

    if (moveNotation == "O-O" ) {
        // king side Castling
        Move attemptedMove = Move(get_whiteToMove() ? "e1" : "e8", get_whiteToMove() ? "g1" : "g8", '\0', MoveType::CASTLING);

        if (boardCopy.isMoveLegal(attemptedMove)) {
            return attemptedMove;
        } else {
            throw InvalidSAN("Invalid attempt to castle kingside");
        }
    }
    else if (moveNotation == "O-O-O") {
        Move attemptedMove = Move(get_whiteToMove() ? "e1" : "c8", get_whiteToMove() ? "g1" : "c8", '\0', MoveType::CASTLING);

        if (boardCopy.isMoveLegal(attemptedMove)) {
            return attemptedMove;
        } else {
            throw InvalidSAN("Invalid attempt to castle queenside");
        }
    }
    if (moveNotation.size() < 2) {
        throw InvalidSAN("All move notations must be at least 2 characters");
    }
    if (moveNotation.size() > 5) {
        throw InvalidSAN("All move notations must be most 5 characters");
    }

    // check first character
    bool isPromotionAttempt = moveNotation[moveNotation.size()-2] == '=';
    size_t expectedStringBegin = moveNotation.size() - (isPromotionAttempt ? 4 : 2);
    string expectedEndingSquareString = moveNotation.substr(expectedStringBegin, 2);
    Square expectedEndingSquare = Square(expectedEndingSquareString);
    if (!expectedEndingSquare.isValid()) {
        throw InvalidSAN("Invalid expected ending square!");
    }

    auto it = std::find(begin(knownPieceTypeChars), end(knownPieceTypeChars), moveNotation[0]);
    if (it != end(knownPieceTypeChars)) {
        PieceType expectedType = getPieceFromSymbol(moveNotation[0]).type;
    
        auto candidateMoves = boardCopy.getSANRegular(expectedType, expectedEndingSquare, moveNotation.substr(1, moveNotation.size()-3));
        if (candidateMoves.empty()) {
            throw InvalidSAN("No SAN move found");
        } else if (candidateMoves.size() > 1) {
            throw InvalidSAN("Error: Ambiguous SAN move. Perhaps put an ambiguator");
        }
        return candidateMoves[0];
    }
    // assert is pawn
    if (!('a' <= moveNotation[0] && moveNotation[0] <= 'h' )) throw InvalidSAN("Invalid piecetype");

    // find disambiguation
    string disambig = moveNotation.substr(0, expectedStringBegin);
    vector<Move> candidates = boardCopy.getSANRegular(PieceType::PAWN, expectedEndingSquare, disambig);
    if (!isPromotionAttempt) {
        assert(candidates.size()==1);
        return candidates[0];
    }
    // get the move that corresponds to that promotion
    char promotionPieceSymbol = moveNotation[moveNotation.size()-1];
    if (promotionPieceSymbol != 'Q' && promotionPieceSymbol != 'R' && promotionPieceSymbol != 'B' && promotionPieceSymbol != 'N') {
        throw InvalidSAN("Unknown promotion piece symbol: " + std::string(1, promotionPieceSymbol));
    }
    // return the move that corresponds to that symbol
    for (const Move& candidate : candidates) {
        if (candidate.promotion == promotionPieceSymbol) return candidate;
    }
    throw InvalidSAN("No move found: " + moveNotation);
}
bool ChessBoard::isMoveLegal(Move m) {
    if (!isMovePsuedoLegal(m)) return false;

    Color movingColor = playerToMove;

    this->processPsuedoLegalMove(m);
    bool inCheck = this->isInCheck(movingColor);
    this->undoMove();

    return !inCheck;
} // return whether a move is legal

void ChessBoard::processEnPassantCapture(Move m, const Piece &start_ptr, const Piece &end_ptr) {
    if (m.type != MoveType::EN_PASSANT) return;
    if (!enPassant_targetSquare || m.endingSquare != *enPassant_targetSquare) return;
    if (start_ptr.type != PieceType::PAWN || !end_ptr.isEmpty()) return;

    Square squareCaptured = Square(m.endingSquare.file(), m.startingSquare.rank());
    // cout << "removing piece at " << squareCaptured.toString() << endl;
    setPiece(squareCaptured, EMPTY_SQUARE);
}
void ChessBoard::processEnPassantUpdate(Move m, const Piece &start_ptr, const Piece &end_ptr) {
	// update enPassantTargetSquare
    // XOR out OLD zobrist hash
    if (enPassant_targetSquare) {
        zobrist_hash ^= ZOBRIST.enPassantFile[enPassant_targetSquare->file()];
    }
	//cout<<"isPawnMove: "<<isPawnMove<<endl;
	//if (enPassant_targetSquare) cout<<"enPassant target Square: "<< (enPassant_targetSquare->toString())<<endl;
	if (start_ptr.type == PieceType::PAWN && maxNorm(m.endingSquare, m.startingSquare) > 1) {
		//cout<<"a Pawn moved 2 squares\n";
		enPassant_targetSquare = Square(m.startingSquare.file(), (m.startingSquare.rank() + m.endingSquare.rank())/2);
	} else {
		enPassant_targetSquare = std::nullopt;
	}
    // 3. XOR IN new en passant
    if (enPassant_targetSquare) {
        zobrist_hash ^= ZOBRIST.enPassantFile[enPassant_targetSquare->file()];
    }
}
void ChessBoard::processCastling(Move m, const Piece &start_ptr) {
	// check it's a king move > 2 squares, else do nothing

	if (start_ptr.type != PieceType::KING || maxNorm(m.endingSquare, m.startingSquare) <= 1)
		return;
	// need to move rook to appropriate location
	Square oldRookPos = Square(-1);
	Square newRookPos = Square(-1);
	if (m == Move("e1", "g1", '\0', MoveType::CASTLING)) {
		oldRookPos = "h1";
		newRookPos = "f1";
	} // white kingside castling
	else if (m == Move("e1", "c1", '\0', MoveType::CASTLING)) {
		oldRookPos = "a1";
		newRookPos = "d1";
	} // white queenside castling
	else if (m == Move("e8", "g8", '\0', MoveType::CASTLING)) {
		oldRookPos = "h8";
		newRookPos = "f8";
	} // black kingside castling
	else if (m == Move("e8", "c8", '\0', MoveType::CASTLING)) {
		oldRookPos = "a8";
		newRookPos = "d8";
	} // black queenside castling
    ENSURE(oldRookPos.isValid() && newRookPos.isValid(), "oldRookPos or newRookPos is invalid");
	// sanity check
	// assert a rook at oldRookPos, and newRookPOs is empty
    ENSURE((pieces[oldRookPos.idx].type == PieceType::ROOK), ("Expected a rook at " + oldRookPos.toString() + " that was not found. debug board: " + this->debug_board()));
    ENSURE(pieces[newRookPos.idx].isEmpty(), ("Expected newRookPos to be empty, but found " + std::string(1, pieces[newRookPos.idx].symbol()) + " instead"));
    setPiece(newRookPos, getPiece(oldRookPos));
    setPiece(oldRookPos, EMPTY_SQUARE);
	return;
}
void ChessBoard::processPsuedoLegalMove(Move m) {
    #ifndef NDEBUG
    this->verifyZobrist();
    #endif

    this->history.push_back(this->buildUndo(m));
    
    // Square rows/cols are 1-indexed, pieces is 0-indexed and rank-major (see getPiece)
	const Piece start_ptr = this->pieces[m.startingSquare.idx];
	const Piece end_ptr = this->pieces[m.endingSquare.idx];
    if (start_ptr.type == PieceType::KING) {
        if (start_ptr.getBelongsToWhite()) whiteKingPos = m.endingSquare;
        else blackKingPos = m.endingSquare;
    }
    assert(start_ptr.color == playerToMove);

	// update fullmove_clock (increments once Black's move completes a full move pair)
	this->fullmove_clock += (playerToMove==Color::BLACK);

	
	bool isCapture = (end_ptr.isValid());
	bool isPawnMove = (start_ptr.type == PieceType::PAWN);
	// the halfmove clock counts moves since the last capture/pawn move (for the 50-move rule), so it resets on either
	this->halfmove_clock = (isCapture || isPawnMove) ? 0 : (this->halfmove_clock + 1);

	// remove old castling rights
    zobrist_hash ^= ZOBRIST.castling[this->castlingBits()];

	// update whitePlayerState, blackPlayerState

	// if rook on a file moved, queenside is gone
	Square queensideRookSquare = get_whiteToMove() ? "a1" : "a8";
	Square kingsideRookSquare = get_whiteToMove() ? "h1" : "h8";
	if (m.endingSquare == Square("a1")) // a1 rook captured
		whitePlayerState.canQueensideCastle = false;
	if (m.endingSquare == Square("h1")) //h1 rook captured
		whitePlayerState.canKingsideCastle = false;
	if (m.endingSquare == Square("a8")) //a8 rook captured
		blackPlayerState.canQueensideCastle = false;
	if (m.endingSquare == Square("h8")) //h8 rook captured
		blackPlayerState.canKingsideCastle = false;
	PlayerState cur_state = get_whiteToMove() ? whitePlayerState : blackPlayerState;
    
	// if king moved, both are gone
	if (start_ptr.type == PieceType::KING) {
		cur_state = PlayerState(false, false);
    }
    if ((start_ptr.type == PieceType::ROOK && m.startingSquare == queensideRookSquare) || m.endingSquare == queensideRookSquare) {
		cur_state.canQueensideCastle = false;
	}
	// if rook on h file moved, kingside is gone
	if ((start_ptr.type == PieceType::ROOK && m.startingSquare == kingsideRookSquare) || m.endingSquare == kingsideRookSquare) {
		cur_state.canKingsideCastle = false;
	}
	if (get_whiteToMove()) {
		whitePlayerState = cur_state;
	} else {
		blackPlayerState = cur_state;
	}
    // and add new ones
    zobrist_hash ^= ZOBRIST.castling[this->castlingBits()];
    
	processCastling(m, start_ptr);
	//if was an enpassant capture, must remove the pawn it en-passanted
	// if it's a promotion
	if (isPawnMove) {
		processEnPassantCapture(m, start_ptr, end_ptr);
	}
	processEnPassantUpdate(m, start_ptr, end_ptr);

    Piece newPiece = m.promotion ? getPieceFromSymbol(m.promotion) : start_ptr;
    // actually move the piece
    this->setPiece(m.endingSquare, newPiece);
    this->setPiece(m.startingSquare, EMPTY_SQUARE);

    this->playerToMove = oppositeColor(playerToMove);
    zobrist_hash ^= ZOBRIST.sideToMove;
    
    #ifndef NDEBUG
    this->verifyZobrist();
    #endif
}

bool ChessBoard::isMovePsuedoLegal(Move m) const {
    std::vector<Move> pseudoLegalMoves =
        allPseudoLegalDestinations(m.startingSquare);

    bool moveIsPseudoLegal =
        std::find(pseudoLegalMoves.begin(),
                  pseudoLegalMoves.end(),
                  m) != pseudoLegalMoves.end();

    return moveIsPseudoLegal;

}
void ChessBoard::processMove(Move m) {
	if (this->isMoveLegal(m)) {
		this->processPsuedoLegalMove(m);
	} else {
        std::string message = "You have attempted an illegal move: " + m.debugString() + ". FEN: " + fen();
        if (!this->isMovePsuedoLegal(m)) message += " (hint: move is not pseudo-legal)";
		throw IllegalMoveError(message);
	}
}

// Is there a piece belonging to `attackerIsWhite` on the far end of the
// ray starting at `from` and stepping by `dir` (one step at a time) that
// could capture along that ray -- i.e. a rook/queen on a rank/file ray,
// or a bishop/queen on a diagonal ray? The ray stops at the first
// occupied square either way (that piece blocks anything behind it).
bool ChessBoard::isSlidingAttacker(Square from, int dir, Color attackerColor, PieceType pieceTypeA, PieceType pieceTypeB) const {
	Square cur = from + dir;
	while (cur.isValid()) {
        Piece p = getPiece(cur);
        if (p.isValid()) {
            if (p.color == attackerColor) {
                if (p.type == pieceTypeA || p.type == pieceTypeB) {
                    return true;
                }
            }
            return false; // occupied, so the ray is blocked past here regardless
        }
		cur = cur + dir;
	}
	return false;
}


std::vector<Move> ChessBoard::whereKingCouldMove(const Square origin) const {
	// get al neighboring places
	Piece king = getAndAssertPiece(origin, PieceType::KING);

	std::vector<Move> places;

	for (int off : kingOffsets) {
		Square target = origin + off;
		if (!target.isValid())
			continue;
		// if there is a piece of a different color, then it's okay
		// else not
        // empty is fine as well
		Piece targetPiece = getPiece(target);
		if (targetPiece.color != king.color)
			places.emplace_back(origin, target);
	}
	// check for castling
	if (get_whiteToMove() && !squareAttackedBy("e1", Color::BLACK)) {
		// if white can kingside castle, and f1 and g1 are open
		if (whitePlayerState.canKingsideCastle && !hasPiece("f1") && !hasPiece("g1")) {
			if (!squareAttackedBy("f1", Color::BLACK))
				places.emplace_back(origin, Square("g1"), '\0', MoveType::CASTLING);
		}

		// check queenside castling
		if (whitePlayerState.canQueensideCastle && !hasPiece("b1") && !hasPiece("c1") && !hasPiece("d1")) {
			if (!squareAttackedBy("d1", Color::BLACK))
				places.emplace_back(origin, Square("c1"), '\0', MoveType::CASTLING);
		}
	}

	if (playerToMove == Color::BLACK && !squareAttackedBy("e8", Color::WHITE)) { // can black castle
		// check kingside castling
		if (blackPlayerState.canKingsideCastle && !hasPiece("f8") && !hasPiece("g8")) {
			if (!squareAttackedBy("f8", Color::WHITE))
				places.emplace_back(origin, Square("g8"), '\0', MoveType::CASTLING);
		}
		// check queenside castling
		if (blackPlayerState.canQueensideCastle && !hasPiece("b8") && !hasPiece("c8") && !hasPiece("d8")) {
			if (!squareAttackedBy("d8", Color::WHITE))
				places.emplace_back(origin, Square("c8"), '\0', MoveType::CASTLING);
		}
	}
	//for (const Square& place : places) cout<<(place.operator()())<<endl;
	return places;
}

std::vector<Move> ChessBoard::wherePawnCouldMove(const Square origin) const {
	Piece pawn = getAndAssertPiece(origin, PieceType::PAWN);
    assert(pawn.color == playerToMove);
	int dir = (pawn.color==Color::WHITE) ? 1 : -1; // rank direction

	bool canMoveTwoSpaces = (pawn.color==Color::WHITE) ? (origin.rank() == 1) : (origin.rank() == 6);
	std::vector<Move> places;

	// captures: dr=dir, df= +/-1
	// Offset(dir, 1) = dir*16 + 1
	const int caps[2] = {dir * 16 + 1, dir * 16 - 1};

	for (int cap : caps) {
		Square target = origin + cap;
		if (!target.isValid())
			continue;
		Piece targetPiece = getPiece(target);
		if (targetPiece.color != pawn.color && targetPiece.color != Color::NONE) {
            places.emplace_back(origin, target);
        }
        if (enPassant_targetSquare && target == *enPassant_targetSquare) {
			places.emplace_back(origin, target, '\0', MoveType::EN_PASSANT);
		}
	}

	// forward moves
	for (int i = 1; i <= (canMoveTwoSpaces ? 2 : 1); ++i) {
		int off = i * dir * 16; // Offset(i*dir, 0)
		Square target = origin + off;
		if (!target.isValid())
			break;
		if (hasPiece(target))
			break; // blocked

		if (i==1) {
            places.emplace_back(origin, target);
        }
        else {
            assert(i==2);
            places.emplace_back(origin, target, '\0', MoveType::DOUBLE_PAWN_PUSH);
        }
	}

	// promotion
	places.reserve(places.size() + 9); // worst case: 3 promos * 3 extra

    for (int i = places.size() - 1; i >= 0; --i) {
        Move &place = places[i];
        bool isPromoRank = (pawn.color==Color::WHITE) ? (place.endingSquare.rank() == 7) : (place.endingSquare.rank() == 0);
        if (!isPromoRank) continue;

        // replace current slot with Q promo (most common, keep in place)
        places[i] = Move{place.startingSquare, place.endingSquare,
            (pawn.color==Color::WHITE)? 'Q' : 'q', place.type};

        

        // append other 3 promos to end
        places.emplace_back(place.startingSquare, place.endingSquare,
            (pawn.color==Color::WHITE)? 'N' : 'n', place.type);
        places.emplace_back(place.startingSquare, place.endingSquare,
            (pawn.color==Color::WHITE)? 'R' : 'r', place.type);
        places.emplace_back(place.startingSquare, place.endingSquare,
            (pawn.color==Color::WHITE)? 'B' : 'b', place.type);
    }
	return places;
}

std::vector<Move> ChessBoard::whereKnightCouldMove(const Square origin) const {
	Piece knight = getAndAssertPiece(origin, PieceType::KNIGHT);

	std::vector<Move> places;
	for (const int &off : knightOffsets) {
		Square target = origin + off;
		if (!target.isValid())
			continue;
		// if there is a piece of a different color, then it's okay
		// else not
		Piece targetPiece = getPiece(target);
		if (targetPiece.color != knight.color) // empty has None COLOR
			places.emplace_back(origin, target);
	}
	return places;
}
// get all pieces an attacker with the ray (dir) starting at from, whose color is belongsToWhite, could reach legally (without considering check)
std::vector<Move> ChessBoard::getSlidingMoverPositions(const Square from, const int dir, Color fromColor) const {
	Square cur = from + dir;
	std::vector<Move> places;
	while (cur.isValid()) {
		Piece p = getPiece(cur);
		if (p.isValid()) {
            assert(p.color != Color::NONE);
			if (p.color != fromColor) {
				// not of same color, so can capture!
				places.emplace_back(from, cur);
			}
			break; // occupied, so the ray is blocked past here regardless
		}
		places.emplace_back(from, cur);
		cur = cur + dir;
	}
	return places;
}

std::vector<Move> ChessBoard::whereBishopCouldMove(const Square origin) const {
	Piece bishop = getAndAssertPiece(origin, PieceType::BISHOP);
	std::vector<Move> places;
	for (int dir : bishopOffsets) {
		mergeSets(places, getSlidingMoverPositions(origin, dir, bishop.color));
	}
	return places;
}
std::vector<Move> ChessBoard::whereRookCouldMove(const Square origin) const {
	Piece rook = getAndAssertPiece(origin, PieceType::ROOK);
	std::vector<Move> places;
	for (int dir : rookOffsets) {
		mergeSets(places, getSlidingMoverPositions(origin, dir, rook.color));
	}
	return places;
}
std::vector<Move> ChessBoard::whereQueenCouldMove(const Square origin) const {
	Piece queen = getAndAssertPiece(origin, PieceType::QUEEN);
	std::vector<Move> places;
	for (int dir : queenOffsets) {
		mergeSets(places, getSlidingMoverPositions(origin, dir, queen.color));
	}
	return places;
}

std::vector<Move> ChessBoard::allPseudoLegalDestinations(const Square origin) const {
	Piece piece = getPiece(origin);
	if (piece.isEmpty()) {
		return {}; // no piece there
    }
    if (piece.color != playerToMove) {
        return {}; // can't move the piece, is of wrong color
    }
	switch (piece.type) {
        case PieceType::KING: return whereKingCouldMove(origin);
        case PieceType::QUEEN: return whereQueenCouldMove(origin);
        case PieceType::ROOK: return whereRookCouldMove(origin);
        case PieceType::BISHOP: return whereBishopCouldMove(origin);
        case PieceType::KNIGHT: return whereKnightCouldMove(origin);
        case PieceType::PAWN: return wherePawnCouldMove(origin);
        default: assert(false && "unknown piece"); return {};
	}
}

// Is `target` attacked by any piece belonging to `attackerIsWhite`?
// This is a raw-attack check (used to detect check): it only asks
// "could this piece capture on `target` right now", not whether doing
// so would be a legal move for the attacker.
bool ChessBoard::squareAttackedBy(Square target, Color attackerColor) const {
	// get all the psuedo-legal moves
	// and check if they would end on the target

	// attack by knight

	for (const int off : knightOffsets) {
		Square sq = target + off;
		if (!sq.isValid())
			continue;
		Piece knightPossibleTarget = getPiece(sq);
		if (knightPossibleTarget.type == PieceType::KNIGHT && knightPossibleTarget.color == attackerColor) {
			return true;
		}
	}

	// attack by king
	for (int dr = -1; dr <= 1; ++dr) {
		for (int dc = -1; dc <= 1; ++dc) {
			if (dr == 0 && dc == 0)
				continue;
			Square sq = target + Offset(dr, dc);
			if (!sq.isValid())
				continue;
			Piece possibleKing = getPiece(sq);
			if (possibleKing.type == PieceType::KING && possibleKing.color == attackerColor) {
				return true;
			}
		}
	}

    // attack by pawn
	// A pawn attacks diagonally, one rank "ahead" of where it sits (from
	// its own side's perspective): white pawns advance toward higher
	// ranks, so an attacking white pawn sits one rank *below* the target.
	// Square.row is the file, Square.col is the rank.
	int behind = isWhite(attackerColor) ? -1 : 1;
	for (int dc : {-1, 1}) {
		Square sq = target + Offset(behind, dc);
		if (!sq.isValid())
			continue;
		Piece possiblePawn = getPiece(sq);
		if (possiblePawn.type == PieceType::PAWN && possiblePawn.color == attackerColor) {
			return true;
		}
	}

    // attack by rook
	for (const int dir : rookOffsets) {
		if (isSlidingAttacker(target, dir, attackerColor, PieceType::ROOK, PieceType::QUEEN)) {
			return true;
		}
	}

    // attack by bishop
	for (const int dir : bishopOffsets) {
		if (isSlidingAttacker(target, dir, attackerColor, PieceType::BISHOP, PieceType::QUEEN)) {
			return true;
		}
	}

	return false;
}


bool ChessBoard::hasInsufficientMaterial() const {
	// is there anything other than a king, bishop, or knight?
	int numBishops = 0;
	int numKnights = 0;
	for (size_t row = 0; row < BOARD_SIZE; ++row) {
		for (size_t col = 0; col < BOARD_SIZE; ++col) {
			Piece piece = pieces[Square(row, col).idx];
			if (piece.isEmpty())
				continue;
			switch (piece.type) {
            case PieceType::NONE:
                continue;
			case PieceType::KING:
				continue;
			case PieceType::BISHOP:
				++numBishops;
				break;
			case PieceType::KNIGHT:
				++numKnights;
				break;
			default:
				return false;
			}
		}
	}
	return (numBishops <= 1) && (numKnights <= 1);
}
GameStatus ChessBoard::getStatus() {
	// return the status of the game (whether white won, black won, it's a draw, or game is still going on)
    if (halfmove_clock >= 100) {
		return GameStatus::DRAW;
	} else if (this->hasInsufficientMaterial()) {
		return GameStatus::DRAW;
	}else if (this->is_threefold_repetition()) {
        return GameStatus::DRAW;
    }
    auto legalMoves = this->allLegalMoves();
    bool inCheck = this->isInCheck(playerToMove);
	if (legalMoves.empty()) { // the game is over, checkmate
        if (inCheck) {
		    // is it black's turn? then white won
		    if (get_whiteToMove()) {
			    return GameStatus::BLACK_WON;
		    } else {
			    return GameStatus::WHITE_WON;
		    }
        }
        else {
            return GameStatus::DRAW;
        }
	} else {
		if (get_whiteToMove()) {
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
	std::string PlayerPart = ParsePieces::getPlayerPart(playerToMove);
	std::string CastlingPart = ParsePieces::getCastlingPart(whitePlayerState, blackPlayerState);
	std::string enPassantPart = ParsePieces::getEnPassantPart(enPassant_targetSquare);
	std::string halfMovePart = std::to_string(halfmove_clock);
	std::string fullMovePart = std::to_string(fullmove_clock);
	return PiecePart + " " + PlayerPart + " " + CastlingPart + " " + enPassantPart + " " + halfMovePart + " " + fullMovePart;
}

std::vector<Move> ChessBoard::allLegalMoves(const Square sq) {
	// get all legal moves from the piece at the square indicated
	// if there is no piece at that square, or if the piece at that square is owned by the opponent, return empty set
    // this will return an empty vector if there are no moves available
	std::vector<Move> moves = allPseudoLegalDestinations(sq);
	
    Piece piece = getPiece(sq);
    if (piece.isEmpty()) return {};

	std::erase_if(moves, [&](const Move& mov){
        this->processPsuedoLegalMove(mov);
        bool illegal = isInCheck(piece.color);
        this->undoMove();
        return illegal;
    });
    return moves;
}
std::vector<Move> ChessBoard::allLegalMoves() {
	// done by Samuel
	// return all legal moves from all pieces that the player owns
	// this function is necessary for the engine
	// it can call allLegalMoves for every piece it owns and splice them together into one set, then return that set
	std::vector<Move> legalMoves;
	for (size_t file = 0; file < BOARD_SIZE; ++file) {
		for (size_t rank = 0; rank < BOARD_SIZE; ++rank) {
            mergeSets(legalMoves, allLegalMoves(Square(file, rank)));
		}
	}
	return legalMoves;
}

std::ostream &operator<<(std::ostream &os, const ChessBoard &board) {
	for (int ranknum = 0; ranknum < 8; ++ranknum) {

		for (int filenum = 0; filenum < 8; ++filenum) {
			auto piece_ptr = board.pieces[Square(filenum, ranknum).idx];
			if (piece_ptr.isEmpty()) {
				os << ' ';
			} else {
				os << piece_ptr.symbol();
			}
		}
		os << endl;
	}
	return os;
}

ChessBoard ChessBoard::board_with_move(const Move &move) const {
	ChessBoard newBoard = (*this);

	//std::cerr << "original: " << this->debug_board() << '\n';
	//std::cerr << "copy:     " << newBoard.debug_board() << '\n';

	newBoard.processMove(move);

	//std::cerr << "original: " << this->debug_board() << '\n';
	//std::cerr << "new:      " << newBoard.debug_board() << '\n';
	return newBoard;
}
bool ChessBoard::move_ends_game(const Move move) {
    this->processMove(move);
    bool isOver = isGameOver(this->getStatus());
    this->undoMove();
    return isOver;
}
bool ChessBoard::move_is_castling(const Move move) const {
	Piece movingPiece = this->getPiece(move.startingSquare);
	if (movingPiece.type != PieceType::KING)
		return false;
	return maxNorm(move.startingSquare, move.endingSquare) > 1;
}
bool ChessBoard::move_is_check(const Move move) {
    this->processMove(move);
    bool inCheck = this->isInCheck(oppositeColor(playerToMove));
    this->undoMove();
    return inCheck;
}
bool ChessBoard::move_is_capture(const Move move) const { return hasPiece(move.endingSquare); } // OR EN PASSANT!
bool ChessBoard::move_is_zeroing(const Move move) const {
	if (this->move_is_capture(move))
		return true;
	Piece pieceAtBeginning = this->getPiece(move.startingSquare);
	return pieceAtBeginning.type == PieceType::PAWN;
}

// this function exists for testing purposes
int ChessBoard::perftCopy(int depth, int divideThreshold) {
	if (depth == 0)
		return 1;
	int perft_res = 0;
	std::vector<Move> moves = this->allLegalMoves();
	//if (depth==1) return moves.size();
	for (Move m : moves) {
		ChessBoard child = this->board_with_move(m);

		int perft_child = child.perft(depth - 1, divideThreshold);
		if (depth == divideThreshold)
			std::cout /*<< "DEPTH = " << depth << " PERFT DIVIDE: m="*/ << m.operator()() << " " << perft_child << std::endl;

		perft_res += perft_child;
	}
	return perft_res;
}

// and so does this
int ChessBoard::perft(int depth, int divideThreshold) {
	if (depth == 0)
		return 1;
	int perft_res = 0;
	std::vector<Move> moves = this->allLegalMoves();
	//if (depth==1) return moves.size();
	for (Move m : moves) {
        this->processMove(m);

		int perft_child = this->perft(depth - 1, divideThreshold);
		if (depth == divideThreshold)
			std::cout /*<< "DEPTH = " << depth << " PERFT DIVIDE: m="*/ << m.operator()() << " " << perft_child << std::endl;

		perft_res += perft_child;
        this->undoMove();
	}
	return perft_res;
}
// and this function exists for debugging purposes
std::string ChessBoard::debug_board() const {
	std::string debugBoard = this->fen();
	debugBoard += " Moves:";
	for (const UndoMove& undo : this->history)
		debugBoard += (" " + undo.move.operator()());
	return debugBoard;
}

uint64_t ChessBoard::zobristFromScratch() const {
    uint64_t hash = ZOBRIST.castling[this->castlingBits()];
    if (!get_whiteToMove()) hash^=ZOBRIST.sideToMove;
    for (int sqnum = 0; sqnum<64; ++sqnum) {
        Square square = Square::from64(sqnum);
        Piece piece = this->getPiece(square);
        if (piece.isValid()) {
            hash ^= ZOBRIST.pieces[piece.colorNum()][piece.pieceNum()][sqnum];
        }
    }
    if (enPassant_targetSquare) {
        hash ^= ZOBRIST.enPassantFile[enPassant_targetSquare->file()];
    }
    return hash;
    
}
Square getRookFrom(Square kingTo) {
    if (kingTo == Square("g1"))
            return Square("h1");
    if (kingTo == Square("c1"))
            return Square("a1");
    if (kingTo == Square("g8"))
            return Square("h8");
    if (kingTo == Square("c8"))
            return Square("a8");
    return Square(-1); // not a castle
}

Square getRookTo(Square kingTo) {
    if (kingTo == Square("g1"))
            return Square("f1");
    if (kingTo == Square("c1"))
            return Square("d1");
    if (kingTo == Square("g8"))
            return Square("f8");
    if (kingTo == Square("c8"))
            return Square("d8");
    return Square(-1);
}
// undo move
UndoMove ChessBoard::buildUndo(const Move &m) const {
    UndoMove u;
    u.move = m;
    u.playerToMove = playerToMove;
    u.zobrist = zobrist_hash;
    u.whiteState = whitePlayerState;
    u.blackState = blackPlayerState;
    u.epTarget = enPassant_targetSquare;
    u.halfmove = halfmove_clock;
    u.fullmove = fullmove_clock;
    u.originalPiece = getPiece(m.startingSquare);

    // what is captured?
    if (m.type == MoveType::EN_PASSANT) {
            
            u.capturedSquare = Square(m.endingSquare.file(), m.startingSquare.rank());
            u.capturedPiece = getPiece(u.capturedSquare);
    } else {
            u.capturedSquare = m.endingSquare;
            u.capturedPiece = getPiece(m.endingSquare);
    }

    if (m.type == MoveType::CASTLING) {
            u.rookFrom = getRookFrom(m.endingSquare);
            u.rookTo = getRookTo(m.endingSquare);
    } else {
            u.rookFrom = Square(-1);
            u.rookTo = Square(-1);
    }
    return u;
}

void ChessBoard::undoMove(const UndoMove &u) {
    setPiece(u.move.startingSquare, u.originalPiece);
    if (u.move.type == MoveType::EN_PASSANT) {
        setPiece(u.move.endingSquare, EMPTY_SQUARE);
        setPiece(u.capturedSquare, u.capturedPiece); // pawn behind
    } else {
            setPiece(u.move.endingSquare, u.capturedPiece); // nullptr if no capture
    }
    if (u.rookFrom.isValid() && u.rookTo.isValid() && u.move.type == MoveType::CASTLING) {
            setPiece(u.rookFrom, getPieceFromSymbol(getPiece(u.rookTo).symbol()));
            setPiece(u.rookTo, EMPTY_SQUARE);
    }

    // must update king cache
    if (u.originalPiece.type == PieceType::KING) {
        if (u.originalPiece.getBelongsToWhite()) whiteKingPos = u.move.startingSquare;
        else {blackKingPos = u.move.startingSquare;}
    }
    playerToMove = u.playerToMove;
    whitePlayerState = u.whiteState;
    blackPlayerState = u.blackState;
    enPassant_targetSquare = u.epTarget;
    halfmove_clock = u.halfmove;
    fullmove_clock = u.fullmove;
    zobrist_hash = u.zobrist;
#ifndef NDEBUG
    verifyZobrist();
#endif
}
bool ChessBoard::undoMove() {
    if (history.empty()) return false;
    undoMove(history.back()); 
    history.pop_back();
    return true;
}

bool ChessBoard::is_threefold_repetition() const {
    if (history.empty()) return false;

    uint64_t current_hash = zobrist_hash;
    int match_count = 1; // current mvoe is NOT the first in history

    // Iterate backward through the history
    for (auto it = history.rbegin(); it != history.rend(); ++it) {
        if (it->zobrist == current_hash) {
            match_count++;
            if (match_count >= 3) {
                return true;
            }
        }

        // Irreversible moves (pawn moves or captures) reset the halfmove clock.
        // Positions before an irreversible move cannot be repeated, so we stop searching.
        if (it->halfmove == 0) {
            break;
        }
    }

    return false;
}