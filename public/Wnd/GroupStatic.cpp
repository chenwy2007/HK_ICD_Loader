// GroupStatic.cpp : 实现文件
//

#include "stdafx.h"


#include "GroupStatic.h"


// CGroupStatic

IMPLEMENT_DYNAMIC(CGroupStatic, CStatic)

CGroupStatic::CGroupStatic()
{
	m_lineColor=RGB(255,255,255);
	m_colorFont =RGB(0,64,128);
	
	m_nFontSize =20;
	
	m_strTitle="";
	m_sFontName="黑体";

}

CGroupStatic::~CGroupStatic()
{
}


BEGIN_MESSAGE_MAP(CGroupStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CGroupStatic 消息处理程序


void CGroupStatic::OnPaint()
{
	CPaintDC dc(this);  
	CDC *pDC = &dc;
	CRect rt;
	GetClientRect(rt);
	OnMyPaint(pDC,rt);	
 
}
void CGroupStatic::ReCreate()
{

}
void CGroupStatic::OnMyPaint(CDC * pDC,CRect rt)
{
	CFont m_fontText;

	if(m_strTitle.IsEmpty())
		GetWindowText(m_strTitle);

	CFont font;
	CFont *pFont;
	font.CreateFont(m_nFontSize,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,m_sFontName.c_str());
	CFont *pOldFont=pDC->SelectObject(&font);


	int nTxtLeft =10;
	CPen pen;
	pen.CreatePen(PS_SOLID ,1,RGB(128,128,128));
	CPen pen2;
	pen2.CreatePen(PS_SOLID ,1,RGB(255,255,255));

 
	CPen *oldPen = pDC->SelectObject(&pen);
	CPen *lpPen[]={&pen,&pen2};
	CSize size  = pDC->GetTextExtent(m_strTitle);
	int cy = size.cy;
	int cx = size.cx;
	CString str = m_strTitle;
	str.Trim();
	if(str.IsEmpty())
		cx =0;
	
	for(int i =0;i<2;i++)
	{
		pDC->SelectObject(lpPen[i]);

		pDC->MoveTo(1+i,cy/2+i);
		pDC->LineTo(1+i,rt.Height()-1-i);

		pDC->LineTo(rt.Width()-1-i,rt.Height()-1-i);
		pDC->LineTo(rt.Width()-1-i,cy/2+i);

		if(cx+nTxtLeft<rt.Width())
			pDC->LineTo(cx+nTxtLeft,cy/2+i);
		else
			pDC->LineTo(rt.Width()-nTxtLeft,cy/2+i);


		pDC->MoveTo(1+i,cy/2+i);
		pDC->LineTo(nTxtLeft,cy/2+i);

	}

	pDC->SetTextColor(m_colorFont);
	pDC->SetBkMode(TRANSPARENT);
	CRect rtTxt;
	rtTxt.top =0;
	rtTxt.bottom = size.cy;

	rtTxt.left =nTxtLeft;
	rtTxt.right=nTxtLeft+cx;
	if(rtTxt.right>rt.Width())
	{
		rtTxt.right= rt.Width()-nTxtLeft;
		rtTxt.bottom = size.cy*2;
	}
	if(!str.IsEmpty())
		pDC->DrawText(m_strTitle,rtTxt,DT_LEFT|DT_VCENTER|DT_WORDBREAK);


	pDC->SelectObject(oldPen);	  
	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}