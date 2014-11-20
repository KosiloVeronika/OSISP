#include "ThreadPool.h"
#include "time.h"

namespace ThreadPool
{
	MyThreadPool::MyThreadPool(int n, void (&function)(char str[255], DWORD threadID))
	{
		LogFunction = function;
		for (int i = 0; i < n; i++)
		{
			WorkThread::MyWorkThread* object = new WorkThread::MyWorkThread(*LogFunction);
			threadList.push_back(object);
		}

		char str[255] = "Thread pool created";
		LogFunction(str, GetThreadId(NULL));
	}

	MyThreadPool::~MyThreadPool()
	{
		for (int i = 0; i < threadList.size(); i++)
		{
			threadList[i]->isEnabled = false;
			threadList[i]->DestroyThread();
			threadList[i]->~MyWorkThread();
		}
		char str[255] = "Thread pool destroyed";
		LogFunction(str, GetThreadId(NULL));
	}

	int MyThreadPool::SetTask(void(&function)(int time))
	{
		srand(time(NULL));
		int freeIndex = GetFreeThread();
		if (freeIndex == -1)
			return -1;
		int time = rand()%10000 + 1;
		EnterCriticalSection(&threadList[freeIndex]->criticalSectionForTask);
		threadList[freeIndex]->WakeThread(time, *function);
		LeaveCriticalSection(&threadList[freeIndex]->criticalSectionForTask);
		char str[255] = "Task added to thread";
		LogFunction(str, GetThreadId(NULL));
		return 0;
	}

	int MyThreadPool::GetFreeThread()
	{
		bool busy = false;
		while (true)
		{
			for (int i = 0; i < threadList.size(); i++)
			{
				if (!threadList[i]->isBusy)
				{
					return i;
				}
				else
				{
					if (threadList[i]->error)
					{
						threadList[i]->DestroyThread();
						threadList[i]->~MyWorkThread();
						threadList.erase(threadList.begin() + i);
						i--;
						continue;
					}
				}
			}
			if (threadList.size() == 0)
				return -1;
			if (!busy)
			{
				char str[255] = "Warning : All threads are busy";
				LogFunction(str, GetThreadId(NULL));
				busy = true;
			}
		}
	}
}