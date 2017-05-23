#pragma once
#include <vector>
#include "person.h"
#include <memory>
class CMonsterServices
{
public:
	CMonsterServices();
	~CMonsterServices();

	void travse(person& role);
	//获取最近的怪物
	person GetNearleastPerson(person& role);
	//获取血量最低的怪物
	person GetHealthLeastPerson(person& role, float Distance);

	//获取当前范围内最近的小兵
	MonsterBase GetHealthLeastMons(person& role, float Dis);
	//获取玩家列表
	std::vector<person> GetPersonList(person& role , float Distance);

	//获取小兵列表
	std::vector<MonsterBase> GetMonsList(person& role ,float Distance);
private:
	//玩家列表
	std::vector<person> m_PersonList;
	//小兵野怪列表
	std::vector<MonsterBase> m_monsList;
};

