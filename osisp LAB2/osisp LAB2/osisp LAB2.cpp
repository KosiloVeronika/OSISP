// osisp LAB2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "ThreadPool.h"

void function();
void Task(int time);

int _tmain(int argc, _TCHAR* argv[])
{
	int n = 0;
	printf("Enter number of threads : ");
	scanf("%d", &n);
	ThreadPool::MyThreadPool* threadPool = new ThreadPool::MyThreadPool(n, *function);
	while (true)
	{
		char c;
		fflush(stdin);
		printf("Press Enter to add task or q to exit...\n");
		scanf("%c", &c);
		if (c == '\n')
		{
			threadPool->SetTask(*Task);
		}
		else if (c == 'q')
		{
			break;
		}

	}
	return 0;
}

void Task(int time)
{
	Sleep(time);
}

void function()
{

}
