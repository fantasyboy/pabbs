#include "stdafx.h"
#include "GameCall.h"
#include "person.h"
//
//全局变量声明
//
GameCall* GameCall::m_pInstance = nullptr;
std::mutex GameCall::m_mutex;
std::mutex g_mutex;
DWORD g_MonsterObj = NULL;
DWORD g_HookCallAddr = 0;
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
		StopAction();
		static float fTargetPointArray[3] = { 0 };
		memcpy(fTargetPointArray, (void*)(dwNodeBase + 0x50), sizeof(float) * 3);
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
		if (monsObj) {
			g_mutex.lock();
			g_MonsterObj = monsObj;
			g_mutex.unlock();
		}
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
	memcpy(&g_HookCallAddr, (void*)(HookAddr + 1), 4);
	char hookData[5] = { 0xe8, 0x0, 0x0, 0x0, 0x0 };
	*(DWORD*)(&hookData[1]) = (DWORD)(&SkillHookStub) - HookAddr - 0x5;
	DWORD oldProtected = 0;
	VirtualProtect((LPVOID)HookAddr, 5,PAGE_READWRITE ,&oldProtected);
	memcpy((void*)HookAddr, (void*)hookData, 5);
	VirtualProtect((LPVOID)HookAddr, 5, oldProtected, &oldProtected);
	return true;
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
	//dd [[1E46A3C]+10]+1c
	EM_POINT_3D temp = { 0 };
	__try
	{
		auto dwBase = utils::GetInstance()->read<DWORD>(Base_MousePointAddr);
		if (dwBase)
		{
			auto Offset1 = utils::GetInstance()->read<DWORD>(dwBase+0x10);
			if (Offset1)
			{
				temp.x = utils::GetInstance()->read<float>(Offset1 + 0x1c);
				temp.z = utils::GetInstance()->read<float>(Offset1 + 0x20);
				temp.y = utils::GetInstance()->read<float>(Offset1 + 0x24);
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

void __stdcall SkillHookStub(DWORD skillObj, PFLOAT xyz, PDWORD monsObj)
{
	__try {
		if (g_MonsterObj)
		{
			//如果有对象存在，就调用自己的
			g_mutex.lock();
			auto temp = g_MonsterObj;
			g_mutex.unlock();

			memcpy(xyz, (float*)(temp + 0x50), 0xc);
			*monsObj = temp;
			g_MonsterObj = NULL; 
			return;
		}
		//调用原始的
		__asm {
			pushad;
			push monsObj;
			push xyz;
			push skillObj;
			mov eax, g_HookCallAddr;
			call eax;
			popad;
		}
	}
	__except (1) {
		utils::GetInstance()->log("ERROR: SkillHookStub(DWORD skillObj, PFLOAT xyz, PDWORD monsObj)出现异常！\n");
	}
}
