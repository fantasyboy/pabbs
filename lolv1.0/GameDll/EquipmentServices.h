#pragma once
#include "equipment.h"
#include <vector>
class CEquipmentServices
{
public:
	CEquipmentServices();
	~CEquipmentServices();

	void travse();
private:
	std::vector<equipment> m_equipmentList;
};

