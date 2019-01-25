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
	string m_strTitle;		//��ʾ����	
	
	COLORREF m_colorFont;	//������ɫ
	
	int  m_nSizeFont;		//�����С
	
	CPoint  m_PtText;		//���ֵ�λ�ã�����
	
	CString m_strFontName;
		
	CBitmap m_bitmap;  

	string m_sPicFileName;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


