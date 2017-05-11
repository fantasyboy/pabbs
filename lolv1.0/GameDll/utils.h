#pragma once
#include <mutex>
#include "StringCovert.h"
class utils
{
public:
	~utils();
    static utils* GetInstance();
	void log(char* _format, ...);
	template<class T> T read(const DWORD& _dwNodeBase);
	template<class T> bool write(DWORD _deNodeBase, const T& value);
private:
	static utils* m_pInstance;
	static std::mutex m_mutex;
private:
	utils();
	utils(const utils& _u);
	void operator= (const utils& _u);
};

template<class T>
T utils::read(const DWORD& _dwNodeBase)
{
	T value;
	__try
	{
		DWORD oldProtected = 0;
		if(VirtualProtect((LPVOID)_dwNodeBase, sizeof(T), PAGE_READWRITE, &oldProtected))
		{
			memcpy(&value, (LPVOID)_dwNodeBase, sizeof(T));
			if (VirtualProtect((LPVOID)_dwNodeBase, sizeof(T), oldProtected, &oldProtected))
			{
				return value;
			}
		}
	}
	__except (1)
	{

	}
	return 0;
}

template<class T>
bool utils::write(DWORD _dwNodeBase, const T& value)
{
	__try
	{
		DWORD oldProtected = 0;
		if (VirtualProtect((LPVOID)_dwNodeBase, sizeof(T), PAGE_READWRITE, &oldProtected))
		{
			memcpy((LPVOID)_dwNodeBase, &value, sizeof(T));
			if (VirtualProtect((LPVOID)_dwNodeBase, sizeof(T), oldProtected, &oldProtected))
			{
				return true;
			}
		}
	}
	__except (1)
	{

	}
	return false;
}

