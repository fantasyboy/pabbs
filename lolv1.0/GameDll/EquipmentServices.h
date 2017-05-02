#pragma once
#include "equipment.h"
#include <vector>
class CEquipmentServices
{
public:
	CEquipmentServices(DWORD dwObjectBase);
	~CEquipmentServices();

	void travse();
private:
	std::vector<equipment> m_equipmentList;
	DWORD m_dwObjectBase;
};

