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
            TS_ASSERT_EQUALS(ParseCastlingPart("KQkq"), {PlayerState(true, true), PlayerState(true, true)});
            TS_ASSERT_EQUALS(ParseCastlingPart("-"), {PlayerState(false, false), PlayerState{false, false}});
            TS_ASSERT_EQUALS(ParseCastlingPart("KQ"), {PlayerState(true, true), PlayerState(false, false)});
            TS_ASSERT_EQUALS(ParseCastlingPart("Kk"), {PlayerState(false, true), PlayerState(false, true)});
        }

        void test
};
#endif
