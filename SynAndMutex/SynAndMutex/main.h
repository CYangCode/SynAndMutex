#include <windows.h>
#include <iostream>

using namespace std;

int flag = false;

HANDLE mutex;
HANDLE thread;

struct Para
{
	LPVOID lpParameter;
	int flag;
};

DWORD WINAPI change(LPVOID  parameter);
void Func1()
{
	mutex = CreateSemaphore(NULL, 1, 1, NULL);
	Para p;
	thread = CreateThread(NULL, 0, change, p, )
	WaitForSingleObject(mutex, INFINITE);
	if (flag == true){
		cout << "hello" << endl;
	}

	CloseHandle(mutex);
}