// ColorButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
 
#include "ColorButton.h"


// CColorButton

IMPLEMENT_DYNAMIC(CColorButton, CButton)

CColorButton::CColorButton()
{
	m_lineColor =RGB(255,255,255);

	m_Style = 0;                //��ť��״���
	b_InRect = false;           //�������־
	m_strText = _T("");         //��ť���֣�ʹ��Ĭ�����֣�
	m_ForeColor = RGB(0,0,0);             //������ɫ����ɫ��
	m_BackColor = RGB(243,243,243);       //����ɫ���Ұ�ɫ��
	m_LockForeColor = GetSysColor(COLOR_GRAYTEXT);     //������ť��������ɫ
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
	ModifyStyle( 0, BS_OWNERDRAW );         //���ð�ť����Ϊ�Ի�ʽ

	CButton::PreSubclassWindow();
} 
// 
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	m_ButRect = lpDrawItemStruct->rcItem;     //��ȡ��ť�ߴ�

	if( m_strText.IsEmpty() )
		GetWindowText( m_strText );           //��ȡ��ť�ı�

	int nSavedDC = pDC->SaveDC();
	VERIFY( pDC );
	DrawButton( pDC );                 //���ư�ť
	pDC->RestoreDC( nSavedDC );
} 
void CColorButton::DrawButton(CDC *pDC)
{
	
	CFont font;
	font.CreatePointFont(m_nFontSize,"����");
	CFont * oldFont = pDC->SelectObject(&font);

	//����״̬
	if( m_Style==3 ) m_Style = 0;
	if( GetStyle() & WS_DISABLED )
		m_Style = 3;     //��ֹ״̬
	//����״̬�����߿���ɫ��������ɫ
	COLORREF bColor, fColor;     //bColorΪ�߿���ɫ��fColorΪ������ɫ
	switch( m_Style )
	{
	case 0: bColor = m_lineColor; fColor = m_ForeColor; break;   //������ť
	case 1: bColor = m_lineColor; fColor = m_ForeColor; break;   //������ʱ��ť
	case 2: bColor = RGB(192,192,192); fColor = m_ForeColor; break;   //���µİ�ť
	case 3: bColor = m_BackColor; fColor = m_LockForeColor; break;    //�����İ�ť
	}
	
	CBrush Brush(m_BackColor);	
	
	CPen Pen(PS_SOLID, 0, bColor);
	CBrush * oldBrush = pDC->SelectObject( &Brush );
	CPen * oldPen =pDC->SelectObject(&Pen);

 
 	pDC->Rectangle(m_ButRect);
 
	pDC->SetTextColor( fColor );         //������
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
	Invalidate();         //�ػ水ť

	CButton::OnLButtonDown(nFlags, point);
 } 
void CColorButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Style = 0;
	Invalidate();         //�ػ水ť

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

//���ñ�����ɫ
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

//��������(����߶ȡ�������)
void CColorButton::SetTextFont(int FontHight,LPCTSTR FontName)
{
	m_nFontSize = FontHight;

}  
