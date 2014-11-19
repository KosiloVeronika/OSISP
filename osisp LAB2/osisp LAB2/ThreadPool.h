#include "WorkThread.h"
#include <vector>

namespace ThreadPool
{
	class MyThreadPool
	{
	public:
		MyThreadPool(int n, void(&function)());
		
		std::vector<WorkThread::MyWorkThread*> threadList;
		void SetTask(void(&function)(int time));
		int GetFreeThread();
		void(*LogFunction)();
	};
}