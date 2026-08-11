#ifndef CS3520_MINI_PROJECT_SQUARE
#define CS3520_MINI_PROJECT_SQUARE
#include <string>
#include <stdexcept>
const int BOARD_SIZE = 8;
struct Square {
    int row, col;
public:
    Square() : Square(0, 0) {}
    Square(int r, int c) : row(r), col(c) {} 
    Square(std::string sq) {
        if (sq.size() != 2) {
            throw std::invalid_argument("Square Constructor must contain exactly 2 characters");
        }
        this->row = (sq[0] - 'a' + 1); // number of characters after a, plus 1
        this->col = (sq[1] - '1' + 1); // number of characters after '1', plus 1
    }
    bool isValid() {return (row>=1 && col>=1 && row<=BOARD_SIZE && col<=BOARD_SIZE);}
    Square operator+(const Square& other) const {
        return Square(row + other.row, col+other.col);
    }
    Square operator-(const Square& other) const {
        return Square(row - other.row, col - other.col);
    }
    bool operator==(const Square& other) const {
        return row==other.row && col==other.col;
    }
    bool operator<(const Square& other) const {
        if (row != other.row) {
            return row<other.row;
        } else {
            return col<other.col;
        }
    }
    bool operator!=(const Square& other) const {
        return !(this->operator==(other));
    }
    std::string toString() {
        return std::string('a' + this->row - 1, 1) + std::string('1' + this->col - 1, 1);
    }
    Square operator-(const Square& other) const {
        return Square(row-other.row, col-other.col);
    }
};
#endif
