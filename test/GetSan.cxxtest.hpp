#include <cxxtest/TestSuite.h>

#include "ChessBoard.hpp"
#include "Errors.hpp"
#include "Move.hpp"

class GetSANTestSuite : public CxxTest::TestSuite {
public:
    void testStartingPositionMove() {
        ChessBoard board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        Move m("e2", "e4", '\0', MoveType::DOUBLE_PAWN_PUSH);
        TS_ASSERT_EQUALS(board.getSAN(m), "e4");
    }
    // 1. Regular moves
    void testSANRegularPawn() {
        ChessBoard board("7k/8/8/8/8/8/4P3/4K3 w - - 0 1");

        Move m("e2", "e3");

        TS_ASSERT_EQUALS(board.getSAN(m), "e3");
    }

    void testSANRegularKnight() {
        ChessBoard board("7k/8/8/8/8/5N2/8/4K3 w - - 0 1");

        Move m("f3", "g5");

        TS_ASSERT_EQUALS(board.getSAN(m), "Ng5");
    }

    void testSANRegularBishop() {
        ChessBoard board("7k/8/8/8/8/8/8/2B1K3 w - - 0 1");

        Move m("c1", "f4");

        TS_ASSERT_EQUALS(board.getSAN(m), "Bf4");
    }

    void testSANRegularRook() {
        ChessBoard board("7k/8/8/8/8/8/8/R3K3 w - - 0 1");

        Move m("a1", "a4");

        TS_ASSERT_EQUALS(board.getSAN(m), "Ra4");
    }

    void testSANRegularQueen() {
        ChessBoard board("6k1/8/8/8/8/8/8/3QK3 w - - 0 1");

        Move m("d1", "h5");

        TS_ASSERT_EQUALS(board.getSAN(m), "Qh5");
    }

    void testSANRegularKing() {
        ChessBoard board("7k/8/8/8/8/8/8/4K3 w - - 0 1");

        Move m("e1", "f2");

        TS_ASSERT_EQUALS(board.getSAN(m), "Kf2");
    }


    // 2. Captures
    void testSANCapturePawn() {
        ChessBoard board("7k/8/8/3p4/4P3/8/8/4K3 w - - 0 1");

        Move m("e4", "d5");

        TS_ASSERT_EQUALS(board.getSAN(m), "exd5");
    }

    void testSANCaptureKnight() {
        ChessBoard board("7k/8/8/8/3p4/5N2/8/4K3 w - - 0 1");

        Move m("f3", "d4");

        TS_ASSERT_EQUALS(board.getSAN(m), "Nxd4");
    }

    void testSANCaptureBishop() {
        ChessBoard board("7k/8/8/3p4/2B5/8/8/4K3 w - - 0 1");

        Move m("c4", "d5");

        TS_ASSERT_EQUALS(board.getSAN(m), "Bxd5");
    }

    void testSANCaptureRook() {
        ChessBoard board("7k/8/8/3p4/R7/8/8/4K3 w - - 0 1");

        Move m("a4", "d4");

        TS_ASSERT_EQUALS(board.getSAN(m), "Rxd4");
    }

    void testSANCaptureQueen() {
        ChessBoard board("7k/3p4/8/8/8/8/8/3QK3 w - - 0 1");

        Move m("d1", "d5");

        TS_ASSERT_EQUALS(board.getSAN(m), "Qxd5");
    }

    void testSANCaptureKing() {
        ChessBoard board("7k/8/8/3p4/4K3/8/8/8 w - - 0 1");

        Move m("e4", "d5");

        TS_ASSERT_EQUALS(board.getSAN(m), "Kxd5");
    }


    // 3. Checks from each piece type
    // King is impossible: a legal king move can never give check.

    void testSANCheckPawn() {
        ChessBoard board("3k4/8/8/4P3/8/8/8/K7 w - - 0 1");

        Move m("e5", "e6");

        TS_ASSERT_EQUALS(board.getSAN(m), "e6+");
    }

    void testSANCheckKnight() {
        ChessBoard board("7k/8/8/8/5N2/8/8/K7 w - - 0 1");

        Move m("f4", "g6");

        TS_ASSERT_EQUALS(board.getSAN(m), "Ng6+");
    }

