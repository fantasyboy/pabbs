#pragma once
#include <vector>
#include "skill.h"
class CSkillServices
{
public:
	CSkillServices();
	~CSkillServices();

	//完成对象的遍历
	void travse();
private:
	std::vector<skill> m_skillList;
};

