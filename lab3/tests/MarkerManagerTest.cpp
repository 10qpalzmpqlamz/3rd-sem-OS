#include "gtest/gtest.h"
#include "MarkerManager.h"
#include <thread>
#include <chrono>
#include <atomic>

namespace TestHelpers {
    bool IsArrayAllZeros(const std::vector<int>& array) {
        for (int value : array) {
            if (value != 0) return false;
        }
        return true;
    }

    bool IsArrayAllMarked(const std::vector<int>& array, int marker) {
        for (int value : array) {
            if (value != marker) return false;
        }
        return true;
    }

    bool IsArrayValid(const std::vector<int>& array, int maxThread) {
        for (int value : array) {
            if (value < 0 || value > maxThread) return false;
        }
        return true;
    }

    int CountMarkedCells(const std::vector<int>& array) {
        int count = 0;
        for (int value : array) {
            if (value != 0) count++;
        }
        return count;
    }

    bool AreAllMarkedCellsFromThread(const std::vector<int>& array, int marker) {
        for (int value : array) {
            if (value != 0 && value != marker) return false;
        }
        return true;
    }

    bool ContainsMarkFromThread(const std::vector<int>& array, int threadId) {
        for (int value : array) {
            if (value == threadId) return true;
        }
        return false;
    }
}

class MarkerManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test 1.1
TEST_F(MarkerManagerTest, SingleThreadMarksAllElements) {
    const int arraySize = 10;
    const int threadCount = 1;

    MarkerManager manager(arraySize, threadCount);

    manager.StartThreads();

    manager.WaitForThreadsToBlock();

    const auto& array = manager.GetArray();
    EXPECT_TRUE(TestHelpers::AreAllMarkedCellsFromThread(array, 1));

    EXPECT_TRUE(TestHelpers::IsArrayValid(array, threadCount));

    manager.TerminateThread(1);
}

// Test 1.2
TEST_F(MarkerManagerTest, SingleThreadCleanupAfterTermination) {
    const int arraySize = 10;
    const int threadCount = 1;

    MarkerManager manager(arraySize, threadCount);

    manager.StartThreads();
    manager.WaitForThreadsToBlock();

    manager.TerminateThread(1);

    const auto& array = manager.GetArray();
    EXPECT_TRUE(TestHelpers::IsArrayAllZeros(array));
}

// Test 2.1
TEST_F(MarkerManagerTest, NoRaceConditionsWithMultipleThreads) {
    const int arraySize = 20;
    const int threadCount = 10;

    MarkerManager manager(arraySize, threadCount);

    manager.StartThreads();

    manager.WaitForThreadsToBlock();

    const auto& array = manager.GetArray();

    EXPECT_TRUE(TestHelpers::IsArrayValid(array, threadCount));

    int markedCells = TestHelpers::CountMarkedCells(array);
    EXPECT_GT(markedCells, 0) << "Should have at least some marked cells";
    EXPECT_LE(markedCells, arraySize) << "Cannot have more marked cells than array size";

    for (int value : array) {
        EXPECT_TRUE(value >= 0 && value <= threadCount)
            << "Invalid value in array: " << value << ". Should be between 0 and " << threadCount;
    }

    for (int i = 1; i <= threadCount; i++) {
        manager.TerminateThread(i);
    }

    const auto& finalArray = manager.GetArray();
    EXPECT_TRUE(TestHelpers::IsArrayAllZeros(finalArray));
}

// Test 2.2
TEST_F(MarkerManagerTest, CorrectSequentialTermination) {
    const int arraySize = 30;
    const int threadCount = 5;

    MarkerManager manager(arraySize, threadCount);

    manager.StartThreads();
    manager.WaitForThreadsToBlock();

    for (int i = 1; i <= threadCount; i++) {
        manager.TerminateThread(i);

        const auto& array = manager.GetArray();
        EXPECT_TRUE(TestHelpers::IsArrayValid(array, threadCount));

        for (int terminated = 1; terminated <= i; terminated++) {
            EXPECT_FALSE(TestHelpers::ContainsMarkFromThread(array, terminated))
                << "Found marks from terminated thread " << terminated;
        }

        if (i < threadCount) {
            manager.ResumeThreads();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            manager.WaitForThreadsToBlock();
        }
    }

    const auto& finalArray = manager.GetArray();
    EXPECT_TRUE(TestHelpers::IsArrayAllZeros(finalArray));
}
