#include <vector>
#include <optional>
#include <string>
#include <sstream>
#include <memory>
#include "Errors.hpp"
#include "Square.hpp"
#include "PlayerState.hpp"
#include "Piece.hpp"
#include "GetPiece.hpp"
namespace ParsePieces {
    std::vector<std::vector<PiecePtr>> parsePiecePart(const std::string& PiecePart) {
        std::stringstream RankReader(PiecePart);
        std::vector<std::string> Ranks;
        Ranks.resize(BOARD_SIZE);
        for (size_t ranknum = 0; ranknum<BOARD_SIZE; ++ranknum) {
            getline(RankReader, Ranks.at(BOARD_SIZE-ranknum-1), '/'); // FEN reads from rank 8 to rank 1
        }
        std::vector<std::vector<PiecePtr>> board;
        for (size_t ranknum = 0; ranknum<Ranks.size(); ranknum++) {
            std::string Rank = Ranks.at(ranknum);
            board.push_back(std::vector<PiecePtr>());
            for (char pieceChar : Rank) {
                // if it's a digit
                if (pieceChar >= '0' && pieceChar <= '9') { // piecechar is a digit
                    int digitNum = pieceChar-'0';
                    for (int i = 0; i<digitNum; ++i) { // add that many free spaces
                        board.at(ranknum).push_back(std::shared_ptr<Piece>()); // a null piece
                    }
                    continue;
                } 
                // this is a piece! add it

                board.at(ranknum).push_back(getPiece(pieceChar));
            }
            // check that it is of size BOARD_SIZE
            if (board.at(ranknum).size() != BOARD_SIZE) {
                throw InvalidFEN("Error: Board rank " + std::to_string(ranknum) + " is not of size 8, but of size " + std::to_string(board.at(ranknum).size()) + ".");
            }
        }
        if (board.size() != BOARD_SIZE) {
            throw InvalidFEN("Board does not have 8 ranks");
        }
        return board;
    }
    bool parsePlayerPart (const std::string& PlayerPart) {
        if (PlayerPart.size() != 1) {
            throw InvalidFEN("Error: Player argument must be 1 character");
        }
        char PlayerChar = PlayerPart[0];
        if (PlayerChar != 'w' && PlayerChar != 'b') throw InvalidFEN("Error: Player Argument must be either \"w\" or \"b\".");
        return (PlayerChar == 'w');
    }
    std::pair<PlayerState, PlayerState> parseCastlingPart(const std::string& CastlingPart) {
        PlayerState whiteState{false, false};
        PlayerState blackState{false, false};
        if (CastlingPart == "-") {return {whiteState, blackState};}
        for (char c : CastlingPart) {
            switch (c) {
                case 'K': whiteState.canKingsideCastle = true;
                case 'Q': whiteState.canQueensideCastle = true;
                case 'k': blackState.canKingsideCastle = true;
                case 'q': blackState.canQueensideCastle = true;
                default: throw InvalidFEN("Unknown castling character " + std::string(c, 1));
            }
        }
        return {whiteState, blackState};
    }
    std::optional<Square> parseEnPassantPart(std::string EnPassantPart) {
        if (EnPassantPart == "-") {
            return std::optional<Square>();
        }
        std::optional<Square> sq(EnPassantPart);
        if (!sq->isValid()) throw InvalidFEN("Square is out of bounds");
        return sq;
    }
    std::string getPiecePart(const std::vector<std::vector<PiecePtr>>& pieces) {
        throw NotImplementedError("std::string getPiecePart(const std::vector<std::vector<PiecePtr>>& pieces) is not implemented yet");
    }
    std::string getCastlingPart(const PlayerState& whiteState, const PlayerState& blackState) {
        throw NotImplementedError("std::string getCastlingPart(const PlayerState& whiteState, const PlayerState& blackState) is not implemented yet");
    }
    std::string getEnPassantPart(std::optional<Square> square) {
        if (!square) {return "-"};
        return square->to_string();
    }
}