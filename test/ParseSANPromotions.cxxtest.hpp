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
        ChessBoard board("7k/4P1Q1/8/8/8/8/8/4K3 w - - 0 1");
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
        ChessBoard board("7k/4P2R/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'R', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=R#");
    }

    void testPromoWhitePushBishopNormal() {
        ChessBoard board("8/k3P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'B', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=B");
    }
    void testPromoWhitePushBishopCheck() {
        ChessBoard board("4k3/4P3/8/8/8/8/4B3/4K3 w - - 0 1"); // Wait, e8-bishop checks e8->h5 diagonal? Let's use diagonal check
        ChessBoard board2("6k1/4P3/8/8/8/8/8/4K1B1 w - - 0 1"); // bishop on g1 doesn't check f8. Let's use:
        // Actually for Bishop check on e8: Black king on g8, bishop on e8 attacks g8.
        ChessBoard boardCheck("6k1/4P3/8/8/8/8/8/4K3 w - - 0 1"); // e7->e8=B attacks g8? No, e8 to g8 is diagonal (e8-f7-g8). Yes!
        Move m("e7", "e8", 'B', MoveType::NORMAL);
        TS_ASSERT_EQUALS(boardCheck.getSAN(m), "e8=B+");
    }
    void testPromoWhitePushBishopCheckmate() {
        ChessBoard board("7k/4P1B1/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'B', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=B#");
    }

    void testPromoWhitePushKnightNormal() {
        ChessBoard board("8/k3P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'N', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=N");
    }
    void testPromoWhitePushKnightCheck() {
        ChessBoard board("6k1/4P3/8/8/8/8/8/4K3 w - - 0 1"); // knight on e8 checks g7 or f6. f6!
        ChessBoard boardCheck("5k2/4P3/8/8/8/8/8/4K3 w - - 0 1"); // knight on e8 checks f6 or d7? e8 to f6 is an L-shape.
        Move m("e7", "e8", 'N', MoveType::NORMAL);
        TS_ASSERT_EQUALS(boardCheck.getSAN(m), "e8=N+");
    }
    void testPromoWhitePushKnightCheckmate() {
        ChessBoard board("7k/4P1N1/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "e8", 'N', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e8=N#");
    }


    // --- WHITE CAPTURE ---
    void testPromoWhiteCaptureQueenNormal() {
        ChessBoard board("3rk3/4P3/8/8/8/8/8/4K3 w - - 0 1");
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
        ChessBoard board("3rk3/4P3/8/8/8/8/8/4K3 w - - 0 1");
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
        ChessBoard board("5q2/4P3/7k/5Q2/7K/8/8/8 w - - 0 1");
        Move m("e7", "f8", 'B', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exf8=B+");
    }
    void testPromoWhiteCaptureBishopCheckmate() {
        ChessBoard board("5q2/4P3/7k/8/7K/8/8/7Q w - - 0 1");
        Move m("e7", "f8", 'B', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exf8=B#");
    }

    void testPromoWhiteCaptureKnightNormal() {
        ChessBoard board("3rk3/4P3/8/8/8/8/8/4K3 w - - 0 1");
        Move m("e7", "d8", 'N', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "exd8=N");
    }
    void testPromoWhiteCaptureKnightCheck() {
        ChessBoard board("3r1k2/4P3/8/8/8/8/8/4K3 w - - 0 1");
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
        ChessBoard board("4k3/8/8/8/8/8/4p1q1/7K b - - 0 1");
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
        ChessBoard board("4k3/8/8/8/8/8/4p2r/7K b - - 0 1");
        Move m("e2", "e1", 'r', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=R#");
    }

    void testPromoBlackPushBishopNormal() {
        ChessBoard board("4k3/8/8/8/8/8/4p2K/8 b - - 0 1");
        Move m("e2", "e1", 'b', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=B");
    }
    void testPromoBlackPushBishopCheck() {
        ChessBoard board("4k3/8/8/8/8/8/4p3/6K1 b - - 0 1");
        Move m("e2", "e1", 'b', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=B+");
    }
    void testPromoBlackPushBishopCheckmate() {
        ChessBoard board("4k3/8/8/8/8/8/4p1b1/7K b - - 0 1");
        Move m("e2", "e1", 'b', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=B#");
    }

    void testPromoBlackPushKnightNormal() {
        ChessBoard board("4k3/8/8/8/8/8/4p2K/8 b - - 0 1");
        Move m("e2", "e1", 'n', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=N");
    }
    void testPromoBlackPushKnightCheck() {
        ChessBoard board("4k3/8/8/8/8/8/4p3/5K2 b - - 0 1");
        Move m("e2", "e1", 'n', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=N+");
    }
    void testPromoBlackPushKnightCheckmate() {
        ChessBoard board("4k3/8/8/8/8/8/4p1n1/7K b - - 0 1");
        Move m("e2", "e1", 'n', MoveType::NORMAL);
        TS_ASSERT_EQUALS(board.getSAN(m), "e1=N#");
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