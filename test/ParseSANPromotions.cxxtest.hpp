#include <cxxtest/TestSuite.h>

#include "ChessBoard.hpp"
#include "Errors.hpp"
#include "Move.hpp"


class ParseSANPromotionTestSuite : public CxxTest::TestSuite {
public:
    // ==========================================
    // 9. Pawn Promotions (All 48 Combinations)
    // ==========================================

    // --- WHITE NON-CAPTURE (PUSH) ---
    void testPromoWhitePushQueenNormal() {
        ChessBoard board("8/k3P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'Q', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=Q");
    }
    void testPromoWhitePushQueenCheck() {
        ChessBoard board("5k2/4P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'Q', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=Q+");
    }
    void testPromoWhitePushQueenCheckmate() {
        ChessBoard board("7k/R3P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'Q', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=Q#");
    }

    void testPromoWhitePushRookNormal() {
        ChessBoard board("8/k3P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'R', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=R");
    }
    void testPromoWhitePushRookCheck() {
        ChessBoard board("5k2/4P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'R', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=R+");
    }
    void testPromoWhitePushRookCheckmate() {
        ChessBoard board("7k/R3P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'R', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=R#");
    }

    void testPromoWhitePushBishopNormal() {
        ChessBoard board("8/k3P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'B', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=B");
    }
    void testPromoWhitePushBishopCheck() {
        ChessBoard board("8/4P3/6k1/8/8/8/8/4K3 w - - 0 1"); 
        Move m("e7", "e8", 'B', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=B+");
    }
    void testPromoWhitePushBishopCheckmate() {
        ChessBoard board("8/5P2/7k/1K3Q2/8/8/8/8 w - - 0 1");
        Move m("f7", "f8", 'B', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "f8=B#");
    }

    void testPromoWhitePushKnightNormal() {
        ChessBoard board("8/k3P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'N', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=N");
    }
    void testPromoWhitePushKnightCheck() {
        ChessBoard boardCheck("8/4P1k1/8/8/8/8/8/4K3 w - - 0 1"); // knight on e8 checks f6 or d7? e8 to f6 is an L-shape.
        Move m("e7", "e8", 'N', MoveType::NORMAL);
        TS_ASSERT_EQUALS(boardCheck.getSAN(m), "e8=N+");
    }
    void testPromoWhitePushKnightCheckmate() {
        ChessBoard board("8/5P1k/4B3/4B1K1/8/8/8/8 w - - 0 1");
        Move m("f7", "f8", 'N', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "f8=N#");
    }


    // --- WHITE CAPTURE ---
    void testPromoWhiteCaptureQueenNormal() {
        ChessBoard board("3r4/4P1k1/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "d8", 'Q', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd8=Q");
    }
    void testPromoWhiteCaptureQueenCheck() {
        ChessBoard board("3r1k2/4P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "d8", 'Q', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd8=Q+");
    }
    void testPromoWhiteCaptureQueenCheckmate() {
        ChessBoard boardMate("3q3k/4PQ2/8/8/8/8/8/4K3 w - - 0 1");
        Move mMat("e7", "d8", 'Q', MoveType::NORMAL);
        TS_ASSERT_EQUALS(boardMate.getSAN(mMat), "exd8=Q#");
    }

    void testPromoWhiteCaptureRookNormal() {
        ChessBoard board("3r4/4P3/5k2/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "d8", 'R', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd8=R");
    }
    void testPromoWhiteCaptureRookCheck() {
        ChessBoard board("3r1k2/4P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "d8", 'R', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd8=R+");
    }
    void testPromoWhiteCaptureRookCheckmate() {
        ChessBoard boardMate("3q3k/4PQ2/8/8/8/8/8/4K3 w - - 0 1");
        Move mMat("e7", "d8", 'R', MoveType::NORMAL);
        TS_ASSERT_EQUALS(boardMate.getSAN(mMat), "exd8=R#");
    }

    void testPromoWhiteCaptureBishopNormal() {
        ChessBoard board("3rk3/4P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "d8", 'B', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd8=B");
    }
    void testPromoWhiteCaptureBishopCheck() {
        ChessBoard board("5q2/4P3/7k/8/7K/7Q/8/8 w - - 0 1");
        Move m("e7", "f8", 'B', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exf8=B+");
    }
    void testPromoWhiteCaptureBishopCheckmate() {
        ChessBoard board("6n1/7P/8/8/8/2Q5/k7/1RR3K1 w - - 0 1");
        Move m("h7", "g8", 'B', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "hxg8=B#");
    }

    void testPromoWhiteCaptureKnightNormal() {
        ChessBoard board("3rk3/4P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "d8", 'N', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd8=N");
    }
    void testPromoWhiteCaptureKnightCheck() {
        ChessBoard board("3r4/1r2Pk2/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "d8", 'N', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd8=N+");
    }
    void testPromoWhiteCaptureKnightCheckmate() {
        ChessBoard board("2q5/k2P4/3B4/1K1B4/8/8/8/8 w - - 0 1");
        Move m("d7", "c8", 'N', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "dxc8=N#");
    }


    // --- BLACK NON-CAPTURE (PUSH) ---
    void testPromoBlackPushQueenNormal() {
        ChessBoard board("4k3/8/8/8/8/8/4p2K/8 b - - 0 1");
        Move m("e2", "e1", 'q', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=Q");
    }
    void testPromoBlackPushQueenCheck() {
        ChessBoard board("4k3/8/8/8/8/8/4p3/5K2 b - - 0 1");
        Move m("e2", "e1", 'q', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=Q+");
    }
    void testPromoBlackPushQueenCheckmate() {
        ChessBoard board("4k3/8/8/8/8/8/q3p3/7K b - - 0 1");
        Move m("e2", "e1", 'q', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=Q#");
    }

    void testPromoBlackPushRookNormal() {
        ChessBoard board("4k3/8/8/8/8/8/4p2K/8 b - - 0 1");
        Move m("e2", "e1", 'r', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=R");
    }
    void testPromoBlackPushRookCheck() {
        ChessBoard board("4k3/8/8/8/8/8/4p3/5K2 b - - 0 1");
        Move m("e2", "e1", 'r', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=R+");
    }
    void testPromoBlackPushRookCheckmate() {
        ChessBoard board("4k3/8/8/8/8/8/r3p3/7K b - - 0 1");
        Move m("e2", "e1", 'r', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=R#");
    }

    void testPromoBlackPushBishopNormal() {
        ChessBoard board("4k3/8/8/8/8/8/4p2K/8 b - - 0 1");
        Move m("e2", "e1", 'b', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=B");
    }
    void testPromoBlackPushBishopCheck() {
        ChessBoard board("4k3/6p1/8/8/8/6K1/4p3/8 b - - 0 1");
        Move m("e2", "e1", 'b', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=B+");
    }
    void testPromoBlackPushBishopCheckmate() {
        ChessBoard board("1b6/8/8/6k1/8/5b1K/5p2/6b1 b - - 0 1");
        Move m("f2", "f1", 'b', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "f1=B#");
    }

    void testPromoBlackPushKnightNormal() {
        ChessBoard board("4k3/8/8/8/8/8/4p2K/8 b - - 0 1");
        Move m("e2", "e1", 'n', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=N");
    }
    void testPromoBlackPushKnightCheck() {
        ChessBoard board("4k3/8/8/8/7p/8/4p1K1/8 b - - 0 1");
        Move m("e2", "e1", 'n', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=N+");
    }
    void testPromoBlackPushKnightCheckmate() {
        ChessBoard board("8/8/8/8/1k6/2bb4/K1pp4/8 b - - 0 1");
        Move m("c2", "c1", 'n', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "c1=N#");
    }


    // --- BLACK CAPTURE ---
    void testPromoBlackCaptureQueenNormal() {
        ChessBoard board("4k3/8/8/8/8/7K/4p3/3R4 b - - 0 1");
        Move m("e2", "d1", 'q', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd1=Q");
    }
    void testPromoBlackCaptureQueenCheck() {
        ChessBoard board("5k2/8/8/8/8/3K4/4p3/3R4 b - - 0 1");
        Move m("e2", "d1", 'q', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd1=Q+");
    }
    void testPromoBlackCaptureQueenCheckmate() {
        ChessBoard board("7k/8/8/8/8/3K4/4p3/2rRr3 b - - 0 1");
        Move m("e2", "d1", 'q', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd1=Q#");
    }

    void testPromoBlackCaptureRookNormal() {
        ChessBoard board("4k3/8/8/8/8/7K/4p3/3R4 b - - 0 1");
        Move m("e2", "d1", 'r', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd1=R");
    }
    void testPromoBlackCaptureRookCheck() {
        ChessBoard board("5k2/8/8/8/8/3K4/4p3/3R4 b - - 0 1");
        Move m("e2", "d1", 'r', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd1=R+");
    }
    void testPromoBlackCaptureRookCheckmate() {
        ChessBoard board("3k4/8/3K4/8/8/8/4p3/2rRr3 b - - 0 1");
        Move m("e2", "d1", 'r', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd1=R#");
    }

    void testPromoBlackCaptureBishopNormal() {
        ChessBoard board("4k3/8/8/8/8/7K/4p3/3R4 b - - 0 1");
        Move m("e2", "d1", 'b', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd1=B");
    }
    void testPromoBlackCaptureBishopCheck() {
        ChessBoard board("2r5/2k5/8/8/8/1K6/4p3/3R4 b - - 0 1");
        Move m("e2", "d1", 'b', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd1=B+");
    }
    void testPromoBlackCaptureBishopCheckmate() {
        ChessBoard board("1rr3k1/K7/2q5/8/8/8/7p/6N1 b - - 0 1");
        Move m("h2", "g1", 'b', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "hxg1=B#");
    }

    void testPromoBlackCaptureKnightNormal() {
        ChessBoard board("4k3/8/8/8/8/7K/4p3/3R4 b - - 0 1");
        Move m("e2", "d1", 'n', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd1=N");
    }
    void testPromoBlackCaptureKnightCheck() {
        ChessBoard board("5k2/8/8/8/8/2K5/4p3/3R4 b - - 0 1");
        Move m("e2", "d1", 'n', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd1=N+");
    }
    void testPromoBlackCaptureKnightCheckmate() {
        ChessBoard board("8/8/8/8/1k6/2bb4/K2p4/2Q5 b - - 0 1");
        Move m("d2", "c1", 'n', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "dxc1=N#");
    }
};