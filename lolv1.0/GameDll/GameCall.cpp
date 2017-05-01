#include "stdafx.h"
#include "GameCall.h"

//
//全局变量声明
//
GameCall* GameCall::m_pInstance = nullptr;
std::mutex GameCall::m_mutex;

GameCall::GameCall()
{
}


GameCall::GameCall(const GameCall& _call)
{

}

GameCall GameCall::operator=(const GameCall& _call)
{

}

GameCall::~GameCall()
{
}

GameCall* GameCall::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_mutex.lock();
		if (nullptr == m_pInstance)
		{
			m_pInstance = new GameCall;
		}
		m_mutex.unlock();
	}
	return m_pInstance;
}

float GameCall::GetClientTickTime() const
{
	return utils::GetInstance()->read<float>(Base_GameStartTime + 0x30);
}
