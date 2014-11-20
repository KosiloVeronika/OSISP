#include "WorkThread.h"

namespace WorkThread
{
	DWORD WINAPI ThreadFunction(LPVOID lParam);

	MyWorkThread::MyWorkThread(void(&Log)(char str[255], DWORD threadID))
	{
		LogFunction = Log;
		isBusy = false;
		isEnabled = true;
		error = false;
		waitTime = 0;
		InitializeCriticalSection(&criticalSection);
		InitializeCriticalSection(&criticalSectionForTask);
		InitializeConditionVariable(&conditionVariable);
		thread = CreateThread(NULL, 0, ThreadFunction, (void*) this, NULL, &threadID);
	}

	MyWorkThread::~MyWorkThread()
	{
		WaitForSingleObject(thread, INFINITE);
	}

	void MyWorkThread::WakeThread(int time, void(&function)(int time))
	{
		waitTime = time;
		TaskFunction = function;
		isBusy = true;
		WakeConditionVariable(&conditionVariable);
	}

	void MyWorkThread::DestroyThread()
	{
		isEnabled = false;
		WakeConditionVariable(&conditionVariable);
	}



	DWORD WINAPI ThreadFunction(LPVOID lParam)
	{
		MyWorkThread *object = (MyWorkThread *)lParam;
		char str[255] = "Thread created";
		object->LogFunction(str, object->threadID);
		while (object->isEnabled)
		{
			EnterCriticalSection(&object->criticalSection);
			LeaveCriticalSection(&object->criticalSectionForTask);
			SleepConditionVariableCS(&object->conditionVariable, &object->criticalSection, INFINITE);
			EnterCriticalSection(&object->criticalSectionForTask);
			if ((object->isBusy) && (object->isEnabled))
			{
				strcpy(str,"Task added");
				object->LogFunction(str, object->threadID);
				try
				{
					object->TaskFunction(object->waitTime);
				}
				catch (...)
				{
					object->error = true;
					strcpy(str, "Error");
					object->LogFunction(str, object->threadID);
					continue;
				}
				object->isBusy = false;
				strcpy(str, "Task done");
				object->LogFunction(str, object->threadID);
			}
		}


		strcpy(str, "Thread destroyed");
		object->LogFunction(str, object->threadID);
		return 0;
	}
}