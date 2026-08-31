#ifndef CS3520_MINIPROJECT_MOVE
#define CS3520_MINIPROJECT_MOVE
#include "Square.hpp"
enum class MoveType : uint8_t {
    NORMAL,
    EN_PASSANT,
    CASTLING,
    DOUBLE_PAWN_PUSH // if you need it to set enPassant_target
};

struct Move {
		Square startingSquare;
		Square endingSquare;
		char promotion = '\0';
        MoveType type = MoveType::NORMAL;
		Move() : startingSquare("a1"), endingSquare("a1"), promotion('\0') {}
		Move(const Square start, const Square end, const char promo = '\0', const MoveType movetype = MoveType::NORMAL) : startingSquare(start), endingSquare(end), promotion(promo), type(movetype) {}

		Move(const Move &other) = default;
		Move(Move &&other) = default;
		Move &operator=(const Move &other) = default;
		Move &operator=(Move &&other) = default;
		~Move() = default;
		bool operator<(const Move &other) const;
		bool operator==(const Move &other) const = default;
		std::string operator()() const;
};
#endif
