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
			return nullptr;
		}
		nodeBase = utils::GetInstance()->read<DWORD>(nodeBase + 0xc);
		if (nodeBase == 0)
		{
			return nullptr;
		}
		return (char*)(utils::GetInstance()->read<DWORD>(nodeBase + 0x0c));
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
