#pragma once

#include <vector>
#include <windows.h>
#include <functional>

class MarkerManager {
public:
    MarkerManager(int arrSize, int tCount);
    ~MarkerManager();

    void Run();

    void StartThreads();
    void WaitForThreadsToBlock();
    void TerminateThread(int threadId);
    void ResumeThreads();
    bool AreAllThreadsTerminated() const;
    void WaitForCompletion();

    void PrintArray() const;
    const std::vector<int>& GetArray() const { return array; }

private:
    struct ThreadData {
        MarkerManager* manager;
        int threadId;
    };

    std::vector<int> array;
    std::vector<HANDLE> threadHandles;
    std::vector<DWORD> threadIds;
    std::vector<HANDLE> stopEvents;
    std::vector<HANDLE> terminateEvents;
    std::vector<HANDLE> continueEvents;
    HANDLE startEvent;
    mutable CRITICAL_SECTION criticalSection;
    int arraySize;
    int threadCount;
    std::vector<bool> threadActive;

    static DWORD WINAPI MarkerThread(LPVOID param);
    void MarkerWorker(int threadId);
};