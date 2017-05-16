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
#include <ctime>
SHARED_MEMORY* pSharedMemoryPointer = nullptr;
//对象声明
CMonsterServices g_cm;
CHookToMainThread g_hk;
std::shared_ptr<ShareMemory<SHARED_MEMORY>> m_pSharedMemory;

void UseSkillByindex(skill& sk, person& mons, person& ps)
{
	if (mons.GetNodeBase()
		&&sk.GetSkillRange() > mons.GetDistance(&ps.GetPoint()) //技能范围 > 玩家距离
		&& sk.GetLevel() //技能等级 > 0
		&& sk.bCoolDown() //技能已经冷却
		&& ps.GetCurMp() > sk.GetExpendMP() //玩家当前MP > 技能消耗的MP
		&& !mons.BDead()  //怪物活着
		&& !ps.BDead()    //玩家活着
		&& mons.BVisableSee() //怪物可见
		)
	{
		utils::GetInstance()->log("TIPS: 开始使用技能 %x 攻击 %x", sk.GetIndex(), mons.GetNodeBase());
		SKILL_TO_MONS temp;
		temp.index = (EM_SKILL_INDEX)sk.GetIndex();
		temp.monsObj = mons.GetNodeBase();
		g_hk.SendMessageToGame(MESSAGE::MSG_SKILLCALL, (LPARAM)(&temp));
		Sleep(3);
	}
}

void UseAttackAA2Mons(person & mons, person& ps)
{
	auto dwZouAms = (DWORD)(((float)(1.0) / ps.GetAttackSpeed())*220.0);
	m_pSharedMemory->GetPointerOfFile()->dwZouAMs = dwZouAms/* > 360 ? 360 : dwZouAms*/;
	static DWORD m_AttackDisTime = 0;
	static DWORD timeSec = 0;
	if (mons.GetNodeBase()
		&&!ps.BDead() 
		&&!mons.BDead() 
		&& ps.GetAttackRange() > ps.GetDistance(&mons.GetPoint()) 
		&&(float)(GetTickCount() - timeSec) >= ((float)(1.05) / ps.GetAttackSpeed())*1000.0
		&& mons.BVisableSee())
	{
		utils::GetInstance()->log("TIPS: 开始普攻逻辑！\n");
		SKILL_TO_MONS temp;
		temp.monsObj = mons.GetNodeBase();
		g_hk.SendMessageToGame(MESSAGE::MSG_ATTACKCALL, (LPARAM)(&temp));
		timeSec = GetTickCount();
		m_AttackDisTime = GetTickCount();
	}
	else
	{
		utils::GetInstance()->log("TIPS: 调用寻路逻辑！\n");
		if ((GetTickCount() - m_AttackDisTime) >= m_pSharedMemory->GetPointerOfFile()->dwZouAMs)
		{
			utils::GetInstance()->log("TIPS: 开始寻路逻辑！\n");
			g_hk.SendMessageToGame(MESSAGE::MSG_FINDWAY, NULL);
		}
	}
}

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
	VMProtectBegin("ThreadProc");
	//创建共享内存
	m_pSharedMemory =  std::shared_ptr<ShareMemory<SHARED_MEMORY>>(new ShareMemory<SHARED_MEMORY>(MAP_NAME));
	if (!m_pSharedMemory->openShareMemory())
	{
		utils::GetInstance()->log("ERROR: m_pSharedMemory->openShareMemory()出现错误！\n");
		return 0;
	}
	pSharedMemoryPointer = m_pSharedMemory->GetPointerOfFile();
	utils::GetInstance()->log("TIPS: Base_GameStartTime = %x", pSharedMemoryPointer->Base_GameStartTime);


	//判断是否进入游戏
	while ((DWORD)GameCall::GetInstance()->GetClientTickTime() < 1 || utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_RoleSelfAddr) < 1)
	{
		utils::GetInstance()->log("TIPS: 等待中， %f %x", GameCall::GetInstance()->GetClientTickTime(), pSharedMemoryPointer->Base_RoleSelfAddr);
		Sleep(3000);
	}

	//挂在到主线程
	if (!g_hk.Hook())
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

	VMProtectEnd();
	//定义玩家对象
	CSkillServices m_roleSkill(utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_RoleSelfAddr));
	person m_role(utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_RoleSelfAddr));

	utils::GetInstance()->log("TIPS: 开启成功！\n");
	while (true)
	{
		

		if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyQ) & 0x8000 && pSharedMemoryPointer->bLockQ)
		{
			auto skillQ = m_roleSkill.GetSkillObjectByIndex(0);
			auto mons = g_cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
			UseSkillByindex(skillQ, mons, m_role);
		}

		if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyW) & 0x8000 && pSharedMemoryPointer->bLockW)
		{
			auto skillQ = m_roleSkill.GetSkillObjectByIndex(1);
			auto mons = g_cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
			UseSkillByindex(skillQ, mons, m_role);
		}

		if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyE) & 0x8000 && pSharedMemoryPointer->bLockE)
		{

			auto skillQ = m_roleSkill.GetSkillObjectByIndex(2);
			auto mons = g_cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
			UseSkillByindex(skillQ, mons, m_role);
		}

		if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyR) & 0x8000 && pSharedMemoryPointer->bLockR)
		{
			auto skillQ = m_roleSkill.GetSkillObjectByIndex(3);
			auto mons = g_cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
			UseSkillByindex(skillQ, mons, m_role);
		}

		if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyAA) & 0x8000 && pSharedMemoryPointer->bOpenAA)
		{
			auto mons = g_cm.GetHealthLeastPerson(&m_role, m_role.GetAttackRange());
			UseAttackAA2Mons(mons, m_role);
		}

		Sleep(3);
	}

	return 0;
}

