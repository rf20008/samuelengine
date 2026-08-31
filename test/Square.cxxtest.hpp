#include "Square.hpp"
#include <cxxtest/TestSuite.h>

class TestSquare : public CxxTest::TestSuite {
	public:
		// testing constructors
		void testDefaultConstructor() {
			Square basicSquare = Square();
			TS_ASSERT_EQUALS(basicSquare.rank(), 0);
			TS_ASSERT_EQUALS(basicSquare.file(), 0);
		}
		void testIndexConstructor() {
			Square s = Square(18);
			TS_ASSERT_EQUALS(s.rank(), 1);
			TS_ASSERT_EQUALS(s.file(), 2);
		}
		void testConstructor() {
			Square sq3 = Square(3, 8);
			TS_ASSERT_EQUALS(sq3.file(), 3);
			TS_ASSERT_EQUALS(sq3.rank(), 8);
		}
		void testStringViewConstructor() {
			Square s = Square("b5");
			TS_ASSERT_EQUALS(s.file(), 1);
			TS_ASSERT_EQUALS(s.rank(), 4);
		}
		void testCharArrayConstructor() {
			char a[2] = {'b', '5'};
			Square s = Square(a);
			TS_ASSERT_EQUALS(s.file(), 1);
			TS_ASSERT_EQUALS(s.rank(), 4);
		}

		// testing operators
		void testAdd() {
			Square sq3 = Square(3, 8) + Offset(2, -1);
			TS_ASSERT_EQUALS(sq3, Square(3 + (-1), 8 + 2));
		}
		void testAdditionAssignment() {
			Square s = Square(2, 5);
			s += 1;
			TS_ASSERT_EQUALS(s, Square(3, 5));
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
		void testSubtract() {
			int s = Square(1, 2) - Square(1, 2);
			TS_ASSERT_EQUALS(s, Offset(1 - 1, 2 - 2));
		}
		void testParentheses() {
			const Square s = Square(3, 5);
			TS_ASSERT_EQUALS(s(), "d6");

			const Square s2 = Square(73);
			TS_ASSERT_EQUALS(s2(), "??");
		}

		// testing methods
		void testToString() {
			// valid
			const Square s = Square(3, 5);
			TS_ASSERT_EQUALS(s.toString(), "d6");

			// invalid
			const Square s2 = Square(73);
			TS_ASSERT_EQUALS(s2.toString(), "??");
		}
		void testIsValid() {
			const Square s = Square(1);
			TS_ASSERT_EQUALS(s.isValid(), true);

			const Square s2 = Square(9);
			TS_ASSERT_EQUALS(s2.isValid(), false);
		}
		void testFile() {
			const Square s = Square(3 * 16 + 5);
			TS_ASSERT_EQUALS(s.file(), 5);
		}
		void testRank() {
			const Square s = Square(3 * 16 + 5);
			TS_ASSERT_EQUALS(s.rank(), 3);
		}
		void testTo64() {
			const Square s = Square(2, 5);
			TS_ASSERT_EQUALS(s.to64(), 2 + 5 * 8);
		}
};
