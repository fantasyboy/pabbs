#pragma once
#include "CShareStruct.h"
#include "skill.h"
#include "person.h"
DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
);

extern SHARED_MEMORY* pSharedMemoryPointer;

struct EM_SKILL_TO_MONS
{
	DWORD skillObj;
	DWORD monsObj;
	DWORD roleObj;
};


//使用技能
void UseSkillByindex(skill& sk,person& mons ,person& ps);
void UseAttackAA2Mons(person & mons, person& ps);

bool UseSkill(DWORD code);