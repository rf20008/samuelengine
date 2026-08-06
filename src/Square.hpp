const int BOARD_SIZE = 8;
struct Square {
    int row, col;
public:
    Square() : Square(1,1);
    Square(int r, int c) : row(r), col(c) {} 
    bool isValid() {return (r>=1 && c>=1 && r<=BOARD_SIZE && c<=BOARD_SIZE)}
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