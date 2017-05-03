#include "stdafx.h"
#include "person.h"


person::person(DWORD dwNodeBase):MonsterBase(dwNodeBase)
{
}


person::~person()
{
}

float person::GetAttackRange() const
{
	return 0;
}
