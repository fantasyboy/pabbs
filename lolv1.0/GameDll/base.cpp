#include "stdafx.h"
#include "base.h"
base::base(_In_ DWORD dwNodeBase): m_nodeBase(dwNodeBase)
{
}

base::~base()
{

}

DWORD base::GetNodeBase() const
{
	return m_nodeBase;
}

char* base::GetName() const
{
	//获取到名字的指针地址
	__try {
		auto temp = utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x20);
		return (char*)(temp);
	}
	__except (1)
	{
		utils::GetInstance()->log("ERROR: base::GetName() 异常！\n");
		return nullptr;
	}
}
