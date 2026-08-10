#include <cxxtest/TestSuite.h>
#include "Board.hpp"

class TestBoard: public CxxTest::TestSuite
{
	public:
		void testSanityCheck(){
			TS_ASSERT_EQUALS(1 + 1, 2);
		}
};
