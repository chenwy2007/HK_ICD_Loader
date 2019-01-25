#pragma once


// CBaseStatic

class CBaseStatic : public CStatic
{
	DECLARE_DYNAMIC(CBaseStatic)

public:
	CBaseStatic();
	virtual ~CBaseStatic();

	virtual void DrawInfoEx(CDC *pDC,CRect rt){};

public:
	void DrawInfo(CDC *pDC,CRect rt);
	
	void SetColor(COLORREF ref);

public:
	
	string m_strTitle;		//显示内容	

 	COLORREF m_colorFont;	//字体颜色
	
	int  m_nSizeFont;		//字体大小

	int m_nAlign;			//默认：DT_VCENTER|DT_CENTER

   	COLORREF m_colorBack;	//背景颜色

	COLORREF m_colorLine;	//边线颜色

	int m_nType;		//0:圆，1：长方形，2：没有背景色，只有文字	

	COLORREF m_colorCircle ;//圆中心颜色

private:

	void DrawInfo_Circle(CDC *pDC,CRect rt);
	void DrawInfo_Rect(CDC *pDC,CRect rt);
	void DrawInfo_Text(CDC *pDc,CRect rt);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


