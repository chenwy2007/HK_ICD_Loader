#include "StdAfx.h"
#include "XWnd_SwitchStatic.h"

XWnd_SwitchStatic::XWnd_SwitchStatic(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo = info;
	m_fScale = fScale;
}

XWnd_SwitchStatic::XWnd_SwitchStatic(void)
{
}


XWnd_SwitchStatic::~XWnd_SwitchStatic(void)
{
}
 void XWnd_SwitchStatic::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow )
 {
	 m_nCtrlId = nCtrlId;
	 
	 CRect rt;
	 rt.left = m_ctrlInfo.nX*m_fScale;
	 rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	 rt.top = m_ctrlInfo.nY*m_fScale;
	 rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	BOOL b= Create(m_strTitle, WS_VISIBLE|WS_CHILD |SS_NOTIFY,rt,pParentWnd,m_nCtrlId);
 }

 BEGIN_MESSAGE_MAP(XWnd_SwitchStatic, CSwitchStatic)
	 ON_WM_PAINT()
	 ON_WM_LBUTTONUP()
 END_MESSAGE_MAP()

 void XWnd_SwitchStatic::OnPaint()
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
 void XWnd_SwitchStatic::SetSelect(bool bSel)
 {
	 m_bSel =bSel;
	 this->Invalidate();
 }
 void XWnd_SwitchStatic::DecodeXml()
 {
	 CMarkup xml(m_ctrlInfo.sAttr.c_str());
	 if(!xml.FindElem("Attr"))
		 return;
	 CString str;
	 str =xml.GetAttrib("Back");
	 m_bBackColor = atoi(str);

	 str  = xml.GetAttrib("BackColor");
	 if(!str.IsEmpty())
		 sscanf(str,"%d",&m_BackColor);

	 str =xml.GetAttrib("LineColor");
	 if(!str.IsEmpty())
		 sscanf(str,"%d",&m_lineColor);

	 int nDefault=-1;
	 str = xml.GetAttrib("Default");
	 if(!str.IsEmpty())
		 nDefault = atoi(str);

	 int nIndex;
	 COLORREF color;
	 while(xml.FindChildElem("Color"))
	 {
		 nIndex =-1;
		 str = xml.GetChildAttrib("Index");
		 if(str.IsEmpty())
			 continue;
		 nIndex =atoi(str);
		 str =xml.GetChildAttrib("Value");
		 if(str.IsEmpty())
			 continue;
		 sscanf(str,"%d",&color);		 
		 m_colorMap[nIndex] =color; 			 
	 }

	 if(nDefault!=-1)
		 SetCurValue(nDefault);
	 
 }
 void XWnd_SwitchStatic::OnLButtonUp(UINT nFlags, CPoint point)
 {
	 if(CAppData::GetInstance().m_bBuilder )	 
	 {
		 CAppData::GetInstance().SetCurSelectWnd(this);
	 }

	 CSwitchStatic::OnLButtonUp(nFlags, point);
	CAppData::GetInstance().SetCurTipWnd(this);
 }

 void XWnd_SwitchStatic::UpdateAttr(__SwitchStaticAttr &attr)
 {
	 m_ctrlInfo.nH = attr.nH;
	 m_ctrlInfo.nW = attr.nW;
	 m_ctrlInfo.nY = attr.nY;
	 m_ctrlInfo.nX = attr.nX;

	 m_ctrlInfo.sTitle = attr.sTitle;
	 m_ctrlInfo.sName = attr.sName;

	 m_ctrlInfo.nSharedId = attr.nSharedId;

	 m_BackColor = attr.backColor;
	 m_bBackColor = attr.bBackColor;
	 
	 m_lineColor = attr.lineColor;
	 
	 SetCurValue(attr.nDefaultValue);
	 m_colorMap.swap(attr.valuesMap);	 
 }
  void XWnd_SwitchStatic::MoveWnd()
  {
	  CRect rt;
	  rt.left = m_ctrlInfo.nX*m_fScale;
	  rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	  rt.top = m_ctrlInfo.nY*m_fScale;
	  rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;
	  MoveWindow(rt);
  }

  void XWnd_SwitchStatic::RefreshData()
  {
	  if(m_ctrlInfo.nSharedId<=0)
		  return ;
	  int nValue =0;
	  int bUpdateFlag;
	  DWORD dwUpdateTime;
	  if(g_XShared.ReadData_Int(m_ctrlInfo.nSharedId,nValue,bUpdateFlag,dwUpdateTime))
	  {
		  if(!bUpdateFlag)
			  return;		  
		  SetCurValue(nValue);
		  Invalidate();
	  }
  }