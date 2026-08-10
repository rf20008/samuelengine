#ifndef CS3520_MINI_PROJECT_FEN
#define CS3520_MINI_PROJECT_FEN
#include <vector>
#include <optional>
#include <string>
#include <memory>
#include "Piece.hpp"
#include "Square.hpp"
#include "PlayerState.hpp"


namespace ParsePieces {
    std::vector<std::vector<std::shared_ptr<Piece>>> parsePiecePart(const std::string& PiecePart);
    bool parsePlayerPart(const std::string& PlayerPart);
    std::pair<PlayerState, PlayerState> parseCastlingPart(const std::string& CastlingPart);
    std::optional<Square> parseEnPassantPart(std::string EnPassantPart);
    std::string getPiecePart(const std::vector<std::vector<PiecePtr>>& pieces);
    std::string getPlayerPart(bool whiteToMove) {return (whiteToMove?"w":"b");}
    std::string getCastlingPart(const PlayerState& whiteState, const PlayerState& blackState);
    std::string getEnPassantPart(std::optional<Square>);
}
#endif