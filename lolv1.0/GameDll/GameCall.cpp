#include "stdafx.h"
#include "GameCall.h"
#include "person.h"
#include "skill.h"
#include "SkillServices.h"
#include "MonsterServices.h"
//
//全局变量声明
//
GameCall* GameCall::m_pInstance = nullptr;
std::mutex GameCall::m_mutex;
std::mutex g_mutex;
DWORD g_MonsterObj = NULL;
DWORD g_HookCallAddr = 0;
CMonsterServices g_cm1;
GameCall::GameCall(): m_bUsedSkill(false), m_bUsedAttack(false)
{
}


GameCall::GameCall(const GameCall& _call)
{

}

GameCall GameCall::operator=(const GameCall& _call)
{
	return *this;
}

GameCall::~GameCall()
{
}

GameCall* GameCall::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_mutex.lock();
		if (nullptr == m_pInstance)
		{
			m_pInstance = new GameCall;
		}
		m_mutex.unlock();
	}
	return m_pInstance;
}

float GameCall::GetClientTickTime() const
{
	__try {
		return utils::GetInstance()->read<float>(pSharedMemoryPointer->Base_GameStartTime + 0x30);
	}
	__except(1)
	{
		utils::GetInstance()->log("ERROR: GameCall::GetClientTickTime()出现异常！\n");
		return 0;
	}
}

bool GameCall::HeroAttack(DWORD dwNodeBase)
{
	__try {
		//如果在使用技能，就不调用普攻
		static float fTargetPointArray[3] = { 0 };
		memcpy(fTargetPointArray, (void*)(dwNodeBase + 0x50), sizeof(float) * 3);
		DWORD  Base_RoleSelfAddr = pSharedMemoryPointer->Base_RoleSelfAddr;
		DWORD Base_AttackHeroCallAddr = pSharedMemoryPointer->Base_AttackHeroCallAddr;
		__asm
		{
			pushad;
			push 1;
			push 0;
			push 0;
			mov esi, dwNodeBase;
			push esi;
			lea eax, fTargetPointArray;
			push eax;
			push 3;
			mov EDX, Base_RoleSelfAddr;
			mov EDX, [EDX];
			mov ecx, edx;
			mov eax, Base_AttackHeroCallAddr;
			call eax;
			popad;
		}
	}
	__except (1)
	{
		utils::GetInstance()->log("ERROR: GameCall::HeroAttack(DWORD dwNodeBase) 出现异常！\n");
		return false;
	}
	return true;
}

bool GameCall::UseSkill(DWORD dwIndex, DWORD monsObj)
{
	__try
	{
		g_mutex.lock();
		g_MonsterObj = NULL;
		g_MonsterObj = monsObj;
		g_mutex.unlock();

		DWORD Base_SkillCallEcxAddr = pSharedMemoryPointer->Base_SkillCallEcxAddr;
		DWORD Base_SkillCallAddr = pSharedMemoryPointer->Base_SkillCallAddr;
		__asm
		{
			pushad;
			mov     ecx, Base_SkillCallEcxAddr;
			mov     ecx, dword ptr[ecx];
			mov     ecx, dword ptr[ecx + 0x30];
			push    0;
			push    2;
			push    dwIndex;
			mov     eax, Base_SkillCallAddr;
			call    eax;
			popad;
		}
	}
	__except (1)
	{
		utils::GetInstance()->log("TIPS: GameCall::UseSkill(DWORD _index) 出现异常！\n");
		return false;
	}
	return true;
}

bool GameCall::HookSkillUse()
{
	VMProtectBegin("cccc");
	DWORD HookAddr = pSharedMemoryPointer->Base_SkillCallHookAddr;
	//保存CALL地址
	g_HookCallAddr =  utils::GetInstance()->read<DWORD>(HookAddr + 0x1) + HookAddr + 5;
	char hookData[5] = { 0xe8, 0x0, 0x0, 0x0, 0x0 };
	*(DWORD*)(&hookData[1]) = (DWORD)(&SkillHookStub) - HookAddr - 0x5;
	DWORD oldProtected = 0;
	VirtualProtect((LPVOID)HookAddr, 5,PAGE_READWRITE ,&oldProtected);
	memcpy((void*)HookAddr, (void*)hookData, 5);
	VirtualProtect((LPVOID)HookAddr, 5, oldProtected, &oldProtected);
	return true;
	VMProtectEnd();
}

bool GameCall::StopAction()
{
	return UseSkill(0xa, 0);
}

