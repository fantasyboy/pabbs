#pragma once
#include "base.h"
class equipment :
	public base
{
public:
	equipment(DWORD dwNodeBase);
	~equipment();


	virtual char* GetName()const;
	//获取物品数量
	DWORD GetCount()const;
	//获取装备buff层数
	DWORD GetEquBuffCount()const;
};

