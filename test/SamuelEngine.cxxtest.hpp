#include "SamuelEngine.hpp"
#include <cxxtest/TestSuite.h>

class TestSamuelEngine : public CxxTest::TestSuite {
	public:
		void testPieceValue() {
			SamuelEngine e(1.0, false);
            (void)e;
        }
};
