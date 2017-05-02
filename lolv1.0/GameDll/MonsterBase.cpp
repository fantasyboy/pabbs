#include "stdafx.h"
#include "MonsterBase.h"


MonsterBase::MonsterBase(DWORD dwNodeBase): base(dwNodeBase)
{

}


MonsterBase::~MonsterBase()
{
}

float MonsterBase::GetCurHp() const
{
	return 0;
}

float MonsterBase::GetMaxHp() const
{
	return 0;
}

float MonsterBase::GetCurMp() const
{
	return 0;
}

float MonsterBase::GetMaxMp() const
{
	return 0;

}

EM_POINT_3D MonsterBase::GetPoint() const
{
	EM_POINT_3D temp;
	memset(&temp, 0, sizeof(EM_POINT_3D));
	return temp;
}

EM_CAMP MonsterBase::GetCamp() const
{
	return EM_CAMP::CAMP_EM;
}

EM_TYPE MonsterBase::GetType() const
{
	return EM_TYPE::CAMP_EM1;
}

bool MonsterBase::BInShowInFag() const
{
	return false;
}

bool MonsterBase::BDead() const
{
	return false;
}
