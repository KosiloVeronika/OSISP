// osisp_3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	char str[255];
	HANDLE mutex = CreateMutex(NULL, false, L"Global\mutex");//объ€вление мьютекса
	HANDLE sharedMemory = CreateFileMapping(NULL, NULL, PAGE_READWRITE, 0, 16, L"memory");//создание в €дре общей пам€ти
	if (sharedMemory != NULL)
	{
		void* adress = MapViewOfFile(sharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, 255);
		while (true)
		{
			WaitForSingleObject(mutex, INFINITE);
			std::cout << "Enter message...\n";
			std::cin >> str;
			memcpy(adress, str, 255);
			ReleaseMutex(mutex);
			std::cout << "Press r to read message...\n";

			char c;
			std::cin >> c;
			if (c != 'r')
			{
				UnmapViewOfFile(adress);
				break;
			}

			WaitForSingleObject(mutex, INFINITE);
			memcpy(str, adress, 255);
			std::cout << str << '\n';
			ReleaseMutex(mutex);
		}
	}
	return 0;
}

