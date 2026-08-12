#include "Errors.hpp"
#include "GetPiece.hpp"
#include "Piece.hpp"
#include "PlayerState.hpp"
#include "Square.hpp"
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>
namespace ParsePieces {
std::vector<std::vector<PiecePtr>> parsePiecePart(const std::string &PiecePart) {
	std::stringstream RankReader(PiecePart);
	std::vector<std::string> Ranks;
	Ranks.resize(BOARD_SIZE);
	for (size_t ranknum = 0; ranknum < BOARD_SIZE; ++ranknum) {
		getline(RankReader, Ranks.at(BOARD_SIZE - ranknum - 1), '/'); // FEN reads from rank 8 to rank 1
	}
	if (!RankReader.eof()) {
		throw InvalidFEN("Board has more than 8 ranks");
	}
	std::vector<std::vector<PiecePtr>> board;
	for (size_t ranknum = 0; ranknum < Ranks.size(); ranknum++) {
		std::string Rank = Ranks.at(ranknum);
		board.push_back(std::vector<PiecePtr>());
		for (char pieceChar : Rank) {
			// if it's a digit
			if (pieceChar >= '0' && pieceChar <= '9') { // piecechar is a digit
				int digitNum = pieceChar - '0';
				for (int i = 0; i < digitNum; ++i) {							 // add that many free spaces
					board.at(ranknum).push_back(std::shared_ptr<const Piece>()); // a null piece
				}
				continue;
			}
			// this is a piece! add it

			board.at(ranknum).push_back(getPiece(pieceChar));
		}
		// check that it is of size BOARD_SIZE
		if (board.at(ranknum).size() != BOARD_SIZE) {
			throw InvalidFEN("Error: Board rank " + std::to_string(ranknum) + " is not of size 8, but of size " + std::to_string(board.at(ranknum).size()) + ".");
		}
	}
	if (board.size() != BOARD_SIZE) {
		throw InvalidFEN("Board has fewer than 8 ranks");
	}
	return board;
}
bool parsePlayerPart(const std::string &PlayerPart) {
	if (PlayerPart.size() != 1) {
		throw InvalidFEN("Error: Player argument must be 1 character");
	}
	char PlayerChar = PlayerPart[0];
	if (PlayerChar != 'w' && PlayerChar != 'b')
		throw InvalidFEN("Error: Player Argument must be either \"w\" or \"b\".");
	return (PlayerChar == 'w');
}
std::pair<PlayerState, PlayerState> parseCastlingPart(const std::string &CastlingPart) {
	PlayerState whiteState{false, false};
	PlayerState blackState{false, false};
	if (CastlingPart == "-") {
		return {whiteState, blackState};
	}
	for (char c : CastlingPart) {
		switch (c) {
		case 'K':
			whiteState.canKingsideCastle = true;
			break;
		case 'Q':
			whiteState.canQueensideCastle = true;
			break;
		case 'k':
			blackState.canKingsideCastle = true;
			break;
		case 'q':
			blackState.canQueensideCastle = true;
			break;
		default:
			throw InvalidFEN("Unknown castling character " + std::string(1, c));
		}
	}
	return {whiteState, blackState};
}
std::optional<Square> parseEnPassantPart(std::string EnPassantPart) {
	if (EnPassantPart == "-") {
		return std::optional<Square>();
	}
	std::optional<Square> sq(EnPassantPart);
	if (!sq->isValid())
		throw InvalidFEN("Square is out of bounds");
	return sq;
}
std::string getPiecePart(const std::vector<std::vector<PiecePtr>> &pieces) {
	std::string PiecePart = "";
	// TODO: figure out why the output string is wrong
	for (std::vector<std::vector<PiecePtr>>::const_reverse_iterator it = pieces.crbegin(); it != pieces.crend(); ++it) { // because FEN stores from rank 8 to rank 1
		// if it's a piece, increment it
		int numWOPiece = 0;
		const std::vector<PiecePtr> rank = *it;
		for (const auto &piece : rank) {
			if (!piece) {
				++numWOPiece;
				continue;
			} else {
				if (numWOPiece != 0) {
					PiecePart.push_back('0' + numWOPiece);
				}
				PiecePart.push_back(piece->symbol());
				numWOPiece = 0;
			}
		}
		if (numWOPiece != 0)
			PiecePart.push_back('0' + numWOPiece);
		if ((it + 1) != pieces.crend())
			PiecePart.push_back('/');
	}
	return PiecePart;
}
std::string getCastlingPart(const PlayerState &whiteState, const PlayerState &blackState) {
	std::string castlingPart = "";
	if (whiteState.canKingsideCastle)
		castlingPart.push_back('K');
	if (whiteState.canQueensideCastle)
		castlingPart.push_back('Q');
	if (blackState.canKingsideCastle)
		castlingPart.push_back('k');
	if (blackState.canQueensideCastle)
		castlingPart.push_back('q');

	if (castlingPart == "")
		castlingPart = "-";
	return castlingPart;
}
std::string getEnPassantPart(std::optional<Square> square) {
	if (!square) {
		return "-";
	}
	return square->toString();
}
} // namespace ParsePieces
