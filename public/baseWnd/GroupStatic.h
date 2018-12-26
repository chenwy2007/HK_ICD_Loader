#pragma once

 
class CGroupStatic : public CStatic
{
	DECLARE_DYNAMIC(CGroupStatic)


public:
	CGroupStatic();
	virtual ~CGroupStatic();

	COLORREF m_lineColor;
	COLORREF m_FontColor;
	CFont m_fontText;
	int m_fontSize;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


