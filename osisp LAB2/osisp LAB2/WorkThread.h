#include "windows.h"
#include "condition_variable"

namespace WorkThread
{
	class MyWorkThread
	{
	public:
		MyWorkThread(void(&function)(char str[255], DWORD threadID));
		~MyWorkThread();

		void				(*LogFunction)(char str[255], DWORD threadID);
		void				(*TaskFunction)(int time);
		void				WakeThread(int time, void(&function)(int time));
		void				DestroyThread();
		
		HANDLE				thread;
		CRITICAL_SECTION	criticalSection;
		CRITICAL_SECTION	criticalSectionForTask;
		CONDITION_VARIABLE	conditionVariable;

		bool				isBusy;
		bool				isEnabled;
		bool				error;
		int					waitTime;
		DWORD				threadID;
	};
}