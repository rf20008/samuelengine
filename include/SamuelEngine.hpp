#ifndef CS3520_MINIPROJECT_SAMUELENGINE
#define CS3520_MINIPROJECT_SAMUELENGINE
#include "Move.hpp"
#include "AbstractPlayer.hpp"
#include "ChessBoard.hpp"

// to be done by Samuel
class SamuelEngine : public AbstractPlayer {
public:
    SamuelEngine();
    virtual Move getMove(const ChessBoard&);
};
#endif