#pragma once

/*输出文字 控件上的文字

1.文字不变

2.颜色，默认值、根据共享内存值类设置
 
*/

class XTextOutColor
{
public:
	XTextOutColor(float fScale,__TextAttr_V02 &attr);
	~XTextOutColor(void);

public:
	void InitData();
	bool RefreshData();
	
	void SetCurTextColor(COLORREF ref);
	void ReSetTextColor();
public:
	void DrawInfoEx(CDC * pDC ,CRect rt);
	 
public:
	int m_nId;
	int m_nCtrlId;

	float m_fScale;

	string m_sTitle;  
	
	COLORREF m_ColorCurFont;

	COLORREF m_ColorDefaultFont;
	
	//字体	
	string m_sFontName;
	
	//字号
	int m_nFontSize;

	//是否粗体,暂时不用
	bool m_bBold;	
	 
	//显示位置
	int m_nAlign; //>=0,表示使用m_nAlign，-1，位置的输出路径为m_nX，m_nY
	int m_nX;
	int m_nY;

	vector<__Ctrl_Event_Text>m_RefreshEventVec;


	int m_nCurValue;
};
 