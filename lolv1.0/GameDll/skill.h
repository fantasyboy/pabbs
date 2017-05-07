#pragma once
#include "base.h"

enum EM_SKILL_INDEX
{
	Q,
	W,
	E,
	R,
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
	//获取技能索引
	DWORD GetIndex()const;
private:
	EM_SKILL_INDEX m_index;
};

