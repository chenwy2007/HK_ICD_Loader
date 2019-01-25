#pragma once
#include "XBaseWnd.h"

// XWnd_Static
/*
�Ƿ��б��� Back :0���ޱ�����1���б���
����ɫ  BackColor: 
�����С��FontSize:
������ɫ:FontColor:
*/

//����������ֵΪnValueʱ����ʾ����ΪsShowInfo
struct __DataVarValue
{
	int nValue;
	string sShowInfo;
};

class XTextStatic : public XBaseWnd 
{
 
public:	
	XTextStatic();
	
	virtual ~XTextStatic();

public:
	virtual void CreateCtrl(CString sCtrlName,CString sTitle,DWORD  nCtrlId,CRect rect,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);

	virtual void OnPaintEx(CDC *pDC,CRect rt);

	virtual void RefreshData();	

	void SetMultiline(bool bMultiline){m_bMultiline = bMultiline;};

	void SetColor(bool bBackColor,COLORREF backColor,COLORREF bLineColor);
  
public:
	void SetFontInfo(int nFontSize=16,int nAlign=DT_LEFT,COLORREF fontColor=RGB(255,255,255),CString fontName="����");
public:
	
	COLORREF m_FontColor;
	
	string m_sFontName;

	int m_nFontSize;
 	
	int m_nAlign; //DT_LEFT DT_CENTER, DT_RIGHT

	COLORREF m_backColor;
	
	COLORREF m_lineColor;

	int m_bBackColor; //1���б���ɫ	

	bool m_bMultiline;

	string m_sDefaultShowInfo;

	map<int ,string>m_valueShowInfo;
	map<int,COLORREF>m_valueShowBackColor;
};


