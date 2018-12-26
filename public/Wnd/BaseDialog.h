#pragma once
 
class CBaseDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CBaseDialog)

public:
	CBaseDialog(int nId,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBaseDialog();
	 
	//窗口的顶部、底部、左右边的高宽
	void SetSideWidth(int nTop =50,int nBottomH=30,int nLeftRight=3);
	
	//标题内容
	void SetTitleTxt(CString strTille) ;
	
	//状态文字 ,最多2个，nIndex：0，左侧输出，1：右侧输出
	void SetStatusTxt(int nIndex,CString sTxt);
	
	void SetBackColor(COLORREF colorBack,COLORREF colorTitle,COLORREF colorfont =RGB(255,255,255));

	void InvalidateStatusTxt(int nIndex);

	void SetEnableBt(bool bCloseBtn,bool bMaxBtn);

	virtual void OnExit();
	 
	virtual void DrawCustom(CDC *pDC,CRect rt){};
	 
//protected:

	COLORREF m_colorBack;
	COLORREF m_colorTitle;
	  
	 
	COLORREF m_colorFont;
	int m_nFontSize;
	
	//标题的高度
	int m_nTopHeight;
	//底部的高度
	int m_nBottomHeight;
	int m_nBorderWidth;
	
	CRect m_RectTitle;
	CString m_strTitle;

	CString m_strLeftState;
	CString m_strRigthState;

//	CFont m_FontTitle;
	CFont m_FontState;

	CRect m_RectLeftState;
	CRect m_RectRightState;

	//top 、buttom 文字输出时，左右预留宽度
	int m_nSideWidth;

	//关闭按钮，最大按钮是否显示，m_nTopHeight>0 m_bHasCloseBtn，m_bHasMaxBtn有效，，m_bHasCloseBtn:TRUE,m_bHasMaxBtn有效
	bool m_bHasCloseBtn;
	int m_nCloseBtState; //0 :默认，1：按下
	
	//是否显示图标LOG
	bool m_bHasTitleLog;

	CRect m_RectCloseBtn;

	bool m_bSideLine;

	bool m_bHasMaxBtn;
	int m_nMaxBtState; //0 :默认，1：按下
	CRect m_RectMaxBtn;
	  
	int m_nCmdShow ;

	

protected:
	//画LOG，位置为窗口的左上角,并返回LOG的输出的右侧位置
	int  DrawTopLog(CDC * pDC);
	void DrawCloseBtn(CDC * pDC,CRect rt);
	void DrawTopTxt(CDC * pDC,CRect rt,int nLeft);
	void DrawBottomTxt(CDC * pDC,CRect rt);
	void OnMyPaint(CDC * pDC,CRect rt);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
 virtual BOOL OnInitDialog();
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
