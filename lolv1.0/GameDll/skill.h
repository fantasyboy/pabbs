#pragma once
#include "base.h"

enum EM_SKILL_INDEX
{
	Q,
	W,
	E,
	R,
};


enum EM_SKILL_TYPE
{
	EM_LOCKING = 0x100, //锁定技能
	EM_UNLOCKING,
	EM_UNKNOWN,
};

class skill :
	public base
{
public:
	skill(EM_SKILL_INDEX _index,DWORD dwNodeBase);
	~skill();
	//获取技能名字
	virtual char* GetName() const;
	//获取技能等级
	DWORD GetLevel()const;
	//技能是否冷却
	bool bCoolDown();
	//技能消耗的蓝量
	float GetExpendMP();
	//技能最大冷却时间
	float GetMaxCoolTime();
	//获取技能范围
	float GetSkillRange();
	//获取技能范围1
	float GetSkillRange1();
	//获取技能范围2
	float GetSkillRange2();
	//获取技能索引
	DWORD GetIndex()const;
	//获取玩家是否是锁定技能 如果是锁定技能，这里就是范围
	float GetSkillType()const;
private:
	EM_SKILL_INDEX m_index;
};

