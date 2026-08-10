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

};
