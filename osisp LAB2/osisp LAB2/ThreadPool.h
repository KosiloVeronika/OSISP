#include "WorkThread.h"
#include <vector>

namespace ThreadPool
{
	class MyThreadPool
	{
	public:
		MyThreadPool(int n, void(&function)(char str[255], DWORD threadID));
		~MyThreadPool();
		
		std::vector<WorkThread::MyWorkThread*> threadList;
		int SetTask(void(&function)(int time));
		int GetFreeThread();
		void(*LogFunction)(char str[255], DWORD threadID);
	};
}