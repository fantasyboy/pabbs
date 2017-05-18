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
EM_SKILL_TO_MONS g_monsArry[5] = { 0 };
CHookToMainThread g_hk;
DWORD g_code = 0;
void UseSkillByindex(skill& sk, person& mons, person& ps)
{
	if (sk.GetSkillRange() > mons.GetDistance(&ps.GetPoint()) //技能范围 > 玩家距离
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
		Sleep(1);
	}
}

void UseAttackAA2Mons(person & mons, person& ps)
{
	auto dwZouAms = (DWORD)(((float)(1.0) / ps.GetAttackSpeed())*pSharedMemoryPointer->dwZouAMs);
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
		if ((GetTickCount() - m_AttackDisTime) >= dwZouAms)
		{
			utils::GetInstance()->log("TIPS: 开始寻路逻辑！\n");
			g_hk.SendMessageToGame(MESSAGE::MSG_FINDWAY, NULL);
		}
	}
	Sleep(1);
}

bool UseSkill(DWORD code)
{
	if ('Q' == code && pSharedMemoryPointer->bLockQ && pSharedMemoryPointer->VirtualKeyQ == code)
	{
		UseSkillByindex(skill(EM_SKILL_INDEX::Q, g_monsArry[0].skillObj), person(g_monsArry[0].monsObj), person(g_monsArry[0].roleObj));
		return true;
	}

	if ('W' == code && pSharedMemoryPointer->bLockW && pSharedMemoryPointer->VirtualKeyW == code)
	{
		UseSkillByindex(skill(EM_SKILL_INDEX::W, g_monsArry[1].skillObj), person(g_monsArry[1].monsObj), person(g_monsArry[1].roleObj));
		return true;
	}

	if ('E' == code && pSharedMemoryPointer->bLockE && pSharedMemoryPointer->VirtualKeyE == code)
	{
		UseSkillByindex(skill(EM_SKILL_INDEX::E, g_monsArry[2].skillObj), person(g_monsArry[2].monsObj), person(g_monsArry[2].roleObj));
		return true;
	}

	if ('R' == code && pSharedMemoryPointer->bLockR && pSharedMemoryPointer->VirtualKeyR == code)
	{
		UseSkillByindex(skill(EM_SKILL_INDEX::R, g_monsArry[3].skillObj), person(g_monsArry[3].monsObj), person(g_monsArry[3].roleObj));
		return true;
	}
	return false;
}

BOOL APIENTRY DllMain(HMODULE hModule,
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
	std::shared_ptr<ShareMemory<SHARED_MEMORY>> m_pSharedMemory(new ShareMemory<SHARED_MEMORY>(MAP_NAME));
	if (!m_pSharedMemory->openShareMemory())
	{
		return 0;
	}
	pSharedMemoryPointer = m_pSharedMemory->GetPointerOfFile();
	//判断是否进入游戏
	while ((DWORD)GameCall::GetInstance()->GetClientTickTime() < 1 || utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_RoleSelfAddr) < 1)
	{
		Sleep(3000);
	}
	
	if (!g_hk.Hook())
	{
		return false;
	}

	if (!GameCall::GetInstance()->HookSkillUse())
	{
		return 0;
	}
	VMProtectEnd();
	//定义玩家对象

	utils::GetInstance()->log("TIPS: 开启成功！\n");

	 CSkillServices m_roleSkill(utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_RoleSelfAddr));
	 person m_role(utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_RoleSelfAddr));


	CMonsterServices cm;

	while (true)
	{
		//锁定Q
		if (pSharedMemoryPointer->bLockQ)
		{
			if (pSharedMemoryPointer->VirtualKeyQ == 'Q') {
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(0);
				g_monsArry[0].skillObj = skillQ.GetNodeBase();
				g_monsArry[0].monsObj = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange()).GetNodeBase();
				g_monsArry[0].roleObj = m_role.GetNodeBase();
			}
			else
			{
				if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyQ) & 0x8000)
				{
					auto skillQ = m_roleSkill.GetSkillObjectByIndex(0);
					auto mons = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
					UseSkillByindex(skillQ, mons, m_role);
				}
			}
		}
		//锁定W
		if ( pSharedMemoryPointer->bLockW)
		{
			if (pSharedMemoryPointer->VirtualKeyW == 'W')
			{
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(1);
				g_monsArry[1].skillObj = skillQ.GetNodeBase();
				g_monsArry[1].monsObj = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange()).GetNodeBase();
				g_monsArry[1].roleObj = m_role.GetNodeBase();
			}
			else
			{
				if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyW) & 0x8000)
				{
					auto skillQ = m_roleSkill.GetSkillObjectByIndex(1);
					auto mons = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
					UseSkillByindex(skillQ, mons, m_role);
				}
			}
		}
		//锁定E
		if (pSharedMemoryPointer->bLockE)
		{
			if (pSharedMemoryPointer->VirtualKeyE == 'E') {
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(2);
				g_monsArry[2].skillObj = skillQ.GetNodeBase();
				g_monsArry[2].monsObj = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange()).GetNodeBase();
				g_monsArry[2].roleObj = m_role.GetNodeBase();
			}
			else
			{
				if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyE) & 0x8000)
				{
					auto skillQ = m_roleSkill.GetSkillObjectByIndex(2);
					auto mons = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
					UseSkillByindex(skillQ, mons, m_role);
				}
			}
		}
		//锁定R
		if (pSharedMemoryPointer->bLockR)
		{

			if (pSharedMemoryPointer->VirtualKeyR == 'R')
			{
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(3);
				g_monsArry[3].skillObj = skillQ.GetNodeBase();
				g_monsArry[3].monsObj = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange()).GetNodeBase();
				g_monsArry[3].roleObj = m_role.GetNodeBase();
			}
			else
			{

				if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyR) & 0x8000)
				{
					auto skillQ = m_roleSkill.GetSkillObjectByIndex(3);
					auto mons = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
					UseSkillByindex(skillQ, mons, m_role);
				}
			}
		}
		//走A
		if (pSharedMemoryPointer->bOpenAA && GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyAA)& 0x8000)
		{
			auto mons = cm.GetHealthLeastPerson(&m_role, m_role.GetAttackRange());
			UseAttackAA2Mons(mons, m_role);
		}



		Sleep(10);
	}

	return 0;
}

