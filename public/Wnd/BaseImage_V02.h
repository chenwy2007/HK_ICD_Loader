#pragma once


// CBaseImage_V02

/*
基础类

1.是否有背景色
2.边框色，背景色
3.背景图片
4，文字的显示子类中实现

按钮
1.背景色，加文字颜色
2.背景色，加文字颜色的变化
3.全部是图片的切换

led
1.背景色，加文字的变化,单文字、多文字
2.背景图， 文字颜色变化，单文字、多文字
3.背景图的变化，文字颜色变化，单文字、多文字

*/
class CBaseImage_V02 : public CWnd
{
	DECLARE_DYNAMIC(CBaseImage_V02)

public:
	CBaseImage_V02();
	virtual ~CBaseImage_V02();


public:
	//虚函数
	virtual void OnChildDrawInfo(CDC * pDc,CRect  rt){};

	virtual void OnChildLButtonUp(UINT nFlags, CPoint point){};

	virtual void OnChildTimer(UINT_PTR nIDEvent){};
	 
	void SetDefulatPicFileName(string sPicFilePath);
	 
	//设置背景图路径
	bool SetPicFileName(string sPicFilePath);
	
	void SetCurFontColor(COLORREF ref);

	void DrawInfo(CDC *pDC,CRect rt);

	void DrawText(CDC  * pDC,CRect rt);
public:

	//是否有背景色
	bool m_bBackColor;
	
	//有背景色，边线颜色
	COLORREF m_ColorLine;
	
	//有背景色，背景颜色
	COLORREF m_ColorBack;

	//是否背景图
	bool m_bBackBmp;

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

	int m_nEType;

protected:

	CBitmap m_bitmap;  
	int m_nbmpWidth;
	int m_nbmpHigth;

	void CreateBmp(bool bReCreate = false);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


