// Wnd_Line.cpp : 实现文件
//

#include "stdafx.h"
#include "Wnd_Line.h"

// Wnd_Line

IMPLEMENT_DYNAMIC(Wnd_Line, CStatic)

Wnd_Line::Wnd_Line(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo = info;
	m_fScale = fScale;

	m_nLineWidth =0;
	m_nLineType = PS_SOLID;

	m_nRealLineLength =10;
	m_nDottedLineLength=10;
}

Wnd_Line::~Wnd_Line()
{
}


BEGIN_MESSAGE_MAP(Wnd_Line, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


 
void Wnd_Line::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(CAppData::GetInstance().m_bBuilder )	 
	{
		CAppData::GetInstance().SetCurSelectWnd(this);
	}

	__super::OnLButtonUp(nFlags, point);
}
void Wnd_Line::OnPaint()
{
	CPaintDC dc(this);  
	CDC *pDC = &dc; 
	CRect rt;
	GetClientRect(rt);

	int nSaveDC = dc.SaveDC();

	DrawInfo(pDC,rt);

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
void Wnd_Line::DrawInfo(CDC * pDC,CRect rt)
{
	CPen pen(PS_SOLID,m_nLineWidth,m_colorLine);
	CPen *oldPen=pDC->SelectObject(&pen);
	int nY = rt.top + rt.Height()/2 - m_nLineWidth/2;
	int nX = rt.left;
	if(m_nDottedLineLength==0)
	{
		pDC->MoveTo(nX,nY);
		pDC->LineTo(rt.right,nY);
	}
	else
	{ 
		int nLineLength =m_nRealLineLength;
		while(nX<rt.right)
		{
			pDC->MoveTo(nX,nY);
			nX+=m_nRealLineLength;

			pDC->LineTo(nX,nY);
			nX+=m_nDottedLineLength;
		}
	}
	
	pDC->SelectObject(oldPen);
}

void Wnd_Line::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow  )
{
	m_nCtrlId = nCtrlId;

	CRect rt;
	rt.left = m_ctrlInfo.nX*m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	BOOL b= Create("", WS_VISIBLE|WS_CHILD |SS_NOTIFY,rt,pParentWnd,m_nCtrlId);

}
void Wnd_Line::DecodeXml()
 {
	 CMarkup xml(m_ctrlInfo.sAttr.c_str());
	 CString str;

	 if(xml.FindElem("Attr"))
	 {		
		// m_nLineType = atoi(xml.GetAttrib("LineType"));
		 m_nLineWidth = atoi(xml.GetAttrib("LineWidth"));

		 m_nRealLineLength =atoi(xml.GetAttrib("RealLineLength"));
		 m_nDottedLineLength =atoi(xml.GetAttrib("DottedLineLength"));

		 m_colorLine =atoi(xml.GetAttrib("colorLine"));
		 m_colorLine = m_colorLine==0?RGB(255,255,255):m_colorLine;
	 }	
 }

 void Wnd_Line::MoveWnd()
 { 
	 CRect rt;
	 rt.left = m_ctrlInfo.nX*m_fScale;
	 rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	 rt.top = m_ctrlInfo.nY*m_fScale;
	 rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	 MoveWindow(rt);
	 Invalidate();
 }
 void Wnd_Line::UpdateAttr(__LineAttr &attr)
 {

	 m_ctrlInfo.sName = attr.baseAttr.sName;
	 m_ctrlInfo.nIndex = attr.baseAttr.nCtrlIndex;

	 m_ctrlInfo.nX = attr.baseAttr.nX;
	 m_ctrlInfo.nY = attr.baseAttr.nY;
	 m_ctrlInfo.nW = attr.baseAttr.nW;
	 m_ctrlInfo.nH = attr.baseAttr.nH;

	 m_colorLine = attr.colorLine;
	 m_nLineWidth = attr.nLineWidth;

	 m_nRealLineLength = attr.nRealLineLength;
	 m_nDottedLineLength = attr.nDottedLineLength;

 }

 void Wnd_Line::ReCreate()
 {
	 CManageDb_Sqllite::GetInstance().Query_CtrlInfo(m_ctrlInfo,m_ctrlInfo.nId);
	 DecodeXml();
 }