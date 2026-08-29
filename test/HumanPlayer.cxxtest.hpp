#include <HumanPlayer.hpp>
#include <cxxtest/TestSuite.h>

class TestHumanPlayer : public CxxTest::TestSuite {
	public:
		void testTryParseMoveInvalid() {
			// empty string
			const Move myMove = {{1, 2}, {3, 4}}; // arbitrary; feel free to change this
			Move m = myMove;
			TS_ASSERT_EQUALS(tryParseMove("", m), false);
			TS_ASSERT_EQUALS(m, myMove);

			// nonempty, too short
			m = myMove;
			TS_ASSERT_EQUALS(tryParseMove("hi", m), false);
			TS_ASSERT_EQUALS(m, myMove);

			// nonempty, too long
			m = myMove;
			TS_ASSERT_EQUALS(tryParseMove("lorem ipsum dolor sit amet consectetur adipiscing elit", m), false);
			TS_ASSERT_EQUALS(m, myMove);

			// fileFrom too low
			m = myMove;
			TS_ASSERT_EQUALS(tryParseMove("`2e4", m), false);
			TS_ASSERT_EQUALS(m, myMove);

			// fileFrom too high
			m = myMove;
			TS_ASSERT_EQUALS(tryParseMove("{2e4", m), false);
			TS_ASSERT_EQUALS(m, myMove);

			// fileTo too low
			m = myMove;
			TS_ASSERT_EQUALS(tryParseMove("e2`4", m), false);
			TS_ASSERT_EQUALS(m, myMove);

			// fileTo too high
			m = myMove;
			TS_ASSERT_EQUALS(tryParseMove("e2{4", m), false);
			TS_ASSERT_EQUALS(m, myMove);

			// rankFrom too low
			m = myMove;
			TS_ASSERT_EQUALS(tryParseMove("e/e4", m), false);
			TS_ASSERT_EQUALS(m, myMove);

			// rankFrom too high
			m = myMove;
			TS_ASSERT_EQUALS(tryParseMove("e:e4", m), false);
			TS_ASSERT_EQUALS(m, myMove);

			// rankTo too low
			m = myMove;
			TS_ASSERT_EQUALS(tryParseMove("e2e/", m), false);
			TS_ASSERT_EQUALS(m, myMove);

			// rankTo too high
			m = myMove;
			TS_ASSERT_EQUALS(tryParseMove("e2e:", m), false);
			TS_ASSERT_EQUALS(m, myMove);
		}
};
