#include "stdafx.h"
#include "ShareMemory.h"


CShareMemory::CShareMemory():m_pSharedMomory(nullptr)
{
}


CShareMemory::~CShareMemory()
{
	if (m_pSharedMomory)
	{
		DestorySharedMemory();
	}
}

bool CShareMemory::CreateSharedMemory()
{
	m_hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,sizeof(SHARED_MEMORY) , MAP_NAME);
	if (!m_hMapping)
	{
		return false;
	}
	m_pSharedMomory = (SHARED_MEMORY *)MapViewOfFile(m_hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (!m_pSharedMomory)
	{
		return false;
	}
	return true;
}

bool CShareMemory::DestorySharedMemory()
{
	if (m_pSharedMomory)
	{
		UnmapViewOfFile(m_pSharedMomory);
		m_pSharedMomory = nullptr;
	}
	if (m_hMapping)
	{
		CloseHandle(m_hMapping);
	}
	return true;
}

PSHARED_MOMERY CShareMemory::GetPointerOfMapView()
{
	return m_pSharedMomory;
}

SHARED_MEMORY* CShareMemory::GetShareMemoryPointer()
{
	return m_pSharedMomory;
}
