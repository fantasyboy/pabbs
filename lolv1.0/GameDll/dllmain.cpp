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
CMonsterServices cm;
void UseSkillByindex(skill& sk, MonsterBase& mons, person& ps)
{
	static bool BUseSkill = false;
	if (/*mons.GetNodeBase()&&*/ 
		sk.GetSkillRange() > mons.GetDistance(&ps.GetPoint()) //技能范围 > 玩家距离
		&& sk.GetLevel() //技能等级 > 0
		&& sk.bCoolDown() //技能已经冷却
		&& ps.GetCurMp() > sk.GetExpendMP() //玩家当前MP > 技能消耗的MP
		&& !mons.BDead()  //怪物活着
		&& !ps.BDead()    //玩家活着
		&& mons.BVisableSee() //怪物可见
		&& !BUseSkill
		)
	{
		utils::GetInstance()->log("TIPS: 开始使用技能 %x 攻击 %x", sk.GetIndex(), mons.GetNodeBase());
		SKILL_TO_MONS temp;
		temp.index = (EM_SKILL_INDEX)sk.GetIndex();
		temp.monsObj = mons.GetNodeBase();
		g_hk.SendMessageToGame(MESSAGE::MSG_SKILLCALL, (LPARAM)(&temp));
		BUseSkill = true;
	}
	else
	{
		BUseSkill = false;
	}
}

void UseAttackAA2Mons(MonsterBase & mons, person& ps)
{
	auto dwZouAms = (DWORD)(((float)(1.5) / ps.GetAttackSpeed() )* ((float)(1.5) / ps.GetAttackSpeed()) * (float)1.5 *pSharedMemoryPointer->dwZouAMs);
	static DWORD timeSec = 0;
	static bool bAA = false;
	if (mons.GetNodeBase()
		&&!ps.BDead() 
		&&!mons.BDead() 
		&& ps.GetAttackRange() > ps.GetDistance(&mons.GetPoint()) 
		&&(float)(GetTickCount() - timeSec) >= ((float)(1.0) / ps.GetAttackSpeed())*1000.0
		&& mons.BVisableSee()
		&& !bAA)
	{
		utils::GetInstance()->log("TIPS: 开始普攻逻辑！\n");
		SKILL_TO_MONS temp;
		temp.monsObj = mons.GetNodeBase();
		g_hk.SendMessageToGame(MESSAGE::MSG_ATTACKCALL, (LPARAM)(&temp));
		timeSec = GetTickCount();
		Sleep(dwZouAms);
		bAA = true;
	}
	else
	{
		g_hk.SendMessageToGame(MESSAGE::MSG_FINDWAY, NULL);
		bAA = false;
	}
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

	auto handleTravse =  ::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(ThreadProcTravse), NULL, NULL, NULL);
	auto handleLockSkill = ::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(ThreadProcLockSkill), NULL, NULL, NULL);
	auto handleLockAA = ::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(ThreadProcAA), NULL, NULL, NULL);
	auto handleEAA = ::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(ThreadProcAutoEAA), NULL, NULL, NULL);
	WaitForSingleObject(handleTravse, INFINITE);
	WaitForSingleObject(handleLockSkill, INFINITE);
	WaitForSingleObject(handleLockAA, INFINITE);
	WaitForSingleObject(handleEAA, INFINITE);
	return 0;
}

