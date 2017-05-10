#include "stdafx.h"
#include "EquipmentServices.h"
#include "BaseAddr.h"

CEquipmentServices::CEquipmentServices(DWORD dwObjectBase): m_dwObjectBase(dwObjectBase)
{
}


CEquipmentServices::~CEquipmentServices()
{
}

void CEquipmentServices::travse()
{
	m_equipmentList.clear();
	auto nodeBase = m_dwObjectBase + pSharedMemoryPointer->Base_equipmentTravseOffset1 + pSharedMemoryPointer->Base_equipmentTravseOffset2;
	for (auto i = 0 ; i != 0x27; i++)
	{
		auto temp = utils::GetInstance()->read<DWORD>(nodeBase + 4 * i);
		if (temp == 0 || utils::GetInstance()->read<DWORD>(temp) == 0)
		{
			continue;
		}

		m_equipmentList.push_back(equipment(temp));
	}

	utils::GetInstance()->log("TIPS: m_equipmentList´óÐ¡Îª£º%d\n", m_equipmentList.size());
	for (auto temp : m_equipmentList)
	{
		utils::GetInstance()->log("TIPS£º %x %s %d\n",temp.GetNodeBase(),
			temp.GetName(),
			temp.GetCount()
		);
	}
}
