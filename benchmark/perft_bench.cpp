#include "ChessBoard.hpp"
#include <chrono>
#include <cstdio>

int main() {
    ChessBoard b("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    
    auto s = std::chrono::high_resolution_clock::now();
    auto nodes = b.perft(3);
    auto e = std::chrono::high_resolution_clock::now();
    long ms = std::chrono::duration_cast<std::chrono::milliseconds>(e-s).count();
    printf("perft3 %lu in %ldms %.0f nps\n", (unsigned long)nodes, ms, nodes*1000.0/ms);
}