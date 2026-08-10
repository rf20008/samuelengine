#include <cxxtest/TestSuite.h>
#include "ChessBoard.hpp"

class TestBoard: public CxxTest::TestSuite
{
	public:
		void testChessBoardMemberConstructor(){
			std::vector<std::vector<std::shared_ptr<Piece>>> pieces = {};
			bool whiteToMove = true;
			PlayerState whitePlayerState = {true, true};
			PlayerState blackPlayerState = {true, true};
			int halfmove_clock = 0;
			int fullmove_clock = 0;
			Square enPassant_targetSquare = { -1, -1 };

			ChessBoard myBoard = ChessBoard(
				pieces,
				whiteToMove,
				whitePlayerState,
				blackPlayerState,
				halfmove_clock,
				fullmove_clock,
				enPassant_targetSquare
			);
			(void)myBoard;
		}
};
