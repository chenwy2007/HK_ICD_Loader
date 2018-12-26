// BaseImage.cpp : 实现文件
//

#include "stdafx.h"
#include "BaseImage.h"


// CBaseImage

IMPLEMENT_DYNAMIC(CBaseImage, CStatic)

CBaseImage::CBaseImage()
{
	m_colorFont = RGB(255,255,255);
	m_nSizeFont = 100;	 
	m_bitmap.m_hObject =NULL;
	m_strFontName ="宋体";
	m_PtText.x =-1;
	m_PtText.y =-1;
}

CBaseImage::~CBaseImage()
{
	if(m_bitmap.m_hObject)
		m_bitmap.DeleteObject();
}


BEGIN_MESSAGE_MAP(CBaseImage, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


BOOL CBaseImage::OnEraseBkgnd(CDC* pDC)
{

	return  true;//CStatic::OnEraseBkgnd(pDC);
}
void CBaseImage::SetPicFileName(string sPicFilePath)
{	
	if(m_sPicFileName == sPicFilePath)
		return ;

	CString strPath ;
	m_sPicFileName = sPicFilePath;

	strPath.Format("%sres\\%d\\%s",GetExecFilePath(),CAppData::GetInstance().m_nDlgId,m_sPicFileName.c_str());		
	HBITMAP	hBitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),strPath,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE); 
	if(hBitMap)
	{ 
		m_bitmap.DeleteObject();
		m_bitmap.Attach(hBitMap);	
	}
	else
	{
		strPath.Format("%sres\\0\\%s",GetExecFilePath(),m_sPicFileName.c_str());		
		HBITMAP	hBitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),strPath,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE); 
		if(hBitMap)
		{ 
			m_bitmap.DeleteObject();
			m_bitmap.Attach(hBitMap);	
		}
	}

	if(GetSafeHwnd())
		Invalidate();	 
}
void CBaseImage::OnPaint()
{
	CPaintDC dc(this);
	
	CDC *pDC = &dc;
	int   nSaveDC=pDC-> SaveDC(); 
	CRect rt;
	GetClientRect(rt);

	DrawInfo(pDC,rt);	 
	pDC-> RestoreDC(nSaveDC);
}
void CBaseImage::DrawTxt(CDC * pDC,CRect rt)
{
	if(m_strTitle.empty()) 
		return ;	 

	CFont font;
	font.CreatePointFont(m_nSizeFont,m_strFontName,NULL);
	CFont * OldFont=pDC->SelectObject(&font);

	CFont*   hFont   =   GetFont(); 		
	CFont*   hOldFont  =NULL;
	
	int   nMode   =  pDC-> SetBkMode(TRANSPARENT); 
	pDC-> SetTextColor(m_colorFont); 

	if(m_PtText.x!=-1)
		pDC->TextOut(m_PtText.x,m_PtText.y,m_strTitle.c_str());
	else
		pDC->DrawText(m_strTitle.c_str(),rt,DT_VCENTER|DT_CENTER|DT_SINGLELINE);

	pDC-> SelectObject(hOldFont); 
	pDC-> SetBkMode(nMode); 	

	pDC->SelectObject(OldFont);
	font.DeleteObject();
}

void CBaseImage::DrawInfo(CDC *pDC,CRect rt)
{
	if(!m_bitmap.m_hObject)
	{
		CPen pen(PS_SOLID, 0, RGB(255,0,0));
		pDC->DrawText("未发现资源",rt,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		DrawInfoEx(pDC,rt);
		return ;
	}
	
	CDC MemDC;
	CDC DisplayDC;
	MemDC.CreateCompatibleDC(pDC); 
	DisplayDC.CreateCompatibleDC(pDC);  
	CBitmap* pOldBitmap= MemDC.SelectObject(&m_bitmap); 	

	CBitmap BmpMem;	
	BmpMem.CreateCompatibleBitmap(pDC,rt.Width(),rt.Height());	

	CBitmap*  pOldDrawbmp = DisplayDC.SelectObject(&BmpMem);

	DisplayDC.BitBlt(0,0, rt.Width(),rt.Height() ,&MemDC,0,0,SRCCOPY);	

	DrawTxt(pDC,rt);
	DrawInfoEx(pDC,rt);

	pDC->StretchBlt(0,0,rt.Width(),rt.Height() ,&DisplayDC,0,0,rt.Width() ,rt.Height() ,SRCCOPY);
	DisplayDC.SelectObject(pOldDrawbmp);
	DisplayDC.DeleteDC();
	MemDC.SelectObject(pOldBitmap); 
	//释放资源   
	MemDC.DeleteDC();
	BmpMem.DeleteObject();	 
}


void CBaseImage::OnLButtonUp(UINT nFlags, CPoint point)
{
	 OnMyLButtonUp(nFlags,point);
}