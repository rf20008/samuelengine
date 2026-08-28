#include "Square.hpp"
#include <cxxtest/TestSuite.h>

class TestSquare : public CxxTest::TestSuite {
	public:
		void testDefaultConstructor() {
			Square basicSquare = Square();
			TS_ASSERT_EQUALS(basicSquare.rank(), 0);
			TS_ASSERT_EQUALS(basicSquare.file(), 0);
		}
		void testConstructor() {
			Square sq3 = Square(3, 8);
			TS_ASSERT_EQUALS(sq3.rank(), 3);
			TS_ASSERT_EQUALS(sq3.file(), 8);
		}
		void testAdd() {
			Square sq3 = Square(3, 8) + Offset(-1, 2);
			TS_ASSERT_EQUALS(sq3, Square(3 + (-1), 8 + 2));
		}
		void testEquals() {
			TS_ASSERT(Square(3, 8) == Square(3, 8));
			TS_ASSERT(!(Square(3, 7) == Square(3, 8)));
			TS_ASSERT(!(Square(4, 8) == Square(3, 8)));
		}
		void testNotEquals() {
			TS_ASSERT(!(Square(3, 8) != Square(3, 8)));
			TS_ASSERT(Square(3, 7) != Square(3, 8));
			TS_ASSERT(Square(4, 8) != Square(3, 8));
		}
		void testLessThan() {
			TS_ASSERT(!(Square(3, 8) < Square(3, 8)));
			TS_ASSERT(Square(2, 8) < Square(3, 8));
			TS_ASSERT(Square(3, 7) < Square(3, 8));
			TS_ASSERT(!(Square(4, 8) < Square(3, 1)));
		}
		void testToString() {
			Square s = Square(4, 6);
			TS_ASSERT_EQUALS(s.toString(), "d6");
		}
		void testSubtract() {
			Offset s = Square(1, 2) - Square(1, 2);
			TS_ASSERT_EQUALS(s, Offset(1 - 1, 2 - 2));
		}
};
