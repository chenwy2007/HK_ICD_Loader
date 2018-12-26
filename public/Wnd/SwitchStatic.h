#pragma once
#include "afxwin.h"
 
/*
属性：
 园：初始值
 背景色
 边线颜色：

*/
 
class CSwitchStatic :
	public CStatic
{
public:
	CSwitchStatic(void);
	~CSwitchStatic(void);

public: //run
	bool SetCurValue(int nValue);
	 
public: //初始化
	void SetValueColor(int nIndex,COLORREF ref);
 	void SetValueColor(map<int ,COLORREF>&ref);
	void SetSideColor(COLORREF ref);

	void SetTitleInfo(CString sInfo, int nAlign =1, int nSize = 80 );

	void SetBkColor(COLORREF ref){m_BackColor =ref;};


	void OnMyPaint(CDC * pDC ,CRect rt);
private:
	void DrawInfo(CDC *pDC ,CRect rt);
	 
public:
	
	//是否有背景色
	bool m_bBackColor;

	//背景色
	COLORREF m_BackColor;
	
	//当前圆背景色
	COLORREF m_CurCircleColor;
	
	//圆边线色
	COLORREF m_lineColor;

	//各个值对应的颜色，默认2种
	map<int ,COLORREF>m_colorMap;

	//默认值
	int m_nCurValue;


	CFont m_fontText;
	int m_fontSize;
	
	COLORREF m_FontColor;
	CString  m_strTitle;

	int m_nAlign ;//0:不显示标题，1：左侧，2：右侧，3：上侧，4：下侧
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};

