#include "stdafx.h"
#include "buffer.h"
#include "dllmain.h"


buffer::buffer(DWORD dwNodeBase):base(dwNodeBase)
{

}

buffer::~buffer()
{
}

char* buffer::GetName() const
{
	__try
	{
		//return (char*)(utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x10));
		auto temp = utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x4);
		if (temp)
		{
			return (char*)(temp + 0xA);
		}
	}
	__except (1) {
		utils::GetInstance()->log("ERROR: buffer::GetName() ³öÏÖÒì³££¡\n");
	}
	return nullptr;
}

DWORD buffer::GetBufferCount() const
{
	__try {
		//return utils::GetInstance()->read<DWORD>(GetNodeBase() + pSharedMemoryPointer->Base_BufferCountOffset);
		auto temp1 = utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x1c);
		auto temp2 = utils::GetInstance()->read<DWORD>(GetNodeBase() + 0x18);

		auto result = (temp1 - temp2) & 0xFFFFFFF8;
		if (result < 0x10)
		{
			return utils::GetInstance()->read<DWORD>(GetNodeBase() + pSharedMemoryPointer->Base_BufferCountOffset);
		}
		else
		{
			return (temp1 - temp2) >> 3;
		}

	}
	__except (1) {
		return 0;
	}
}
