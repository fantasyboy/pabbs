
// ConsoleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Console.h"
#include "ConsoleDlg.h"
#include "afxdialogex.h"
#include "ShareMemory.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "InjectDll.h"
#include "virtualKey.h"
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

DWORD WINAPI ThreadProcA(_In_ LPVOID lpParameter)
{
	//监视窗口的回调函数
	CInjectDll injectClass("GameDll.dll");
	while (true)
	{

		auto hwnd = FindWindow("RiotWindowClass", NULL);
		if (hwnd)
		{
			//如果没有注入
			if (!injectClass.GetInject()) {
				injectClass.injectDll(hwnd);
			}
		}
		else
		{
			//如果游戏退出，设置成没有注入
			injectClass.SetInject(false);

		}
		Sleep(100);
	}
}

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CConsoleDlg 对话框



CConsoleDlg::CConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONSOLE_DIALOG, pParent)
	, m_radiobtngroup1(1)
	, m_ShowZouAMs(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CConsoleDlg::~CConsoleDlg()
{

}

void CConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_bLockQ);
	DDX_Control(pDX, IDC_CHECK2, m_bLockW);
	DDX_Control(pDX, IDC_CHECK3, m_bLockE);
	DDX_Control(pDX, IDC_CHECK4, m_bLockR);
	DDX_Control(pDX, IDC_CHECK5, m_bLockQAA);
	//  DDX_Text(pDX, IDC_STATIC_ZOUA, m_showZouAMs);
	DDX_Control(pDX, IDC_SLIDER1, m_ZouAliderCtl);
	DDX_Radio(pDX, IDC_RADIO1, m_radiobtngroup1);
	DDX_Control(pDX, IDC_EDIT1, m_LockQCtl);
	DDX_Control(pDX, IDC_EDIT2, m_lockWCtl);
	DDX_Control(pDX, IDC_EDIT3, m_lockECtl);
	DDX_Control(pDX, IDC_EDIT4, m_lockRCtl);
	DDX_Control(pDX, IDC_EDIT5, m_LockAACtl);
	DDX_Text(pDX, IDC_STATIC_TIPS, m_ShowZouAMs);
	DDX_Control(pDX, IDC_EDIT6, m_clearAACtl);
	DDX_Control(pDX, IDC_CHECK11, m_bOpenClear);
	DDX_Control(pDX, IDC_CHECK6, m_bHuabanAutoE);
	DDX_Control(pDX, IDC_CHECK9, m_autoEToHero);
	DDX_Control(pDX, IDC_CHECK10, m_autoEToMons);
}

BEGIN_MESSAGE_MAP(CConsoleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, &CConsoleDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CConsoleDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CConsoleDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CConsoleDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CConsoleDlg::OnBnClickedCheck5)
	ON_WM_CLOSE()
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CConsoleDlg::OnNMCustomdrawSlider1)
ON_BN_CLICKED(IDC_RADIO1, &CConsoleDlg::OnBnClickedRadio1)
ON_BN_CLICKED(IDC_RADIO2, &CConsoleDlg::OnBnClickedRadio1)
//ON_EN_SETFOCUS(IDC_EDIT1, &CConsoleDlg::OnEnSetfocusEdit1)
//ON_WM_KEYDOWN()
ON_BN_CLICKED(IDC_CHECK11, &CConsoleDlg::OnBnClickedCheck11)
ON_BN_CLICKED(IDC_CHECK6, &CConsoleDlg::OnBnClickedCheck6)
ON_BN_CLICKED(IDC_CHECK9, &CConsoleDlg::OnBnClickedCheck9)
ON_BN_CLICKED(IDC_CHECK10, &CConsoleDlg::OnBnClickedCheck10)
END_MESSAGE_MAP()


// CConsoleDlg 消息处理程序

