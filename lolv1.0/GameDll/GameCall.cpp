#include "stdafx.h"
#include "GameCall.h"

//
//全局变量声明
//
GameCall* GameCall::m_pInstance = nullptr;
std::mutex GameCall::m_mutex;

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

bool GameCall::UseSkill(DWORD _index)
{
	__try
	{ 
		__asm
		{

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
	return false;
}
