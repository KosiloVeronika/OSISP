// osisp LAB2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "time.h"
#include "stdlib.h"
#include "ThreadPool.h"
#include <iostream>
#include <fstream>

void function(char str[255], DWORD threadID);
void Task(int time);
CRITICAL_SECTION fileCriticalSection;

int _tmain(int argc, _TCHAR* argv[])
{
	int n = 0;
	printf("Enter number of threads : ");
	scanf("%d", &n);
	InitializeCriticalSection(&fileCriticalSection);
	ThreadPool::MyThreadPool* threadPool = new ThreadPool::MyThreadPool(n, *function);
	while (true)
	{
		char c;
		fflush(stdin);
		printf("Press Enter to add task or q to exit...\n");
		scanf("%c", &c);
		if (c == '\n')
		{
			if (threadPool->SetTask(*Task) == -1)
			{
				threadPool->~MyThreadPool();
				break;
			}
		}
		else if (c == 'q')
		{
			threadPool->~MyThreadPool();
			break;
		}
	}
	return 0;
}

void Task(int time)
{
	throw 1;
}

void function(char str[255], DWORD threadID)
{
	EnterCriticalSection(&fileCriticalSection);
	time_t currentTime = time(NULL);
	std::ofstream f("log.log", std::ios::app);
	f << threadID << ' ' << asctime(localtime(&currentTime)) << ':' << str << '\n';
	LeaveCriticalSection(&fileCriticalSection);
}
