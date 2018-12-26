#pragma once


// CBaseImage

class CBaseImage : public CStatic
{
	DECLARE_DYNAMIC(CBaseImage)

public:
	CBaseImage();
	
	virtual ~CBaseImage();
	
	virtual void DrawInfoEx(CDC *pDC,CRect rt){};
	
	virtual void  OnMyLButtonUp(UINT nFlags, CPoint point){};
public:
	
	void SetPicFileName(string sPicFilePath);
	
	void DrawInfo(CDC *pDC,CRect rt);
	
	void DrawTxt(CDC * pDC,CRect rt);

public:	
	string m_strTitle;		//显示内容	
	
	COLORREF m_colorFont;	//字体颜色
	
	int  m_nSizeFont;		//字体大小
	
	CPoint  m_PtText;		//文字的位置，保留
	
	CString m_strFontName;
		
	CBitmap m_bitmap;  

	string m_sPicFileName;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


