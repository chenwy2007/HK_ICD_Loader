#pragma once
#include "afxwin.h"
//#include "e:\non-standardteq-hk\trunk\commonlib\gridctrl\gridctrl.h"


// CDlgCANProtocol 对话框

class CDlgCANProtocol : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCANProtocol)

public:
	CDlgCANProtocol(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCANProtocol();

// 对话框数据
	enum { IDD = IDD_DIALOG_CAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void InitCtrl();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo_protocol;
	afx_msg void OnBnClickedButtonSelectfile();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();

protected:
	void SaveProtocolItemConfig(void);

private:
		int m_nSysDriveId;
		int m_nDriveId;
		vector<__SYS_Drive>m_DriveVec;
		CGridCtrl m_grid;
		vector<CString>m_protocoNameVec;
		Execel_Base  m_excelFile;
		int m_nGirdColCount;
		int m_nProtocolId;
		int m_nProtoclItem_Index;

		int m_nPulicProId;
public:
	CComboBox m_comboDrive;
};
