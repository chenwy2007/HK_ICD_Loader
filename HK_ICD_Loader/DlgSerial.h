#pragma once

#include "afxwin.h"


// CDlgSerial �Ի���

#include "Execel_Serial.h"
 

class CDlgSerial : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSerial)

public:
	CDlgSerial(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSerial();

// �Ի�������
	enum { IDD = IDD_DIALOG_Serial };

	vector<__SYS_Drive>m_DriveVec;
	
	vector<CString>m_protocoNameVec;

	int m_nProtoclItem_Index;
	int m_nProtocolId;

	Execel_Serial m_Serial_File;
	int m_nGirdColCount;

	int m_nDriveId;
	int m_nSysDriveId;
	
	void ReadSubInfo(CString sItemName);

	void WriteSubInfoToXml(CMarkup &xml,CString sProtocoName,CString sProtocoItemName,CString sRecSend,int nParent_ConfigId,int nParent_SharedId);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel(){};
	virtual void OnOK(){};
	CGridCtrl m_grid;
	CComboBox m_comboDrive;
	int m_nChannelNo;
	CComboBox m_combo_protocol;
	afx_msg void OnBnClickedButtonSelectfile();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonRead();
	CGridCtrl m_GridChild;
	afx_msg void OnBnClickedButtonSave();
};
