#pragma once
#ifndef CS3520_MINIPROJECT_PIECE
#define CS3520_MINIPROJECT_PIECE

#include <memory>
#include <string>
#include <cstdint>
#include <cassert>
constexpr char PIECETYPES[7] = {' ', 'k', 'q', 'r', 'b', 'n', 'p'};
enum class PieceType : uint8_t {
    NONE = 0,
    KING = 1,
    QUEEN = 2,
    ROOK = 3,
    BISHOP = 4,
    KNIGHT = 5,
    PAWN = 6,
};
enum class Color: uint8_t {
    NONE = 0,
    WHITE = 1,
    BLACK = 2,
};
constexpr const char* ColorNames[3] = {"White", "Black", "None^"};
constexpr const char* colorName(Color c) {
    return ColorNames[static_cast<int>(c)];
}


struct Piece {
    public:
        PieceType type;
		Color color;
        constexpr Piece() : type(PieceType::NONE), color(Color::NONE) {}
		constexpr Piece(PieceType type, Color c) : type(type), color(c) {}
		constexpr bool getBelongsToWhite() const { return color == Color::WHITE; }
        constexpr char symbol() const {
            char c;
            switch(this->type) {
                case PieceType::KING: c='k'; break;
                case PieceType::QUEEN: c='q'; break;
                case PieceType::ROOK: c='r'; break;
                case PieceType::BISHOP: c='b'; break;
                case PieceType::KNIGHT: c='n'; break;
                case PieceType::PAWN: c='p'; break;
                default: return ' ';
            }
            if (color == Color::WHITE) c += 'A' - 'a'; // -32
            return c;
        }
		constexpr bool operator==(const Piece &other) const { 
            return this->color == other.color && type == other.type;
        }
        constexpr bool isEmpty() const {
            return this->color == Color::NONE && this->type == PieceType::NONE;
        }
};

constexpr Piece EMPTY_SQUARE = {PieceType::NONE, Color::NONE};
constexpr Piece WHITE_KING = {PieceType::KING, Color::WHITE};
constexpr Piece BLACK_KING = {PieceType::KING, Color::BLACK};
constexpr Piece WHITE_QUEEN = {PieceType::QUEEN, Color::WHITE};
constexpr Piece BLACK_QUEEN = {PieceType::QUEEN, Color::BLACK};
constexpr Piece WHITE_ROOK = {PieceType::ROOK, Color::WHITE};
constexpr Piece BLACK_ROOK = {PieceType::ROOK, Color::BLACK};
constexpr Piece WHITE_BISHOP = {PieceType::BISHOP, Color::WHITE};
constexpr Piece BLACK_BISHOP = {PieceType::BISHOP, Color::BLACK};
constexpr Piece WHITE_KNIGHT = {PieceType::KNIGHT, Color::WHITE};
constexpr Piece BLACK_KNIGHT = {PieceType::KNIGHT, Color::BLACK};
constexpr Piece WHITE_PAWN = {PieceType::PAWN, Color::WHITE};
constexpr Piece BLACK_PAWN = {PieceType::PAWN, Color::BLACK};

static_assert(WHITE_KING.symbol() == 'K');
static_assert(WHITE_QUEEN.symbol() == 'Q');
static_assert(WHITE_ROOK.symbol() == 'R');
static_assert(WHITE_BISHOP.symbol() == 'B');
static_assert(WHITE_KNIGHT.symbol() == 'N');
static_assert(WHITE_PAWN.symbol() == 'P');

static_assert(BLACK_KING.symbol() == 'k');
static_assert(BLACK_QUEEN.symbol() == 'q');
static_assert(BLACK_ROOK.symbol() == 'r');
static_assert(BLACK_BISHOP.symbol() == 'b');
static_assert(BLACK_KNIGHT.symbol() == 'n');
static_assert(BLACK_PAWN.symbol() == 'p');

static_assert(EMPTY_SQUARE.symbol() == ' ');
static_assert(EMPTY_SQUARE.isEmpty());
static_assert(!WHITE_KING.isEmpty());
static_assert(!BLACK_KING.isEmpty());
static_assert(!WHITE_QUEEN.isEmpty());
static_assert(!BLACK_QUEEN.isEmpty());
static_assert(!WHITE_ROOK.isEmpty());
static_assert(!BLACK_ROOK.isEmpty());
static_assert(!WHITE_BISHOP.isEmpty());
static_assert(!BLACK_BISHOP.isEmpty());
static_assert(!WHITE_KNIGHT.isEmpty());
static_assert(!BLACK_KNIGHT.isEmpty());
static_assert(!WHITE_PAWN.isEmpty());
static_assert(!BLACK_PAWN.isEmpty());


constexpr int pieceNum(char c) {
    char u = (c >= 'a' && c <= 'z') ? c - 32 : c;
    switch (u) {
        case 'K': return 5;
        case 'Q': return 4;
        case 'R': return 3;
        case 'B': return 2;
        case 'N': return 1;
        case 'P': return 0;
        default: 
            assert(false && "invalid piece char "); return 6;
    }
}
constexpr Piece getPiece(char c) {
	switch(c) {
        case 'K': return WHITE_KING;
        case 'Q': return WHITE_QUEEN;
        case 'R': return WHITE_ROOK;
        case 'B': return WHITE_BISHOP;
        case 'N': return WHITE_KNIGHT;
        case 'P': return WHITE_PAWN;

        case 'k': return BLACK_KING;
        case 'q': return BLACK_QUEEN;
        case 'r': return BLACK_ROOK;
        case 'b': return BLACK_BISHOP;
        case 'n': return BLACK_KNIGHT;
        case 'p': return BLACK_PAWN;
        case '\0': case '.': case ' ': return EMPTY_SQUARE;
        default: 
            assert(false && (("unknown piece type: " + std::to_string(c)).c_str()));
            return {PieceType::KING, Color::NONE}; // an invalid piece
    }
}
static_assert(getPiece('K') == WHITE_KING);
static_assert(getPiece('Q') == WHITE_QUEEN);
static_assert(getPiece('R') == WHITE_ROOK);
static_assert(getPiece('B') == WHITE_BISHOP);
static_assert(getPiece('N') == WHITE_KNIGHT);
static_assert(getPiece('P') == WHITE_PAWN);

static_assert(getPiece('k') == BLACK_KING);
static_assert(getPiece('q') == BLACK_QUEEN);
static_assert(getPiece('r') == BLACK_ROOK);
static_assert(getPiece('b') == BLACK_BISHOP);
static_assert(getPiece('n') == BLACK_KNIGHT);
static_assert(getPiece('p') == BLACK_PAWN);

static_assert(getPiece(' ') == EMPTY_SQUARE);
static_assert(getPiece('.') == EMPTY_SQUARE);
static_assert(getPiece('\0') == EMPTY_SQUARE);



#endif
