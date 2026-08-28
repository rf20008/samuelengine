#ifndef CS3520_MINI_PROJECT_SQUARE
#define CS3520_MINI_PROJECT_SQUARE
#include <stdexcept>
#include <string>
#include <string_view>
#include <cstdint>
#include <array>

const int BOARD_SIZE = 8;
inline constexpr std::array<std::string_view, 128> SQUARE_NAMES = {
    "a1","b1","c1","d1","e1","f1","g1","h1", "??","??","??","??","??","??","??","??",
    "a2","b2","c2","d2","e2","f2","g2","h2", "??","??","??","??","??","??","??","??",
    "a3","b3","c3","d3","e3","f3","g3","h3", "??","??","??","??","??","??","??","??",
    "a4","b4","c4","d4","e4","f4","g4","h4", "??","??","??","??","??","??","??","??",
    "a5","b5","c5","d5","e5","f5","g5","h5", "??","??","??","??","??","??","??","??",
    "a6","b6","c6","d6","e6","f6","g6","h6", "??","??","??","??","??","??","??","??",
    "a7","b7","c7","d7","e7","f7","g7","h7", "??","??","??","??","??","??","??","??",
    "a8","b8","c8","d8","e8","f8","g8","h8", "??","??","??","??","??","??","??","??",
};
constexpr int Offset(int drank, int dfile) {return 16*drank + dfile;}


struct Square {
    public:
        uint8_t idx;
        constexpr explicit Square(): idx(0) {}
        constexpr explicit Square(int idx_) : idx(static_cast<uint8_t>(idx_)) {}
        // FILE, RANK order - your preference
        constexpr Square(int file, int rank) : idx(rank*16 + file){}
        constexpr Square(const std::string_view& s) : Square(s[0]-'a', s[1]-'1') {}
        constexpr Square(const char* s) : Square(s[0]-'a', s[1]-'1') {}
        constexpr std::string toString() const {
            if (!isValid()) return "??";
            return std::string(SQUARE_NAMES[idx]);
        }
        constexpr bool isValid() const {return (idx & 0x88) == 0;}
        constexpr bool operator==(const Square& other) const {return idx==other.idx;}
        constexpr std::strong_ordering operator<=>(const Square& other) const {return idx<=>other.idx;}
        constexpr Square operator+(int other) const {return Square(idx + other);}
        constexpr Square& operator+=(int off) {
            idx = static_cast<uint8_t>(int(idx) + off);
            return *this;
        }
        constexpr int file() const { return idx & 7; } // %8
        constexpr int rank() const { return idx >> 4; } // /16
        constexpr int operator-(const Square& other) const {return int(idx) - int(other.idx);}
        constexpr std::string operator()() const {return this->toString();}
};

constexpr int maxNorm(const Square& a, const Square& b) {
    return std::max(std::abs(a.rank()-b.rank()), std::abs(a.file()-b.file()));
}
#endif