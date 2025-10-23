#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

void ShowError(const char* message) {
    DWORD errorCode = GetLastError();
    std::cerr << message << " Error code: " << errorCode << std::endl;
}

void childMode() {
    std::vector<int> array;
    int size;

    DWORD dwRead;
    if (!ReadFile(GetStdHandle(STD_INPUT_HANDLE), &size, sizeof(size), &dwRead, NULL)) {
        ShowError("Error reading array size in child process");
        return;
    }

    array.resize(size);
    if (!ReadFile(GetStdHandle(STD_INPUT_HANDLE), array.data(), size * sizeof(int), &dwRead, NULL)) {
        ShowError("Error reading array data in child process");
        return;
    }

    int sum = 0;
    for (int num : array) {
        if (num > 0) {
            sum += num;
        }
    }

    DWORD dwWritten;
    if (!WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), &sum, sizeof(sum), &dwWritten, NULL)) {
        ShowError("Error writing result in child process");
        return;
    }
}

void parentMode() {
    int size;
    std::cout << "Size: ";
    std::cin >> size;

    std::vector<int> array(size);
    std::cout << "Elements:" << std::endl;
    for (int i = 0; i < size; ++i) {
        std::cin >> array[i];
    }

    HANDLE hStdinRd = NULL, hStdinWr = NULL;
    HANDLE hStdoutRd = NULL, hStdoutWr = NULL;
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    if (!CreatePipe(&hStdinRd, &hStdinWr, &saAttr, 0)) {
        ShowError("Error creating stdin pipe");
        return;
    }

    if (!CreatePipe(&hStdoutRd, &hStdoutWr, &saAttr, 0)) {
        ShowError("Error creating stdout pipe");
        CloseHandle(hStdinRd);
        CloseHandle(hStdinWr);
        return;
    }

    STARTUPINFOW si = { sizeof(STARTUPINFOW) };
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hStdinRd;
    si.hStdOutput = hStdoutWr;
    si.hStdError = hStdoutWr;

    PROCESS_INFORMATION pi = { 0 };

    wchar_t currentExePath[MAX_PATH];
    if (!GetModuleFileNameW(NULL, currentExePath, MAX_PATH)) {
        ShowError("Error getting executable path");
        CloseHandle(hStdinRd);
        CloseHandle(hStdinWr);
        CloseHandle(hStdoutRd);
        CloseHandle(hStdoutWr);
        return;
    }

    std::wstring commandLine = std::wstring(currentExePath) + L" child";

    if (!CreateProcessW(NULL, (LPWSTR)commandLine.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        ShowError("Error creating child process");
        CloseHandle(hStdinRd);
        CloseHandle(hStdinWr);
        CloseHandle(hStdoutRd);
        CloseHandle(hStdoutWr);
        return;
    }

    if (!CloseHandle(hStdinRd)) {
        ShowError("Error closing stdin read handle");
    }
    if (!CloseHandle(hStdoutWr)) {
        ShowError("Error closing stdout write handle");
    }

    DWORD dwWritten;
    if (!WriteFile(hStdinWr, &size, sizeof(size), &dwWritten, NULL)) {
        ShowError("Error writing array size to pipe");
    }

    if (!WriteFile(hStdinWr, array.data(), size * sizeof(int), &dwWritten, NULL)) {
        ShowError("Error writing array data to pipe");
    }

    if (!CloseHandle(hStdinWr)) {
        ShowError("Error closing stdin write handle");
    }

    int result;
    DWORD dwRead;
    if (!ReadFile(hStdoutRd, &result, sizeof(result), &dwRead, NULL)) {
        ShowError("Error reading result from pipe");
    }

    if (WaitForSingleObject(pi.hProcess, INFINITE) == WAIT_FAILED) {
        ShowError("Error waiting for child process");
    }

    std::cout << "Sum of elements > 0: " << result << std::endl;

    if (!CloseHandle(pi.hProcess)) {
        ShowError("Error closing process handle");
    }
    if (!CloseHandle(pi.hThread)) {
        ShowError("Error closing thread handle");
    }
    if (!CloseHandle(hStdoutRd)) {
        ShowError("Error closing stdout read handle");
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "child") {
        childMode();
    }
    else {
        parentMode();
    }
    return 0;
}