#pragma once


// XWnd_Static
/*
是否有背景 Back :0，无背景，1：有背景
背景色  BackColor: 
字体大小：FontSize:
字体颜色:FontColor:
*/
class XWnd_Static : public CStatic,public CBaseElemCtrl
{
	DECLARE_DYNAMIC(XWnd_Static)

public:
	XWnd_Static(__CtrlInfo &info,float fScale);
	
	XWnd_Static();
	
	virtual ~XWnd_Static();

	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);
	
	void SetParentBase(CBaseElemCtrl* pBase) {m_pTopBase = pBase;}

	virtual void DecodeXml();
	
	virtual void SetSelect(bool bSel);
	
	virtual void MoveWnd();
	
	virtual void RefreshData();
	
	void UpdateAttr(__StaticAttr & attrr);

	CString m_strTitle;		
	
	COLORREF m_FontColor;
		
	string m_sFontName;

	int m_nFontSize; 
	
	int m_nAlign; //DT_LEFT DT_CENTER, DT_RIGHT

	COLORREF m_backColor;
	
	int m_bBack; //1：有背景色	
	
	//共享内存值，对应的显示颜色
	__Value_Color m_ValueColor[2];

	bool m_bIsAlert;

	CString m_sToolTipInfor;
	
	map<int,string>m_valueShowInfo;
private:
	void DrawInfo(CDC *pDC,CRect rt);

protected:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

private:
	HICON m_hAlerthIcon;
	bool m_bIsDrawTip;
	CBaseElemCtrl* m_pTopBase;
};


