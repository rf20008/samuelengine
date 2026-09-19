#include <cxxtest/TestSuite.h>
#include "TranspositionTable.hpp"

class TranspositionTableTest : public CxxTest::TestSuite {
public:

    void testEmptyTableProbe() {
        TranspositionTable tt(100);

        TTEntry entry = tt.probe(12345);

        TS_ASSERT(!entry);
    }

    void testStoreAndProbe() {
        TranspositionTable tt(100);

        const std::uint64_t hash = 12345;

        TTEntry stored(
            hash,
            5,
            395,
            TTFlag::EXACT,
            Move(Square(0x88), Square(0x88))
        );

        tt.store(stored);

        TTEntry result = tt.probe(hash);

        TS_ASSERT(result);
        TS_ASSERT_EQUALS(result.zobrist_hash, hash);
        TS_ASSERT_EQUALS(result.depth, 5);
        TS_ASSERT_EQUALS(result.value, 395);
        TS_ASSERT_EQUALS(result.flag, TTFlag::EXACT);
    }

    void testWrongHashIsMiss() {
        TranspositionTable tt(100);

        const std::uint64_t hash = 12345;

        tt.store(TTEntry(
            hash,
            5,
            395,
            TTFlag::EXACT,
            Move(Square(0x88), Square(0x88))
        ));

        // Same table index, but different hash.
        const std::uint64_t differentHash = hash + tt.size();

        TTEntry result = tt.probe(differentHash);

        TS_ASSERT(!result);
    }

    void testDeeperEntryReplacesShallowerEntry() {
        TranspositionTable tt(100);

        const std::uint64_t hash = 12345;

        tt.store(TTEntry(
            hash,
            4,
            100,
            TTFlag::EXACT,
            Move(Square(0x88), Square(0x88))
        ));

        tt.store(TTEntry(
            hash,
            8,
            200,
            TTFlag::EXACT,
            Move(Square(0x88), Square(0x88))
        ));

        TTEntry result = tt.probe(hash);

        TS_ASSERT(result);
        TS_ASSERT_EQUALS(result.depth, 8);
        TS_ASSERT_EQUALS(result.value, 200);
    }

    void testShallowerEntryDoesNotReplaceDeeperEntry() {
        TranspositionTable tt(100);

        const std::uint64_t hash = 12345;

        tt.store(TTEntry(
            hash,
            8,
            200,
            TTFlag::EXACT,
            Move(Square(0x88), Square(0x88))
        ));

        tt.store(TTEntry(
            hash,
            4,
            100,
            TTFlag::EXACT,
            Move(Square(0x88), Square(0x88))
        ));

        TTEntry result = tt.probe(hash);

        TS_ASSERT(result);
        TS_ASSERT_EQUALS(result.depth, 8);
        TS_ASSERT_EQUALS(result.value, 200);
    }

    void testSameHashSameDepthIsReplaced() {
        TranspositionTable tt(100);

        const std::uint64_t hash = 12345;

        tt.store(TTEntry(
            hash,
            5,
            100,
            TTFlag::EXACT,
            Move(Square(0x88), Square(0x88))
        ));

        tt.store(TTEntry(
            hash,
            5,
            200,
            TTFlag::LOWER_BOUND,
            Move(Square(0x88), Square(0x88))
        ));

        TTEntry result = tt.probe(hash);

        TS_ASSERT(result);
        TS_ASSERT_EQUALS(result.depth, 5);
        TS_ASSERT_EQUALS(result.value, 200);
        TS_ASSERT_EQUALS(result.flag, TTFlag::LOWER_BOUND);
    }

    void testClear() {
        TranspositionTable tt(100);

        const std::uint64_t hash = 12345;

        tt.store(TTEntry(
            hash,
            5,
            395,
            TTFlag::EXACT,
            Move(Square(0x88), Square(0x88))
        ));

        TS_ASSERT(tt.probe(hash));

        tt.clear();

        TS_ASSERT(!tt.probe(hash));
    }
};
