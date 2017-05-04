// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "dllmain.h"
#include "SkillServices.h"
#include "BufferServices.h"
#include "EquipmentServices.h"
#include "MonsterServices.h"
#include "BaseAddr.h"
#include "CShareStruct.h"
#include "ShareMemory.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(ThreadProc), NULL, NULL, NULL);
		break;
	}
	case DLL_THREAD_ATTACH: {
		break;
	}
	case DLL_THREAD_DETACH: {
		break;
	}
	case DLL_PROCESS_DETACH: {
		break;
	}
	}
	return TRUE;
}

DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
{
	//CSkillServices cs(utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr));
	//CBufferServices bf;
	//CEquipmentServices eq(utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr));
	//CMonsterServices cm;

	//创建共享内存
	std::shared_ptr<ShareMemory<SHARED_MEMORY>> m_pSharedMemory(new ShareMemory<SHARED_MEMORY>(MAP_NAME));
	if (!m_pSharedMemory->openShareMemory())
	{
		utils::GetInstance()->log("ERROR: m_pSharedMemory->openShareMemory()出现错误！\n");
		return 0;
	}
	auto pSharedMemoryPointer = m_pSharedMemory->GetPointerOfFile();

	while (true)
	{
		//走A判断
		//如果 （最近玩家的距离 < 攻击距离 && 延时 > 走A延时） 就调用 寻路到（鼠标位置） 和 调用普通攻击CALL
		//技能判断
		//如果 (最近玩家的距离 < 技能Q的距离 && 玩家当前的蓝 > 技能消耗的蓝 && 技能已经学习 && 技能已经冷却) 就调用 （技能CALL（Q））；
		//其他技能同理

		utils::GetInstance()->log("TIPS: q = %d w = %d e = %d r = %d",
			pSharedMemoryPointer->bLockQ,
			pSharedMemoryPointer->bLockW,
			pSharedMemoryPointer->bLockE, 
			pSharedMemoryPointer->bLockR);

		//判断按键是否被按下
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			
		}

		Sleep(3);
	}

	return 0;
}

