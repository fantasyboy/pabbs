#pragma once
#include "CShareStruct.h"
#include "skill.h"
#include "person.h"
DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
);

extern SHARED_MEMORY* pSharedMemoryPointer;


//使用技能
void UseSkillByindex(skill& sk,person& mons ,person& ps);
void UseAttackAA2Mons(person & mons, person& ps);

void UseSkill(DWORD code);