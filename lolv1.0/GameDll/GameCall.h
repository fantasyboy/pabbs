#pragma once
#include <mutex>
#include "utils.h"
#include "BaseAddr.h"


class GameCall
{
public:
	~GameCall();
	static GameCall* GetInstance();
	//获取到游戏时间
	float GetClientTickTime()const;
	//普通攻击call
	bool HeroAttack(DWORD dwNodeBase);
	//技能攻击
	bool UseSkill(DWORD _index, DWORD monsObj);
	//Hook 技能call
	bool HookSkillUse();
private:
	static GameCall* m_pInstance;
	static std::mutex m_mutex;
private:
	GameCall();
	GameCall(const GameCall& _call);
	GameCall operator= (const GameCall& _call);
};


void __stdcall SkillHookStub(DWORD skillObj, PFLOAT xyz, PDWORD monsObj);