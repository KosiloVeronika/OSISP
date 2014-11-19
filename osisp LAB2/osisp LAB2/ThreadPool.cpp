#include "ThreadPool.h"
#include "time.h"

namespace ThreadPool
{
	MyThreadPool::MyThreadPool(int n, void (&function)())
	{
		LogFunction = function;
		for (int i = 0; i < n; i++)
		{
			WorkThread::MyWorkThread* object = new WorkThread::MyWorkThread(*LogFunction);
			threadList.push_back(object);
		}
	}

	void MyThreadPool::SetTask(void(&function)(int time))
	{
		srand(time(NULL));
		int freeIndex = GetFreeThread();
		int time = rand()%10000 + 1;
		threadList[freeIndex]->WakeThread(time, *function);
	}

	int MyThreadPool::GetFreeThread()
	{
		while (true)
		{
			for (int i = 0; i < threadList.size(); i++)
			{
				if (!threadList[i]->isBusy)
				{
					return i;
				}
			}
			//логировать
		}
	}
}