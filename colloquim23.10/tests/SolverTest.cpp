#include <gtest/gtest.h>
#include "Solver.h"
#include <vector>
#include <limits>

using namespace std;

class SolverTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SolverTest, Fibonacci_Zero) {
    auto result = Solver::fibonacciSequence(0);
    EXPECT_TRUE(result.empty());
}

TEST_F(SolverTest, Fibonacci_One) {
    auto result = Solver::fibonacciSequence(1);
    EXPECT_EQ(result, vector<long long>({ 0 }));
}

TEST_F(SolverTest, Fibonacci_Five) {
    auto result = Solver::fibonacciSequence(5);
    EXPECT_EQ(result, vector<long long>({ 0, 1, 1, 2, 3 }));
}

TEST_F(SolverTest, Fibonacci_Negative) {
    EXPECT_THROW(Solver::fibonacciSequence(-1), string);
}

TEST_F(SolverTest, Palindrome_True) {
    EXPECT_TRUE(Solver::isPalindrome(0));
    EXPECT_TRUE(Solver::isPalindrome(121));
    EXPECT_TRUE(Solver::isPalindrome(12321));
}

TEST_F(SolverTest, Palindrome_False) {
    EXPECT_FALSE(Solver::isPalindrome(123));
    EXPECT_FALSE(Solver::isPalindrome(-121));
}

TEST_F(SolverTest, ReverseList_Empty) {
    EXPECT_EQ(Solver::reverseList(nullptr), nullptr);
}

TEST_F(SolverTest, ReverseList_Single) {
    ListNode* head = new ListNode(5);
    ListNode* result = Solver::reverseList(head);
    EXPECT_EQ(result->val, 5);
    EXPECT_EQ(result->next, nullptr);
    delete result;
}

TEST_F(SolverTest, ReverseList_Multiple) {
    ListNode* list = Solver::createList({ 1, 2, 3, 4, 5 });
    ListNode* reversed = Solver::reverseList(list);
    vector<int> result = Solver::listToVector(reversed);
    EXPECT_EQ(result, vector<int>({ 5, 4, 3, 2, 1 }));
    Solver::deleteList(reversed);
}

TEST_F(SolverTest, ListToVector_Empty) {
    vector<int> result = Solver::listToVector(nullptr);
    EXPECT_TRUE(result.empty());
}

TEST_F(SolverTest, CreateList_Empty) {
    ListNode* result = Solver::createList({});
    EXPECT_EQ(result, nullptr);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}