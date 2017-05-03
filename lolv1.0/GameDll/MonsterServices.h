#pragma once
#include <vector>
#include "person.h"
#include <memory>
class CMonsterServices
{
public:
	CMonsterServices();
	~CMonsterServices();

	void travse();
	void* GetNearleastPerson();
private:
	//Íæ¼ÒÁÐ±í
	std::vector<person> m_PersonList;
	std::shared_ptr<person> m_role;
};

