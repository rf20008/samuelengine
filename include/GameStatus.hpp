#ifndef CS3520_MINI_PROJECT_GAMESTATUS
#define CS3520_MINI_PROJECT_GAMESTATUS
enum class GameStatus {
    WHITE_WON,
    BLACK_WON,
    DRAW,
    STILL_GOING_WHITE_TURN,
    STILL_GOING_BLACK_TURN,
};

inline bool isGameOver(const GameStatus& status) {
    return status==GameStatus::WHITE_WON || status == GameStatus::BLACK_WON || status == GameStatus::DRAW;
}
#endif