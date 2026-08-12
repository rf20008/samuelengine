#include "Bishop.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Pawn.hpp"
#include "Piece.hpp"
#include "Queen.hpp"
#include "Rook.hpp"

#include "Errors.hpp"

#include <map>
#include <memory>

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

const std::map<char, PiecePtr> pieceMap{{'B', WhiteBishop}, {'b', BlackBishop}, {'N', WhiteKnight}, {'n', BlackKnight}, {'R', WhiteRook}, {'r', BlackRook}, {'K', WhiteKing}, {'k', BlackKing}, {'P', WhitePawn}, {'p', BlackPawn}, {'Q', WhiteQueen}, {'q', BlackQueen}};
PiecePtr getPiece(char c) {
	std::map<char, PiecePtr>::const_iterator it = pieceMap.find(c);
	if (it == pieceMap.end()) {
		throw UnknownPiece("Unknown piece: " + std::string{c});
	}
	return it->second;
}