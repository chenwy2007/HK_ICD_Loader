// BaseStatic.cpp : 实现文件
//

#include "stdafx.h"

#include "BaseStatic.h"


// CBaseStatic

IMPLEMENT_DYNAMIC(CBaseStatic, CStatic)

CBaseStatic::CBaseStatic()
{
	m_nAlign = DT_VCENTER|DT_CENTER|DT_SINGLELINE;

	m_nSizeFont =80;	

 
	m_colorLine =RGB(255,255,255);

	m_colorFont = RGB(255,255,255);
}

CBaseStatic::~CBaseStatic()
{
}


BEGIN_MESSAGE_MAP(CBaseStatic, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CBaseStatic 消息处理程序
 
BOOL CBaseStatic::OnEraseBkgnd(CDC* pDC)
{
	 
	return true;// CStatic::OnEraseBkgnd(pDC);
}


void CBaseStatic::OnPaint()
{
	CPaintDC dc(this);
	CDC *pDC = &dc;
	CRect rt;
	GetClientRect(rt);

	DrawInfo(pDC,rt);	 
}


void CBaseStatic::DrawInfo(CDC *pDC,CRect rt)
{	
	if(m_nType ==2)
	{
		DrawInfo_Text(pDC,rt);
	}
	if(m_nType ==1)
	{
		DrawInfo_Rect(pDC,rt);
	}
	else if(m_nType ==0)
	{
		DrawInfo_Circle(pDC,rt);
	}
	 
}
 
void CBaseStatic::SetColor(COLORREF ref)
{
	if(m_nType ==1)
	{ 
		m_colorBack = ref;
	}
	else
	{
		m_colorCircle = ref;
	}
}

void CBaseStatic::DrawInfo_Circle(CDC *pDC,CRect rt)
{

}
void CBaseStatic::DrawInfo_Rect(CDC *pDC,CRect rt)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC); 
	CBitmap BmpMem;	
	BmpMem.CreateCompatibleBitmap(pDC,rt.Width(),rt.Height());
	CBitmap* pOldBitmap= MemDC.SelectObject(&BmpMem); 

	//画长方形
	CBrush brush(m_colorBack);
	CPen pen(PS_SOLID,0,m_colorLine);
	CBrush *oldBrush = MemDC.SelectObject(&brush);	
	CPen *oldPen = MemDC.SelectObject(&pen);

	MemDC.Rectangle(rt);
	MemDC.SelectObject(oldPen);
	MemDC.SelectObject(oldBrush);
	
	DrawInfo_Text(&MemDC,rt);
	DrawInfoEx(&MemDC,rt);

	pDC->BitBlt(0, 0, rt.Width(), rt.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBitmap); 
	MemDC.DeleteDC();
	BmpMem.DeleteObject();  
}
void CBaseStatic::DrawInfo_Text(CDC *pDC,CRect rt)
{
	if(m_strTitle.empty())
		return ;

	CFont font;
	font.CreatePointFont(m_nSizeFont,"黑体",NULL);
	CFont * OldFont=pDC->SelectObject(&font);

	pDC->SetTextColor(m_colorFont);
	pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(m_strTitle.c_str(),rt,m_nAlign);
	 
	pDC->SelectObject(OldFont);
	font.DeleteObject();
}