bool GameCall::FindWay(EM_POINT_3D pnt)
{
	__try
	{
		static float fArray[32] = { 0 };
		fArray[0] = pnt.x;
		fArray[1] = pnt.z;
		fArray[2] = pnt.y;
		fArray[3] = pnt.x;
		fArray[4] = pnt.z;
		fArray[5] = pnt.y;

		DWORD Base_RoleSelfAddr = pSharedMemoryPointer->Base_RoleSelfAddr;
		DWORD Base_FindWayCallAddr = pSharedMemoryPointer->Base_FindWayCallAddr;
		__asm
		{
			PUSHAD;
			PUSH 1;
			PUSH 0;
			PUSH 0;
			PUSH 0;
			LEA EAX, fArray;
			PUSH EAX;
			PUSH 2;
			MOV ECX, Base_RoleSelfAddr;
			MOV ECX, DWORD PTR DS : [ECX];
			MOV EAX, Base_FindWayCallAddr;
			CALL EAX;
			POPAD;
		}
	}
	__except (1)
	{
		utils::GetInstance()->log("ERROR: 寻路出现异常！\n");
		return false;
	}
	return TRUE;
}

EM_POINT_3D GameCall::GetMousePnt() const
{
	EM_POINT_3D temp = { 0 };
	__try
	{
		auto dwBase = utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_MousePointAddr);
		if (dwBase)
		{
			auto Offset1 = utils::GetInstance()->read<DWORD>(dwBase+0x10);
			if (Offset1)
			{
				temp.x = utils::GetInstance()->read<float>(Offset1 + 0x10);
				temp.z = utils::GetInstance()->read<float>(Offset1 + 0x14);
				temp.y = utils::GetInstance()->read<float>(Offset1 + 0x18);
				return temp;
			}
		}
	}
	__except (1)
	{
		memset(&temp, 0, sizeof(EM_POINT_3D));
		return temp;
	}
	return temp;
}

bool GameCall::SetMousePnt(EM_POINT_3D pnt)
{
	__try
	{
		auto dwBase = utils::GetInstance()->read<DWORD>(pSharedMemoryPointer->Base_MousePointAddr);
		if (dwBase)
		{
			auto Offset1 = utils::GetInstance()->read<DWORD>(dwBase + 0x10);
			if (Offset1)
			{
				utils::GetInstance()->write<float>(Offset1 + 0x10, pnt.x);
				utils::GetInstance()->write<float>(Offset1 + 0x14, pnt.z);
				utils::GetInstance()->write<float>(Offset1 + 0x18, pnt.y);
				utils::GetInstance()->write<float>(Offset1 + 0x1c, pnt.x);
				utils::GetInstance()->write<float>(Offset1 + 0x20, pnt.z);
				utils::GetInstance()->write<float>(Offset1 + 0x24, pnt.y);

				return true;
			}
		}
	}
	__except (1)
	{
		return false;
	}

	return false;
}

void __stdcall SkillHookStub(DWORD skillObj, PFLOAT xyz, PDWORD monsObj)
{
	try
	{
		skill sk(EM_SKILL_INDEX::Q, skillObj);
		if (g_MonsterObj)
		{
			//拷贝对象
			g_mutex.lock();
			person temp(g_MonsterObj);
			g_MonsterObj = NULL;
			g_mutex.unlock();

			if (!temp.BDead())
			{
				utils::GetInstance()->log("TIPS: 当前怪物对象为: %x 技能对象为: %x", temp.GetNodeBase() ,skillObj);
				//玩家移动  并且不是锁定技能
				if (temp.GetBMoving()&& (DWORD)sk.GetSkillType() == 0 /*&& (DWORD)sk.GetSkillRange2() != 0*/)
				{
					auto rang = sk.GetSkillRange2();
					utils::GetInstance()->log("TIPS: 技能范围！%f\n", rang);
					if (rang >= 0.1) {
						utils::GetInstance()->log("TIPS: 调用预判逻辑！%f\n", sk.GetSkillRange2());
						EM_POINT_3D pnt = { 0 };
						pnt.x = temp.GetPoint().x + temp.GetMonsterOrientation().x * (float)(200.0);
						pnt.z = temp.GetPoint().z + temp.GetMonsterOrientation().z * (float)(200.0);
						pnt.y = temp.GetPoint().y + temp.GetMonsterOrientation().y * (float)(200.0);

						memcpy(xyz, &pnt, 0xc);
					}//预判逻辑
					else
					{
						utils::GetInstance()->log("TIPS: 调用锁定技能逻辑！ %f \n", sk.GetSkillRange2());
						*monsObj = temp.GetNodeBase();
					}
				}
				else
				{
					utils::GetInstance()->log("TIPS: 调用正常逻辑！\n");
					memcpy(xyz, &temp.GetPoint(), 0xc);
					*monsObj = temp.GetNodeBase();
				}
			}
			else {
				//如何对象死亡，忽悠清空对象
				//memset(xyz, 0, 0xc);
				*monsObj = 0;
			}
			return;
		}
		//调用原始的
		__asm {
			push monsObj;
			push xyz;
			push skillObj;
			mov eax, g_HookCallAddr;
			call eax;
		}
	}
	catch(...) {
		utils::GetInstance()->log("ERROR: SkillHookStub(DWORD skillObj, PFLOAT xyz, PDWORD monsObj)出现异常！\n");
	}
}
