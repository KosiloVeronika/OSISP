#include "windows.h"
#include "condition_variable"

namespace WorkThread
{
	class MyWorkThread
	{
	public:
							MyWorkThread(void (&function)());

		void				(*LogFunction)();
		void				(*TaskFunction)(int time);
		void				WakeThread(int time, void(&function)(int time));
		
		HANDLE				thread;
		CRITICAL_SECTION	criticalSection;
		CONDITION_VARIABLE	conditionVariable;

		bool				isBusy;
		bool				isEnabled;
		int					waitTime;
		DWORD				threadID;
	};
}