#include <iostream>
#include <vector>
#include "Solver.h"

using namespace std;

int main() {

    //1
	int n;
	cout << "Enter n for Fibonacci sequence: ";
	cin >> n;

    try {
        auto fib = Solver::fibonacciSequence(n);

        for (auto i : fib) {
            cout << i << " ";
        }
        cout << endl;
    }
    catch (const string& error) {
        cout << "Error: " << error << endl;
    }

    //2
    cout << "Enter number to check for palindrome: ";
    cin >> n;

    if (Solver::isPalindrome(n)) cout << n << " is palindrom" << endl;
    else cout << n << " is not polindrom" << endl;

    //3
    cout << "Enter size of list: ";
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    ListNode* list = Solver::createList(a);
    ListNode* reversed = Solver::reverseList(list);
    vector<int> result = Solver::listToVector(reversed);

    cout << "Original: ";
    for (int val : a) cout << val << " ";
    cout << "\nReversed: ";
    for (int val : result) cout << val << " ";
    cout << endl;

    Solver::deleteList(reversed);


	return 0;
}