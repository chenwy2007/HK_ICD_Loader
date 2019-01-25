#pragma once


// CColorButton

class CColorButton : public CButton 
{
	DECLARE_DYNAMIC(CColorButton)

public:
	CColorButton();
	virtual ~CColorButton();
 

public:
	void SetText(CString str);
	void SetForeColor(COLORREF color);       //设置文本颜色
	void SetBkColor(COLORREF color);         //设置背景颜色
	void SetLineColor(COLORREF color);
	COLORREF GetBkColor(){return m_BackColor;};
	void SetTextFont(int FontHight,LPCTSTR FontName);    //设置字体 

public:
	int m_nFontSize;
	COLORREF	m_lineColor;
	COLORREF     m_ForeColor;         //文本颜色
	COLORREF     m_BackColor;         //背景色
	CString      m_strText;           //按钮文字

protected:
	void   DrawButton(CDC *pDC);
	DECLARE_MESSAGE_MAP()

private:
	int          m_Style;     //按钮形状（0-正常，1-当前，2-按下，3-锁定）
	BOOL         b_InRect;            //鼠标进入标志
	
	
	
	COLORREF     m_LockForeColor;     //锁定按钮的文字颜色
	CRect        m_ButRect;           //按钮尺寸
	 
	
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


