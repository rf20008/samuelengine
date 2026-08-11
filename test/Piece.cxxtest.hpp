#include <cxxtest/TestSuite.h>
#include "Piece.hpp"
#include "Bishop.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Pawn.hpp"
#include "Queen.hpp"
#include "Rook.hpp"

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
		void testWhiteKing(){
			King k = King(true);
			TS_ASSERT_EQUALS(k.getBelongsToWhite(), true);
			TS_ASSERT_EQUALS(k.symbol(), 'K');
		}
		void testBlackKing(){
			King k = King(false);
			TS_ASSERT_EQUALS(k.getBelongsToWhite(), false);
			TS_ASSERT_EQUALS(k.symbol(), 'k');
		}
		void testWhiteKnight(){
			Knight n = Knight(true);
			TS_ASSERT_EQUALS(n.getBelongsToWhite(), true);
			TS_ASSERT_EQUALS(n.symbol(), 'N');
		}
		void testBlackKnight(){
			Knight n = Knight(false);
			TS_ASSERT_EQUALS(n.getBelongsToWhite(), false);
			TS_ASSERT_EQUALS(n.symbol(), 'n');
		}
		void testWhitePawn(){
			Pawn p = Pawn(true);
			TS_ASSERT_EQUALS(p.getBelongsToWhite(), true);
			TS_ASSERT_EQUALS(p.symbol(), 'P');
		}
		void testBlackPawn(){
			Pawn p = Pawn(false);
			TS_ASSERT_EQUALS(p.getBelongsToWhite(), false);
			TS_ASSERT_EQUALS(p.symbol(), 'p');
		}
		void testWhiteQueen(){
			Queen q = Queen(true);
			TS_ASSERT_EQUALS(q.getBelongsToWhite(), true);
			TS_ASSERT_EQUALS(q.symbol(), 'Q');
		}
		void testBlackQueen(){
			Queen q = Queen(false);
			TS_ASSERT_EQUALS(q.getBelongsToWhite(), false);
			TS_ASSERT_EQUALS(q.symbol(), 'q');
		}
		void testWhiteRook(){
			Rook r = Rook(true);
			TS_ASSERT_EQUALS(r.getBelongsToWhite(), true);
			TS_ASSERT_EQUALS(r.symbol(), 'R');
		}
		void testBlackRook(){
			Rook r = Rook(false);
			TS_ASSERT_EQUALS(r.getBelongsToWhite(), false);
			TS_ASSERT_EQUALS(r.symbol(), 'r');
		}
};
