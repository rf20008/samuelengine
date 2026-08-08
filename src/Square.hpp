#ifndef CS3520_MINI_PROJECT_SQUARE
#define CS3520_MINI_PROJECT_SQUARE
const int BOARD_SIZE = 8;
struct Square {
    int row, col;
public:
    Square() : Square(1,1) {}
    Square(int r, int c) : row(r), col(c) {} 
    bool isValid() {return (row>=1 && col>=1 && row<=BOARD_SIZE && col<=BOARD_SIZE);}
    Square operator+(const Square& other) {
        return Square(row + other.row, col+other.col);
    }
    bool operator==(const Square& other) {
        return row==other.row && col==other.col;
    }
    bool operator<(const Square& other) {
        if (row != other.row) {
            return row<other.row;
        } else {
            return col<other.col;
        }
    }
    bool operator!=(const Square& other) {
        return !(this->operator==(other));
    }
    
};

#endif