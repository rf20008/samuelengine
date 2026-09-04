#include "SamuelEngine.hpp"
#include <cxxtest/TestSuite.h>

class TestSamuelEngine : public CxxTest::TestSuite {
	public:
		void testPieceValue() {
			SamuelEngine e(1.0, false);
			TS_ASSERT_EQUALS(e.relative_value(getPieceFromSymbol('Q')), 9);
			TS_ASSERT_EQUALS(e.relative_value(getPieceFromSymbol('R')), 5);
			TS_ASSERT_EQUALS(e.relative_value(getPieceFromSymbol('B')), 3);
			TS_ASSERT_EQUALS(e.relative_value(getPieceFromSymbol('N')), 3);
			TS_ASSERT_EQUALS(e.relative_value(getPieceFromSymbol('P')), 1);
			//TS_ASSERT_THROWS_ANYTHING(e.relative_value(PiecePtr()));
			TS_ASSERT_EQUALS(e.relative_value(getPieceFromSymbol('q')), 9);
			TS_ASSERT_EQUALS(e.relative_value(getPieceFromSymbol('r')), 5);
			TS_ASSERT_EQUALS(e.relative_value(getPieceFromSymbol('b')), 3);
			TS_ASSERT_EQUALS(e.relative_value(getPieceFromSymbol('n')), 3);
			TS_ASSERT_EQUALS(e.relative_value(getPieceFromSymbol('p')), 1);
		}
};