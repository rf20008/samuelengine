#ifndef CS3520_MINIPROJECT_ZOBRIST
#define CS3520_MINIPROJECT_ZOBRIST
#include <cstdint>
#include <random>
struct Zobrist {
    // [color][pieceType][square64] color 0=white 1=black, piece 0..5 = P,N,B,R,Q,K
    uint64_t pieces[2][6][64];
    uint64_t castling[16];     // 0-15 for KQkq bits
    uint64_t enPassantFile[8]; // 0-7 file, 8 = no ep
    uint64_t sideToMove;       // xor if black to move

    Zobrist() {
            std::mt19937_64 rng(0x53414D55); // "SAMU"
            for (int c = 0; c < 2; c++)
                    for (int p = 0; p < 6; p++)
                            for (int sq = 0; sq < 64; sq++)
                                    pieces[c][p][sq] = rng();
            for (int i = 0; i < 16; i++)
                    castling[i] = rng();
            for (int f = 0; f < 8; f++)
                    enPassantFile[f] = rng();
            sideToMove = rng();
    }

    static constexpr int to64(int sq88) {
            // 0x88 -> 0..63
            return (sq88 & 7) + ((sq88 >> 4) * 8);
    }
};
inline Zobrist ZOBRIST;
#endif