    void testSANCheckBishop() {
        ChessBoard board("6k1/8/8/8/8/4B3/8/K7 w - - 0 1");

        Move m("e3", "h6");

        TS_ASSERT_EQUALS(board.getSAN(m), "Bh6+");
    }

    void testSANCheckRook() {
        ChessBoard board("4k3/R7/8/8/8/8/8/K7 w - - 0 1");

        Move m("a7", "e7");

        TS_ASSERT_EQUALS(board.getSAN(m), "Re7+");
    }

    void testSANCheckQueen() {
        ChessBoard board("4k3/8/8/8/8/8/8/3QK3 w - - 0 1");

        Move m("d1", "h5");

        TS_ASSERT_EQUALS(board.getSAN(m), "Qh5+");
    }
    void testSANCheckKing() {
        ChessBoard board("8/3k4/8/8/8/3K4/3R4/8 w - - 0 1");
        Move m("d3", "e3");
        TS_ASSERT_EQUALS(board.getSAN(m), "Ke3+");
    }

    // 4. Checks from each piece type that are captures
    // Again, king is impossible.

    void testSANCaptureCheckPawn() {
        ChessBoard board("8/4k3/3p4/4P3/8/8/8/K7 w - - 0 1");

        Move m("e5", "d6");

        TS_ASSERT_EQUALS(board.getSAN(m), "exd6+");
    }

    void testSANCaptureCheckKnight() {
        ChessBoard board("8/8/5k2/7p/5N2/8/8/K7 w - - 0 1");

        Move m("f4", "h5");

        TS_ASSERT_EQUALS(board.getSAN(m), "Nxh5+");
    }

    void testSANCaptureCheckBishop() {
        ChessBoard board("7k/6p1/7B/8/8/8/8/K7 w - - 0 1");

        Move m("h6", "g7");

        TS_ASSERT_EQUALS(board.getSAN(m), "Bxg7+");
    }

    void testSANCaptureCheckRook() {
        ChessBoard board("6rk/8/8/8/8/8/8/K5R1 w - - 0 1");

        Move m("g1", "g8");

        TS_ASSERT_EQUALS(board.getSAN(m), "Rxg8+");
    }

    void testSANCaptureCheckQueen() {
        ChessBoard board("7k/6p1/6Q1/8/8/8/8/K7 w - - 0 1");

        Move m("g6", "g7");

        TS_ASSERT_EQUALS(board.getSAN(m), "Qxg7+");
    }
    void testSANCaptureCheckKing() {
        ChessBoard board("8/3k4/8/8/8/3Kr3/3R4/8 w - - 0 1");
        Move m("d3", "e3");
        TS_ASSERT_EQUALS(board.getSAN(m), "Kxe3+");
    }

    // 5. Checkmates from each piece type, without captures
    // King checkmate is impossible.

    void testSANCheckmatePawn() {
        ChessBoard board("7k/7P/6PK/8/8/8/8/8 w - - 0 1");

        Move m("g6", "g7");

        TS_ASSERT_EQUALS(board.getSAN(m), "g7#");
    }

    void testSANCheckmateKnight() {
        ChessBoard board(
            "7k/"
            "8/"
            "3NB3/"
            "8/"
            "8/"
            "8/"
            "2B5/"
            "K5R1 w - - 0 1"
        );

        Move m("d6", "f7");

        TS_ASSERT_EQUALS(board.getSAN(m), "Nf7#");
    }

    void testSANCheckmateBishop() {
        ChessBoard board(
            "7k/"
            "8/"
            "5N1B/"
            "8/"
            "8/"
            "8/"
            "8/"
            "K5R1 w - - 0 1"
        );

        Move m("h6", "g7");

        TS_ASSERT_EQUALS(board.getSAN(m), "Bg7#");
    }

    void testSANCheckmateRook() {
        ChessBoard board(
            "7k/"
            "8/"
            "8/"
            "8/"
            "8/"
            "1B6/"
            "2B5/"
            "K5R1 w - - 0 1"
        );

        Move m("g1", "g8");

        TS_ASSERT_EQUALS(board.getSAN(m), "Rg8#");
    }

