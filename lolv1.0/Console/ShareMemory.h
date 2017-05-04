#pragma once
#include "CShareStruct.h"
class CShareMemory
{
public:
	CShareMemory();
	~CShareMemory();

	//创建共享内存
	bool CreateSharedMemory();
	//销毁共享内存
	bool DestorySharedMemory();
	PSHARED_MOMERY GetPointerOfMapView();
private:
	SHARED_MEMORY *m_pSharedMomory;
	HANDLE m_hMapping;
};

