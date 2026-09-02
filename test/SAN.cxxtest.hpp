#include <cxxtest/TestSuite.h>

#include "ChessBoard.hpp"
#include "Errors.hpp"
#include "Move.hpp"


class SANTestSuite : public CxxTest::TestSuite {
public:

    // ============================================================
    // Basic piece moves
    // ============================================================

    void testPawnMove() {
        std::string fen =
            "4k3/8/8/8/8/8/4P3/4K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("e4");

        TS_ASSERT_EQUALS(move.startingSquare, Square("e2"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("e4"));
    }

    void testKnightMove() {
        std::string fen =
            "4k3/8/8/8/8/8/N7/4K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("Nb4");

        TS_ASSERT_EQUALS(move.startingSquare, Square("a2"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("b4"));
    }

    void testBishopMove() {
        std::string fen =
            "4k3/8/8/8/8/8/B7/4K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("Bb3");

        TS_ASSERT_EQUALS(move.startingSquare, Square("a2"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("b3"));
    }

    void testRookMove() {
        std::string fen =
            "4k3/8/8/8/8/8/R7/4K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("Ra3");

        TS_ASSERT_EQUALS(move.startingSquare, Square("a2"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("a3"));
    }

    void testQueenMove() {
        std::string fen =
            "4k3/8/8/8/8/8/Q7/4K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("Qb3");

        TS_ASSERT_EQUALS(move.startingSquare, Square("a2"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("b3"));
    }


    // ============================================================
    // Piece disambiguation
    // ============================================================

    void testKnightFileDisambiguation() {
        // Knights on b1 and f1 can both move to d2.
        // White king is on e2 so it doesn't occupy d1/e1/f1.
        std::string fen =
            "4k3/8/8/8/8/8/4K3/1N2N3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("Nbd2");

        TS_ASSERT_EQUALS(move.startingSquare, Square("b1"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("d2"));
    }

    void testKnightRankDisambiguation() {
        // Knights on b1 and b3 can both move to d2.
        std::string fen =
            "4k3/8/8/8/1N6/8/8/1N2K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("N1d2");

        TS_ASSERT_EQUALS(move.startingSquare, Square("b1"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("d2"));
    }

    void testRookFileDisambiguation() {
        // Rooks on a1 and h1 can both move to d1.
        // White king is on e2, so d1 is empty and both paths are clear.
        std::string fen =
            "4k3/8/8/8/8/8/4K3/R6R w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("Rad1");

        TS_ASSERT_EQUALS(move.startingSquare, Square("a1"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("d1"));
    }

    void testRookRankDisambiguation() {
        // Rooks on a1 and a3 can both move to a2.
        std::string fen =
            "4k3/8/8/R7/8/8/8/R3K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("R1a2");

        TS_ASSERT_EQUALS(move.startingSquare, Square("a1"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("a2"));
    }

    void testAmbiguousMoveRejected() {
        // Both rooks can move to d1.
        // "Rd1" therefore isn't sufficiently disambiguated.
        std::string fen =
            "4k3/8/8/8/8/8/4K3/R6R w - - 0 1";

        ChessBoard board(fen);

        TS_ASSERT_THROWS_ANYTHING(board.getMove("Rd1"));
    }


    // ============================================================
    // Captures
    // ============================================================

    void testPawnCapture() {
        // White pawn e3 captures black pawn on d4.
        std::string fen =
            "4k3/8/8/8/3p4/4P3/8/4K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("exd4");

        TS_ASSERT_EQUALS(move.startingSquare, Square("e3"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("d4"));
    }

    void testPieceCapture() {
        // White knight c2 captures black pawn d4.
        std::string fen =
            "4k3/8/8/8/3p4/8/2N5/4K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("Nxd4");

        TS_ASSERT_EQUALS(move.startingSquare, Square("c2"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("d4"));
    }

    void testPawnCaptureWithoutXRejected() {
        std::string fen =
            "4k3/8/8/8/3p4/4P3/8/4K3 w - - 0 1";

        ChessBoard board(fen);

        TS_ASSERT_THROWS_ANYTHING(board.getMove("ed4"));
    }


    // ============================================================
    // Castling
    // ============================================================

    void testWhiteKingsideCastle() {
        std::string fen =
            "4k3/8/8/8/8/8/8/R3K2R w K - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("O-O");

        TS_ASSERT_EQUALS(move.startingSquare, Square("e1"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("g1"));
    }

    void testWhiteQueensideCastle() {
        std::string fen =
            "4k3/8/8/8/8/8/8/R3K2R w Q - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("O-O-O");

        TS_ASSERT_EQUALS(move.startingSquare, Square("e1"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("c1"));
    }

    void testBlackKingsideCastle() {
        std::string fen =
            "r3k2r/8/8/8/8/8/8/4K3 b k - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("O-O");

        TS_ASSERT_EQUALS(move.startingSquare, Square("e8"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("g8"));
    }

    void testBlackQueensideCastle() {
        std::string fen =
            "r3k2r/8/8/8/8/8/8/4K3 b q - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("O-O-O");

        TS_ASSERT_EQUALS(move.startingSquare, Square("e8"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("c8"));
    }

    void testIllegalCastleRejected() {
        // The pieces are in castling positions, but there are
        // no castling rights in the FEN.
        std::string fen =
            "4k3/8/8/8/8/8/8/R3K2R w - - 0 1";

        ChessBoard board(fen);

        TS_ASSERT_THROWS_ANYTHING(board.getMove("O-O"));
    }


    // ============================================================
    // Promotions
    // ============================================================

    void testQueenPromotion() {
        // Black king is on a8, so e8 is available for promotion.
        std::string fen =
            "k7/4P3/8/8/8/8/8/4K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("e8=Q");

        TS_ASSERT_EQUALS(move.startingSquare, Square("e7"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("e8"));
        TS_ASSERT_EQUALS(move.promotion, 'Q');
    }

    void testKnightPromotion() {
        std::string fen =
            "k7/4P3/8/8/8/8/8/4K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("e8=N");

        TS_ASSERT_EQUALS(move.startingSquare, Square("e7"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("e8"));
        TS_ASSERT_EQUALS(move.promotion, 'N');
    }

    void testRookPromotion() {
        std::string fen =
            "k7/4P3/8/8/8/8/8/4K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("e8=R");

        TS_ASSERT_EQUALS(move.startingSquare, Square("e7"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("e8"));
        TS_ASSERT_EQUALS(move.promotion, 'R');
    }

    void testBishopPromotion() {
        std::string fen =
            "k7/4P3/8/8/8/8/8/4K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("e8=B");

        TS_ASSERT_EQUALS(move.startingSquare, Square("e7"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("e8"));
        TS_ASSERT_EQUALS(move.promotion, 'B');
    }

    void testCapturePromotion() {
        // White pawn a7 captures black rook b8 and promotes.
        std::string fen =
            "1r2k3/P7/8/8/8/8/8/4K3 w - - 0 1";

        ChessBoard board(fen);

        Move move = board.getMove("axb8=Q");

        TS_ASSERT_EQUALS(move.startingSquare, Square("a7"));
        TS_ASSERT_EQUALS(move.endingSquare, Square("b8"));
        TS_ASSERT_EQUALS(move.promotion, 'Q');
    }

    void testInvalidPromotionPieceRejected() {
        std::string fen =
            "k7/4P3/8/8/8/8/8/4K3 w - - 0 1";

        ChessBoard board(fen);

        TS_ASSERT_THROWS_ANYTHING(board.getMove("e8=K"))
    }


    // ============================================================
    // Invalid SAN
    // ============================================================

    void testInvalidPieceSymbolRejected() {
        std::string fen =
            "4k3/8/8/8/8/8/4P3/4K3 w - - 0 1";

        ChessBoard board(fen);

        TS_ASSERT_THROWS_ANYTHING(board.getMove("Xe4"));
    }

    void testNonexistentMoveRejected() {
        std::string fen =
            "4k3/8/8/8/8/8/4P3/4K3 w - - 0 1";

        ChessBoard board(fen);

        // e2-e5 is not a legal pawn move.
        TS_ASSERT_THROWS_ANYTHING(board.getMove("e5"));
    }

    void testInvalidAmbiguatorRejected() {
        std::string fen =
            "4k3/8/8/8/8/8/N7/4K3 w - - 0 1";

        ChessBoard board(fen);

        // "i" isn't a valid file/rank disambiguator.
        TS_ASSERT_THROWS_ANYTHING(board.getMove("Nib4"));
    }
};
