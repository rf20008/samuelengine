#include <HumanPlayer.hpp>
#include <cxxtest/TestSuite.h>

class TestHumanPlayer : public CxxTest::TestSuite {
	public:
		void testTryParseMoveInvalid(){
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
			
			// fileFrom too high

			// fileTo too low
			
			// fileTo too high

			// rankFrom too low

			// rankFrom too high

			// rankTo too low

			// rankTo too high
		}
};
