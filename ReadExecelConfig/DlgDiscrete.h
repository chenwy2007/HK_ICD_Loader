#pragma once
#include "afxwin.h"


// CDlgDiscrete 对话框

class CDlgDiscrete : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDiscrete)

public:
	CDlgDiscrete(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDiscrete();

// 对话框数据
	enum { IDD = IDD_DIALOG_Discrete };

	Execel_Base  m_excelFile;
	vector<CString>m_protocoNameVec;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonSelectfile();
	CComboBox m_combo_protocol;
	CGridCtrl m_grid;
	afx_msg void OnBnClickedButtonSave();
	void SaveTblData(CString& sPageName);
private:
	CComboBox m_combox_type;


};
