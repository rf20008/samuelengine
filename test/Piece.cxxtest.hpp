// All tests are now in Piece (they are static asserts!)
#include <cxxtest/TestSuite.h>
#include "Piece.hpp"

class PieceTest : public CxxTest::TestSuite {
public:
    void testCompiles(void) {
        Piece p;
        TS_ASSERT(p.isEmpty()); // or p.type == PieceType::EMPTY
    }

    void testEquality(void) {
        Piece a(PieceType::ROOK, Color::WHITE);
        Piece b(PieceType::ROOK, Color::WHITE);
        TS_ASSERT_EQUALS(a, b);
    }
};