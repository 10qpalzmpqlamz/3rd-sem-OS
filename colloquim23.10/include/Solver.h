#pragma once

#include <vector>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solver {
public:
    static std::vector<long long> fibonacciSequence(int n);

    static bool isPalindrome(int number);

    static ListNode* reverseList(ListNode* head);

    static ListNode* createList(const std::vector<int>& values);
    static std::vector<int> listToVector(ListNode* head);
    static void deleteList(ListNode* head);

private:
    Solver() = delete;
};