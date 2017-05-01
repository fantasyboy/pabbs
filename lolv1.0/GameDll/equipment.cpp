#include "stdafx.h"
#include "equipment.h"

equipment::equipment(DWORD dwNodeBase):base(dwNodeBase)
{

}

equipment::~equipment()
{
}

char* equipment::GetName() const
{
	__try {
		DWORD nodeBase = utils::GetInstance()->read<DWORD>(GetNodeBase());
		if (nodeBase == 0)
		{
			utils::GetInstance()->log("ERROR: equipment::GetName() nodeBase read failed!\n");
			return nullptr;
		}
		nodeBase = utils::GetInstance()->read<DWORD>(nodeBase + 0xc);
		if (nodeBase == 0)
		{
			utils::GetInstance()->log("ERROR: equipment::GetName() nodeBase-1 read failed!\n");
			return nullptr;
		}
		return (char*)(nodeBase + 0x8);
	}
	__except (1) {
		return nullptr;
	}
}

DWORD equipment::GetCount() const
{
	__try {
		return utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x4);
	}
	__except (1) {
		return 0;
	}
}

DWORD equipment::GetEquBuffCount() const
{
	__try {
		return utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x8);
	}
	__except (1) {
		return 0;
	}
}
