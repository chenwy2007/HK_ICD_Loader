// BaseImage_Rotate_V02.cpp : 实现文件
//

#include "stdafx.h"

#include "BaseImage_Rotate_V02.h"


// CBaseImage_Rotate_V02

IMPLEMENT_DYNAMIC(CBaseImage_Rotate_V02, CWnd)

CBaseImage_Rotate_V02::CBaseImage_Rotate_V02()
{
	m_bBackColor =FALSE;	

	m_ColorLine =RGB(255,255,255);
	m_ColorBack =RGB(28,28,28);

	m_sPicFileName ="";	

	m_sFontName="黑体";
	m_nFontSize =20;

	m_ColorFont =RGB(255,255,255);

	m_fRotate =0;

	m_lpImage =NULL;

}

CBaseImage_Rotate_V02::~CBaseImage_Rotate_V02()
{
	delete_safe(m_lpImage);
}


BEGIN_MESSAGE_MAP(CBaseImage_Rotate_V02, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CBaseImage_Rotate_V02::OnEraseBkgnd(CDC* pDC)
{ 
	return  true;
}
void CBaseImage_Rotate_V02::OnPaint()
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

	dc.BitBlt(0, 0, rt.Width(), rt.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap); 
	//释放资源   
	MemDC.DeleteDC();
	BmpMem.DeleteObject();

}

void CBaseImage_Rotate_V02::OnLButtonUp(UINT nFlags, CPoint point)
{
	OnChildLButtonUp(nFlags,point);
}
void CBaseImage_Rotate_V02::OnTimer(UINT_PTR nIDEvent)
{
	OnChildTimer(nIDEvent);
	CWnd::OnTimer(nIDEvent);
}
void CBaseImage_Rotate_V02::Rotate()
{
	CxImage::InterpolationMethod intm =CxImage::IM_BSPLINE;
	CxImage::OverflowMethod overm =CxImage::OM_BACKGROUND;
	RGBQUAD bkg;
	 bkg.rgbBlue = GetBValue(m_ColorBack);
	 bkg.rgbGreen = GetGValue(m_ColorBack);
	 bkg.rgbRed = GetRValue(m_ColorBack);
	 bkg.rgbReserved=0;	
	 m_lpImage->Load(m_strPicFilePath,m_nFileType);
	 m_lpImage->Rotate2(m_fRotate, 0, intm, overm, &bkg,true,TRUE);
	 
}
 void CBaseImage_Rotate_V02::SetRotate(float fRotate)
 {
	 m_fRotate = fRotate;
	 if(!m_lpImage)
		 return;
	 Rotate();
	 if(GetSafeHwnd())
		 Invalidate();
 }
bool CBaseImage_Rotate_V02::SetPicFileName(string sPicFilePath)
{ 
	delete_safe(m_lpImage);
	if(sPicFilePath.size()<5)
		return false;
	m_sPicFileName = sPicFilePath;	
	
 
	m_strPicFilePath.Format("%sres\\%d\\%s",GetExecFilePath(),CAppData::GetInstance().m_nDlgId,m_sPicFileName.c_str());

	string sExt = sPicFilePath.substr(sPicFilePath.size()-4,3);
	m_nFileType = CxImage::GetTypeIdFromName(sExt.c_str());
	m_lpImage =new CxImage(m_strPicFilePath,m_nFileType);
	if (!m_lpImage->IsValid())
	{
		delete_safe(m_lpImage);
		m_strPicFilePath.Format("%sres\\%d\\%s",GetExecFilePath(),0,m_sPicFileName.c_str());
		m_lpImage =new CxImage(m_strPicFilePath,m_nFileType);
		if (!m_lpImage->IsValid())
		{
			delete_safe(m_lpImage);
		}
	}

	return true;
}

void CBaseImage_Rotate_V02::DrawInfo(CDC *pDC,CRect rt)
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
 
	if(!m_lpImage)
	{ 
		CPen pen(PS_SOLID, 0, RGB(255,0,0));
		pDC->DrawText("未发现资源",rt,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		OnChildDrawInfo(pDC,rt);
		return ;
	} 

	 m_lpImage->Draw2(pDC->m_hDC,m_nBitX,m_nBitY);

	OnChildDrawInfo(pDC,rt);
}