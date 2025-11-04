#include "MarkerManager.h"
#include <iostream>

int main() {
    int arraySize, threadCount;

    std::cout << "Enter array size: ";
    std::cin >> arraySize;

    std::cout << "Enter number of threads: ";
    std::cin >> threadCount;

    if (arraySize <= 0 || threadCount <= 0) {
        std::cout << "Invalid input parameters!" << std::endl;
        return 1;
    }

    if (threadCount > arraySize) {
        std::cout << "Warning: More threads than array elements may cause immediate blocking." << std::endl;
    }

    MarkerManager manager(arraySize, threadCount);
    manager.Run();

    return 0;
}