#pragma once


// CBaseImage_Rotate_V02
//背景图，根据给定的角度旋转



class CBaseImage_Rotate_V02 : public CWnd
{
	DECLARE_DYNAMIC(CBaseImage_Rotate_V02)

public:
	CBaseImage_Rotate_V02();
	virtual ~CBaseImage_Rotate_V02();


public:
	//虚函数
	virtual void OnChildDrawInfo(CDC * pDc,CRect  rt){};

	virtual void OnChildLButtonUp(UINT nFlags, CPoint point){};

	virtual void OnChildTimer(UINT_PTR nIDEvent){};
  
	//设置背景图路径
	bool SetPicFileName(string sPicFilePath);
	
	void SetRotate(float fRotate);

	void SetCurFontColor(COLORREF ref);

	void DrawInfo(CDC *pDC,CRect rt);
 
	void Rotate();
public:

	//是否有背景色
	bool m_bBackColor;

	//有背景色，边线颜色
	COLORREF m_ColorLine;

	//有背景色，背景颜色
	COLORREF m_ColorBack;

	//图片路径
	string m_sPicFileName;

	string m_sDefaultPicFileName;
	//字体颜色
	COLORREF m_ColorFont;

	string m_sFontName;

	int m_nFontSize;

	int m_nAlign;

	//显示内容
	string m_sTitle;

	//图片的显示的开始位置，结束位置
	int m_nBitX;

	int m_nBitY;
	
	//显示角度
	float m_fRotate;

	CString m_strPicFilePath;
protected:
	CxImage * m_lpImage;

	DWORD m_nFileType;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


