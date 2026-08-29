// TUI entry point. Supports playing against another human on the same
// computer, or against SamuelEngine.

#include "ChessBoard.hpp"
#include "GameStatus.hpp"
#include "HumanPlayer.hpp"
#include "SamuelEngine.hpp"

#include "board-display.hpp" // lib/: TUI rendering of the board

#include <cctype>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace {
const double ENGINE_TIME_LIMIT_SECONDS = 0.1;

std::unique_ptr<AbstractPlayer> makePlayer(const std::string &colorName) {
	while (true) {
		std::cout << "Who plays " << colorName << "? [h]uman or [e]ngine: ";
		std::string choice;
		if (!std::getline(std::cin, choice)) {
			throw std::runtime_error("main: no more input available while choosing players");
		}
		if (choice.empty()) {
			continue;
		}
		switch (std::tolower(static_cast<unsigned char>(choice[0]))) {
		case 'h':
			return std::make_unique<HumanPlayer>();
		case 'e':
			return std::make_unique<SamuelEngine>(ENGINE_TIME_LIMIT_SECONDS);
		default:
			std::cout << "Please enter 'h' or 'e'." << std::endl;
		}
	}
}

void printResult(GameStatus status) {
	switch (status) {
	case GameStatus::WHITE_WON:
		std::cout << "Checkmate -- White wins!" << std::endl;
		break;
	case GameStatus::BLACK_WON:
		std::cout << "Checkmate -- Black wins!" << std::endl;
		break;
	case GameStatus::DRAW:
		std::cout << "Draw." << std::endl;
		break;
	default:
		break; // game's still going; nothing to announce
	}
}
} // namespace

int main(int argc, char **argv) {
	(void)argc;
	(void)argv;

	std::cout << "Welcome to Samuel's Chess Engine!" << std::endl;
	std::unique_ptr<AbstractPlayer> whitePlayer = makePlayer("White");
	std::unique_ptr<AbstractPlayer> blackPlayer = makePlayer("Black");

	ChessBoard board;

	while (true) {
		std::cout << std::endl << ChessUI::renderBoard(board) << std::endl;

		GameStatus status = board.getStatus();
		if (isGameOver(status)) {
			printResult(status);
			break;
		}

		bool whiteToMove = board.get_whiteToMove();
		AbstractPlayer &toMove = whiteToMove ? *whitePlayer : *blackPlayer;
		std::cout << "Move #" << board.get_fullmove_clock() << ": " << (whiteToMove ? "White" : "Black") << " to move." << std::endl;
		Move move = toMove.getMove(board);
		board.processMove(move);
	}

	return 0;
}
