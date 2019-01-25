#pragma once


// XWnd_Dlg 对话框
/*
窗口属性：
1.style：弹出窗口、子窗口
2.标题内容

3.弹出窗口：标题高度、背景颜色、字体大小、字体颜色
4.弹出窗口：状态栏，高度

5.背景色

6.x,y,w,h
*/
class XWnd_Dlg : public  CBaseDialog ,public CBaseElemCtrl
{
	DECLARE_DYNAMIC(XWnd_Dlg)

public:
	XWnd_Dlg(int nId =IDD_DIALOG_BASE,CWnd* pParent = NULL);
	virtual ~XWnd_Dlg();
	   
	virtual void  SetScale(float fScale);
	virtual void DecodeXml();
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW );
	
	virtual void SetSelect(bool bSel);
	
	void MoveWnd();
	void MoveWnd(CRect clientRect);
	virtual void ReSizeCtrl(CBaseElemCtrl * pCtrl);
 
	virtual void AddChildCtrl_ID(int nWndId);
	virtual void AddChildCtrl(__CtrlInfo&info);

	virtual CWnd * GetParentWnd(){return GetParent();};
	
	void MoveCtrl();
	
	void ShowWnd(int bShow);

	virtual void RefreshData();
	virtual void OnExit();
	
	void ShowWindow_Ex(int bShow);
	
	CBaseElemCtrl * FindElemCtrlByEventType(int nEventType);

public:
	void InitCtrl(); 	
	void DealTimer();

	virtual void DealInitEvent();
	virtual  void DealCloseEvent();
	
	void DealEvent();
	void ReleaseRec();
	
	void StartReadData(int nStartFlag);
	void DrawToolTip(RECT rect,CString sInfor,int nShow=SW_SHOW);

	//virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	int m_nEventNo;

	bool m_bScale;;
	vector<CBaseElemCtrl *>m_CtrlVec;
	vector<__CtrlInfo>m_subCtrlVec;

	int m_nTimer;
	int m_nDutIndex;

	//CToolTipCtrl m_tooltip;
	CWnd* m_pToolTip;
 
	vector<__Ctrl_Event>m_InitEventVec;
	vector<__Ctrl_Event>m_ColseEventVec;
public:
	//窗口的属性
	//标题高度、标题颜色、标题字体大小颜色
	//状态栏，高度，内容
	
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();
	//virtual void ShowWindow()
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//afx_msg LRESULT OnNcHitTest(CPoint point);	 
};
