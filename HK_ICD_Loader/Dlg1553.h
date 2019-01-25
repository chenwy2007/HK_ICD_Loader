#pragma once
#include "afxwin.h"


// CDlg1553 对话框
#include "Execel_1553.h"

class CDlg1553 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg1553)

public:
	CDlg1553(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg1553();

// 对话框数据
	enum { IDD = IDD_DIALOG_1553 };
	 
	vector<__SYS_Drive>m_DriveVec;
	vector<__DLGInfo>m_dlgVec;

	Execel_1553 m_config_1553;

	__protocol_1553 m_data_1553;

	vector<CString>m_protocoNameVec;
	
	//主--协议项
	vector<vector<string>>m_itemInfoVec;

	void InitCtrl();

	void  ReadSubInfo(CString sProtocoName);
 	 
	bool CheckData();
  
//	void WriteSubInfoToXml(CMarkup &xml,CString sProtocoName,CString sProtocoItemName,int nParent_ConfigId,int nParent_SharedId);
	void WriteSubInfoToXml(CMarkup &xml,CString sProtocoName,CString sProtocoItemName,CString sRecSend,int nParent_ConfigId,int nParent_SharedId);
	//设备ID
	int m_nSysDriveId;
	int m_nDriveId;

	int m_nProtocolId;
	int m_nProtoclItem_Index;

	int m_nGirdColCount;
protected:
	int m_nPulicProId;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboDrive;
	//CComboBox m_comboSys;
	CComboBox m_combo_protocol;
	CGridCtrl m_grid;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSelectfile();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonSave();
	void SaveProtocolItemConfig(void);
	virtual void OnOK(){};
	virtual void OnCancel(){};

	afx_msg void OnSize(UINT nType, int cx, int cy);
	CGridCtrl m_GridChild;
};
