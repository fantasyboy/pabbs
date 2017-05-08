// Test.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CTestApp

BEGIN_MESSAGE_MAP(CTestApp, CWinApp)
END_MESSAGE_MAP()


// CTestApp 构造

CTestApp::CTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTestApp 对象

CTestApp theApp;


// CTestApp 初始化

CTestDlg* dDlgMain = NULL;
DWORD WINAPI CreateClentWindow(void* lpParamater)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	dDlgMain = new CTestDlg;
	theApp.m_pMainWnd = dDlgMain;
	dDlgMain->DoModal();
	delete dDlgMain;
	dDlgMain = NULL;
	FreeLibraryAndExitThread(theApp.m_hInstance, 0);
	return 0;
}

void CreatClent()
{
	HANDLE hThreadID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CreateClentWindow, NULL, 0, NULL);
	CloseHandle(hThreadID);
}

BOOL CTestApp::InitInstance()
{
	CWinApp::InitInstance();
	CreatClent();
	return TRUE;
}
