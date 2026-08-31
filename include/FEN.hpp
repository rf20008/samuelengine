#ifndef CS3520_MINI_PROJECT_FEN
#define CS3520_MINI_PROJECT_FEN
#include "Piece.hpp"
#include "PlayerState.hpp"
#include "Square.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace ParsePieces {
void parsePiecePart(const std::string &PiecePart, Piece out[128]);
Color parsePlayerPart(const std::string &PlayerPart);
std::pair<PlayerState, PlayerState> parseCastlingPart(const std::string &CastlingPart);
std::optional<Square> parseEnPassantPart(std::string EnPassantPart);
std::string getPiecePart(const Piece pieces[128]);
inline std::string getPlayerPart(Color playerToMove) { 
    assert(playerToMove != Color::NONE); 
    return ((playerToMove==Color::WHITE) ? "w" : "b"); 
}
std::string getCastlingPart(const PlayerState &whiteState, const PlayerState &blackState);
std::string getEnPassantPart(std::optional<Square>);
} // namespace ParsePieces
#endif