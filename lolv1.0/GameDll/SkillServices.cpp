#include "stdafx.h"
#include "SkillServices.h"
#include "BaseAddr.h"

CSkillServices::CSkillServices(DWORD dwObjectBase): m_dwObjectBase(dwObjectBase)
{

}

CSkillServices::~CSkillServices()
{
}

void CSkillServices::travse()
{
	m_skillList.clear();

	auto dwBase = m_dwObjectBase + pSharedMemoryPointer->Base_SkillTravseOffset1 + pSharedMemoryPointer->Base_SkillTravseOffset2;
	for (auto i = 0 ; i != 0x3f; i++)
	{
		auto skillBase = utils::GetInstance()->read<DWORD>(dwBase + i * 4);
		//技能地址 或者 技能对象 不存在，就继续遍历
		if (skillBase == 0 || 0 == utils::GetInstance()->read<DWORD>(skillBase + pSharedMemoryPointer->Base_SkillOffset_Object))
		{
			continue;
		}
		
		m_skillList.push_back(skill((EM_SKILL_INDEX)i, skillBase));
	}
	//utils::GetInstance()->log("TIPS: 技能数量为：%d\n", m_skillList.size());
	//for (auto temp : m_skillList)
	//{
	//	utils::GetInstance()->log("TIPS: %x %s %d %d %d %f %f %f\n",
	//		temp.GetNodeBase(),
	//		temp.GetName(),
	//		temp.GetIndex(),
	//		temp.GetLevel(),
	//		temp.bCoolDown(),
	//		temp.GetExpendMP(),
	//		temp.GetMaxCoolTime(),
	//		temp.GetSkillRange());
	//}
}

skill CSkillServices::GetSkillObjectByIndex(DWORD dwIndex)
{
	travse();
	if (dwIndex >= m_skillList.size())
	{
		//如果传入的索引 > 技能的索引 返回第一个技能
		return m_skillList.at(0);
	}
	return m_skillList.at(dwIndex);
}

