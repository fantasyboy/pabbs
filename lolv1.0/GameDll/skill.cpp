#include "stdafx.h"
#include "skill.h"
#include "GameCall.h"
skill::skill(DWORD _index, DWORD dwNodeBase) :base(dwNodeBase), m_index(_index)
{

}

skill::~skill()
{
}

char* skill::GetName() const
{
	__try {
		auto temp = utils::GetInstance()->read<DWORD>(GetNodeBase() + Base_SkillOffset_Object);
		if (temp == 0)
		{
			return nullptr;
		}
		//读名字或者名字指针
		if (utils::GetInstance()->read<DWORD>(temp +0x28) <0xf)
		{
			return (char*)(temp + 0x18);
		}
		else
		{
			auto nameptr = utils::GetInstance()->read<DWORD>(temp + 0x18);
			return ((char*)nameptr);
		}
		
	}
	__except (1) {
		utils::GetInstance()->log("ERROR: skill::GetName()异常！\n");
		return nullptr;
	}
}

DWORD skill::GetLevel() const
{
	__try
	{
		return utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x10);
	}
	__except(1)
	{
		utils::GetInstance()->log("ERROR: skill::GetLevel() 调用异常！\n");
		return 0;
	}
}

bool skill::bCoolDown()
{
	return GameCall::GetInstance()->GetClientTickTime() > utils::GetInstance()->read<float>(GetNodeBase() + 0x18);
}

//bool skill::bCouldUse()
//{
//	//技能可用的条件为 等级 > 0  && 冷却 && 当前蓝 > 技能消耗的蓝
//	return GetLevel() > 0 && bCoolDown();
//}

float skill::GetExpendMP()
{
	__try {
		auto temp = utils::GetInstance()->read<DWORD>(GetNodeBase() + Base_SkillOffset_Object);
		if (temp == 0) {
			utils::GetInstance()->log("ERROR: 读取技能对象偏移1失败！\n");
			return 0;
		}
		auto temp2 = utils::GetInstance()->read<DWORD>(temp + 0x34);
		if (temp2 == 0)
		{
			utils::GetInstance()->log("ERROR: 读取技能对象偏移2失败！\n");
			return 0;
		}
		return utils::GetInstance()->read<float>(temp2 + Base_SkillOffset_MP + GetLevel() * 4);
	}
	__except (1)
	{
		utils::GetInstance()->log("ERORR: skill::GetExpendMP()出现异常！\n");
		return 0;
	}
}

float skill::GetMaxCoolTime()
{
	return utils::GetInstance()->read<float>(GetNodeBase() + 0x44);
}

float skill::GetSkillRange()
{
	__try {
		auto temp = utils::GetInstance()->read<DWORD>(GetNodeBase() + Base_SkillOffset_Object);
		if (temp == 0) {
			utils::GetInstance()->log("ERROR: 读取技能对象偏移1失败！\n");
			return 0;
		}
		auto temp2 = utils::GetInstance()->read<DWORD>(temp + 0x34);
		if (temp2 == 0)
		{
			utils::GetInstance()->log("ERROR: 读取技能对象偏移2失败！\n");
			return 0;
		}

		return utils::GetInstance()->read<float>(temp2 + Base_SkillOffset_Range + GetLevel() * 4);
	}
	__except (1) {
		utils::GetInstance()->log("ERROR: skill::GetSkillRange()出现异常！\n");
		return 0;
	}
}

DWORD skill::GetIndex() const
{
	return m_index;
}
