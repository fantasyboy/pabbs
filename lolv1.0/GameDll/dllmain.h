#pragma once
#include "CShareStruct.h"
DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
);

extern SHARED_MEMORY* pSharedMemoryPointer;