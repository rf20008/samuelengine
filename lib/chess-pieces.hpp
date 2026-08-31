#ifndef CHESS_PIECES_HPP
#define CHESS_PIECES_HPP

#include <stdexcept>
#include <string>

enum class PieceType {
	WPAWN,
	WKNIGHT,
	WBISHOP,
	WROOK,
	WQUEEN,
	WKING,
	// black pieces
	BPAWN,
	BKNIGHT,
	BBISHOP,
	BROOK,
	BQUEEN,
	BKING
};

inline std::string display_piece(PieceType pt) {
	switch (pt) {
	case PieceType::BPAWN:
		return "♟";
	case PieceType::BKNIGHT:
		return "♞";
	case PieceType::BBISHOP:
		return "♝";
	case PieceType::BROOK:
		return "♜";
	case PieceType::BQUEEN:
		return "♛";
	case PieceType::BKING:
		return "♚";
	case PieceType::WPAWN:
		return "♙";
	case PieceType::WKNIGHT:
		return "♘";
	case PieceType::WBISHOP:
		return "♗";
	case PieceType::WROOK:
		return "♖";
	case PieceType::WQUEEN:
		return "♕";
	case PieceType::WKING:
		return "♔";
	default:
		return "?";
	}
};

// Maps a chess-engine piece symbol (Piece::symbol(), e.g. 'P'/'p', 'N'/'n',
// ... -- see include/Piece.hpp and its subclasses) to the PieceType this
// library knows how to display. Uppercase is white, lowercase is black,
// matching the engine's own convention.
inline PieceType piece_type_from_symbol(char symbol) {
	switch (symbol) {
	case 'P':
		return PieceType::WPAWN;
	case 'N':
		return PieceType::WKNIGHT;
	case 'B':
		return PieceType::WBISHOP;
	case 'R':
		return PieceType::WROOK;
	case 'Q':
		return PieceType::WQUEEN;
	case 'K':
		return PieceType::WKING;
	case 'p':
		return PieceType::BPAWN;
	case 'n':
		return PieceType::BKNIGHT;
	case 'b':
		return PieceType::BBISHOP;
	case 'r':
		return PieceType::BROOK;
	case 'q':
		return PieceType::BQUEEN;
	case 'k':
		return PieceType::BKING;
	default:
		throw std::invalid_argument("piece_type_from_symbol: unknown piece symbol '" + std::string(1, symbol) + "'");
	}
};

// Convenience: go straight from an engine piece symbol to its display glyph.
inline std::string display_piece_symbol(char symbol) { return display_piece(piece_type_from_symbol(symbol)); }

#endif
