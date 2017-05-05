#pragma once
#include <string>
class CInjectDll
{
public:
	CInjectDll(std::string wndName , std::string className,std::string dllName);
	~CInjectDll();

	bool injectDll();
	bool getResult()const;
private:
	bool m_bInjectDllSuccess;
	std::string m_wndName;
	std::string m_className;
	std::string m_dllName;
};

