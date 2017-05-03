#include "stdafx.h"
#include "MonsterServices.h"
#include "BaseAddr.h"

CMonsterServices::CMonsterServices()
{
	auto temp = utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr);
	m_role = std::shared_ptr<person>(new person(temp));
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
		if (per.GetCamp() != m_role->GetCamp() && per.GetCamp() != CAM_NEUTRAL && per.GetType() == 0x1401)
		{
			m_PersonList.push_back(person(temp));
		}

	}
	utils::GetInstance()->log("TIPS: 当前怪物个数为：%d\n", m_PersonList.size());

}

void* CMonsterServices::GetNearleastPerson()
{
	//根据 玩家坐标 和 m_PersonList 进行比较，取最近的对象
	travse();
	float aaa = 100000.0;
	void* p = nullptr;
	for (auto temp : m_PersonList)
	{
		if (temp.GetDistance(dynamic_cast<MonsterBase*>(m_role.get())) < aaa)
		{
			aaa = temp.GetDistance(dynamic_cast<MonsterBase*>(m_role.get()));
			utils::GetInstance()->log("TIPS: DIS = %f", aaa);
			p  = (void*)(temp.GetName());
		}
	}
	return p;
}
