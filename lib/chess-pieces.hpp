#ifndef CHESS_PIECES_HPP
#define CHESS_PIECES_HPP

#include <stdexcept>
#include <string>



// Maps a chess-engine piece symbol (Piece::symbol(), e.g. 'P'/'p', 'N'/'n',
// ... -- see include/Piece.hpp and its subclasses) to the PieceType this
// library knows how to display. Uppercase is white, lowercase is black,
// matching the engine's own convention.
inline std::string piece_type_from_symbol(char symbol) {
    // according to Wikipedia, the outlined pieces are White, and the filled in pieces are Black
	switch (symbol) {
	case 'P': return "♙";
	case 'N': return "♘";
	case 'B': return "♗";
	case 'R': return "♖";
	case 'Q': return "♕";
	case 'K': return "♔";
	case 'p': return "♟";
	case 'n': return "♞";
	case 'b': return "♝";
	case 'r': return "♜";
	case 'q': return "♛"; 
	case 'k': return "♚";
	default:
		throw std::invalid_argument("piece_type_from_symbol: unknown piece symbol '" + std::string(1, symbol) + "'");
	}
};

// Convenience: go straight from an engine piece symbol to its display glyph.
inline std::string display_piece_symbol(char symbol) { return piece_type_from_symbol(symbol); }

#endif
