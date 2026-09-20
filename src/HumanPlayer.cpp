#include "HumanPlayer.hpp"
#include "ChessBoard.hpp"

#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>

// Parses text like "e2e4" or "e2 e4" into a Move. Returns false (leaving
// `out` untouched) if `raw` isn't a well-formed <file><rank><file><rank>
// move, e.g. wrong length or a square off the board.
bool tryParseMove(
    ChessBoard &board,
    const std::string &raw,
    Move &out)
{
    std::string s;
    for (char c : raw) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            s += c;
        }
    }

    // Try coordinate notation: e2e4
    if (s.size() == 4) {
        char fileFrom = static_cast<char>(std::tolower(
            static_cast<unsigned char>(s[0])));
        char rankFrom = s[1];
        char fileTo = static_cast<char>(std::tolower(
            static_cast<unsigned char>(s[2])));
        char rankTo = s[3];

        if (fileFrom >= 'a' && fileFrom <= 'h' &&
            fileTo >= 'a' && fileTo <= 'h' &&
            rankFrom >= '1' && rankFrom <= '8' &&
            rankTo >= '1' && rankTo <= '8') {

            Square start(fileFrom - 'a', rankFrom - '1');
            Square end(fileTo - 'a', rankTo - '1');
            Move candidate{start, end};

            // Make sure we return the actual legal Move, including
            // its MoveType.
            for (const Move &move : board.allLegalMoves()) {
                if (move.startingSquare == candidate.startingSquare &&
                    move.endingSquare == candidate.endingSquare) {
                    out = move;
                    return true;
                }
            }
        }
    }

    // Try SAN, e.g. e4, Nf3, O-O, Qxe5+, etc.
    try {
        out = board.getMove(s);
        return true;
    }
    catch (const std::exception &) {
        return false;
    }
}
HumanPlayer::HumanPlayer() {}

Move HumanPlayer::getMove(const ChessBoard &board) {
	// isMoveLegal() isn't const in ChessBoard's current interface, even
	// though checking legality doesn't change the board. Rather than change
	// that shared signature out from under Joshua's half of the work, we
	// const_cast here -- getMove is only ever handed a real, mutable board
	// by the caller, so this doesn't cost us any actual safety.
    ChessBoard mutableBoard = board;
	Move move;
	while (true) {
        // we need to consider flags!!!
        /*for (Move m : mutableBoard.allLegalMoves()) {
            std::cout<<m.debugString()<<std::endl;
        }*/
		std::cout << "Enter your move (e.g. e2e4): ";
		std::string line;
		if (!std::getline(std::cin, line)) {
			throw std::runtime_error("HumanPlayer::getMove: no more input available");
		}

		if (!tryParseMove(mutableBoard, line, move)) {
			std::cout << "I couldn't understand that move. Use the form <file><rank><file><rank>, e.g. e2e4." << std::endl;
			continue;
		}
        //std::cout<<"Parsed move: "<<move.debugString()<<std::endl;
		if (!mutableBoard.isMoveLegal(move)) {
			std::cout << "That move isn't legal. Try again." << std::endl;
			continue;
		}

		return move;
	}
}
