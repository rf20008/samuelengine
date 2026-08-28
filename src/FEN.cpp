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

void parsePiecePart(const std::string &PiecePart, PiecePtr board[128]) {
	// 1. init all to null
	for (int i = 0; i < 128; ++i)
		board[i] = nullptr;

	std::stringstream RankReader(PiecePart);
	std::vector<std::string> Ranks;
	Ranks.reserve(BOARD_SIZE);
	std::string rankStr;
	while (std::getline(RankReader, rankStr, '/')) {
		Ranks.push_back(rankStr);
	}

	if (Ranks.size() != BOARD_SIZE) {
		throw InvalidFEN("Board has " + std::to_string(Ranks.size()) + " ranks, expected 8");
	}

	// FEN goes 8 -> 1, we store 0 -> 7
	for (size_t i = 0; i < BOARD_SIZE; ++i) {
		size_t ranknum = BOARD_SIZE - 1 - i; // rank 7,6,5...0
		const std::string &Rank = Ranks[i];
		int file = 0;

		for (char pieceChar : Rank) {
			if (pieceChar >= '1' && pieceChar <= '8') {
				file += pieceChar - '0';
			} else {
				if (file >= BOARD_SIZE) {
					throw InvalidFEN("Too many squares on rank " + std::to_string(ranknum));
				}
				Square sq(file, (int) ranknum);
				board[sq.idx] = getPiece(pieceChar);
				file++;
			}
		}
		if (file != BOARD_SIZE) {
			throw InvalidFEN("Rank " + std::to_string(ranknum) + " has " + std::to_string(file) + " squares, expected 8");
		}
	}
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
std::string getPiecePart(const PiecePtr pieces[128]) {
	std::string PiecePart;
	// FEN goes rank 8 -> 1, so loop 7 down to 0
	for (int rank = 7; rank >= 0; --rank) {
		int numWOPiece = 0;
		for (int file = 0; file < 8; ++file) {
			Square sq(file, rank);
			PiecePtr piece = pieces[sq.idx];
			if (!piece) {
				++numWOPiece;
			} else {
				if (numWOPiece != 0) {
					PiecePart.push_back('0' + numWOPiece);
					numWOPiece = 0;
				}
				PiecePart.push_back(piece->symbol());
			}
		}
		if (numWOPiece != 0) {
			PiecePart.push_back('0' + numWOPiece);
		}
		if (rank != 0) {
			PiecePart.push_back('/');
		}
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
