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
#include "HookToMainThread.h"

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
	//对象声明
	CMonsterServices cm;
	CHookToMainThread hk;


	//判断是否进入游戏
	while ((DWORD)GameCall::GetInstance()->GetClientTickTime() < 1 || utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr) < 1)
	{
		Sleep(3000);
	}

	if (!hk.Hook())
	{
		utils::GetInstance()->log("ERROR: 挂载到游戏主线程失败哦！\n");
		return false;
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
	CSkillServices m_roleSkill(utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr));

	utils::GetInstance()->log("TIPS: 开启成功！\n");
	while (true)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			person m_role(utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr));
			//如果锁定了技能Q
			if (pSharedMemoryPointer->bLockQ)
			{
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(0);
				auto mons = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
				//如果 (最近玩家的距离 < 技能Q的距离 && 玩家当前的蓝 > 技能消耗的蓝 && 技能已经学习 && 技能已经冷却 && 玩家活着)  就调用 （技能CALL（Q））；
				if (m_role.GetDistance(&mons.GetPoint()) < skillQ.GetSkillRange() &&
					m_role.GetDistance(&mons.GetPoint()) >0 &&
					skillQ.GetLevel() > 0 && 
					m_role.GetCurMp() > skillQ.GetExpendMP()&&
					!m_role.BDead() &&
					skillQ.bCoolDown()&&
					mons.GetNodeBase()&&
					!mons.BDead())
				{
					SKILL_TO_MONS temp;
					temp.index = EM_SKILL_INDEX::Q;
					temp.monsObj = mons.GetNodeBase();
					hk.SendMessageToGame(MESSAGE::MSG_SKILLCALL, (LPARAM)(&temp));
				}
			}
			//如果锁定了技能W
			if (pSharedMemoryPointer->bLockW)
			{
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(1);
				auto mons = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
				if (m_role.GetDistance(&mons.GetPoint()) < skillQ.GetSkillRange() &&
					skillQ.GetLevel() > 0 &&
					m_role.GetDistance(&mons.GetPoint()) >0 &&
					m_role.GetCurMp() > skillQ.GetExpendMP() &&
					!m_role.BDead() &&
					skillQ.bCoolDown()&&
					mons.GetNodeBase()&&
					!mons.BDead())
				{
					SKILL_TO_MONS temp;
					temp.index = EM_SKILL_INDEX::W;
					temp.monsObj = mons.GetNodeBase();
					hk.SendMessageToGame(MESSAGE::MSG_SKILLCALL, (LPARAM)(&temp));
				}
			}
			if (pSharedMemoryPointer->bLockE)
			{
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(2);
				auto mons = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
				if (m_role.GetDistance(&mons.GetPoint()) < skillQ.GetSkillRange() &&
					skillQ.GetLevel() > 0 &&
					m_role.GetDistance(&mons.GetPoint()) >0 &&
					m_role.GetCurMp() > skillQ.GetExpendMP() &&
					!m_role.BDead() &&
					skillQ.bCoolDown()&&
					mons.GetNodeBase()&&
					!mons.BDead())
				{
					SKILL_TO_MONS temp;
					temp.index = EM_SKILL_INDEX::E;
					temp.monsObj = mons.GetNodeBase();
					hk.SendMessageToGame(MESSAGE::MSG_SKILLCALL, (LPARAM)(&temp));
				}
			}
			
			if (pSharedMemoryPointer->bOpenAA)
			{
				//获取在玩家攻击范围内的血量最低的怪物
				auto mons = cm.GetHealthLeastPerson(&m_role, m_role.GetAttackRange());
				//攻击间隔
				static float timeSec = 0;
				//如果攻击间隔 > 攻击需要的秒数 && 怪物存在  && 玩家活着
				if (mons.GetNodeBase() && 
					!m_role.BDead()&&
					!mons.BDead()&&
					m_role.GetDistance(&mons.GetPoint()) < m_role.GetAttackRange())
				{
					SKILL_TO_MONS temp;
					temp.monsObj = mons.GetNodeBase();
					if ((GameCall::GetInstance()->GetClientTickTime() - timeSec) > ((float)(1.1) / m_role.GetAttackSpeed()))
					{
						utils::GetInstance()->log("TIPS:当前时间差为: %f", (GameCall::GetInstance()->GetClientTickTime() - timeSec));
						hk.SendMessageToGame(MESSAGE::MSG_ATTACKCALL, (LPARAM)(&temp));
						//重新计算攻击间隔
						timeSec = GameCall::GetInstance()->GetClientTickTime(); //100
					}

				}
			}

		}

		//如果按下了T & 就自动释放R
		if (GetAsyncKeyState(0x54)&0x8000)
		{
			person m_role(utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr));
			if (pSharedMemoryPointer->bLockR)
			{
				//auto mons = cm.GetNearleastPerson(&m_role);
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(3);
				auto mons = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
				if (m_role.GetDistance(&mons.GetPoint()) < skillQ.GetSkillRange() &&
					m_role.GetDistance(&mons.GetPoint()) >0 &&
					skillQ.GetLevel() > 0 &&
					m_role.GetCurMp() > skillQ.GetExpendMP() &&
					!m_role.BDead() &&
					skillQ.bCoolDown()&&
					mons.GetNodeBase()&&
					!mons.BDead())
				{
					SKILL_TO_MONS temp;
					temp.index = EM_SKILL_INDEX::R;
					temp.monsObj = mons.GetNodeBase();
					hk.SendMessageToGame(MESSAGE::MSG_SKILLCALL, (LPARAM)(&temp));
				}
			}
		}

		Sleep(1);
	}

	return 0;
}

