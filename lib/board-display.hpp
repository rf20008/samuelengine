#ifndef BOARD_DISPLAY_HPP
#define BOARD_DISPLAY_HPP

// TUI presentation for ChessBoard. This is UI logic only: it reads the
// board exclusively through ChessBoard's public interface (getPiece) and
// hands piece symbols off to chess-pieces.hpp for their display glyphs, so
// the engine itself doesn't need to know anything about how it's shown to
// a user.

#include "chess-pieces.hpp"

#include "ChessBoard.hpp"
#include "Piece.hpp"
#include "Square.hpp"

#include <sstream>
#include <string>

namespace ChessUI {

inline std::string squareGlyph(const ChessBoard &board, Square sq) {
	PiecePtr piece = board.getPiece(sq);
	if (!piece) {
		return ".";
	}
	return display_piece_symbol(piece->symbol());
}

// Renders the board with rank numbers (8 down to 1) down the left side and
// file letters (a-h) along the bottom, so a user can read off coordinates
// directly, e.g.:
//
//   8 r n b q k b n r
//   7 p p p p p p p p
//   6 . . . . . . . .
//   5 . . . . . . . .
//   4 . . . . . . . .
//   3 . . . . . . . .
//   2 P P P P P P P P
//   1 R N B Q K B N R
//     a b c d e f g h
inline std::string renderBoard(const ChessBoard &board) {
	std::ostringstream out;
	for (int rank = BOARD_SIZE; rank >= 1; --rank) {
		out << rank << ' ';
		for (int file = 1; file <= BOARD_SIZE; ++file) {
			out << squareGlyph(board, Square(file, rank));
			if (file != BOARD_SIZE) {
				out << ' ';
			}
		}
		out << '\n';
	}
	out << "  ";
	for (int file = 1; file <= BOARD_SIZE; ++file) {
		out << static_cast<char>('a' + file - 1);
		if (file != BOARD_SIZE) {
			out << ' ';
		}
	}
	out << '\n';
	return out.str();
}

} // namespace ChessUI

#endif
