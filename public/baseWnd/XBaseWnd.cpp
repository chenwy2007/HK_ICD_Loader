// XBaseWnd.cpp : 实现文件
//

#include "stdafx.h"

#include "XBaseWnd.h"


// XBaseWnd

IMPLEMENT_DYNAMIC(XBaseWnd, CStatic)

XBaseWnd::XBaseWnd()
{
	m_nSharedId =0;
}

XBaseWnd::~XBaseWnd()
{
}


BEGIN_MESSAGE_MAP(XBaseWnd, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()
 

void XBaseWnd::CreateCtrl(CString sCtrlName,CString sTitle,DWORD  nCtrlId,CRect rect,CWnd * pParentWnd,int nShow)
{
	m_sCtrlName = sCtrlName;
	m_sTitle = sTitle;
	
	m_nCtrlId = nCtrlId;
	m_pParentWnd =pParentWnd;
	
	m_Rect = rect;
}

void XBaseWnd::OnPaint()
{
	CPaintDC dc(this);
	int   nSaveDC=dc.SaveDC(); 

	CRect rt;
	GetClientRect(rt);
	OnPaintEx(&dc,rt);
	dc.RestoreDC(nSaveDC);
}

void XBaseWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	OnLButtonUpEx(nFlags,point);

}
void XBaseWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	OnLButtonDownEx(nFlags,point);
}