void CConsoleDlg::InitGameStruct()
{
	m_sharedMemory.GetPointerOfMapView()->Base_GameStartTime = 0x01AA2768;    //游戏开局时间     偏移:1    //OD地址:0x008DF27E
	m_sharedMemory.GetPointerOfMapView()->Base_RoleSelfAddr = 0x01A9EA20;    //玩家基地址     偏移:2    //OD地址:0x005E0482
	m_sharedMemory.GetPointerOfMapView()->Base_BufferAddr = 0x01A9C290;    //buff数组基地址     偏移:2    //OD地址:0x00BB4BEA
	m_sharedMemory.GetPointerOfMapView()->Base_MonsterArrayAddr = 0x02AC8104;    //怪物数组基地址     偏移:2    //OD地址:0x00C688C2
	m_sharedMemory.GetPointerOfMapView()->Base_GameWndHwndAddr = 0x01A9C508;    //游戏窗口基地址     偏移:2    //OD地址:0x008301D4
	m_sharedMemory.GetPointerOfMapView()->Base_SkillTravseOffset1 = 0x00002948;    //技能数组第一层偏移     偏移:1    //OD地址:0x0059478D
	m_sharedMemory.GetPointerOfMapView()->Base_SkillTravseOffset2 = 0x00000560;    //技能数组第二层偏移     偏移:2    //OD地址:0x007426C0
	m_sharedMemory.GetPointerOfMapView()->Base_SkillOffset_Object = 0x000000F4;    //技能对象偏移     偏移:2    //OD地址:0x005947A4
	m_sharedMemory.GetPointerOfMapView()->Base_SkillCallEcxAddr = 0x01A9D2FC;    //技能CALL ECX     偏移:2    //OD地址:0x00E55E48
	m_sharedMemory.GetPointerOfMapView()->Base_SkillCallAddr = 0x00BDB500;    //技能CALL     偏移:1    //OD地址:0x00E55E6F
	m_sharedMemory.GetPointerOfMapView()->Base_SkillCallHookAddr = 0x00B0236D;    //技能CALL Hook     偏移:1    //OD地址:0x00B0236D
	m_sharedMemory.GetPointerOfMapView()->Base_AttackHeroCallAddr = 0x007E7560;    //普攻CALL     偏移:1    //OD地址:0x0076168E
	m_sharedMemory.GetPointerOfMapView()->Base_FindWayCallAddr = 0x007E7560;    //寻路call     偏移:1    //OD地址:0x0062FC72
	m_sharedMemory.GetPointerOfMapView()->Base_MonsterCurrentMpOffset = 0x000001F4;    //当前蓝量偏移     偏移:5    //OD地址:0x00C5F30F
	m_sharedMemory.GetPointerOfMapView()->Base_MonsterCurrentHpOffset = 0x0000056C;    //当前血量偏移     偏移:4    //OD地址:0x007C43B7（已更新）
	m_sharedMemory.GetPointerOfMapView()->Base_MonsterBVisableOffset = 0x00003A58;    //是否可见偏移     偏移:2    //OD地址:0x00A5BF03
	m_sharedMemory.GetPointerOfMapView()->Base_equipmentTravseOffset1 = 0x000015D8;    //装备数组遍历偏移1     偏移:2    //OD地址:0x00C2227B
	m_sharedMemory.GetPointerOfMapView()->Base_equipmentTravseOffset2 = 0x00000288;    //装备数组遍历偏移2     偏移:3    //OD地址:0x00C40349
	m_sharedMemory.GetPointerOfMapView()->Base_MousePointAddr = 0x1A9D2FC;            //鼠标位置基地址（已更新）
	m_sharedMemory.GetPointerOfMapView()->Base_SkillOffset_Range1 = 0x3b0;		 //技能范围偏移（已更新）
	m_sharedMemory.GetPointerOfMapView()->Base_SkillOffset_MP = 0x000005AC;    //技能蓝耗偏移     偏移:5    //OD地址:0x0077FBDB（已更新）
	m_sharedMemory.GetPointerOfMapView()->Base_MonsterOrientationXOffset = 0x312C;  //玩家X朝向
	m_sharedMemory.GetPointerOfMapView()->Base_MonsterBMovingOffset = 0x4C54; //玩家是否移动
	m_sharedMemory.GetPointerOfMapView()->Base_BufferOffset = 0x2FD0;
	m_sharedMemory.GetPointerOfMapView()->Base_BufferCountOffset = 0x7C;
}

