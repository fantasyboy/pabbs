#include "stdafx.h"
#include "MonsterBase.h"


MonsterBase::MonsterBase(DWORD dwNodeBase): base(dwNodeBase)
{

}


MonsterBase::~MonsterBase()
{
}

char* MonsterBase::GetName() const
{
	__try
	{
		auto stringSize = utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x30);
		auto maxSize = utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x34);
		if (stringSize < 0x10 && maxSize == 0xf)
		{
			return (char*)(GetNodeBase() + 0x20);
		}
		else
		{
			return (char*)(utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x20));
		}
	}
	__except (1) {
		return nullptr;
	}
}

float MonsterBase::GetCurHp() const
{
	return 0;
}

float MonsterBase::GetMaxHp() const
{
	return 0;
}

float MonsterBase::GetCurMp() const
{
	return 0;
}

float MonsterBase::GetMaxMp() const
{
	return 0;

}

EM_POINT_3D MonsterBase::GetPoint() const
{
	EM_POINT_3D temp = { 0 };
	__try
	{
		//读取坐标
		temp.x = utils::GetInstance()->read<float>(GetNodeBase() + 0x50);
		temp.z = utils::GetInstance()->read<float>(GetNodeBase() + 0x54);
		temp.y = utils::GetInstance()->read<float>(GetNodeBase() + 0x58);
	}
	__except (1)
	{
		utils::GetInstance()->log("ERROR: MonsterBase::GetPoint() 出现异常！\n");
	}
	return temp;
}

EM_CAMP MonsterBase::GetCamp() const
{
	__try {
		return (EM_CAMP)utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x14);
	}
	__except (1) {
		utils::GetInstance()->log("ERROR: MonsterBase::GetCamp()出现异常！\n");
		return EM_CAMP::CAM_UNKNOW;
	}
}

DWORD MonsterBase::GetType() const
{
	__try {
		return utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x18);
	}
	__except (1) {
		utils::GetInstance()->log("ERROR: MonsterBase::GetType()出现异常！\n");
		return 0;
	}
}

bool MonsterBase::BInShowInFag() const
{
	return false;
}

bool MonsterBase::BDead() const
{
	return false;
}

float MonsterBase::GetDistance(MonsterBase* mon)
{
	return sqrt((GetPoint().x - mon->GetPoint().x)*(GetPoint().x - mon->GetPoint().x) + (GetPoint().y - mon->GetPoint().y)*((GetPoint().y - mon->GetPoint().y)));
}
