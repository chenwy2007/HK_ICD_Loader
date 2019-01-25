
// ReadExecelConfigDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Execel_429.h"

// CReadExecelConfigDlg 对话框
class CReadExecelConfigDlg : public CDialogEx
{
// 构造
public:
	CReadExecelConfigDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_READEXECELCONFIG_DIALOG };

	//灯光告警系统
	void InitList_2();
	void InitListData_2();
	void Analysis_2();

	void InitList(int nType);

	void ReadSysGoods();

	bool Analysis(CString &sInfo,CString& sSysName,CString &sDriveName,CString& sChannelIndex);
	void InsertData(int nDut,CString strInfo[9]);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	
	int m_nUnitNum;
	int m_nShowDataType;

	vector<__ConfigInfo>m_configVec;
	XReadConfig	  m_config;

 
	Execel_429  m_config_429;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelectfile();
	CComboBox m_comboGoods;
	CListCtrl m_list;
	afx_msg void OnBnClickedButtonRead();
	CListCtrl m_list_1;
	afx_msg void OnBnClickedButtonChanage();
	afx_msg void OnBnClickedButtonSave();
	CComboBox m_comboBoxGoods_1;
	CComboBox m_comboSys;
	int m_nUnitNums;
	 
	afx_msg void OnBnClickedRadioMode();
	afx_msg void OnBnClickedRadioMode2();
};
