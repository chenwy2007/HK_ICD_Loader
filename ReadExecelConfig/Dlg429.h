#pragma once
#include "afxwin.h"


// CDlg429 �Ի���
#include "Execel_429.h"
class CDlg429 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg429)

public:
	CDlg429(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg429();

// �Ի�������
	enum { IDD = IDD_DIALOG_429 };

	Execel_429  m_config_429;
	// ʵ��
 
	vector<__SYS_Drive>m_DriveVec;
	vector<__DLGInfo>m_dlgVec;

	int m_nDriveId;
	int m_nSysDriveId;
	
	int m_nProtoclItem_Index;
	int m_nProtocolId;
	BOOL CheckData();
	 
	CString GetSimData();

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelectfile();
	virtual BOOL OnInitDialog();
	CComboBox m_combo_protocol;
	CGridCtrl m_grid;
 
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonSave();
	CComboBox m_comboDrive;
	 
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK(){};
	virtual void OnCancel(){};

};
