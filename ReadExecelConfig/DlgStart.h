#pragma once
#include "../public/Wnd/BaseDialog.h"
#include "../public/Wnd/ColorButton.h"
#include "afxwin.h"
#include "../commonlib/gridctrl/gridctrl.h"



// CDlgSysStart 对话框

class CDlgSysStart : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSysStart)

public:
	CDlgSysStart(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSysStart();

// 对话框数据
	enum { IDD = IDD_DIALOG_START };

	vector<__SysInfo> m_Sys_vec;
 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void  OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	//CGridCtrl m_grid;

private:
	CStatic m_static_selectsys;
	CGridCtrl m_grid_selectsys;
	//CColorButton m_btnOk;
	//CColorButton m_btnCancel;
	CButton m_btnOk;
	CButton m_btnCancel;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};
