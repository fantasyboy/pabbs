#pragma once
#include "utils.h"
#include "buffer.h"
#include <vector>
class CBufferServices
{
public:
	CBufferServices(DWORD dwObjBase);
	~CBufferServices();

	void travse();

private:
	std::vector<buffer> m_bufferList;
	DWORD m_dwObjectBase;
};

