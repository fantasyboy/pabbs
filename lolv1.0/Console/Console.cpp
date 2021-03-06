
// Console.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Console.h"
#include "ConsoleDlg.h"
#include "LogainVierify.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CConsoleApp

BEGIN_MESSAGE_MAP(CConsoleApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CConsoleApp 构造

CConsoleApp::CConsoleApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CConsoleApp 对象

CConsoleApp theApp;


// CConsoleApp 初始化

BOOL CConsoleApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//防止多开
	auto handle = ::CreateMutex(NULL, FALSE, ("Mutex2010073"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(handle);
		return FALSE;
	}

	CString temp;
	char zpath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, zpath);
	temp.Append(zpath);
	temp.Append("\\AllpurAuthentic.dll");

	CString aa;
	aa.Format("regsvr32  \"%s\" ", temp);
	WinExec(aa.GetBuffer(), SW_HIDE);


	Sleep(1000);
	::CoInitialize(NULL);
	pAuth.CreateInstance(__uuidof(CurrencyAuth));

	//加载资源
	

	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(IDR_VNC2), "vnc");
	if (!hRsrc)
	{
		return FALSE;
	}
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (!dwSize)
	{
		return FALSE;
	}
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (!hGlobal)
	{
		return FALSE;
	}
	LPVOID pBuffer = LockResource(hGlobal);
	if (NULL == pBuffer)
		return FALSE;
	pAuth->InputVNC((int)pBuffer, dwSize);

	Sleep(100);
	VMProtectBegin("mad");
	auto result  = pAuth->Initialize("{C1E14188-7629-4B10-B2B5-E03B523C90E6}");//验证组件初始化
	VMProtectEnd();
	if (result == -1)
	{
		//销毁验证组件
		pAuth.Release();
		::CoUninitialize();
		return FALSE;
	}



	//CConsoleDlg dlg;
	CLogainVierify dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	//销毁验证组件
	pAuth.Release();
	::CoUninitialize();

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

