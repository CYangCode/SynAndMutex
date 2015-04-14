/**
作者：wwj
时间：2012/4/12
功能：实现生产者和消费者正常活动

题目内容：生产者-消费者问题，是指两组进程共享一个环形的缓冲区。一组进程被称为生产者，另一组进程被称为消费者。
缓冲池是由若干个（程序假设为4个）大小相等的缓冲区组成的，每个缓冲区可以容纳一个产品。生产者进程不断地将生产的产品放入缓冲池，
消费者进程不断地将产品从缓冲池取出。用PV操作实现生产者和消费者的正常活动的程序

题目分析：
在生产者-消费者问题中，既存在进程同步问题，也存在着临界区的互斥问题。当缓冲区都满时，表示供大于求，
生产者停止生产，进入等待状态，同时唤醒消费者；当缓冲区都空时，表示供不应求，消费者停止消费，唤醒生产者。
这说明了，生产者和消费者存在同步关系。
对于缓冲池，它显然是一个临界资源，所有的生产者和消费者都要使用它，
而且都要改变它的状态，故对于缓冲池的操作必须是互斥的。

*/

#include<Windows.h>
#include<iostream>
using namespace std;
const int n = 4;

//声明全局变量
int i = 0, j = 0;	//i和j分别指向缓冲区
int buffer[n];	//缓冲池
int ItemP = 0;	//用来存放生产的产品
int ItemC = 0;	//用来存放消费的产品

//声明句柄
HANDLE mutex;	//缓冲池信号量
HANDLE empty;	//空缓冲区信号量
HANDLE full;	//满缓冲区信号量
HANDLE pThread;	//producer线程句柄
HANDLE p2Thread;
HANDLE cThread;	//consumer线程句柄

//声明进程函数
DWORD WINAPI producer(LPVOID IpParameter);
DWORD WINAPI consumer(LPVOID IpParameter);

int main()
{
	//创建信号量
	mutex = CreateSemaphore(NULL, 1, 1, NULL);
	empty = CreateSemaphore(NULL, 1, 4, NULL);
	full = CreateSemaphore(NULL, 0, 4, NULL);

	cout << "市场运作开始。。。。。" << endl;

	//创建线程
	pThread = CreateThread(NULL, 0, producer, NULL, 0, NULL);
	//p2Thread = CreateThread(NULL, 0, producer, NULL, 0, NULL);
	cThread = CreateThread(NULL, 0, consumer, NULL, 0, NULL);

	//等待相应线程结束
	WaitForSingleObject(pThread, INFINITE);
	WaitForSingleObject(cThread, INFINITE);


	//关闭线程句柄
	CloseHandle(pThread);
	CloseHandle(cThread);
	//CloseHandle(p2Thread);

	//关闭信号量句柄
	CloseHandle(mutex);
	CloseHandle(empty);
	CloseHandle(full);

	cout << "整个市场运营结束。。。。" << endl;
	return 0;
}

//producer线程函数的定义
DWORD WINAPI producer(LPVOID IpParameter)
{
	for (int k = 0; k < 5; k++)
	{
		cout << "\nproducer生产一个产品" << endl;
		ItemP = ItemP + 1;			//增加一个产品
		WaitForSingleObject(empty, INFINITE);	//P操作
		WaitForSingleObject(mutex, INFINITE);
		cout << "\n把一个产品放入了一个空的缓冲区" << endl;
		buffer[i] = ItemP;		//将产品放入缓冲区
		i = (i + 1) % n;
		ReleaseSemaphore(mutex, 1, NULL);		//V操作
		ReleaseSemaphore(full, 1, NULL);
	}
	return 0;
}

DWORD WINAPI consumer(LPVOID IpParameter)
{
	for (int k = 0; k < 5; k++)
	{
		WaitForSingleObject(full, INFINITE);	//P操作
		WaitForSingleObject(mutex, INFINITE);
		ItemC = buffer[j];		//将缓冲区里的产品取出，放入消费产品里头
		j = (j + 1) % n;				//j指向下一个满的缓冲区
		ReleaseSemaphore(mutex, 1, NULL);		//V操作
		ReleaseSemaphore(empty, 1, NULL);
		cout << "\n消费者消费一个产品" << endl;
		ItemC = ItemC - 1;			//消费一个产品

	}
	return 0;
}


