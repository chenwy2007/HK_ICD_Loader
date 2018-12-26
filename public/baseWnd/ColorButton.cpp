// ColorButton.cpp : 实现文件
//

#include "stdafx.h"
 
#include "ColorButton.h"


// CColorButton

IMPLEMENT_DYNAMIC(CColorButton, CButton)

CColorButton::CColorButton()
{
	m_lineColor =RGB(255,255,255);

	m_Style = 0;                //按钮形状风格
	b_InRect = false;           //鼠标进入标志
	m_strText = _T("");         //按钮文字（使用默认文字）
	m_ForeColor = RGB(0,0,0);             //文字颜色（黑色）
	m_BackColor = RGB(243,243,243);       //背景色（灰白色）
	m_LockForeColor = GetSysColor(COLOR_GRAYTEXT);     //锁定按钮的文字颜色
	m_nFontSize =100;
	 
}

CColorButton::~CColorButton()
{ 
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
 
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()
//
void CColorButton::PreSubclassWindow()
{
	ModifyStyle( 0, BS_OWNERDRAW );         //设置按钮属性为自画式

	CButton::PreSubclassWindow();
} 
// 
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	m_ButRect = lpDrawItemStruct->rcItem;     //获取按钮尺寸

	if( m_strText.IsEmpty() )
		GetWindowText( m_strText );           //获取按钮文本

	int nSavedDC = pDC->SaveDC();
	VERIFY( pDC );
	DrawButton( pDC );                 //绘制按钮
	pDC->RestoreDC( nSavedDC );
} 
void CColorButton::DrawButton(CDC *pDC)
{
	
	CFont font;
	font.CreatePointFont(m_nFontSize,"宋体");
	CFont * oldFont = pDC->SelectObject(&font);

	//调整状态
	if( m_Style==3 ) m_Style = 0;
	if( GetStyle() & WS_DISABLED )
		m_Style = 3;     //禁止状态
	//根据状态调整边框颜色和文字颜色
	COLORREF bColor, fColor;     //bColor为边框颜色，fColor为文字颜色
	switch( m_Style )
	{
	case 0: bColor = m_lineColor; fColor = m_ForeColor; break;   //正常按钮
	case 1: bColor = m_lineColor; fColor = m_ForeColor; break;   //鼠标进入时按钮
	case 2: bColor = RGB(192,192,192); fColor = m_ForeColor; break;   //按下的按钮
	case 3: bColor = m_BackColor; fColor = m_LockForeColor; break;    //锁定的按钮
	}
	
	CBrush Brush(m_BackColor);	
	
	CPen Pen(PS_SOLID, 0, bColor);
	CBrush * oldBrush = pDC->SelectObject( &Brush );
	CPen * oldPen =pDC->SelectObject(&Pen);

 
 	pDC->Rectangle(m_ButRect);
 
	pDC->SetTextColor( fColor );         //画文字
	pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText( m_strText, &m_ButRect, DT_SINGLELINE | DT_CENTER
		| DT_VCENTER | DT_END_ELLIPSIS);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldFont);

	font.DeleteObject();
 
} 
void CColorButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Style = 2;
	Invalidate();         //重绘按钮

	CButton::OnLButtonDown(nFlags, point);
 } 
void CColorButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Style = 0;
	Invalidate();         //重绘按钮

	CButton::OnLButtonUp(nFlags, point);
} 
void CColorButton::SetText(CString str)
{
	m_strText =  str;
	//SetWindowText(str);
}
void CColorButton::SetForeColor(COLORREF color)
{
	m_ForeColor = color;
	if(GetSafeHwnd())
	Invalidate();
} 

//设置背景颜色
void CColorButton::SetBkColor(COLORREF color)
{
	m_BackColor = color;
	if(GetSafeHwnd())
		Invalidate();
} 
void CColorButton::SetLineColor(COLORREF color)
{
	m_lineColor =color;
}

//设置字体(字体高度、字体名)
void CColorButton::SetTextFont(int FontHight,LPCTSTR FontName)
{
	m_nFontSize = FontHight;

}  
