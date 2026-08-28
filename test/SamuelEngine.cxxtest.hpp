#include "SamuelEngine.hpp"
#include "GetPiece.hpp"
#include <cxxtest/TestSuite.h>

class TestSamuelEngine : public CxxTest::TestSuite {
public:
    void testPieceValue() {
        SamuelEngine e(1.0, false);
        TS_ASSERT_EQUALS(e.relative_value(getPiece('Q')), 9);
        TS_ASSERT_EQUALS(e.relative_value(getPiece('R')), 5);
        TS_ASSERT_EQUALS(e.relative_value(getPiece('B')), 3);
        TS_ASSERT_EQUALS(e.relative_value(getPiece('N')), 3);
        TS_ASSERT_EQUALS(e.relative_value(getPiece('P')), 1);
        //TS_ASSERT_THROWS_ANYTHING(e.relative_value(PiecePtr()));
        TS_ASSERT_EQUALS(e.relative_value(getPiece('q')), 9);
        TS_ASSERT_EQUALS(e.relative_value(getPiece('r')), 5);
        TS_ASSERT_EQUALS(e.relative_value(getPiece('b')), 3);
        TS_ASSERT_EQUALS(e.relative_value(getPiece('n')), 3);
        TS_ASSERT_EQUALS(e.relative_value(getPiece('p')), 1);

    }

};