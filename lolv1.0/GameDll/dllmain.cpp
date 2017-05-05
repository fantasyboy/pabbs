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
#include "GameCall.h"
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
	CMonsterServices cm;

	
	//判断是否进入游戏
	while ((DWORD)GameCall::GetInstance()->GetClientTickTime() < 1 || utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr) < 1)
	{
		Sleep(3000);
	}
	//HOOK技能CALL
	if (!GameCall::GetInstance()->HookSkillUse())
	{
		utils::GetInstance()->log("ERROR: ameCall::GetInstance()->HookSkillUse()失败！");
		return 0;
	}

	//创建共享内存
	std::shared_ptr<ShareMemory<SHARED_MEMORY>> m_pSharedMemory(new ShareMemory<SHARED_MEMORY>(MAP_NAME));
	if (!m_pSharedMemory->openShareMemory())
	{
		utils::GetInstance()->log("ERROR: m_pSharedMemory->openShareMemory()出现错误！\n");
		return 0;
	}
	auto pSharedMemoryPointer = m_pSharedMemory->GetPointerOfFile();


	//定义玩家对象
	auto BasePlayerObj = utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr);
	if (BasePlayerObj < 1)
	{
		utils::GetInstance()->log("ERROR: utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr)出现错误！\n");
		return 0;
	}
	//玩家对象
	person m_role(BasePlayerObj);
	CSkillServices m_roleSkill(BasePlayerObj);

	utils::GetInstance()->log("TIPS: 开启成功！\n");
	while (true)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			
			auto mons = cm.GetNearleastPerson();
			//如果锁定了技能Q
			if (pSharedMemoryPointer->bLockQ)
			{
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(0);
				//如果 (最近玩家的距离 < 技能Q的距离 && 玩家当前的蓝 > 技能消耗的蓝 && 技能已经学习 && 技能已经冷却 && 玩家活着)  就调用 （技能CALL（Q））；
				if (m_role.GetDistance(dynamic_cast<MonsterBase*>(&mons)) < skillQ.GetSkillRange() && 
					skillQ.GetLevel() > 0 && 
					m_role.GetCurMp() > skillQ.GetExpendMP()&&
					!m_role.BDead() &&
					skillQ.bCoolDown())
				{
					utils::GetInstance()->log("TIPS: 当前敌人名字:%s 使用技能Q！", Utf8ToAnsi(mons.GetName()));
					GameCall::GetInstance()->UseSkill(0, mons.GetNodeBase());
					Sleep(10);
				}
			}
			if (pSharedMemoryPointer->bLockW)
			{
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(1);
				//如果 (最近玩家的距离 < 技能Q的距离 && 玩家当前的蓝 > 技能消耗的蓝 && 技能已经学习 && 技能已经冷却 && 玩家活着) && 敌人可见 就调用 （技能CALL（Q））；
				if (m_role.GetDistance(dynamic_cast<MonsterBase*>(&mons)) < skillQ.GetSkillRange() &&
					skillQ.GetLevel() > 0 &&
					m_role.GetCurMp() > skillQ.GetExpendMP() &&
					!m_role.BDead() &&
					skillQ.bCoolDown())
				{
					utils::GetInstance()->log("TIPS: 当前敌人名字:%s 使用技能W！", Utf8ToAnsi(mons.GetName()));
					GameCall::GetInstance()->UseSkill(1, mons.GetNodeBase());
					Sleep(10);
				}
			}
			if (pSharedMemoryPointer->bLockE)
			{
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(2);
				//如果 (最近玩家的距离 < 技能Q的距离 && 玩家当前的蓝 > 技能消耗的蓝 && 技能已经学习 && 技能已经冷却 && 玩家活着)  就调用 （技能CALL（Q））；
				if (m_role.GetDistance(dynamic_cast<MonsterBase*>(&mons)) < skillQ.GetSkillRange() &&
					skillQ.GetLevel() > 0 &&
					m_role.GetCurMp() > skillQ.GetExpendMP() &&
					!m_role.BDead() &&
					skillQ.bCoolDown())
				{
					utils::GetInstance()->log("TIPS: 当前敌人名字:%s 使用技能E！", Utf8ToAnsi(mons.GetName()));
					GameCall::GetInstance()->UseSkill(2, mons.GetNodeBase());
					Sleep(10);
				}
			}
			if (pSharedMemoryPointer->bLockR)
			{
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(3);
				//如果 (最近玩家的距离 < 技能Q的距离 && 玩家当前的蓝 > 技能消耗的蓝 && 技能已经学习 && 技能已经冷却 && 玩家活着)  就调用 （技能CALL（Q））；
				if (m_role.GetDistance(dynamic_cast<MonsterBase*>(&mons)) < skillQ.GetSkillRange() &&
					skillQ.GetLevel() > 0 &&
					m_role.GetCurMp() > skillQ.GetExpendMP() &&
					!m_role.BDead() &&
					skillQ.bCoolDown())
				{
					utils::GetInstance()->log("TIPS: 当前敌人名字:%s 使用技能R！", Utf8ToAnsi(mons.GetName()));
					GameCall::GetInstance()->UseSkill(3, mons.GetNodeBase());
					Sleep(10);
				}
			}

			//如果开启走A
			//如果 （最近玩家的距离 < 攻击距离 && 延时 > 走A延时） 就调用 寻路到（鼠标位置） 和 调用普通攻击CALL
		}

		Sleep(50);
	}

	return 0;
}

