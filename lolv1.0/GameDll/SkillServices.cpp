#include "stdafx.h"
#include "SkillServices.h"
#include "BaseAddr.h"

CSkillServices::CSkillServices()
{
}


CSkillServices::~CSkillServices()
{
}

void CSkillServices::travse()
{
	m_skillList.clear();

	auto dwBase = utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr) + Base_SkillTravseOffset1 + Base_SkillTravseOffset2;
	for (auto i = 0 ; i != 0x3f; i++)
	{
		auto skillBase = utils::GetInstance()->read<DWORD>(dwBase + i * 4);
		//技能地址 或者 技能对象 不存在，就继续遍历
		if (skillBase == 0 || 0 == utils::GetInstance()->read<DWORD>(skillBase + Base_SkillOffset_Object))
		{
			continue;
		}
		
		m_skillList.push_back(skill(i, skillBase));
	}
	utils::GetInstance()->log("TIPS: 技能数量为：%d\n", m_skillList.size());
	for (auto temp : m_skillList)
	{
		utils::GetInstance()->log("TIPS: %x %s %d %d %d %f %f %f\n",
			temp.GetNodeBase(),
			temp.GetName(),
			temp.GetIndex(),
			temp.GetLevel(),
			temp.bCoolDown(),
			temp.GetExpendMP(),
			temp.GetMaxCoolTime(),
			temp.GetSkillRange());
	}
}

