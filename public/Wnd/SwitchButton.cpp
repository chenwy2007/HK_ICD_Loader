// SwitchButton.cpp : 实现文件
//

#include "stdafx.h"
#include "SwitchButton.h"


// CSwitchButton

IMPLEMENT_DYNAMIC(CSwitchButton, CButton)

CSwitchButton::CSwitchButton()
{
	m_nItemCount =2;
	m_nDefaultValue =0;
	m_bHorizontal =false;
	m_bsequence =false;
	m_bEnable =true;
	//0：开，1：关
	m_nValues[0]=1;
	m_nValues[1]=0;
	m_nIndex=1;
}

CSwitchButton::~CSwitchButton()
{
}


BEGIN_MESSAGE_MAP(CSwitchButton, CButton)
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CSwitchButton 消息处理程序
 
// 63,63,63
//127,127,127
//191,191,191,
//179,179,179
//217,217,217

//89,89,89,
void CSwitchButton::OnMyPaint(CDC * pDC)
{
	CRect rt;
	GetClientRect(rt);
}
void CSwitchButton::DrawItem(LPDRAWITEMSTRUCT  lpDrawItemStruct )
{

	if(lpDrawItemStruct->CtlType!=ODT_BUTTON)
		return;

	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	int nSavedDC=pDC->SaveDC();
 	 
	CRect rt;
	GetClientRect(rt);

	CDC MemDC;

	MemDC.CreateCompatibleDC(pDC); 

	CBitmap BmpMem;	
	BmpMem.CreateCompatibleBitmap(pDC,rt.Width(),rt.Height());
	CBitmap* pOldBitmap= MemDC.SelectObject(&BmpMem); 

	//MemDC.SetTextColor(m_FontColor);
	//MemDC.SetBkMode(TRANSPARENT);

	DrawButton(&MemDC,rt);

	pDC->BitBlt(0, 0, rt.Width(), rt.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap); 
	//释放资源   
	MemDC.DeleteDC();
	BmpMem.DeleteObject();

	pDC->RestoreDC(nSavedDC);
}

void CSwitchButton::DrawButton(CDC * pDC,CRect rt)
{

	/*CRect rt;
	GetClientRect(rt);*/
	//4边的颜色
	CPen pen(PS_SOLID,1,RGB(63,63,63));
	CPen pen1(PS_SOLID,1,RGB(191,191,191));
	//按钮
	CPen pen2(PS_SOLID,1,RGB(217,217,217));
	CPen pen3(PS_SOLID,1,RGB(89,89,89));
	CBrush brush_button(RGB(179,179,179));


 	CPen * oldPen = pDC->SelectObject(&pen);
	
	CBrush brush(RGB(127,127,127));
	
	//背景色
	CBrush * oldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(rt);

	int  nW =rt.Width()-2;
	int nH = rt.Height()-2;
	int x,y,y1;


	
	pDC->SelectObject(&pen2);
	pDC->SelectObject(&brush_button);
	//水平
	if(m_bHorizontal) 
	{
		nW=rt.Width()/m_nItemCount-2;
		nH = rt.Height()-2;	

		x = nW *m_nIndex+1;
		y = 1;
		pDC->Rectangle(x,y,x+nW,y+nH);
		x = nW *m_nIndex/2-1+nW/2;
	
		pDC->MoveTo(x,y);
		pDC->LineTo(x+nW,y+nH);
		pDC->SelectObject(&pen3);
		pDC->MoveTo(x+1,y);
		pDC->LineTo(x+nW,y+nH);
	}
	else //垂直
	{
		nW = rt.Width()-2;
		nH = (rt.Height()-2)/m_nItemCount;
	
		x = 1;
		y = nH *m_nIndex+1;		 		 
		pDC->Rectangle(x,y,x+nW,y+nH);
		 
		y1 = y+nH/2-1;
		
		pDC->MoveTo(x,y1);
		pDC->LineTo(x+nW,y1);

		pDC->SelectObject(&pen3);
		pDC->MoveTo(x,y1+1);
		pDC->LineTo(x+nW,y1+1);

		pDC->MoveTo(x+nW-1,y);
		pDC->LineTo(x+nW-1, y+nH-1);
		pDC->LineTo(0,y+nH-1);
	}
	 
	//4边
	pDC->SelectObject(&pen);
	pDC->MoveTo(0,0);
	pDC->LineTo(0,rt.Width());
	pDC->MoveTo(0,0);
	pDC->LineTo(0,rt.Height());
	
	pDC->SelectObject(&pen1);
	
	pDC->MoveTo(rt.Width()-1,rt.Height()-1);
	pDC->LineTo(rt.Width()-1,0);
	pDC->MoveTo(rt.Width()-1,rt.Height()-1);
	pDC->LineTo(0,rt.Height()-1);	 

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);

}

void CSwitchButton::SetValue(int nValue)
{	
	for(int i =0;i<m_nItemCount;i++)
	{
		if(m_nValues[i]==nValue)
		{
			m_nDefaultValue = nValue;
			m_nIndex =i;
		}
	}
	
	if(nValue>=m_nItemCount)
		return ;
	

	//m_nCurItemValue =m_bsequence? (m_nItemCount-nValue-1):nValue;
}

void CSwitchButton::OnMButtonDown(UINT nFlags, CPoint point)
{
	 
	CButton::OnMButtonDown(nFlags, point);
}

void CSwitchButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!m_bEnable )
		return;

	CRect rt;
	CRect rtPt;
	GetClientRect(rt);
 
	for(int i =0;i<m_nItemCount;i++)
	{ 
		if(i !=m_nIndex)
		{
			if(m_bHorizontal)
			{
				rtPt.left =rt.left+rt.Width()/m_nItemCount * i;
				rtPt.right =rt.left+rt.Width()/m_nItemCount * (i+1);
				rtPt.bottom =rt.bottom;
				rtPt.top = rt.top;
			}
			else 
			{
				rtPt.left =rt.left;
				rtPt.right = rt.right;
				rtPt.top = rt.top+rt.Height()/m_nItemCount*i;
				rtPt.bottom = rt.top +rt.Height()/m_nItemCount*(i+1);
			}
			if(rtPt.PtInRect(point))
			{
				m_nIndex =i;
				Invalidate();
				break;
			}
		}
	}
	CButton::OnLButtonUp(nFlags, point);
}
void CSwitchButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	CButton::OnLButtonDown(nFlags, point);
}
