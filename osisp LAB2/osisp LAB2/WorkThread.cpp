#include "WorkThread.h"

namespace WorkThread
{
	DWORD WINAPI ThreadFunction(LPVOID lParam);

	MyWorkThread::MyWorkThread(void(&Log)())
	{
		LogFunction = Log;
		isBusy = false;
		isEnabled = true;
		waitTime = 0;
		InitializeCriticalSection(&criticalSection);
		InitializeConditionVariable(&conditionVariable);
		thread = CreateThread(NULL, 0, ThreadFunction, (void*) this, NULL, &threadID);
	}

	void MyWorkThread::WakeThread(int time, void(&function)(int time))
	{
		waitTime = time;
		TaskFunction = function;
		isBusy = true;
		WakeConditionVariable(&conditionVariable);
	}

	DWORD WINAPI ThreadFunction(LPVOID lParam)
	{
		MyWorkThread *object = (MyWorkThread *)lParam;
		while (object->isEnabled)
		{
			EnterCriticalSection(&object->criticalSection);
			SleepConditionVariableCS(&object->conditionVariable, &object->criticalSection, INFINITE);
			if ((object->isBusy) && (object->isEnabled))
			{
				object->TaskFunction(object->waitTime);
				object->isBusy = false;
			}
		}
		return 0;
	}
}