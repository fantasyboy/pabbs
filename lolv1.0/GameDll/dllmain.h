#pragma once
#include "CShareStruct.h"
#include "skill.h"
#include "person.h"
DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
);

DWORD WINAPI ThreadProcLockSkill(
	_In_ LPVOID lpParameter
);

DWORD WINAPI ThreadProcAA(
	_In_ LPVOID lpParameter
);
DWORD WINAPI ThreadProcTravse(
	_In_ LPVOID lpParameter
);
DWORD WINAPI ThreadProcAutoEAA(
	_In_ LPVOID lpParameter
);
extern SHARED_MEMORY* pSharedMemoryPointer;

struct EM_SKILL_TO_MONS
{
	DWORD skillObj;
	DWORD monsObj;
	DWORD roleObj;
};


//ʹ�ü���
void UseSkillByindex(skill& sk,MonsterBase& mons ,person& ps);
void UseAttackAA2Mons(MonsterBase & mons, person& ps);

bool UseSkill(DWORD code);