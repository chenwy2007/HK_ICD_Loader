// XTextStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "XTextStatic.h"


// XTextStatic

XTextStatic::XTextStatic()
{	
	 m_nFontSize =20;
	 m_sFontName="宋体";
	m_bBackColor =0;
	m_backColor=RGB(128,128,128);
	m_FontColor =RGB(255,255,255);	 	
	m_lineColor=RGB(255,255,255);	 	
	m_nAlign = DT_LEFT;
	m_bMultiline =false;
	m_valueShowInfo.clear();
}

XTextStatic::~XTextStatic()
{
}
 
void XTextStatic::CreateCtrl(CString sCtrlName,CString sTitle,DWORD  nCtrlId,CRect rect,CWnd * pParentWnd,int nShow )
{
	XBaseWnd::CreateCtrl(sCtrlName,sTitle,nCtrlId,rect,pParentWnd,nShow);
		
	Create(sTitle,WS_VISIBLE|WS_CHILD|SS_NOTIFY,rect,pParentWnd,m_nCtrlId);	
}

void XTextStatic::SetFontInfo(int nFontSize,int nAlign,COLORREF fontColor,CString fontName)
{
	m_sFontName = fontName.GetBuffer();
	m_FontColor = fontColor;
	m_nFontSize = nFontSize;
	m_nAlign = nAlign;
}
void XTextStatic::SetColor(bool bBackColor,COLORREF backColor,COLORREF LineColor)
{
	m_bBackColor = bBackColor;
	m_backColor = backColor;
	m_lineColor =LineColor;
}
 void XTextStatic::OnPaintEx(CDC *pDC,CRect rt) 
 { 
	 if(m_sTitle.IsEmpty())
		 GetWindowText(m_sTitle);
	 if(m_bBackColor)
	 {
		 CBrush brush(m_backColor);
		 CPen pen(PS_SOLID,0,m_lineColor);
		 CBrush *oldBrush = pDC->SelectObject(&brush);
		 CPen *oldPen = pDC->SelectObject(&pen);
		 pDC->Rectangle(rt);
		 pDC->SelectObject(oldPen);
		 pDC->SelectObject(oldBrush);
	 }
	 CFont font;
	 CFont *pFont;
	 font.CreateFont(m_nFontSize,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		 CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,m_sFontName.c_str());
	 
	 CFont * OldFont=pDC->SelectObject(&font);
	 pDC->SetTextColor(m_FontColor);
	 pDC->SetBkMode(TRANSPARENT);

	 CSize size = pDC->GetTextExtent(m_sTitle);
	 if(size.cx<rt.Width() )	  //|| !m_bMultiline
		pDC->DrawText(m_sTitle,rt,m_nAlign|DT_VCENTER|DT_SINGLELINE);
	 else
	 { 		 
		 int height = pDC->DrawText(m_sTitle,rt,m_nAlign | DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); 
		 rt.DeflateRect(0,(rt.Height() - height) / 2); 
		 pDC->DrawText(m_sTitle,rt, m_nAlign | DT_EDITCONTROL | DT_WORDBREAK|DT_VCENTER);
	 }
	 pDC->SelectObject(OldFont);
	 font.DeleteObject();
 }
 void XTextStatic::RefreshData()
 {
	 if(m_nSharedId==0)
		return;
	 char lpbuf[256];
	 int nReSize;
	 int nDataType;
	 #ifdef HK_STDSYS_SDK_API
	 if(HK_GetValue_To_String(m_nSharedId,256,lpbuf,&nReSize,&nDataType)==0)
	 {
		 if(nDataType==DATA_TYPE_INT)
		 {
			 int nV =atoi(lpbuf);
			 map<int ,string>::iterator it =m_valueShowInfo.find(nV);
			 if(it!=m_valueShowInfo.end())
				 m_sTitle = it->second.c_str() ;			 
			 else if(m_valueShowInfo.size()>0)
				 m_sTitle = m_sDefaultShowInfo.c_str();
			 else
				 m_sTitle.Format("%d",nV);
			
			 map<int,COLORREF>::iterator it_1=  m_valueShowBackColor.find(nV);;
			 if(it_1!=m_valueShowBackColor.end())
			 	  m_backColor = it_1->second;
			 
		 }
		 else
			 m_sTitle = lpbuf;

	 }

	 Invalidate();
	 #endif

 }