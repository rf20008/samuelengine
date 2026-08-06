#include <cxxtest/TestSuite.h>
#include "Square.hpp"

class TestSquare : public CxxTest::TestSuite
{
public:
    void testDefaultConstructor()
    {
        Square basicSquare = Square();
        TS_ASSERT_EQUALS(basicSquare.row, 0);
        TS_ASSERT_EQUALS(basicSquare.col, 0);
    }
    void testConstructor() {
        Square sq3 = Square(3,8);
        TS_ASSERT_EQUALS(sq3.row, 3);
        TS_ASSERT_EQUALS(sq3.col, 8);
    }
    void testAdd() {
        Square sq3 = Square(3,8) + Square(-1,2);
        TS_ASSERT_EQUALS(sq3, Square(3 + (-1), 8 + 2));
    }
    void testEquals() {
        TS_ASSERT(Square(3,8) == Square(3,8));
        TS_ASSERT(!(Square(3,7) == Square(3,8)));
        TS_ASSERT(!(Square(4, 8) == Square(3,8)));
    }
    void testNotEquals() {
        TS_ASSERT(!(Square(3,8) != Square(3,8)));
        TS_ASSERT(Square(3,7) != Square(3,8));
        TS_ASSERT(Square(4, 8) != Square(3,8));
    }
    void testLessThan() {
        TS_ASSERT(!(Square(3,8) < Square(3,8)));
        TS_ASSERT(Square(2,8) < Square(3,8));
        TS_ASSERT(Square(3,7) < Square(3,8));
        TS_ASSERT(!(Square(4,8) < Square(3, 1)));
    }
};
