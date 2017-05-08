#pragma once
#include "MonsterBase.h"
class person :
	public MonsterBase
{
public:
	person(DWORD dwNodeBase);
	~person();

	//获取玩家的普通攻击距离
	float GetAttackRange()const;
	//获取玩家攻击速度
	float GetAttackSpeed()const;
	//获取玩家金币
	float GetGold()const;
};

