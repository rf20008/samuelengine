#include <Move.hpp>
#include <cxxtest/TestSuite.h>

class TestMove : public CxxTest::TestSuite {
	public:
		void testMoveConstructor() {
			Square start = {1, 2};
			Square end = {4, 8};
			Move m = {start, end};
			TS_ASSERT_EQUALS(m.startingSquare, start);
			TS_ASSERT_EQUALS(m.endingSquare, end);
		}

		void testMoveComparison() {
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
			Move m40 = {{8, 9}, {0, 5}, 'a'};

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

			// promotion
			TS_ASSERT_EQUALS(m0 < m40, true);
			TS_ASSERT_EQUALS(m40 < m0, false);
			TS_ASSERT_EQUALS(m0 < m0, false);
		};
		void testMoveEquality() {
			Move m = {{1, 2}, {3, 4}, '5'};
			Move m1 = {{3, 2}, {3, 4}, '5'};
			Move m2 = {{1, 4}, {3, 4}, '5'};
			Move m3 = {{1, 2}, {5, 4}, '5'};
			Move m4 = {{1, 2}, {3, 6}, '5'};
			Move m5 = {{1, 2}, {3, 4}, '7'};

			TS_ASSERT(m != m1);
			TS_ASSERT(m != m2);
			TS_ASSERT(m != m3);
			TS_ASSERT(m != m4);
			TS_ASSERT(m != m5);
			TS_ASSERT(m == m);
		};
};
