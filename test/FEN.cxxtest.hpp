#ifndef CS3520_FEN_CXXTEST_HPP
#define CS3520_FEN_CXXTEST_HPP
#include "FEN.hpp"

#include "cxxtest/TestSuite.h"
using namespace ParsePieces;

class TestFEN : public CxxTest::TestSuite {
	public:
		void testPlayerGet() {
			TS_ASSERT_EQUALS(true, ParsePieces::parsePlayerPart("w"));
			TS_ASSERT_EQUALS(false, ParsePieces::parsePlayerPart("b"));
			TS_ASSERT_THROWS_ANYTHING(ParsePieces::parsePlayerPart(""));
			TS_ASSERT_THROWS_ANYTHING(parsePlayerPart("t"));
			TS_ASSERT_THROWS_ANYTHING(parsePlayerPart("ttt"));
		}
        void testGetPlayerPart() {
            TS_ASSERT_EQUALS(getPlayerPart(true), "w");
            TS_ASSERT_EQUALS(getPlayerPart(false), "b");
        }
        void testParseCastlingPart() {
            TS_ASSERT_EQUALS(parseCastlingPart("KQkq"), std::make_pair(PlayerState(true, true), PlayerState(true, true)));
            TS_ASSERT_EQUALS(parseCastlingPart("-"), std::make_pair(PlayerState(false, false), PlayerState(false, false)));
            TS_ASSERT_EQUALS(parseCastlingPart("KQ"), std::make_pair(PlayerState(true, true), PlayerState(false, false)));
            TS_ASSERT_EQUALS(parseCastlingPart("Kk"), std::make_pair(PlayerState(false, true), PlayerState(false, true)));
        }

        void testGetCastlingPart() {
            TS_ASSERT_EQUALS(getCastlingPart(PlayerState(true, true), PlayerState(true, true)), "KQkq");
            TS_ASSERT_EQUALS(getCastlingPart(PlayerState(false, false), PlayerState(false, false)), "-");
            TS_ASSERT_EQUALS(getCastlingPart(PlayerState(true, true), PlayerState(false, false)), "KQ");
            TS_ASSERT_EQUALS(getCastlingPart(PlayerState(false, true), PlayerState(false, true)), "Kk");
        }
        void testGetEnPassantPart() {
            TS_ASSERT_EQUALS(parseEnPassantPart("e3"), std::optional<Square>(Square("e3")));
            TS_ASSERT_THROWS_ANYTHING(parseEnPassantPart("iurektwnv"));
            TS_ASSERT_THROWS_ANYTHING(parseEnPassantPart("h9"));
            TS_ASSERT_THROWS_ANYTHING(parseEnPassantPart("9h"));
            TS_ASSERT_THROWS_ANYTHING(parseEnPassantPart(""));
        }
};  
#endif
