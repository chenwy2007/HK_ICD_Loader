// GroupStatic.cpp : 实现文件
//

#include "stdafx.h"

#include "GroupStatic.h"


// CGroupStatic

IMPLEMENT_DYNAMIC(CGroupStatic, CStatic)

CGroupStatic::CGroupStatic()
{
	m_lineColor=RGB(255,255,255);
	m_FontColor =RGB(255,255,255);
	m_fontSize =100;
	m_fontText.CreatePointFont(m_fontSize,"宋体",NULL);

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

	CString strText;
	GetWindowText(strText);
	CDC *pDC = &dc;
	CRect rt;
	GetClientRect(rt);
	int nTxtLeft =10;
	CPen pen;
	pen.CreatePen(PS_SOLID ,1,RGB(128,128,128));
	CPen pen2;
	pen2.CreatePen(PS_SOLID ,1,RGB(255,255,255));

 	CFont * OldFont=dc.SelectObject(&m_fontText);
	CPen *oldPen = dc.SelectObject(&pen);
	CPen *lpPen[]={&pen,&pen2};
	CSize size  = dc.GetTextExtent(strText);
	 
	for(int i =0;i<2;i++)
	{
		dc.SelectObject(lpPen[i]);

		dc.MoveTo(1+i,size.cy/2);
		dc.LineTo(1+i,rt.Height()-1-i);

		dc.LineTo(rt.Width()-1-i,rt.Height()-1-i);
		dc.LineTo(rt.Width()-1-i,size.cy/2+i);

		dc.LineTo(size.cx+nTxtLeft,size.cy/2+i);
	
		dc.MoveTo(1+i,size.cy/2+i);
		dc.LineTo(nTxtLeft,size.cy/2+i);
 
	}

	dc.SetTextColor(m_FontColor);
	dc.SetBkMode(TRANSPARENT);
	CRect rtTxt;
	rtTxt.left =nTxtLeft;
	rtTxt.right=nTxtLeft+size.cx;
	rtTxt.top =0;
	rtTxt.bottom = size.cy;
	dc.DrawText(strText,rtTxt,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	 

	dc.SelectObject(oldPen);	  
	dc.SelectObject(OldFont);
 	  
 
}
