#include "stdafx.h"
#include "person.h"
#include "BaseAddr.h"

person::person(DWORD dwNodeBase):MonsterBase(dwNodeBase)
{
}


person::~person()
{
}

//dd [[[[1A7383C]+20]+4]+10] + 68
float person::GetAttackRange() const
{
	__try {
		//return utils::GetInstance()->read<float>(GetNodeBase() + Base_AttackDistanceOffset);
		auto dwBase = utils::GetInstance()->read<DWORD>(Base_BufferAddr);
		if (dwBase)
		{
			auto dwOffset1 = utils::GetInstance()->read<DWORD>(dwBase+0x20);
			if (dwOffset1)
			{
				auto dwOffset2 = utils::GetInstance()->read<DWORD>(dwOffset1 + 0x4);
				if (dwOffset2)
				{
					auto dwOffset3 = utils::GetInstance()->read<DWORD>(dwOffset2 + 0x10);
					if (dwOffset3)
					{
						return utils::GetInstance()->read<float>(dwOffset3 + 0x68);
					}
				}
			}
		}
	}
	__except (1) {
		utils::GetInstance()->log("ERROR: person::GetAttackRange() 出现异常！\n");
		return 0;
	}
	return 0;
}
//dd [[[[1A7383C]+20]+4]+10] + 18
float person::GetAttackSpeed() const
{
	__try{
		auto dwBase = utils::GetInstance()->read<DWORD>(Base_BufferAddr);
		if (dwBase)
		{
			auto dwOffset1 = utils::GetInstance()->read<DWORD>(dwBase + 0x20);
			if (dwOffset1)
			{
				auto dwOffset2 = utils::GetInstance()->read<DWORD>(dwOffset1 + 0x4);
				if (dwOffset2)
				{
					auto dwOffset3 = utils::GetInstance()->read<DWORD>(dwOffset2 + 0x10);
					if (dwOffset3)
					{
						return utils::GetInstance()->read<float>(dwOffset3 + 0x18);
					}
				}
			}
		}
	}
	__except (1)
	{
		utils::GetInstance()->log("ERROR: person::GetAttackSpeed()　出现异常！\n");
		return 0;
	}
	return 0;
}

float person::GetGold() const
{
	return 0;
}
