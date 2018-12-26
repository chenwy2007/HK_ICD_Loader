#pragma once


// COwnerDrawTabCtrl
/*
属性
TAB页:高度，字体大小、非选中背景色、选中背景色

主背景色，边线颜色

*/
class COwnerDrawTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(COwnerDrawTabCtrl)

public:
	COwnerDrawTabCtrl();
	virtual ~COwnerDrawTabCtrl();

public:
	void SetItemTabSize(int nW,int nH);
	void SetFontInfo(int nSize,COLORREF colorFont=RGB(255,255,255),CString sFontName="黑体");
	void SetItemTabColor(COLORREF  selectTabColor,COLORREF  unSelectTabColor);
public:
	void DrawInfo(CDC * pDC ,CRect rt);
	virtual void DrawInfoEx(CDC *pDC,CRect rt){};

public:

	//背景色 、边线色、标签宽度，标签高度、字体颜色、字体、字号、、选中标签颜色，未选中标签颜色

	COLORREF  m_selectTabColor;
	
	COLORREF  m_unSelectTabColor;

	COLORREF  m_LineColor;

	COLORREF  m_backColor;

	COLORREF m_colorText;

	int m_nFontSize;

	string m_sFontName;

	int m_nTabWidth;

	int m_nTabHight;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	 
	 
};


