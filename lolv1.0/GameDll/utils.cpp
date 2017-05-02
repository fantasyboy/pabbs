#include "stdafx.h"
#include "utils.h"

//
//声明单例模式用到的静态变量
//
utils* utils::m_pInstance = nullptr;
std::mutex utils::m_mutex;
utils::utils()
{
}


utils::utils(const utils& _u)
{

}

void utils::operator=(const utils& _u)
{

}

utils::~utils()
{
}

utils* utils::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_mutex.lock();
		if (nullptr == m_pInstance)
		{
			m_pInstance = new utils;
		}
		m_mutex.unlock();
	}
	return m_pInstance;
}

void utils::log(char* _format, ...)
{
#ifdef US_DEBUG
	std::string temp;
	va_list marker = { 0 };
	va_start(marker, _format);
	size_t num_of_chars = _vscprintf(_format, marker);
	if (num_of_chars > temp.capacity())
	{
		temp.reserve(num_of_chars + 1);
	}
	vsprintf_s(const_cast<char*>(temp.c_str()), num_of_chars+1, _format, marker);
	//vsprintf(const_cast<char*>(temp.c_str()), _format, marker);
	OutputDebugString(temp.c_str());
#endif // _debug
}
