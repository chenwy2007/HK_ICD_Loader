// MyToolBar.cpp : ʵ���ļ�
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



// CMyToolBar ��Ϣ�������



BOOL CMyToolBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect,RGB(205,205,0));

	return TRUE;

	//return CToolBar::OnEraseBkgnd(pDC);
}

//void CMyToolBar::OnPaint()
//{
//	//CPaintDC dc(this); // device context for painting
//	// TODO: �ڴ˴������Ϣ����������
//	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
//	
//	//
//	// ����������ӱ���
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
//	GetClientRect( &rc );// ��ȡ�ͻ���
//	CPaintDC dc(this);
//	dc.FillSolidRect(&rc, RGB(0,160,0));   // ���ͻ�����ɫ
//	__super::OnPaint();           
//}
