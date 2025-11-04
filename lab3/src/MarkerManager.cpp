#include "MarkerManager.h"
#include <iostream>
#include <random>

MarkerManager::MarkerManager(int arrSize, int tCount)
    : arraySize(arrSize), threadCount(tCount) {

    array.resize(arraySize, 0);
    stopEvents.resize(threadCount);
    terminateEvents.resize(threadCount);
    continueEvents.resize(threadCount);
    threadHandles.resize(threadCount);
    threadIds.resize(threadCount);
    threadActive.resize(threadCount, true);

    InitializeCriticalSection(&criticalSection);
    startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    for (int i = 0; i < threadCount; i++) {
        stopEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        terminateEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        continueEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }
}

MarkerManager::~MarkerManager() {
    for (int i = 0; i < threadCount; i++) {
        if (threadHandles[i]) {
            SetEvent(terminateEvents[i]);
            WaitForSingleObject(threadHandles[i], 1000);
            CloseHandle(threadHandles[i]);
        }
    }

    DeleteCriticalSection(&criticalSection);
    CloseHandle(startEvent);

    for (int i = 0; i < threadCount; i++) {
        CloseHandle(stopEvents[i]);
        CloseHandle(terminateEvents[i]);
        CloseHandle(continueEvents[i]);
    }
}

DWORD WINAPI MarkerManager::MarkerThread(LPVOID param) {
    ThreadData* data = static_cast<ThreadData*>(param);
    data->manager->MarkerWorker(data->threadId);
    delete data;
    return 0;
}

void MarkerManager::MarkerWorker(int threadId) {
    WaitForSingleObject(startEvent, INFINITE);

    std::mt19937 generator(threadId);
    int markedCount = 0;
    bool working = true;

    while (working) {
        int index = generator() % arraySize;

        EnterCriticalSection(&criticalSection);

        if (array[index] == 0) {
            Sleep(5);
            array[index] = threadId;
            Sleep(5);
            markedCount++;
            LeaveCriticalSection(&criticalSection);
        }
        else {
            std::cout << "Thread " << threadId << " stopped. Marked " << markedCount
                << " cells. Blocked at index " << index << std::endl;
            LeaveCriticalSection(&criticalSection);

            SetEvent(stopEvents[threadId - 1]);

            HANDLE waitEvents[2] = { terminateEvents[threadId - 1], continueEvents[threadId - 1] };
            DWORD result = WaitForMultipleObjects(2, waitEvents, FALSE, INFINITE);

            if (result == WAIT_OBJECT_0) {
                EnterCriticalSection(&criticalSection);
                for (int i = 0; i < arraySize; i++) {
                    if (array[i] == threadId) {
                        array[i] = 0;
                    }
                }
                LeaveCriticalSection(&criticalSection);
                working = false;
                std::cout << "Thread " << threadId << " terminated." << std::endl;
            }
            else if (result == WAIT_OBJECT_0 + 1) {
                ResetEvent(stopEvents[threadId - 1]);
                markedCount = 0;
                std::cout << "Thread " << threadId << " resumed work." << std::endl;
            }
        }
    }
}

void MarkerManager::StartThreads() {
    for (int i = 0; i < threadCount; i++) {
        ThreadData* data = new ThreadData{ this, i + 1 };
        threadHandles[i] = CreateThread(NULL, 0, MarkerThread, data, 0, &threadIds[i]);

        if (threadHandles[i] == NULL) {
            std::cerr << "Failed to create thread " << (i + 1) << std::endl;
            return;
        }
    }

    SetEvent(startEvent);
}

void MarkerManager::WaitForThreadsToBlock() {
    std::vector<HANDLE> currentStopEvents;
    for (int i = 0; i < threadCount; i++) {
        if (threadActive[i]) {
            currentStopEvents.push_back(stopEvents[i]);
        }
    }

    if (!currentStopEvents.empty()) {
        WaitForMultipleObjects(currentStopEvents.size(), currentStopEvents.data(), TRUE, INFINITE);
    }
}

void MarkerManager::TerminateThread(int threadId) {
    if (threadId < 1 || threadId > threadCount || !threadActive[threadId - 1]) {
        return;
    }

    SetEvent(terminateEvents[threadId - 1]);
    WaitForSingleObject(threadHandles[threadId - 1], 5000);
    threadActive[threadId - 1] = false;
}

void MarkerManager::ResumeThreads() {
    for (int i = 0; i < threadCount; i++) {
        if (threadActive[i]) {
            SetEvent(continueEvents[i]);
        }
    }
}

bool MarkerManager::AreAllThreadsTerminated() const {
    for (bool active : threadActive) {
        if (active) return false;
    }
    return true;
}

void MarkerManager::WaitForCompletion() {
    for (int i = 0; i < threadCount; i++) {
        if (threadHandles[i]) {
            WaitForSingleObject(threadHandles[i], INFINITE);
        }
    }
}

void MarkerManager::Run() {
    std::cout << "Creating " << threadCount << " marker threads..." << std::endl;

    StartThreads();

    std::cout << "All threads created. Starting work..." << std::endl;

    int activeThreads = threadCount;

    while (activeThreads > 0) {
        std::cout << "\n=== Waiting for threads to block (" << activeThreads << " active) ===" << std::endl;

        WaitForThreadsToBlock();

        std::cout << "\nAll active threads are blocked!" << std::endl;
        PrintArray();

        int threadToTerminate;
        std::cout << "Enter thread number to terminate (1-" << threadCount << "): ";
        std::cin >> threadToTerminate;

        if (threadToTerminate < 1 || threadToTerminate > threadCount) {
            std::cout << "Invalid thread number! Must be between 1 and " << threadCount << std::endl;
            continue;
        }

        if (!threadActive[threadToTerminate - 1]) {
            std::cout << "Thread " << threadToTerminate << " is already terminated!" << std::endl;
            continue;
        }

        std::cout << "Terminating thread " << threadToTerminate << "..." << std::endl;

        TerminateThread(threadToTerminate);
        activeThreads--;

        std::cout << "Array after thread " << threadToTerminate << " cleanup:" << std::endl;
        PrintArray();

        if (activeThreads > 0) {
            std::cout << "Resuming " << activeThreads << " remaining threads..." << std::endl;
            ResumeThreads();
            Sleep(500);
        }
    }

    std::cout << "\n=== All threads completed ===" << std::endl;
    std::cout << "Final array:" << std::endl;
    PrintArray();
}

void MarkerManager::PrintArray() const {
    EnterCriticalSection(&criticalSection);
    std::cout << "Array: [";
    for (int i = 0; i < arraySize; i++) {
        std::cout << array[i];
        if (i < arraySize - 1) std::cout << " ";
    }
    std::cout << "]" << std::endl;
    LeaveCriticalSection(&criticalSection);
}