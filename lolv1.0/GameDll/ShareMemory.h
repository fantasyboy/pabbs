#pragma once
#include "stdafx.h"

struct AAAA 
{
	int a;
};

template<class T>
class ShareMemory
{
public:
	ShareMemory(std::string fileName);
	~ShareMemory();
	bool openShareMemory();
	T* GetPointerOfFile(){ return m_pFileData; }
private:
	HANDLE m_FileHandle;
	T* m_pFileData;
	std::string m_fileMapName;
};

template<class T>
bool ShareMemory<T>::openShareMemory()
{
	m_FileHandle = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, false, m_fileMapName.c_str());
	//utils::GetInstance()->log("HXL:窗口句柄:%x %s", m_FileHandle, m_fileMapName.c_str());
	if (m_FileHandle != NULL)
	{
		m_pFileData = (T*)MapViewOfFile(m_FileHandle, FILE_MAP_READ | FILE_MAP_WRITE, NULL, NULL, NULL);
	}
	return (m_pFileData != nullptr);
}

template<class T>
ShareMemory<T>::~ShareMemory()
{
	if (m_pFileData)
	{
		UnmapViewOfFile(m_pFileData);
		CloseHandle(m_FileHandle); //是否需要关闭句柄
		m_pFileData = nullptr;
	}
}

template<class T>
ShareMemory<T>::ShareMemory(std::string fileName)
{
	m_fileMapName = fileName;
	m_pFileData = nullptr;
	m_FileHandle = { 0 };
}