BOOL CConsoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ZouAliderCtl.SetRange(0, 50);

	SetDlgItemText(IDC_STATIC_LOG, pAuth->GetValidity());

	//创建共享内存
	if (!m_sharedMemory.CreateSharedMemory())
	{
		AfxMessageBox("出现未知错误，请关闭程序！");
		exit(1);
		return FALSE;
	}
	m_sharedMemory.GetPointerOfMapView()->dwZouAMs = 50;
	m_ZouAliderCtl.SetPos(50);

	//设置默认的编辑框
	m_LockQCtl.SetWindowTextA(KeyNames[VK_SPACE].text);
	m_lockWCtl.SetWindowTextA(KeyNames[VK_SPACE].text);
	m_lockECtl.SetWindowTextA(KeyNames[VK_SPACE].text);
	m_lockRCtl.SetWindowTextA(KeyNames[VK_SPACE].text);
	m_LockAACtl.SetWindowTextA(KeyNames[VK_SPACE].text);
	m_clearAACtl.SetWindowTextA(KeyNames['C'].text);
	m_sharedMemory.GetPointerOfMapView()->VirtualKeyQ = VK_SPACE;
	m_sharedMemory.GetPointerOfMapView()->VirtualKeyW = VK_SPACE;
	m_sharedMemory.GetPointerOfMapView()->VirtualKeyE = VK_SPACE;
	m_sharedMemory.GetPointerOfMapView()->VirtualKeyR = VK_SPACE;
	m_sharedMemory.GetPointerOfMapView()->VirtualKeyAA = VK_SPACE;
	m_sharedMemory.GetPointerOfMapView()->VirtualKeyOpenClear = 'C';


	VMProtectBegin("InitGameStruct");
	InitGameStruct();
	VMProtectEnd();
	//创建注入线程
	m_ThreadHanle = ::CreateThread(NULL, NULL,LPTHREAD_START_ROUTINE(ThreadProcA) , this, NULL, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CConsoleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CConsoleDlg::OnBnClickedCheck1()
{
	m_sharedMemory.GetPointerOfMapView()->bLockQ = m_bLockQ.GetCheck() == BST_CHECKED ? true : false;
	m_LockQCtl.EnableWindow( !(m_bLockQ.GetCheck() == BST_CHECKED) );
}

void CConsoleDlg::OnBnClickedCheck2()
{
	m_sharedMemory.GetPointerOfMapView()->bLockW = m_bLockW.GetCheck() == BST_CHECKED ? true : false;
	m_lockWCtl.EnableWindow(!(m_bLockW.GetCheck() == BST_CHECKED));
}


void CConsoleDlg::OnBnClickedCheck3()
{
	m_sharedMemory.GetPointerOfMapView()->bLockE = m_bLockE.GetCheck() == BST_CHECKED ? true : false;
	m_lockECtl.EnableWindow(!(m_bLockE.GetCheck() == BST_CHECKED));
}


void CConsoleDlg::OnBnClickedCheck4()
{
	m_sharedMemory.GetPointerOfMapView()->bLockR = m_bLockR.GetCheck() == BST_CHECKED ? true : false;
	m_lockRCtl.EnableWindow(!(m_bLockR.GetCheck() == BST_CHECKED));
}


void CConsoleDlg::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sharedMemory.GetPointerOfMapView()->bOpenAA = m_bLockQAA.GetCheck() == BST_CHECKED ? true : false;
	m_LockAACtl.EnableWindow(!(m_bLockQAA.GetCheck() == BST_CHECKED));
}




void CConsoleDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//屏蔽掉原生的WM_CLOSE
	CDialogEx::OnClose();
}