DWORD WINAPI ThreadProcLockSkill(_In_ LPVOID lpParameter)
{
	person m_role(utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_RoleSelfAddr));
	CSkillServices m_roleSkill(m_role.GetNodeBase());
	while (true)
	{
		//锁定Q

		if (pSharedMemoryPointer->bLockQ )
		{
			auto skillQ = m_roleSkill.GetSkillObjectByIndex(0);
			if (pSharedMemoryPointer->VirtualKeyQ == 'Q')
			{
				g_monsArry[0].skillObj = skillQ.GetNodeBase();
				g_monsArry[0].monsObj = cm.GetHealthLeastPerson(m_role, skillQ.GetSkillRange()).GetNodeBase();
				g_monsArry[0].roleObj = m_role.GetNodeBase();
			}
			else
			{
				if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyQ) & 0x8000) {
					auto mons = cm.GetHealthLeastPerson(m_role, skillQ.GetSkillRange());
					UseSkillByindex(skillQ, mons, m_role);
				}
			}
		}
		//锁定W
		if (pSharedMemoryPointer->bLockW )
		{
			auto skillQ = m_roleSkill.GetSkillObjectByIndex(1);
			if (pSharedMemoryPointer->VirtualKeyW == 'W')
			{
				g_monsArry[1].skillObj = skillQ.GetNodeBase();
				g_monsArry[1].monsObj = cm.GetHealthLeastPerson(m_role, skillQ.GetSkillRange()).GetNodeBase();
				g_monsArry[1].roleObj = m_role.GetNodeBase();
			}
			else
			{
				if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyW) & 0x8000) {
					auto mons = cm.GetHealthLeastPerson(m_role, skillQ.GetSkillRange());
					UseSkillByindex(skillQ, mons, m_role);
				}
			}
		}
		//锁定E
		if (pSharedMemoryPointer->bLockE )
		{
			auto skillQ = m_roleSkill.GetSkillObjectByIndex(2);
			if (pSharedMemoryPointer->VirtualKeyE == 'E')
			{
				g_monsArry[2].skillObj = skillQ.GetNodeBase();
				g_monsArry[2].monsObj = cm.GetHealthLeastPerson(m_role, skillQ.GetSkillRange()).GetNodeBase();
				g_monsArry[2].roleObj = m_role.GetNodeBase();
			}
			else
			{
				if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyE) & 0x8000) {
					auto mons = cm.GetHealthLeastPerson(m_role, skillQ.GetSkillRange());
					UseSkillByindex(skillQ, mons, m_role);
				}
			}
		}
		//锁定R
		if (pSharedMemoryPointer->bLockR  )
		{
			auto skillQ = m_roleSkill.GetSkillObjectByIndex(3);
			if (pSharedMemoryPointer->VirtualKeyR == 'R')
			{
				g_monsArry[3].skillObj = skillQ.GetNodeBase();
				g_monsArry[3].monsObj = cm.GetHealthLeastPerson(m_role, skillQ.GetSkillRange()).GetNodeBase();
				g_monsArry[3].roleObj = m_role.GetNodeBase();
			}
			else
			{
				if (GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyR) & 0x8000)
				{
					auto mons = cm.GetHealthLeastPerson(m_role, m_role.GetAttackRange());
					UseSkillByindex(skillQ, mons, m_role);
				}
			}
		}

		Sleep(15);
	}
}

DWORD WINAPI ThreadProcAA(_In_ LPVOID lpParameter)
{
	person m_role(utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_RoleSelfAddr));
	while (true)
	{
		//走A
		if (pSharedMemoryPointer->bOpenAA && GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyAA) & 0x8000)
		{
			auto mons = cm.GetHealthLeastPerson(m_role, m_role.GetAttackRange());
			UseAttackAA2Mons(mons, m_role);
		}
		//清线
		if (pSharedMemoryPointer->bOpenClearAA && GetAsyncKeyState(pSharedMemoryPointer->VirtualKeyOpenClear) & 0x8000)
		{
			//获取范围内最近的小兵
			auto mons = cm.GetHealthLeastMons(m_role, m_role.GetAttackRange());
			UseAttackAA2Mons(mons, m_role);

		}

		Sleep(15);
	}
}

DWORD WINAPI ThreadProcTravse(_In_ LPVOID lpParameter)
{
	person m_role(utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_RoleSelfAddr));
	while (true)
	{
		cm.travse(m_role);
		Sleep(15);
	}
}