    void testSANCheckmateQueen() {
        ChessBoard board(
            "7k/"
            "8/"
            "6Q1/"
            "8/"
            "8/"
            "8/"
            "8/"
            "K5R1 w - - 0 1"
        );

        Move m("g6", "g7");

        TS_ASSERT_EQUALS(board.getSAN(m), "Qg7#");
    }
    void testSANCheckmateKing() {
        ChessBoard board("8/3k4/8/8/2R1R3/3K4/3R4/8 w - - 0 1");
        Move m("d3", "e3");
        TS_ASSERT_EQUALS(board.getSAN(m), "Ke3#");
    }

    // 6. Checkmates from each piece type, with captures
    // King checkmate by capture is impossible.

    void testSANCaptureCheckmatePawn() {
        ChessBoard board(
            "7k/6qP/5P1K/8/8/8/8/8 w - - 0 1"
        );

        Move m("f6", "g7");

        TS_ASSERT_EQUALS(board.getSAN(m), "fxg7#");
    }

    void testSANCaptureCheckmateKnight() {
        ChessBoard board(
            "7k/"
            "5p2/"
            "3NB3/"
            "8/"
            "8/"
            "8/"
            "2B5/"
            "K5R1 w - - 0 1"
        );

        Move m("d6", "f7");

        TS_ASSERT_EQUALS(board.getSAN(m), "Nxf7#");
    }

    void testSANCaptureCheckmateBishop() {
        ChessBoard board(
            "7k/"
            "6p1/"
            "5N1B/"
            "8/"
            "8/"
            "8/"
            "8/"
            "K5R1 w - - 0 1"
        );

        Move m("h6", "g7");

        TS_ASSERT_EQUALS(board.getSAN(m), "Bxg7#");
    }

    void testSANCaptureCheckmateRook() {
        ChessBoard board(
            "6rk/"
            "8/"
            "8/"
            "8/"
            "8/"
            "1B6/"
            "2B5/"
            "K5R1 w - - 0 1"
        );

        Move m("g1", "g8");

        TS_ASSERT_EQUALS(board.getSAN(m), "Rxg8#");
    }

    void testSANCaptureCheckmateQueen() {
        ChessBoard board(
            "7k/"
            "6r1/"
            "6Q1/"
            "8/"
            "8/"
            "8/"
            "8/"
            "K5R1 w - - 0 1"
        );

        Move m("g6", "g7");

        TS_ASSERT_EQUALS(board.getSAN(m), "Qxg7#");
    }
    void testKingCaptureCheckmate() {
        ChessBoard board("8/3k4/8/8/2R1R3/3Kr3/3R4/8 w - - 0 1");
        Move m("d3", "e3");
        TS_ASSERT_EQUALS(board.getSAN(m), "Kxd3#");
    }
    void testKingCaptureCheck() {
        ChessBoard board("8/3k4/8/8/8/3Kr3/3R4/8 w - - 0 1");
        Move m("d3", "e3");
        TS_ASSERT_EQUALS(board.getSAN(m), "Kxd3+");
    }

    // 7. Castling, and castling with check

    void testSANCastling() {
        ChessBoard board("4k3/8/8/8/8/8/8/4K2R w K - 0 1");

        Move m("e1", "g1", '\0', MoveType::CASTLING);

        TS_ASSERT_EQUALS(board.getSAN(m), "O-O");
    }

    void testSANCastlingWithCheck() {
        ChessBoard board("5k2/8/8/8/8/8/8/4K2R w K - 0 1");

        Move m("e1", "g1", '\0', MoveType::CASTLING);

        TS_ASSERT_EQUALS(board.getSAN(m), "O-O+");
    }


    // 8. Castling to give checkmate

    void testSANCastlingCheckmate() {
        ChessBoard board(
            "5k2/"
            "8/"
            "8/"
            "1B6/"
            "2B2N2/"
            "8/"
            "8/"
            "4K2R w K - 0 1"
        );

        Move m("e1", "g1", '\0', MoveType::CASTLING);

        TS_ASSERT_EQUALS(board.getSAN(m), "O-O#");
    }
};