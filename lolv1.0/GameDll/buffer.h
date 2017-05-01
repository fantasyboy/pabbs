#pragma once
#include "base.h"
/*
玩家buff
*/

class buffer :
	public base
{
public:
	buffer(DWORD dwNodeBase);
	~buffer();

	virtual char* GetName()const;
	//获取buff层数
	DWORD GetBufferCount()const;
};

