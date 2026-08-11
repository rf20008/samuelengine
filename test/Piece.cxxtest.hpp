#include <cxxtest/TestSuite.h>
#include "Piece.hpp"
#include "Bishop.hpp"

class TestPiece: public CxxTest::TestSuite{
	public:
		void testWhiteBishop(){
			Bishop b = Bishop(true);
			TS_ASSERT_EQUALS(b.getBelongsToWhite(), true);
			TS_ASSERT_EQUALS(b.symbol(), 'B');
		}
		void testBlackBishop(){
			Bishop b = Bishop(false);
			TS_ASSERT_EQUALS(b.getBelongsToWhite(), false);
			TS_ASSERT_EQUALS(b.symbol(), 'b');
		}
};
