#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define PROGRESS_POS (WM_USER+1)
#define EXIT_PROGRESS (WM_USER+2)

// CDlgProgress 对话框

class CDlgProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProgress)

public:
	CDlgProgress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProgress();

// 对话框数据
	enum { IDD = IDD_DLG_PROGRESS };
	void SetProgressTitle(CString csTitle);
	void InitialProgress(int nMin, int nCurPos, int nMax);
	void SetProgressPos(int nPos);

	void SetProgressType(int nType) {m_nProgressType = nType;}
	void SetInfor(CString csInfor);
	void ExitWindow(void);
	int GetExitCode(void) {return m_nExitCode;}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CProgressCtrl m_progressctrl;
	CStatic m_static_progress;
	int m_nMaxCount;
	int m_nMinCount;
	int m_nCurPos;

	int m_nProgressType;
	int m_nExitCode;
public:
	afx_msg void OnClose();
	CString m_strTitle;
	CString m_strInfor;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnSetPos(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExitProgress(WPARAM wParam, LPARAM lParam);
};
