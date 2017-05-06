#include "stdafx.h"
#include "MonsterServices.h"
#include "BaseAddr.h"

CMonsterServices::CMonsterServices()
{
}


CMonsterServices::~CMonsterServices()
{
}

void CMonsterServices::travse()
{
	m_PersonList.clear();
	auto startAddr = utils::GetInstance()->read<DWORD>(Base_MonsterArrayAddr);
	auto endAddr = utils::GetInstance()->read<DWORD>(Base_MonsterArrayAddr + 0x4);
	for (auto i = startAddr; i != endAddr; i += 4)
	{
		//temp就是具体的怪物对象。
		auto temp = utils::GetInstance()->read<DWORD>(i);

		if (temp == 0)
		{
			continue;
		}
		person per(temp);
		//判断类型 = 玩家 && 阵营 = 敌人 && 对象存在
		if (per.GetCamp() != CAM_NEUTRAL && per.GetType() == 0x1401)
		{
			m_PersonList.push_back(person(temp));
		}

	}
	//utils::GetInstance()->log("TIPS: 当前怪物个数为：%d\n", m_PersonList.size());
	//for (auto temp : m_PersonList)
	//{
	//	utils::GetInstance()->log("TIPS: %x %s %x %x ", temp.GetNodeBase(), temp.GetName(),temp.GetCamp());
	//}
}

person CMonsterServices::GetNearleastPerson(person* role)
{
	//根据 玩家坐标 和 m_PersonList 进行比较，取最近的对象
	travse();
	float MaxDistance = FLT_MAX;
	DWORD minDistanceObj = 0;
	for (auto temp : m_PersonList)
	{
		if (temp.GetDistance(&role->GetPoint()) < MaxDistance &&role->GetCamp() != temp.GetCamp()&& !temp.BDead())
		{
			MaxDistance = temp.GetDistance(&role->GetPoint());
			minDistanceObj = temp.GetNodeBase();
		}
	}
	return person(minDistanceObj);
}

person CMonsterServices::GetHealthLeastPerson(person* role,float SkillRange)
{
	travse();
	float MaxHealth = FLT_MAX;
	DWORD minDistanceObj = 0;
	for (auto temp : m_PersonList)
	{
		if (temp.GetCurHp() < MaxHealth &&role->GetCamp() != temp.GetCamp()&& !temp.BDead() && temp.GetDistance(&role->GetPoint()) < SkillRange)
		{
			MaxHealth = temp.GetCurHp();
			minDistanceObj = temp.GetNodeBase();
		}
	}
	return person(minDistanceObj);
}
