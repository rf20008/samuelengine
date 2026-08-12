#ifndef CS3520_MINIPROJECT_ABSTRACTPLAYER
#define CS3520_MINIPROJECT_ABSTRACTPLAYER
#include "ChessBoard.hpp"
#include "Move.hpp"
class AbstractPlayer {
	public:
		AbstractPlayer();
		virtual Move getMove(const ChessBoard &) = 0;
		virtual ~AbstractPlayer() {}
};
#endif
