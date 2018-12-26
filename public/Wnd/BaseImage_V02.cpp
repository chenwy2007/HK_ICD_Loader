// BaseImage_V02.cpp : 实现文件
//

#include "stdafx.h"

#include "BaseImage_V02.h"


// CBaseImage_V02

IMPLEMENT_DYNAMIC(CBaseImage_V02, CWnd)

CBaseImage_V02::CBaseImage_V02()
{
	m_bBackColor =FALSE;
	m_bBackBmp =false;

	m_ColorLine =RGB(255,255,255);
	m_ColorBack =RGB(28,28,28);

	m_sPicFileName ="";
	m_bitmap.m_hObject =NULL;
	
	m_sFontName="黑体";
	m_nFontSize =20;

	m_ColorFont =RGB(255,255,255);
	m_nbmpWidth =0;
	m_nbmpHigth =0;

	m_nEType = 0;
}

CBaseImage_V02::~CBaseImage_V02()
{
}


BEGIN_MESSAGE_MAP(CBaseImage_V02, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CBaseImage_V02::OnEraseBkgnd(CDC* pDC)
{ 
	return  true;//CWnd::OnEraseBkgnd(pDC);
}
void CBaseImage_V02::OnPaint()
{
	CPaintDC dc(this);
	
	CDC *pDC = &dc;
	CDC MemDC;
	CRect rt;
	GetClientRect(rt);

	MemDC.CreateCompatibleDC(pDC); 

	CBitmap BmpMem;	
	BmpMem.CreateCompatibleBitmap(pDC,rt.Width(),rt.Height());
	CBitmap* pOldBitmap= MemDC.SelectObject(&BmpMem); 
	 

	DrawInfo(&MemDC,rt);

	DrawText(&MemDC,rt);

	dc.BitBlt(0, 0, rt.Width(), rt.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap); 
	//释放资源   
	MemDC.DeleteDC();
	BmpMem.DeleteObject();

}

void CBaseImage_V02::OnLButtonUp(UINT nFlags, CPoint point)
{
	OnChildLButtonUp(nFlags,point);
}
void CBaseImage_V02::OnTimer(UINT_PTR nIDEvent)
{
	OnChildTimer(nIDEvent);
	CWnd::OnTimer(nIDEvent);
}
void CBaseImage_V02::SetDefulatPicFileName(string sPicFilePath)
{
	m_sDefaultPicFileName = sPicFilePath;
	SetPicFileName(sPicFilePath);
}
bool CBaseImage_V02::SetPicFileName(string sPicFilePath)
{	
	if(m_sPicFileName == sPicFilePath)
		return false;

	if(m_sDefaultPicFileName.empty())
		m_sDefaultPicFileName = sPicFilePath;
 
	m_sPicFileName = sPicFilePath;
	CreateBmp(true);

	if(GetSafeHwnd())
		Invalidate();

	return true;
}
void CBaseImage_V02::CreateBmp(bool bReCreate)
{
	if(!bReCreate && m_bitmap.m_hObject)
		return; 

	CString strPath;
	strPath.Format("%sres\\%d\\%s",GetExecFilePath(),CAppData::GetInstance().m_nDlgId,m_sPicFileName.c_str());

	HBITMAP	hBitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),strPath,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE); 
	if(hBitMap)
	{ 
		m_bitmap.DeleteObject();
		m_bitmap.Attach(hBitMap);
	} 
	else
	{
		strPath.Format("%sres\\Comm\\%s",GetExecFilePath(),m_sPicFileName.c_str());		
		HBITMAP	hBitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),strPath,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE); 
		if(hBitMap)
		{ 
			m_bitmap.DeleteObject();
			m_bitmap.Attach(hBitMap);	
		}
	} 
	if(m_nbmpWidth==0 || m_nbmpHigth ==0)
		GetBmpSize(strPath,m_nbmpWidth,m_nbmpHigth);

}
void CBaseImage_V02::DrawInfo(CDC *pDC,CRect rt)
{
	if(m_bBackColor)
	{
		CBrush brush(m_ColorBack);
		CPen pen(PS_SOLID,0,m_ColorLine );
		CBrush *oldBrush = pDC->SelectObject(&brush);
		CPen *oldPen = pDC->SelectObject(&pen);
		
		pDC->Rectangle(rt);

		pDC->MoveTo(0,0);
		pDC->LineTo(0,rt.bottom-1);
		pDC->LineTo(rt.right-1,rt.bottom-1);
		pDC->LineTo(rt.right-1,0);
		pDC->LineTo(0,0);
		pDC->SelectObject(oldPen);
 
		pDC->SelectObject(oldBrush);
	}
	 
	if(!m_bBackBmp)
	{ 
		OnChildDrawInfo(pDC,rt);
		return;
	}
	CreateBmp(FALSE);
	if(!m_bitmap.m_hObject)
	{ 
		CPen pen(PS_SOLID, 0, RGB(255,0,0));
		pDC->DrawText("未发现资源",rt,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		OnChildDrawInfo(pDC,rt);
		return ;
	} 
	 
	CDC MemDC;
	CDC DisplayDC;
	MemDC.CreateCompatibleDC(pDC); 
	DisplayDC.CreateCompatibleDC(pDC);  
	CBitmap* pOldBitmap= MemDC.SelectObject(&m_bitmap); 	

	if(m_nbmpHigth==0 || m_nbmpWidth==0)
	{
		m_nbmpWidth = rt.Width();
		m_nbmpHigth =rt.Height();
	}

	CBitmap BmpMem;	
	BmpMem.CreateCompatibleBitmap(pDC,m_nbmpWidth,m_nbmpHigth);	

	CBitmap*  pOldDrawbmp = DisplayDC.SelectObject(&BmpMem);

	DisplayDC.BitBlt(0,0, m_nbmpWidth,m_nbmpHigth ,&MemDC,0,0,SRCCOPY);	
	 
	pDC->StretchBlt(m_nBitX,m_nBitY,m_nbmpWidth,m_nbmpHigth ,&DisplayDC,0,0,m_nbmpWidth,m_nbmpHigth ,SRCCOPY);
	DisplayDC.SelectObject(pOldDrawbmp);
	DisplayDC.DeleteDC();
	MemDC.SelectObject(pOldBitmap); 
	//释放资源   
	MemDC.DeleteDC();
	BmpMem.DeleteObject();

	OnChildDrawInfo(pDC,rt);
 
}
void CBaseImage_V02::DrawText(CDC  * pDC,CRect rt)
{
	if(m_sTitle.empty()) 
		return;

	CFont font;
	CFont *pFont;
	font.CreateFont(m_nFontSize,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,m_sFontName.c_str());
	
	CFont * OldFont=pDC->SelectObject(&font);

	pDC->SetTextColor(m_ColorFont);
	pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(m_sTitle.c_str(),rt,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	font.DeleteObject();	  
}
void CBaseImage_V02::SetCurFontColor(COLORREF ref)
{
	m_ColorFont = ref;
}