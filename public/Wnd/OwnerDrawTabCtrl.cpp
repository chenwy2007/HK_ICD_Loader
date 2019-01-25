// OwnerDrawTabCtrl.cpp : 实现文件
//

#include "stdafx.h"

#include "OwnerDrawTabCtrl.h"


// COwnerDrawTabCtrl

IMPLEMENT_DYNAMIC(COwnerDrawTabCtrl, CTabCtrl)

COwnerDrawTabCtrl::COwnerDrawTabCtrl()
{
	m_selectTabColor =RGB(0,162,232);
	m_unSelectTabColor =RGB( 28,28,28);
	m_LineColor = RGB(255,255,255);
	m_backColor = RGB(28,28,28);
	m_colorText = RGB(255,255,255);
	
	m_nTabWidth =150;
	m_nTabHight =30;
	m_sFontName ="黑体";
	m_nFontSize =18;

}

COwnerDrawTabCtrl::~COwnerDrawTabCtrl()
{

}


BEGIN_MESSAGE_MAP(COwnerDrawTabCtrl, CTabCtrl)
	 
END_MESSAGE_MAP()



// COwnerDrawTabCtrl 消息处理程序

//
//

void COwnerDrawTabCtrl::DrawItem(LPDRAWITEMSTRUCT  lpDrawItemStruct)
{
	if(lpDrawItemStruct->CtlType != ODT_TAB)
		return ;	
	CDC * pDC = CDC::FromHandle( lpDrawItemStruct->hDC );	
	CRect rtClient;
	GetClientRect(rtClient);

	DrawInfo(pDC,rtClient);
	DrawInfoEx(pDC,rtClient);
 
}
 
void COwnerDrawTabCtrl::DrawInfo(CDC * pDC ,CRect rt)
{ 
	CFont font;
	CFont *pFont;
	font.CreateFont(m_nFontSize,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,m_sFontName.c_str());
	CFont *pOldFont=pDC->SelectObject(&font);
	  

	CPen pen(PS_SOLID,0,m_LineColor);
	CBrush  brushBack(m_backColor);
	CBrush brushSelect(m_selectTabColor);
	CBrush brushUnSelect(m_unSelectTabColor);
	
	CPen  *oldLine = pDC->SelectObject(&pen);
	CBrush *oldBrush = pDC->SelectObject(&brushBack);		 
	//背景色
	pDC->SetTextColor(m_colorText);
	pDC->SetBkMode(TRANSPARENT);
	
 	pDC->Rectangle(rt);
	int nleft =1;
	int nTop =1;
	 
	for(int i =0,end = GetItemCount();i<end;i++)
	{ 
		// 获取选项卡文字
		TCHAR tzText[24] = { 0 };
		TCITEM tci;
		tci.mask = TCIF_TEXT;
		tci.pszText = tzText;
		tci.cchTextMax = sizeof(tzText);
		GetItem( i,&tci );
		
		CRect rcItem;		 
		
		rcItem.left=nleft +i*(m_nTabWidth);
		rcItem.right=nleft +(i+1)*(m_nTabWidth)+1;
		rcItem.top = nTop;
		rcItem.bottom = m_nTabHight+3;

		CRect rcText( rcItem ); 
		 
		if( i == GetCurSel() )
		{			 
			//CPen pen1(PS_SOLID,0,m_selectTabColor);
			//pDC->SelectObject(&pen1);
			pDC->SelectObject(&brushSelect);
			 
			pDC->Rectangle(rcItem); 
	 
		}
		else
		{
			 
			//CPen pen1(PS_SOLID,0,m_unSelectTabColor);
			pDC->SelectObject(&pen);
			pDC->SelectObject(&brushUnSelect);
			pDC->Rectangle(rcItem);  
		}
	 
		 pDC->DrawText( tzText,rcText,DT_VCENTER | DT_CENTER | DT_SINGLELINE );//*/
	 
		}
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldLine);
	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}

void COwnerDrawTabCtrl::SetItemTabSize(int nW,int nH)
{
	m_nTabWidth =nW;
	m_nTabHight =nH;
	SetItemSize(CSize(nW,nH));
	SetMinTabWidth(nW);
}

void COwnerDrawTabCtrl::SetFontInfo(int nSize,COLORREF colorFont,CString sFontName)
{
	m_nFontSize = nSize;
	m_colorText = colorFont;
	m_sFontName =sFontName.GetBuffer();
}
void COwnerDrawTabCtrl::SetItemTabColor(COLORREF  selectTabColor,COLORREF  unSelectTabColor)
{
	m_selectTabColor =selectTabColor;
	m_unSelectTabColor = unSelectTabColor;
}