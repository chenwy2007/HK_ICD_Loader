#include "StdAfx.h"
#include "SwitchStatic.h"


CSwitchStatic::CSwitchStatic(void)
{
	 
	m_colorMap[0] = RGB(0,128,64);
	m_colorMap[1] =RGB(0,64,0);
	
	m_nCurValue =0;
	m_CurCircleColor=RGB(0,128,64);

	m_nAlign  = 0;
	m_fontSize = 80;
	m_fontText.DeleteObject();
	m_fontText.CreatePointFont(m_fontSize,"宋体",NULL);

	 m_BackColor = RGB(205,220,239);
	 m_bBackColor =true;
	
	m_lineColor= RGB(255,255,255);
	
}

CSwitchStatic::~CSwitchStatic(void)
{
}

void CSwitchStatic::SetValueColor(int nIndex,COLORREF ref)
{
	m_colorMap[nIndex] = ref;
}

void CSwitchStatic::SetValueColor(map<int ,COLORREF>&ref)
{
	m_colorMap.swap(ref);
}

bool CSwitchStatic::SetCurValue(int nValue)
{
	map<int ,COLORREF>::iterator it = m_colorMap.find(nValue);
 	if(it==m_colorMap.end())
		return false;

	m_nCurValue = nValue;
	m_CurCircleColor = it->second;
	return true;
}

void CSwitchStatic::SetTitleInfo(CString sInfo, int nAlign , int nSize )
{
	m_strTitle = sInfo;
	m_nAlign  = nAlign;
	m_fontSize = nSize;
	m_fontText.DeleteObject();
	m_fontText.CreatePointFont(m_fontSize,"宋体",NULL);
}

BEGIN_MESSAGE_MAP(CSwitchStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CSwitchStatic::OnPaint()
{
	CPaintDC dc(this); 

	CDC *pDC = &dc;
	CRect rt;
	GetClientRect(rt);

	OnMyPaint(pDC,rt);	 
}
void CSwitchStatic::OnMyPaint(CDC * pDC ,CRect rt)
{
	//int nSavedDC = pDC->SaveDC();
	if(m_bBackColor)
	{
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC); 
		CBitmap BmpMem;	
		BmpMem.CreateCompatibleBitmap(pDC,rt.Width(),rt.Height());
		CBitmap* pOldBitmap= MemDC.SelectObject(&BmpMem); 

		DrawInfo(&MemDC,rt);

		pDC->BitBlt(0, 0, rt.Width(), rt.Height(), &MemDC, 0, 0, SRCCOPY);

		MemDC.SelectObject(pOldBitmap); 
		//释放资源   
		MemDC.DeleteDC();
		BmpMem.DeleteObject();
	}
	else 
	{
		DrawInfo(pDC,rt);
	}

//	pDC->RestoreDC( nSavedDC );
}
void CSwitchStatic::DrawInfo(CDC *pDC ,CRect rt)
{
	if(m_bBackColor)
	{
		CPen pen(PS_SOLID,0,m_BackColor);
		CBrush brush(m_BackColor);
		CPen * oldPen = pDC->SelectObject(&pen);
		pDC->SelectObject(&brush);
		pDC->Rectangle(rt);
	}

	CPen pen1(PS_SOLID,0,m_lineColor);
	CBrush brush1(m_CurCircleColor);

	pDC->SelectObject(&pen1);		
	pDC->SelectObject(&brush1);

	CRect rtCircle;	
	int nW = rt.Width();
	int nH = rt.Height();
	int nR=nW>nH ? nH:nW;
	
	rtCircle.left = rt.left;
	rtCircle.top = rt.top;	
	rtCircle.right = rtCircle.left + nR;
	rtCircle.bottom = rtCircle.top +nR;
	pDC->Ellipse(rtCircle);	
	
	
}