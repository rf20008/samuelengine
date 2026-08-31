
#include "board-display.hpp"
#include <cxxtest/TestSuite.h>
#include <iostream>
class TestBoardDisplay : public CxxTest::TestSuite {
	public:
		void testDisplayBasicBoard() {
			TS_ASSERT_EQUALS(ChessUI::renderBoard(ChessBoard()), "8 ♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜\n"
																 "7 ♟ ♟ ♟ ♟ ♟ ♟ ♟ ♟\n"
																 "6 . . . . . . . .\n"
																 "5 . . . . . . . .\n"
																 "4 . . . . . . . .\n"
																 "3 . . . . . . . .\n"
																 "2 ♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙\n"
																 "1 ♖ ♘ ♗ ♕ ♔ ♗ ♘ ♖\n"
																 "  a b c d e f g h\n");
		}
};
