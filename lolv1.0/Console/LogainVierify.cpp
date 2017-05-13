// LogainVierify.cpp : 实现文件
//

#include "stdafx.h"
#include "Console.h"
#include "LogainVierify.h"
#include "afxdialogex.h"
#include "ConsoleDlg.h"

// CLogainVierify 对话框

IMPLEMENT_DYNAMIC(CLogainVierify, CDialogEx)

CLogainVierify::CLogainVierify(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGIN, pParent)
	, m_szAcc(_T(""))
	, m_szPass(_T(""))
{

}

CLogainVierify::~CLogainVierify()
{
}

void CLogainVierify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_szAcc);
	DDX_Text(pDX, IDC_EDIT2, m_szPass);
}


BEGIN_MESSAGE_MAP(CLogainVierify, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLogainVierify::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLogainVierify 消息处理程序


void CLogainVierify::OnBnClickedButton1()
{
	// TODO: 
	VMProtectBegin("ccc");
	char szAcc[40];
	GetDlgItem(IDC_EDIT1)->GetWindowText(szAcc, sizeof(szAcc));
	char szPwd[40];
	GetDlgItem(IDC_EDIT2)->GetWindowText(szPwd, sizeof(szPwd));

	auto ret = pAuth->UserAuth(_bstr_t(szAcc), _bstr_t(szPwd));

	switch (ret)
	{
	case 0:
		((CLogainVierify*)(theApp.m_pMainWnd))->OnOK();
		break;
	case -1:
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText("注册码不存在");
		break;
	case -2:
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText("注册码被禁用");
		break;
	case -3:
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText("绑定机器超限");
		break;
	case -4:
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText("注册码已在线");
		break;
	case -5:
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText("已过期");
		break;
	case -6:
		GetDlgItem(IDC_STATIC_LOG)->SetWindowText("用户余额不足");
		break;
	}
	VMProtectEnd();
}


BOOL CLogainVierify::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLogainVierify::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialog::OnOK();

	CConsoleDlg dlg;
	theApp.m_pMainWnd = &dlg;
	dlg.DoModal();
}
