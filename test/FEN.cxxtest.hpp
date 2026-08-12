#ifndef CS3520_FEN_CXXTEST_HPP
#define CS3520_FEN_CXXTEST_HPP
#include "FEN.hpp"
#include <ranges>

#include "cxxtest/TestSuite.h"
using namespace ParsePieces;

class TestFEN : public CxxTest::TestSuite {
	public:
		void testParsePiecePart() {
			// generated using http://bernd.bplaced.net/fengenerator/fengenerator.html; feel free to change this
			const std::string myPiecePart =
				"5r2/nK6/6Pk/1b2p3/N2p1pP1/3p2Pp/3B4/1Q6";
			// TODO: investigate why parsedPieces returns an upside down board
			const std::vector<std::string> groundTruth = {
				" Q      ", "   B    ", "   p  Pp", "N  p pP ",
				" b  p   ", "      Pk", "nK      ", "     r  ",
			};

			const std::vector<std::vector<PiecePtr>> parsedPieces =
				parsePiecePart(myPiecePart);
			TS_ASSERT_EQUALS(parsedPieces.size(), 8);

			for (size_t i = 0; i < 8; i++) {
				const std::vector<PiecePtr> &parsedRow = parsedPieces.at(i);
				TS_ASSERT_EQUALS(parsedRow.size(), 8);
				for (size_t j = 0; j < 8; j++) {
					const PiecePtr &parsedCell = parsedRow.at(j);
					const char &groundTruthCell = groundTruth.at(i).at(j);
					if (groundTruthCell == ' ') {
						TS_ASSERT_EQUALS(parsedCell, nullptr);
					} else {
						TS_ASSERT_DIFFERS(parsedCell, nullptr);
						TS_ASSERT_EQUALS(parsedCell->symbol(), groundTruthCell);
					}
				}
			}

			// test invalid piece parts
			TS_ASSERT_THROWS_ANYTHING(parsePiecePart(""));
			TS_ASSERT_THROWS_ANYTHING(parsePiecePart("\42"));
			TS_ASSERT_THROWS_ANYTHING(parsePiecePart("9/8/7/6/5/4/3/2/1"));
			TS_ASSERT_THROWS_ANYTHING(parsePiecePart("8/8/8/8/8/8/8"));
			TS_ASSERT_THROWS_ANYTHING(parsePiecePart("6r1/1R1Q4/7N/4pP2/2K1Pn1k/3p2pp/2p1P3/7B/2N5/1P3Kpp/pp6"));

		}
		void testParsePlayerPart() {
			TS_ASSERT_EQUALS(true, parsePlayerPart("w"));
			TS_ASSERT_EQUALS(false, parsePlayerPart("b"));
			TS_ASSERT_THROWS_ANYTHING(parsePlayerPart(""));
			TS_ASSERT_THROWS_ANYTHING(parsePlayerPart("t"));
			TS_ASSERT_THROWS_ANYTHING(parsePlayerPart("ttt"));
		}
		void testGetPlayerPart() {
			TS_ASSERT_EQUALS(getPlayerPart(true), "w");
			TS_ASSERT_EQUALS(getPlayerPart(false), "b");
		}
		void testParseCastlingPart() {
			TS_ASSERT_EQUALS(parseCastlingPart("KQkq"),
							 std::make_pair(PlayerState(true, true),
											PlayerState(true, true)));
			TS_ASSERT_EQUALS(parseCastlingPart("-"),
							 std::make_pair(PlayerState(false, false),
											PlayerState(false, false)));
			TS_ASSERT_EQUALS(parseCastlingPart("KQ"),
							 std::make_pair(PlayerState(true, true),
											PlayerState(false, false)));
			TS_ASSERT_EQUALS(parseCastlingPart("Kk"),
							 std::make_pair(PlayerState(false, true),
											PlayerState(false, true)));
		}

		void testGetCastlingPart() {
			TS_ASSERT_EQUALS(getCastlingPart(PlayerState(true, true),
											 PlayerState(true, true)),
							 "KQkq");
			TS_ASSERT_EQUALS(getCastlingPart(PlayerState(false, false),
											 PlayerState(false, false)),
							 "-");
			TS_ASSERT_EQUALS(getCastlingPart(PlayerState(true, true),
											 PlayerState(false, false)),
							 "KQ");
			TS_ASSERT_EQUALS(getCastlingPart(PlayerState(false, true),
											 PlayerState(false, true)),
							 "Kk");
		}
		void testGetEnPassantPart() {
			TS_ASSERT_EQUALS(parseEnPassantPart("e3"),
							 std::optional<Square>(Square("e3")));
			TS_ASSERT_THROWS_ANYTHING(parseEnPassantPart("iurektwnv"));
			TS_ASSERT_THROWS_ANYTHING(parseEnPassantPart("h9"));
			TS_ASSERT_THROWS_ANYTHING(parseEnPassantPart("9h"));
			TS_ASSERT_THROWS_ANYTHING(parseEnPassantPart(""));
		}
};
#endif
