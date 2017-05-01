#include "stdafx.h"
#include "BufferServices.h"
#include "BaseAddr.h"

CBufferServices::CBufferServices()
{
}


CBufferServices::~CBufferServices()
{
}

void CBufferServices::travse()
{
	m_bufferList.clear();

	DWORD dwBase = utils::GetInstance()->read<DWORD>(Base_BufferAddr);
	if (dwBase == 0)
	{
		utils::GetInstance()->log("ERROR: CBufferServices::travse() dwBase 出现异常！\n");
		return;
	}

	auto nodeBase = utils::GetInstance()->read<DWORD>(dwBase + 0x14);
	for (auto i = 0 ; i != 0x40; i++)
	{
		//读取buff对象
		auto temp = nodeBase + i * 0x34;
		if (utils::GetInstance()->read<DWORD>(temp+0x30) == 0xffffffff)
		{
			continue;
		}
		m_bufferList.push_back(buffer(temp));
		
	}
	utils::GetInstance()->log("TIPS: buff数量为：%d\n", m_bufferList.size());
	for (auto temp: m_bufferList)
	{
		utils::GetInstance()->log("TIPS: %x %s %d", temp.GetNodeBase(), temp.GetName(),temp.GetBufferCount());
	}
}
