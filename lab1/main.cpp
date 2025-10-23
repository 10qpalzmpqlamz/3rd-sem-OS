#include <Windows.h>
#include <iostream>

struct Data {
	char* str;
	int size;
};

bool IsLatin(char c) { 
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); 
}

DWORD WINAPI WorkerThread(LPVOID _data)
{
	Data* data = static_cast<Data*>(_data);

	for (int i = 0; i < data->size; i++) {
		if (!IsLatin(data->str[i])) std::cout << data->str[i];
	}

	return 0;
}

int main() {
	
	int size;
	std::cin >> size;

	std::cin.ignore();
	char* str = new char[size + 1];
	std::cin.getline(str, size + 1);
	str[size] = '\0';

	DWORD sleep_time;
	std::cin >> sleep_time;

	Data* data = new Data{ str, size };

	HANDLE WThread = CreateThread(nullptr, 0, WorkerThread, data, CREATE_SUSPENDED, nullptr);

	Sleep(sleep_time);
	ResumeThread(WThread);
	WaitForSingleObject(WThread, INFINITE);

	CloseHandle(WThread);
	delete[] str;
	delete data;

	return 0;
}




