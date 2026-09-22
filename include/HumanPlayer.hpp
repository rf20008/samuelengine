#ifndef CS3520_MINIPROJECT_HUMANPLAYER
#define CS3520_MINIPROJECT_HUMANPLAYER
#include "AbstractPlayer.hpp"
#include "ChessBoard.hpp"

bool tryParseMove(ChessBoard &board, const std::string &raw, Move &out);

class HumanPlayer : public AbstractPlayer {
	public:
		HumanPlayer();
		virtual Move getMove(const ChessBoard &);
		virtual ~HumanPlayer() {}
};
#endif
