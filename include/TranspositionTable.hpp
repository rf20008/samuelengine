#pragma once
#ifndef SAMUELENGINE_TRANSPOSITION_TABLE_HPP
#define SAMUELENGINE_TRANSPOSITION_TABLE_HPP
#include <cstdint>
#include <vector>
#include "Move.hpp"
enum class TTFlag : std::uint8_t{
    UNKNOWN = 0,
    EXACT = 1,
    LOWER_BOUND = 2,
    UPPER_BOUND = 3,
};

struct TTEntry {
    uint64_t zobrist_hash;
    int depth;
    int value;
    TTFlag flag;
    Move bestMove;
    constexpr TTEntry() : TTEntry(0, -1, 0, TTFlag::UNKNOWN, Move(Square(0x88), Square(0x88))) {}
    constexpr TTEntry(uint64_t hash, int depth, int value, TTFlag flag, Move bestMove) : zobrist_hash(hash), depth(depth), value(value), flag(flag), bestMove(bestMove) {}
    constexpr explicit operator bool() const {
        return flag != TTFlag::UNKNOWN;
    }
    constexpr bool matches(std::uint64_t hash) const {
        return zobrist_hash == hash;
    }
};

struct TranspositionTable {
    private:
        std::vector<TTEntry> table;
    public:
        TranspositionTable(size_t numEntries) {
            table.resize(numEntries);

        }
        TTEntry probe(std::uint64_t hash) const {
            const std::size_t index = hash % table.size();
            const TTEntry& entry = table[index];
        
            if (entry && entry.matches(hash))
                return entry;
        
            return TTEntry{};  // invalid entry
        }
        void clear() {
            for (size_t idx = 0; idx<table.size(); ++idx) table[idx] = TTEntry();
        }
        void store(TTEntry entry) {
            const std::size_t index =
                entry.zobrist_hash % table.size();
        
            TTEntry& oldEntry = table[index];
        
            if (
                !oldEntry ||
                    (oldEntry.zobrist_hash == entry.zobrist_hash &&
                    entry.depth >= oldEntry.depth)
            ) {
        
                oldEntry = entry;
            }
        }
        size_t size() const {
            return table.size();
        }
};
#endif