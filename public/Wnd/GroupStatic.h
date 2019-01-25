#pragma once

 
class CGroupStatic : public CStatic
{
	DECLARE_DYNAMIC(CGroupStatic)

public:
	CGroupStatic();

	virtual ~CGroupStatic();

	void ReCreate();

	void OnMyPaint(CDC * pDC,CRect rt);

public:
	COLORREF m_lineColor;

	COLORREF m_colorFont;
	
	int m_nFontSize;

	CString m_strTitle;
	 
	string m_sFontName;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


