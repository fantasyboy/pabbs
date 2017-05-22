#include "stdafx.h"
#include "BufferServices.h"
#include "BaseAddr.h"

CBufferServices::CBufferServices(DWORD dwObjBase) : m_dwObjectBase(dwObjBase)
{
}


CBufferServices::~CBufferServices()
{
}

void CBufferServices::travse()
{
	m_bufferList.clear();

	auto startAddr = utils::GetInstance()->read<DWORD>(m_dwObjectBase + pSharedMemoryPointer->Base_BufferOffset + 0x10);
	auto endAddr = utils::GetInstance()->read<DWORD>(m_dwObjectBase + pSharedMemoryPointer->Base_BufferOffset + 0x14);
	auto size = (endAddr - startAddr) >> 3;
	for (auto i = 0; i != size ; i++)
	{
		auto tempObj = utils::GetInstance()->read<DWORD>( startAddr + 8 * i);
		//utils::GetInstance()->log("tempObj %x ", tempObj);
		if (utils::GetInstance()->read<DWORD>(tempObj) == 0xFFFFFFFF)
		{
			//不是图标对象 继续
			continue;
		}

		m_bufferList.push_back(buffer(tempObj));
	}

 	for (auto temp : m_bufferList)
 	{
 		utils::GetInstance()->log("TIPS: BUFF NAME = %s count = %d", temp.GetName(), temp.GetBufferCount());
 	}

}

std::vector<buffer>& CBufferServices::GetBuffList()
{
	return m_bufferList;
}
