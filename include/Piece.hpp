#ifndef CS3520_MINIPROJECT_PIECE
#define CS3520_MINIPROJECT_PIECE

#include <memory>

constexpr char PIECETYPES[7] = {' ', 'k', 'q', 'r', 'b', 'n', 'p'};
enum class PieceType : uint8_t {
    NONE = 0,
    KING = 1,
    QUEEN = 2,
    ROOK = 3,
    BISHOP = 4,
    KNIGHT = 5,
    PAWN = 6,
}
enum class Color: uint8_t {
    NONE = 0,
    WHITE = 1,
    BLACK = 2,
}


struct Piece {
    public:
        PieceType type;
		Color color;
        
		Piece(PieceType type, Color c) : color(c), piecetype(type) {}
		constexpr bool getBelongsToWhite() const { return c == Color::WHITE; }
        constexpr char symbol() const {
            if (type == PieceType::NONE) return ' ';
            char c = PIECETYPES[static_cast<int>(type)];
            if (color == Color::WHITE) c += 'A' - 'a'; // -32
            return c;
        }
		constexpr bool operator==(const Piece &other) const { 
            return this->color == other.color && piecetype == other.piecetype;
        }
        constexpr operator bool() const {
            return this->color != Color::NONE || this->type != PieceType::NONE;
        }
};

constexpr Piece EMPTY_SQUARE = Piece{PieceType::NONE, Color::NONE};
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

static_assert(WHITE_KING.symbol(), 'K');
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

static_assert(EMPTY_PIECE.symbol() == ' ');
static_assert(!EMPTY_PIECE);

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
PiecePtr getPiece(char c) {
	switch(c):
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
            assert(false && ("unknown piece type: " + std::to_string(c)));
            return {PieceType::KING, Color::EMPTY}; // an invalid piece

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

static_assert(getPiece(' ') == EMPTY_PIECE);
static_assert(getPiece('.') == EMPTY_PIECE);
static_assert(getPiece('\0') == EMPTY_PIECE);

static_assert(WHITE_KING.symbol() == 'K');
static_assert(getPiece(WHITE_KING.symbol()) == WHITE_KING);

#endif
