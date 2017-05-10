#pragma once
#include <string>
class CInjectDll
{
public:
	CInjectDll(std::string dllName);
	~CInjectDll();

	bool injectDll(HWND hwnd);
	void SetInject(bool val);
	bool GetInject() { return m_bInjectDllSuccess; }
private:
	bool m_bInjectDllSuccess;
	std::string m_dllName;
};

