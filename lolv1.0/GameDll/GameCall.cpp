#include "stdafx.h"
#include "GameCall.h"

//
//全局变量声明
//
GameCall* GameCall::m_pInstance = nullptr;
std::mutex GameCall::m_mutex;
DWORD g_MonsterObj = NULL;
GameCall::GameCall()
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
		return utils::GetInstance()->read<float>(Base_GameStartTime + 0x30);
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

		float* xyz = (float*)utils::GetInstance()->read<DWORD>(dwNodeBase + 0x50);
		__asm
		{
			pushad;
			push 1;
			push 0;
			push 0;
			mov esi, dwNodeBase;
			lea eax, xyz;
			push esi;
			push eax;
			push 3;
			mov EDX, Base_RoleSelfAddr;
			mov EDX, [EDX];
			mov ecx, edx;
			mov eax, 0x00827650;
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
		g_MonsterObj = monsObj;
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
	DWORD HookAddr = Base_SkillCallHookAddr;
	char hookData[5] = { 0xe8, 0x0, 0x0, 0x0, 0x0 };
	*(DWORD*)(&hookData[1]) = (DWORD)(&SkillHookStub) - HookAddr - 0x5;
	DWORD oldProtected = 0;
	VirtualProtect((LPVOID)HookAddr, 5,PAGE_READWRITE ,&oldProtected);
	memcpy((void*)HookAddr, (void*)hookData, 5);
	VirtualProtect((LPVOID)HookAddr, 5, oldProtected, &oldProtected);
	return true;
}

void __stdcall SkillHookStub(DWORD skillObj, PFLOAT xyz, PDWORD monsObj)
{
	if (g_MonsterObj)
	{
		//如果有对象存在，就调用自己的
		memcpy(xyz, (float*)(g_MonsterObj + 0x50), 0xc);
		*monsObj = g_MonsterObj;
		g_MonsterObj = NULL;
	}
	else
	{
		//调用原始的
		__asm {
			push monsObj;
			push xyz;
			push skillObj;
			mov eax, 0x006874A0;
			call eax;
		}
	}
}
