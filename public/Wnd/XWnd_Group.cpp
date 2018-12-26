#include "StdAfx.h"
#include "XWnd_Group.h"

XWnd_Group::XWnd_Group(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo = info;
	m_fScale =fScale;
	m_nfontSize_define =20;
}
XWnd_Group::XWnd_Group(void)
{
}


XWnd_Group::~XWnd_Group(void)
{
}
BEGIN_MESSAGE_MAP(XWnd_Group, CGroupStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

 void XWnd_Group::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow  )
{
	m_nCtrlId = nCtrlId;
	CRect rt;
	rt.left = m_ctrlInfo.nX*m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	Create("", WS_CHILD | WS_VISIBLE   ,rt,pParentWnd,m_nCtrlId);

}
void XWnd_Group::DecodeXml()
{
	m_strTitle = m_ctrlInfo.sTitle.c_str();
	
	CMarkup xml(m_ctrlInfo.sAttr.c_str());
	CString str;

	if(xml.FindElem("Attr"))
	{		
		m_nfontSize_define = atoi(xml.GetAttrib("FontSize"));
		m_nfontSize_define = m_nfontSize_define==0 ?20:m_nfontSize_define;
		
		m_colorFont =atoi(xml.GetAttrib("FontColor"));		
		m_colorFont = m_colorFont==0?RGB(255,255,255):m_colorFont;

		m_sFontName =atoi(xml.GetAttrib("FontName"));
		bool b = m_sFontName.empty();
		m_sFontName= m_sFontName.empty()?m_sFontName:"ºÚÌå";
	}

	m_nFontSize = m_nfontSize_define *m_fScale;
}

void XWnd_Group::SetSelect(bool bSel)
{
	m_bSel =bSel;
	this->Invalidate();
}
void XWnd_Group::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(CAppData::GetInstance().m_bBuilder )	 
	{
		CAppData::GetInstance().SetCurSelectWnd(this);
	}

	__super::OnLButtonUp(nFlags, point);
}
void XWnd_Group::MoveWnd()
{
	CRect rt;
	rt.left = m_ctrlInfo.nX*m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;
	MoveWindow(rt);
	Invalidate();
}
void XWnd_Group::OnPaint()
{
	CPaintDC dc(this);  
	int nSaveDC = dc.SaveDC();

	CDC *pDC = &dc;
	CRect rt;
	GetClientRect(rt);
	OnMyPaint(pDC,rt);	 

	if(!CAppData::GetInstance().m_bBuilder  || !m_bSel)	 
		return;

	CPen pen(PS_DASHDOTDOT,0,RGB(255,0,0)); 
	pDC->SelectObject(&pen);

	pDC->MoveTo(0,0);
	pDC->LineTo(0,rt.bottom-1);
	pDC->LineTo(rt.right-1,rt.bottom-1);
	pDC->LineTo(rt.right-1,0);
	pDC->LineTo(0,0);

	dc.RestoreDC(nSaveDC);
}

void XWnd_Group::UpdateAttr(__GroupAttr& attr)
{
	m_ctrlInfo.nX = attr.nX;
	m_ctrlInfo.nY =attr.nY;
	m_ctrlInfo.nW = attr.nW;
	m_ctrlInfo.nH = attr.nH;
	m_ctrlInfo.sTitle = attr.sTitle;
	m_strTitle = attr.sTitle.c_str();
	m_colorFont = attr.fontColor;
	m_nfontSize_define =attr.nFontSize;
	m_nFontSize =m_nfontSize_define*m_fScale;

}