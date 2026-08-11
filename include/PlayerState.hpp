#ifndef CS3520_MINI_PROJECT_PLAYERSTATE
#define CS3520_MINI_PROJECT_PLAYERSTATE
struct PlayerState {
public:
    bool canQueensideCastle;
    bool canKingsideCastle;
    bool operator==(const PlayerState& other) const {
        return canQueensideCastle==other.canQueensideCastle && canKingsideCastle==other.canKingsideCastle;
    }
};
#endif