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
void parsePiecePart(const std::string &PiecePart, PiecePtr out[128]);
bool parsePlayerPart(const std::string &PlayerPart);
std::pair<PlayerState, PlayerState> parseCastlingPart(const std::string &CastlingPart);
std::optional<Square> parseEnPassantPart(std::string EnPassantPart);
std::string getPiecePart(const PiecePtr pieces[128]);
inline std::string getPlayerPart(bool whiteToMove) { return (whiteToMove ? "w" : "b"); }
std::string getCastlingPart(const PlayerState &whiteState, const PlayerState &blackState);
std::string getEnPassantPart(std::optional<Square>);
} // namespace ParsePieces
#endif