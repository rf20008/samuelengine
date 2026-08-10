#include <cxxtest/TestSuite.h>
#include <Move.hpp>

class TestMove: public CxxTest::TestSuite{
	public:
		void testMoveConstructor(){
			Square start = { 1, 2 };
			Square end = { 4, 8 };
			Move m = { start, end };
			TS_ASSERT_EQUALS(m.startingSquare, start);
			TS_ASSERT_EQUALS(m.endingSquare, end);
		}

		void testMoveComparison(){
			// chosen with the help of an RNG (you can change this if you wish)
			Move m0 = {{8, 9}, {0, 5}};
			Move m00 = {{0, 5}, {4, 2}};
			Move m01 = {{9, 6}, {4, 7}};
			Move m10 = {{8, 7}, {9, 0}};
			Move m11 = {{8, 12}, {7, 3}};
			Move m20 = {{8, 9}, {-1, 6}};
			Move m21 = {{8, 9}, {6, 2}};
			Move m30 = {{8, 9}, {0, 0}};
			Move m31 = {{8, 9}, {0, 6}};

			// row of startingSquare
			TS_ASSERT_EQUALS(m0 < m00, false);
			TS_ASSERT_EQUALS(m0 < m01, true);

			// col of startingSquare
			TS_ASSERT_EQUALS(m0 < m10, false);
			TS_ASSERT_EQUALS(m0 < m11, true);

			// row of endingSquare
			TS_ASSERT_EQUALS(m0 < m20, false);
			TS_ASSERT_EQUALS(m0 < m21, true);

			// col of endingSquare
			TS_ASSERT_EQUALS(m0 < m30, false);
			TS_ASSERT_EQUALS(m0 < m31, true);
			TS_ASSERT_EQUALS(m0 < m0, false);
		};
};
