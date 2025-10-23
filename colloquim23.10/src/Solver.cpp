#include "Solver.h"
#include <vector>
#include <cmath>
#include <limits>
#include <string>

using namespace std;

vector<long long> Solver::fibonacciSequence(int n) {
    if (n < 0) {
        throw string("n must be >= 0");
    }

    if (n == 0) {
        return {};
    }

    vector<long long> result;
    result.reserve(n);

    if (n >= 1) {
        result.push_back(0);
    }
    if (n >= 2) {
        result.push_back(1);
    }

    for (int i = 2; i < n; ++i) {
        if (result[i - 1] > numeric_limits<long long>::max() - result[i - 2]) {
            throw string("Fibonacci sequence overflow");
        }

        long long next = result[i - 1] + result[i - 2];
        result.push_back(next);
    }

    return result;
}

bool Solver::isPalindrome(int number) {
    if (number < 0) {
        return false;
    }

    if (number < 10) {
        return true;
    }

    int original = number;
    int reversed = 0;

    while (number > 0) {
        int digit = number % 10;

        if (reversed > (numeric_limits<int>::max() - digit) / 10) {
            return false;
        }

        reversed = reversed * 10 + digit;
        number /= 10;
    }

    return original == reversed;
}

ListNode* Solver::reverseList(ListNode* head) {
    if (head == nullptr) {
        return nullptr;
    }

    ListNode* prev = nullptr;
    ListNode* current = head;
    ListNode* next = nullptr;

    while (current != nullptr) {
        next = current->next;
        current->next = prev;    
        prev = current;          
        current = next;          
    }

    return prev;
}

ListNode* Solver::createList(const vector<int>& values) {
    if (values.empty()) {
        return nullptr;
    }

    ListNode* head = new ListNode(values[0]);
    ListNode* current = head;

    for (size_t i = 1; i < values.size(); ++i) {
        current->next = new ListNode(values[i]);
        current = current->next;
    }

    return head;
}

vector<int> Solver::listToVector(ListNode* head) {
    vector<int> result;
    ListNode* current = head;

    while (current != nullptr) {
        result.push_back(current->val);
        current = current->next;
    }

    return result;
}

void Solver::deleteList(ListNode* head) {
    ListNode* current = head;

    while (current != nullptr) {
        ListNode* next = current->next;
        delete current;
        current = next;
    }
}