#pragma once


// CLogainVierify 对话框

class CLogainVierify : public CDialogEx
{
	DECLARE_DYNAMIC(CLogainVierify)

public:
	CLogainVierify(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogainVierify();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void Onshow();
	CString m_szAcc;
	CString m_szPass;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
