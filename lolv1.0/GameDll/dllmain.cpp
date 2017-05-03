// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "dllmain.h"
#include "SkillServices.h"
#include "BufferServices.h"
#include "EquipmentServices.h"
#include "MonsterServices.h"
#include "BaseAddr.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(ThreadProc), NULL, NULL, NULL);
		break;
	}
	case DLL_THREAD_ATTACH: {
		break;
	}
	case DLL_THREAD_DETACH: {
		break;
	}
	case DLL_PROCESS_DETACH: {
		break;
	}
	}
	return TRUE;
}

DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
{
	CSkillServices cs;
	CBufferServices bf;
	CEquipmentServices eq(utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr));
	CMonsterServices cm;
	while (true)
	{
		//cs.travse();
		//bf.travse();
		//eq.travse();
		utils::GetInstance()->log("name = %s", Utf8ToAnsi((char*)cm.GetNearleastPerson()).c_str()) ;
		Sleep(300);
	}

	return 0;
}

