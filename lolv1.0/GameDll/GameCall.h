#pragma once
#include <mutex>
#include "utils.h"
#include "BaseAddr.h"

extern DWORD g_MonsterObj;
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
	//停止动作
	bool StopAction();
private:
	static GameCall* m_pInstance;
	static std::mutex m_mutex;
private:
	GameCall();
	GameCall(const GameCall& _call);
	GameCall operator= (const GameCall& _call);
private:
	bool m_bUsedAttack;
	bool m_bUsedSkill;
};


void __stdcall SkillHookStub(DWORD skillObj, PFLOAT xyz, PDWORD monsObj);