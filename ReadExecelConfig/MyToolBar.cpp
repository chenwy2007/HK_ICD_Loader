// MyToolBar.cpp : 实现文件
//

#include "stdafx.h"
//#include "tset_toolbar.h"
#include "MyToolBar.h"


// CMyToolBar

IMPLEMENT_DYNAMIC(CMyToolBar, CToolBar)

CMyToolBar::CMyToolBar()
{

}

CMyToolBar::~CMyToolBar()
{
}


BEGIN_MESSAGE_MAP(CMyToolBar, CToolBar)
	ON_WM_ERASEBKGND()
	//ON_WM_PAINT()
END_MESSAGE_MAP()



// CMyToolBar 消息处理程序



BOOL CMyToolBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect,RGB(205,205,0));

	return TRUE;

	//return CToolBar::OnEraseBkgnd(pDC);
}

//void CMyToolBar::OnPaint()
//{
//	//CPaintDC dc(this); // device context for painting
//	// TODO: 在此处添加消息处理程序代码
//	// 不为绘图消息调用 CDialogEx::OnPaint()
//	
//	//
//	// 给工具栏添加背景
//	//
//	//CPaintDC dc(this);
//	//CRect rc;
//	//GetClientRect(&rc);
//	//CDC dcMem;
//	//dcMem.CreateCompatibleDC(&dc);
//	//CBitmap bmpBackground;
//	//bmpBackground.LoadBitmap(IDB_BMP_BK);
//
//	//BITMAP bitmap;
//	//bmpBackground.GetBitmap(&bitmap);
//	//CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
//	//dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
//
//	CRect   rc;
//	GetClientRect( &rc );// 获取客户区
//	CPaintDC dc(this);
//	dc.FillSolidRect(&rc, RGB(0,160,0));   // 填充客户区颜色
//	__super::OnPaint();           
//}
