#pragma once
#include <vector>
#include "person.h"
#include <memory>
class CMonsterServices
{
public:
	CMonsterServices();
	~CMonsterServices();

	void travse();
	//获取最近的怪物
	person GetNearleastPerson(person* role);
	//获取血量最低的怪物
	person GetHealthLeastPerson(person* role, float SkillRange);
private:
	//玩家列表
	std::vector<person> m_PersonList;
};

