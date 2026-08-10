#ifndef CS3520_MINIPROJECT_HUMANPLAYER
#define CS3520_MINIPROJECT_HUMANPLAYER
#include "AbstractPlayer.hpp"

class HumanPlayer : public AbstractPlayer {
public:
    HumanPlayer();
    virtual Move getMove(const ChessBoard&);
};
#endif