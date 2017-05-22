#include "stdafx.h"
#include "MonsterServices.h"
#include "BaseAddr.h"
#include "BufferServices.h"

CMonsterServices::CMonsterServices()
{
}


CMonsterServices::~CMonsterServices()
{
}

void CMonsterServices::travse(person& role)
{
	m_PersonList.clear();
	m_monsList.clear();
	auto startAddr = utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_MonsterArrayAddr);
	auto endAddr = utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_MonsterArrayAddr + 0x4);
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
		if (role.GetCamp() != per.GetCamp() &&  per.GetCamp() != CAM_NEUTRAL && per.GetType() == 0x1401)
		{
			m_PersonList.push_back(per);
		}

		//是小兵或者野怪 并且不是自己方的
		if (role.GetCamp() != per.GetCamp() && per.GetType() == 0xc01 && role.GetDistance(&per.GetPoint()) < 2000)
		{
			m_monsList.push_back(per);
		}
	}

}

person CMonsterServices::GetNearleastPerson(person& role)
{
	//根据 玩家坐标 和 m_PersonList 进行比较，取最近的对象
	float MaxDistance = FLT_MAX;
	DWORD minDistanceObj = 0;
	for (auto temp : m_PersonList)
	{
		if (temp.GetDistance(&role.GetPoint()) < MaxDistance &&role.GetCamp() != temp.GetCamp()&& !temp.BDead())
		{
			MaxDistance = temp.GetDistance(&role.GetPoint());
			minDistanceObj = temp.GetNodeBase();
		}
	}
	return person(minDistanceObj);
}

person CMonsterServices::GetHealthLeastPerson(person& role, float Distance)
{
	float MaxHealth = FLT_MAX;
	DWORD minDistanceObj = 0;
	for (auto temp : m_PersonList)
	{
		if (temp.GetCurHp() < MaxHealth 
			&&role.GetCamp() != temp.GetCamp()
			&&!temp.BDead() 
			&& temp.GetDistance(&role.GetPoint()) < Distance
			&& temp.BVisableSee())
		{
			MaxHealth = temp.GetCurHp();
			minDistanceObj = temp.GetNodeBase();
		}
	}
		
	return person(minDistanceObj);
}

MonsterBase CMonsterServices::GetHealthLeastMons(person& role, float Dis)
{
	float MaxHealth = FLT_MAX;
	DWORD minDistanceObj = 0;
	for (auto temp : m_monsList)
	{
		if (temp.GetCurHp() < MaxHealth
			&&role.GetCamp() != temp.GetCamp()
			&& !temp.BDead()
			&& temp.GetDistance(&role.GetPoint()) < Dis
			&& temp.BVisableSee())
		{
			MaxHealth = temp.GetCurHp();
			minDistanceObj = temp.GetNodeBase();
		}
	}

	return MonsterBase(minDistanceObj);

}

std::vector<person>& CMonsterServices::GetPersonList()
{
	return m_PersonList;
}

std::vector<MonsterBase>& CMonsterServices::GetMonsList()
{
	return m_monsList;
}
