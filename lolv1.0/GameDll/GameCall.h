#pragma once
#include <mutex>
#include "utils.h"
#include "BaseAddr.h"
class GameCall
{
public:
	~GameCall();
	static GameCall* GetInstance();
	float GetClientTickTime()const;
private:
	static GameCall* m_pInstance;
	static std::mutex m_mutex;
private:
	GameCall();
	GameCall(const GameCall& _call);
	GameCall operator= (const GameCall& _call);
};

