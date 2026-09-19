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
		constexpr Move() : startingSquare("a1"), endingSquare("a1"), promotion('\0') {}
		constexpr Move(const Square start, const Square end, const char promo = '\0', const MoveType movetype = MoveType::NORMAL) : startingSquare(start), endingSquare(end), promotion(promo), type(movetype) {}

		constexpr Move(const Move &other) = default;
		constexpr Move(Move &&other) = default;
		constexpr Move &operator=(const Move &other) = default;
		constexpr Move &operator=(Move &&other) = default;
		constexpr ~Move() = default;
		constexpr bool operator<(const Move &other) const {
            if (this->startingSquare != other.startingSquare) {
                return (this->startingSquare < other.startingSquare);
            }
            if (this->endingSquare != other.endingSquare) {
                return (this->endingSquare < other.endingSquare);
            }
            return (this->promotion < other.promotion);
        }
		constexpr bool operator==(const Move &other) const {return startingSquare==other.startingSquare && endingSquare ==other.endingSquare && promotion == other.promotion && type == other.type;}
		constexpr std::string operator()() const {
            std::string startEnd = startingSquare.operator()() + endingSquare.operator()();
            if (promotion)
                startEnd += std::string(1, promotion);
            return startEnd;
        }
        std::string debugString() const {
            return "START=" + startingSquare.toString() + ", END=" + endingSquare.toString() + ", PROMO="+ (promotion ? std::string(1, promotion) : "0") + ", TYPE="+ std::to_string(static_cast<int>(type));
        }
};
#endif