DWORD WINAPI ThreadProcAutoEAA(_In_ LPVOID lpParameter)
{
	person m_role(utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_RoleSelfAddr));
	CSkillServices m_roleSkill(m_role.GetNodeBase());
	CBufferServices m_roleBuff(m_role.GetNodeBase());
	while (true)
	{
		if (pSharedMemoryPointer->bAutoHuabanE)
		{
			//遍历周围玩家和怪物
			if (pSharedMemoryPointer->bAutoEToHero) {
				auto perList = cm.GetPersonList(m_role, m_roleSkill.GetSkillObjectByIndex(2).GetSkillRange());
				for (auto temp : perList)
				{
					utils::GetInstance()->log("TIPS: PLAYER OBJ = %x", temp.GetNodeBase());
					//遍历玩家身上的buff
					if (temp.BDead())
					{
						continue;
					}
					CBufferServices cbf(temp.GetNodeBase());
					cbf.travse();
					for (auto buf : cbf.GetBuffList())
					{
						if (buf.GetName() == nullptr)
						{
							continue;
						}
						if (strstr(buf.GetName(), "marker") != NULL)
						{
							if (buf.GetBufferCount() > 0) {

								if (((buf.GetBufferCount() - 2)*(m_roleSkill.GetSkillObjectByIndex(2).GetSkillGetAggressivity() + 0.2 * m_role.GetAggressivity())) + ((m_roleSkill.GetSkillObjectByIndex(2).GetSkillGetAggressivity() + m_roleSkill.GetSkillObjectByIndex(2).GetSkillPlusProportion() * m_role.GetAggressivity())) > (temp.GetCurHp() + 10.0))
								{
									UseSkillByindex(m_roleSkill.GetSkillObjectByIndex(2), temp, m_role);
								}
							}
							else
							{

								if (((m_roleSkill.GetSkillObjectByIndex(2).GetSkillGetAggressivity() + m_roleSkill.GetSkillObjectByIndex(2).GetSkillPlusProportion() * m_role.GetAggressivity())) > (temp.GetCurHp() + 10.0))
								{
									UseSkillByindex(m_roleSkill.GetSkillObjectByIndex(2), temp, m_role);
								}
							}
						}
					}
					utils::GetInstance()->log("TIPS: PLAYER OBJ = %x endl", temp.GetNodeBase());
				}

			}

			if (pSharedMemoryPointer->bAutoEToMons) {
				auto monsList = cm.GetMonsList(m_role, m_roleSkill.GetSkillObjectByIndex(2).GetSkillRange());
				for (auto temp : monsList)
				{
					utils::GetInstance()->log("TIPS: MONS OBJ = %x", temp.GetNodeBase());
					//遍历玩家身上的buff
					if (temp.BDead())
					{
						continue;
					}
					CBufferServices cbf(temp.GetNodeBase());
					cbf.travse();
					for (auto buf : cbf.GetBuffList())
					{
						if (buf.GetName() == nullptr)
						{
							continue;
						}
						if (strstr(buf.GetName(), "marker") != NULL)
						{
							if (buf.GetBufferCount() > 0) {
								utils::GetInstance()->log("TIPS: 技能攻击力 %f %f %f  HP = %f", m_roleSkill.GetSkillObjectByIndex(2).GetSkillGetAggressivity(), m_roleSkill.GetSkillObjectByIndex(2).GetSkillPlusProportion(), m_role.GetAggressivity(), temp.GetCurHp());
								if (((buf.GetBufferCount() - 2)*(m_roleSkill.GetSkillObjectByIndex(2).GetSkillGetAggressivity() + 0.2 * m_role.GetAggressivity())) + ((m_roleSkill.GetSkillObjectByIndex(2).GetSkillGetAggressivity() + m_roleSkill.GetSkillObjectByIndex(2).GetSkillPlusProportion() * m_role.GetAggressivity())) > (temp.GetCurHp() + 10.0))
								{
									UseSkillByindex(m_roleSkill.GetSkillObjectByIndex(2), temp, m_role);
								}
							}
							else
							{
								if (((m_roleSkill.GetSkillObjectByIndex(2).GetSkillGetAggressivity() + m_roleSkill.GetSkillObjectByIndex(2).GetSkillPlusProportion() * m_role.GetAggressivity())) > (temp.GetCurHp() + 10.0))
								{
									UseSkillByindex(m_roleSkill.GetSkillObjectByIndex(2), temp, m_role);
								}
							}
						}
					}

					//如果buff名字 == 滑板鞋的E  && （技能基础伤害 + 玩家攻击力* 技能加成比例） > 当前怪物的血量  使用 E
					utils::GetInstance()->log("TIPS: MONS OBJ = %x endl", temp.GetNodeBase());
				}

			}
		}
		Sleep(15);
	}
}

