#pragma once
#include "afxcmn.h"


// CDlgMain 对话框
#include "Dlg1553.H"
#include "Dlg429.h"
#include "DlgSerial.h"
#include "DlgDiscrete.h"
#include "DlgCANProtocol.h"

class CDlgMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMain)

public:
	CDlgMain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMain();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

	void InitTree();
	void InitCtrl();

	void ShowDlg(int nIndex);
	CDlg1553 * m_pDlg1553;
	CDlg429  * m_pDlg429;
	CDlgSerial *m_pDlgSerial;
	CDlgCANProtocol *m_pDlgCanPro;

	CDlgDiscrete * m_pDlgDiscrete;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_tree;
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1553();
	afx_msg void OnBnClickedButton429();
	afx_msg void OnBnClickedButtonSerial();
	afx_msg void OnBnClickedButtonsignal();
	afx_msg void OnBnClickedButtonCan();
};
