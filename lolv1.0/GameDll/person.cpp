#include "stdafx.h"
#include "person.h"
#include "BaseAddr.h"

person::person(DWORD dwNodeBase):MonsterBase(dwNodeBase)
{
}


person::~person()
{
}

float person::GetAttackRange() const
{
	__try {
		return utils::GetInstance()->read<float>(GetNodeBase() + Base_AttackDistanceOffset);
	}
	__except (1) {
		utils::GetInstance()->log("ERROR: person::GetAttackRange() 出现异常！\n");
		return 0;
	}
}

float person::GetAttackSpeed() const
{
	__try{
		return utils::GetInstance()->read<float>(GetNodeBase() + Base_AttackSpeedOffset);
	}
	__except (1)
	{
		utils::GetInstance()->log("ERROR: person::GetAttackSpeed()　出现异常！\n");
		return 0;
	}
}

float person::GetGold() const
{
	return 0;
}
