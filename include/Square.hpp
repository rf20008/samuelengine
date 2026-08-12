#ifndef CS3520_MINI_PROJECT_SQUARE
#define CS3520_MINI_PROJECT_SQUARE
#include <stdexcept>
#include <string>
const int BOARD_SIZE = 8;
struct Square {
		int row, col;

	public:
		Square() : Square(0, 0) {}
		Square(int r, int c) : row(r), col(c) {}
		Square(const char *sq) : Square(std::string(sq)) {}
		Square(const std::string &sq) {
			if (sq.size() != 2) {
				throw std::invalid_argument("Square Constructor must contain exactly 2 characters");
			}
			this->row = (sq[0] - 'a' + 1); // number of characters after a, plus 1
			this->col = (sq[1] - '1' + 1); // number of characters after '1', plus 1
		}
		bool isValid() { return (row >= 1 && col >= 1 && row <= BOARD_SIZE && col <= BOARD_SIZE); }
		Square operator+(const Square &other) const { return Square(row + other.row, col + other.col); }
		bool operator==(const Square &other) const { return row == other.row && col == other.col; }
		bool operator<(const Square &other) const {
			if (row != other.row) {
				return row < other.row;
			} else {
				return col < other.col;
			}
		}
		bool operator!=(const Square &other) const { return !(this->operator==(other)); }
		std::string toString() const {
			// note: number has to go first because the second argument is of type char, not char*
			return std::string(1, 'a' + this->row - 1) + std::string(1, '1' + this->col - 1);
		}
		Square operator-(const Square &other) const { return Square(row - other.row, col - other.col); }
		std::string operator()() const { return toString(); }
};
inline int maxNorm(Square sq) { return std::max(std::abs(sq.row), std::abs(sq.col)); }
#endif
