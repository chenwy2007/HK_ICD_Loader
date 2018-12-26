#include "StdAfx.h"
#include "XWnd_Button.h"

XWnd_Button::XWnd_Button(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo = info;
	m_fScale =fScale;
	m_bExit =false;
}

XWnd_Button::XWnd_Button(void)
{
	m_bExit =false;
}


XWnd_Button::~XWnd_Button(void)
{
}
void XWnd_Button::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow)
{
	m_nCtrlId = nCtrlId;
	CRect rt;
	rt.left = m_ctrlInfo.nX *m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	Create(m_ctrlInfo.sTitle.c_str(),WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,rt,pParentWnd,nCtrlId);
}
void XWnd_Button::DecodeXml()
{
	SetText(m_ctrlInfo.sTitle.c_str());
	CMarkup xml(m_ctrlInfo.sAttr.c_str());
	vector<__BT_ACT>().swap(m_actVec);
	
	if(xml.FindElem("Attr"))
	{
		m_BackColor = atoi(xml.GetAttrib("BackColor"));
		if(m_BackColor==0)
			m_BackColor =RGB(243,243,243);
		
		m_lineColor = atoi(xml.GetAttrib("LineColor"));
		if(m_lineColor==0)
			m_lineColor=RGB(255,255,255);
		
		m_ForeColor =atoi(xml.GetAttrib("FontSize"));

		m_nFontSize =atoi(xml.GetAttrib("FontSize"));
		if(m_nFontSize==0)
			m_nFontSize =100;
		
		while(xml.FindChildElem("Act"))
		{	
			__BT_ACT act;
			act.sName="";
			act.sName = xml.GetChildAttrib("Name");
			if(act.sName.empty())
				continue;
			act.sParam = xml.GetChildAttrib("Param");
			m_actVec.push_back(act);
		}
		 
	}
}
void XWnd_Button::SetSelect(bool bSel)
{
	CBaseElemCtrl::SetSelect(bSel);
	this->Invalidate();
}
void XWnd_Button::MoveWnd()
{ 
	int nW = m_ctrlInfo.nW*m_fScale;
	int nH = m_ctrlInfo.nH*m_fScale;
	int x = m_ctrlInfo.nX*m_fScale;
	int y = m_ctrlInfo.nY*m_fScale;

	MoveWindow(x,y, nW, nH );
	this->Invalidate();
}
 
BEGIN_MESSAGE_MAP(XWnd_Button, CColorButton)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void XWnd_Button::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags, point);
}
void XWnd_Button::OnLButtonUp(UINT nFlags, CPoint point)
{ 
	if(CAppData::GetInstance().m_bBuilder)
	{
		CAppData::GetInstance().SetCurSelectWnd(this);
		SetSelect(true);		
	}
	else 
	{	
		for(int i =0,end = m_actVec.size();i<end;i++)
		{
			if(m_actVec[i].sName ==ACT_Show_dlg)
			{ 
				int nId = atoi(m_actVec[i].sParam.c_str());
				if(nId!=0)
				{
					GetParent()->ShowWindow(SW_HIDE);
					XWnd_Dlg dlg1(IDD_DIALOG_BASE);

					CManageDb_Sqllite::GetInstance().Query_CtrlInfo(dlg1.m_ctrlInfo,nId);

					dlg1.DecodeXml();
					dlg1.DoModal();
				}
			}
			else if(m_actVec[i].sName==ACT_Exit_Sys)
			{
				exit(0);
			}
				
		
		}
		GetParent()->ShowWindow(SW_SHOW);
	}
	 
	__super::OnLButtonUp(nFlags, point);
}


void XWnd_Button::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	__super::DrawItem(lpDrawItemStruct);

	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	//构件模式
	if(!CAppData::GetInstance().m_bBuilder)
		return ;

	if(!m_bSel)
		return ;
	CRect rt = lpDrawItemStruct->rcItem;
 	
	int nSavedDC = pDC->SaveDC();
	VERIFY( pDC );
	
	CPen pen(PS_DASHDOTDOT,0,RGB(255,0,0));
	CPen *oldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(0,0);
	pDC->LineTo(0,rt.bottom-1);
	pDC->LineTo(rt.right-1,rt.bottom-1);
	pDC->LineTo(rt.right-1,0);
	pDC->LineTo(0,0);
	
	pDC->RestoreDC( nSavedDC );
} 

void XWnd_Button::UpdateAttr(__ButtonAttr & attr)
{
	m_ctrlInfo.nH = attr.nH;
	m_ctrlInfo.nW = attr.nW;
	m_ctrlInfo.nY = attr.nY;
	m_ctrlInfo.nX = attr.nX;
	
	m_ctrlInfo.sTitle = attr.sTitle;

	m_strText = attr.sTitle.c_str();	
	m_ctrlInfo.sName = attr.sTitle;
	m_BackColor = attr.backColor;	
	m_lineColor = attr.lineColor;
	m_ForeColor =attr.fontColor;
	m_nFontSize =attr.nFontSize;
	 
}
 