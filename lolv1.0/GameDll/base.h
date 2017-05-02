#pragma once
#include "utils.h"
class base {
	
public:
	base(_In_ DWORD dwNodeBase);
	~base();

	DWORD GetNodeBase()const;
	virtual char* GetName() const;

protected:
	DWORD m_nodeBase;
};


