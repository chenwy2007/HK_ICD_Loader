#include "StdAfx.h"
#include "Wnd_Static_Rectangle.h"


Wnd_Static_Rectangle::Wnd_Static_Rectangle(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo = info;
	m_nSizeFont_Define =80;
	m_fScale = fScale;
	m_ValueColor[0].nValue=0;
	m_ValueColor[1].nValue=1;
	m_ValueColor[0].colorRef=RGB(112,146,19);
	m_ValueColor[1].colorRef=RGB(0,128,64);

}


Wnd_Static_Rectangle::~Wnd_Static_Rectangle(void)
{
}
BEGIN_MESSAGE_MAP(Wnd_Static_Rectangle, CBaseStatic)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void Wnd_Static_Rectangle::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(CAppData::GetInstance().m_bBuilder )	 
	{
		CAppData::GetInstance().SetCurSelectWnd(this);
	}

	__super::OnLButtonUp(nFlags, point);
	CAppData::GetInstance().SetCurTipWnd(this);
}

void Wnd_Static_Rectangle:: DrawInfoEx(CDC *pDC,CRect rt)
{
	if(!CAppData::GetInstance().m_bBuilder || !m_bSel )	 
		return;

	CPen pen(PS_DASHDOTDOT,0,RGB(255,0,0));

	CPen *oldPen =pDC->SelectObject(&pen);

	pDC->MoveTo(0,0);
	pDC->LineTo(0,rt.bottom-1);
	pDC->LineTo(rt.right-1,rt.bottom-1);
	pDC->LineTo(rt.right-1,0);
	pDC->LineTo(0,0);
	pDC->SelectObject(oldPen);

}
void Wnd_Static_Rectangle::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow )
{
	m_nCtrlId = nCtrlId;
	CRect rt;
	rt.left = m_ctrlInfo.nX * m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	Create(m_strTitle.c_str(), WS_VISIBLE|WS_CHILD|SS_NOTIFY,rt,pParentWnd,m_nCtrlId);

}
void Wnd_Static_Rectangle::MoveWnd()
{
	MoveWindow(m_ctrlInfo.nX *m_fScale,m_ctrlInfo.nY * m_fScale,m_ctrlInfo.nW * m_fScale,m_ctrlInfo.nH*m_fScale);

}
void Wnd_Static_Rectangle::DecodeXml()
{

	m_nType =1;
	m_strTitle = m_ctrlInfo.sTitle;

	CMarkup xml(m_ctrlInfo.sAttr.c_str());
	CString str;
 
	if(xml.FindElem("Attr"))
	{  
		str = xml.GetAttrib("FontColor");
		if(!str.IsEmpty())
			m_colorFont = atoi(str);
		str = xml.GetAttrib("FontSize");
		if(!str.IsEmpty())
		{
				m_nSizeFont_Define =atoi(str);
				m_nSizeFont = m_nSizeFont_Define*m_fScale;
		}
		int n=0;
		while(xml.FindChildElem("Shared") && n<2)
		{
			str = xml.GetChildAttrib("Value");
			if(str.IsEmpty())
				continue;
			m_ValueColor[n].nValue =atoi(str);
			str = xml.GetChildAttrib("Color");
			m_ValueColor[n].colorRef = atoi(str);
			n++;
		}
	}

 
	SetColor(m_ValueColor[0].colorRef);

}
void Wnd_Static_Rectangle::UpdateAttr(__RectangleAttr &attr)
{
	m_strTitle = attr.sTitle;
	m_ctrlInfo.nX = attr.nX;
	m_ctrlInfo.nY = attr.nY;
	m_ctrlInfo.sName =attr.sName;
	m_ctrlInfo.sTitle = attr.sTitle;
	m_ctrlInfo.nSharedId = attr.nSharedId;
	m_ctrlInfo.nW = attr.nW;
	m_ctrlInfo.nH = attr.nH;

	m_nSizeFont_Define =attr.nFontSize;
	m_nSizeFont = m_nSizeFont_Define*m_fScale;
	m_colorFont = attr.colorFont;

	m_colorLine =attr.colorLine;
	
	m_colorBack =attr.vs[0].colorRef;

	this->Invalidate();

}