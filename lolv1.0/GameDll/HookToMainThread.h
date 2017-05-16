#pragma once
#include "skill.h"
//游戏自定义消息
enum MESSAGE
{
	MSG_SKILLCALL = 100, //技能CALL
	MSG_ATTACKCALL, //普攻CALL
	MSG_FINDWAY, //寻路CALL
};


//使用技能结构体
struct SKILL_TO_MONS
{
	EM_SKILL_INDEX index;
	DWORD monsObj;
};

class CHookToMainThread
{
public:
	CHookToMainThread();
	~CHookToMainThread();

	//挂载到主线程
	bool Hook();
	//卸载主线程
	bool UnHook();
	//发送消息
	void SendMessageToGame(MESSAGE msg, LPARAM lparam);

	static DWORD m_msgCode;
public:
	static HHOOK m_hHook;
private:
	HWND GetGameHwnd()const;
};

LRESULT CALLBACK CallWndProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