BOOL CConsoleDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 屏蔽ESC
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	//设置Q
	if (::GetFocus() == m_LockQCtl.GetSafeHwnd()&& pMsg->message == WM_KEYDOWN)
	{
		m_LockQCtl.SetWindowTextA(KeyNames[pMsg->wParam].text);
		m_sharedMemory.GetPointerOfMapView()->VirtualKeyQ = pMsg->wParam;
	}
	//设置W
	if (::GetFocus() == m_lockWCtl.GetSafeHwnd() && pMsg->message == WM_KEYDOWN)
	{
		m_lockWCtl.SetWindowTextA(KeyNames[pMsg->wParam].text);
		m_sharedMemory.GetPointerOfMapView()->VirtualKeyW = pMsg->wParam;
	}
	//设置E
	if (::GetFocus() == m_lockECtl.GetSafeHwnd() && pMsg->message == WM_KEYDOWN)
	{
		m_lockECtl.SetWindowTextA(KeyNames[pMsg->wParam].text);
		m_sharedMemory.GetPointerOfMapView()->VirtualKeyE = pMsg->wParam;
	}
	//设置R
	if (::GetFocus() == m_lockRCtl.GetSafeHwnd() && pMsg->message == WM_KEYDOWN)
	{
		m_lockRCtl.SetWindowTextA(KeyNames[pMsg->wParam].text);
		m_sharedMemory.GetPointerOfMapView()->VirtualKeyR = pMsg->wParam;
	}

	//设置走A
	if (::GetFocus() == m_LockAACtl.GetSafeHwnd() && pMsg->message == WM_KEYDOWN)
	{
		m_LockAACtl.SetWindowTextA(KeyNames[pMsg->wParam].text);
		m_sharedMemory.GetPointerOfMapView()->VirtualKeyAA = pMsg->wParam;
	}

	//设置走A清线热键
	if (::GetFocus() == m_clearAACtl.GetSafeHwnd() && pMsg->message == WM_KEYDOWN)
	{
		m_clearAACtl.SetWindowTextA(KeyNames[pMsg->wParam].text);
		m_sharedMemory.GetPointerOfMapView()->VirtualKeyOpenClear = pMsg->wParam;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CConsoleDlg::PreInitDialog()
{
	// TODO: 在此添加专用代码和/或调用基类
	//
	CDialogEx::PreInitDialog();
}


void CConsoleDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	UpdateData(TRUE);
	m_sharedMemory.GetPointerOfMapView()->dwZouAMs = m_ZouAliderCtl.GetPos();
	m_ShowZouAMs = m_ZouAliderCtl.GetPos();
	UpdateData(FALSE);
	*pResult = 0;
}


void CConsoleDlg::OnBnClickedRadio1()
{
	// TODO: 单选框事件处理

	UpdateData(TRUE);

	switch (m_radiobtngroup1)
	{
	case 0:
	{
		m_sharedMemory.GetPointerOfMapView()->dwTargetSelecter = 0;
		break;
	}
	case 1:
	{
		m_sharedMemory.GetPointerOfMapView()->dwTargetSelecter = 1;
		break;
	}
	default:
		AfxMessageBox("选择目标错误！");
		break;
	}
}





void CConsoleDlg::OnBnClickedCheck11()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sharedMemory.GetPointerOfMapView()->bOpenClearAA = m_bOpenClear.GetCheck() == BST_CHECKED ? true : false;
	m_clearAACtl.EnableWindow(!(m_bOpenClear.GetCheck() == BST_CHECKED));
}


void CConsoleDlg::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sharedMemory.GetPointerOfMapView()->bAutoHuabanE = m_bHuabanAutoE.GetCheck() == BST_CHECKED ? true : false;
}


void CConsoleDlg::OnBnClickedCheck9()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sharedMemory.GetPointerOfMapView()->bAutoEToHero = m_autoEToHero.GetCheck() == BST_CHECKED ? true : false;
}


void CConsoleDlg::OnBnClickedCheck10()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sharedMemory.GetPointerOfMapView()->bAutoEToMons = m_autoEToMons.GetCheck() == BST_CHECKED ? true : false;
}
