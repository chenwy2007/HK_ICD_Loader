// ImageCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageCtrl.h"
 #include <shellapi.h>

//#include "BitmapButton.h" 
// CImageCtrl

IMPLEMENT_DYNAMIC(CImageCtrl, CBitmapButton)

CImageCtrl::CImageCtrl()
{
	m_pFont =NULL;
	LOGFONT lf; 
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT),sizeof(lf),&lf); 
	sprintf(lf.lfFaceName,"宋体");
	lf.lfHeight = 20;		
	lf.lfWeight = 600;
	m_font.CreateFontIndirect(&lf);

	m_TextColor = RGB(255,255,255);
	m_strTitle ="";

	m_hBitMap =NULL;

	m_nCurValue =0;
	m_nFontSize =30;
	m_bitmap.m_hObject =NULL;
}

CImageCtrl::~CImageCtrl()
{
}


BEGIN_MESSAGE_MAP(CImageCtrl, CBitmapButton)
	 ON_WM_ERASEBKGND()
END_MESSAGE_MAP()




void CImageCtrl::SetTextInfo(CString sText,int nFontSize,COLORREF  ref)
{
	m_strTitle =sText;
	m_TextColor = ref;
	m_nFontSize = nFontSize;

	m_font.DeleteObject();
	LOGFONT lf; 
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT),sizeof(lf),&lf); 
	sprintf(lf.lfFaceName,"宋体");
	lf.lfHeight = nFontSize;		
	lf.lfWeight = 600;
	m_font.CreateFontIndirect(&lf);
}


void CImageCtrl::SetValue(int nValue,bool bInvalidate)
{
	map<int,string>::iterator it = m_ImageMap.find(nValue);

	if(it==m_ImageMap.end())
	{
		return ;
	}
	if(m_nCurValue==nValue && !bInvalidate)
		return ;
	m_nCurValue = nValue;

	if(!GetSafeHwnd())
		return ;

	string sPath;
	GetCurImagePath(sPath);

	CString strPath = GetExecFilePath()+"res\\"+sPath.c_str();
	strPath.Format("%sres\\%d\\%s",GetExecFilePath(),CAppData::GetInstance().m_nDlgId,sPath.c_str());

	m_hBitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),strPath,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE); 
	  
	m_bitmap.DeleteObject();
	m_bitmap.Attach(m_hBitMap);
	this->Invalidate();	 
}
void CImageCtrl::SetNextValue()
{
	int nValue = m_nCurValue;
	nValue++;
	map<int,string>::iterator it = m_ImageMap.find(nValue);

	if(it==m_ImageMap.end())
	{
		 SetValue(0);
	}
	else
	{
		SetValue(nValue);
	}
}

void CImageCtrl::AddImagePath(int nIndex,string sPicPath)
{
	m_ImageMap[nIndex] = sPicPath;
}
bool CImageCtrl::GetCurImagePath(string &sPath)
{
	bool bRe =false;
	map<int,string>::iterator it = m_ImageMap.find(m_nCurValue);
	if(it!=m_ImageMap.end())
	{
		sPath = it->second;
		bRe =true;
	}
	return bRe;
}
void CImageCtrl::SetShowImage(string &sPath)
{
	if(m_strBmpPath==sPath)
		return ;
	m_strBmpPath = sPath ;
	CString strPath;
	strPath.Format("%sres\\%d\\%s",GetExecFilePath(),CAppData::GetInstance().m_nDlgId,sPath.c_str());

	m_hBitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),strPath,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE); 

	m_bitmap.DeleteObject();
	m_bitmap.Attach(m_hBitMap);
	if(GetSafeHwnd())
		this->Invalidate();
}
void CImageCtrl::CreateBmpInfo()
{
	if(m_bitmap.m_hObject)
		return ;

	string sPath;
	GetCurImagePath(sPath);

	CString strPath;
	strPath.Format("%sres\\%d\\%s",GetExecFilePath(),CAppData::GetInstance().m_nDlgId,sPath.c_str());

	m_hBitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),strPath,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE); 

	m_bitmap.DeleteObject();
	m_bitmap.Attach(m_hBitMap);
}

void CImageCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	 
	CRect   rect=lpDrawItemStruct-> rcItem; 
	CDC   *pDC=CDC::FromHandle(lpDrawItemStruct-> hDC); 
	int   nSaveDC=pDC-> SaveDC(); 
	
	CreateBmpInfo();
	if(!m_bitmap.m_hObject)
	{

		CPen pen(PS_SOLID, 0, RGB(255,0,0));
		pDC->DrawText("未发现资源",rect,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	
		return ;
	}
	{
	 
 			CDC MemDC;
			CDC DisplayDC;
			MemDC.CreateCompatibleDC(pDC); 
			DisplayDC.CreateCompatibleDC(pDC);  
			CBitmap* pOldBitmap= MemDC.SelectObject(&m_bitmap); 	
	
			CBitmap BmpMem;	
 			BmpMem.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
		//	BmpMem.CreateCompatibleBitmap(pDC,bm.bmWidth,bm.bmHeight);
	
			CBitmap*  pOldDrawbmp = DisplayDC.SelectObject(&BmpMem);
	
			DisplayDC.BitBlt(0,0, rect.Width(),rect.Height() ,&MemDC,0,0,SRCCOPY);	
			 
			pDC->StretchBlt(0,0,rect.Width(),rect.Height() ,&DisplayDC,0,0,rect.Width() ,rect.Height() ,SRCCOPY);
			DisplayDC.SelectObject(pOldDrawbmp);
			DisplayDC.DeleteDC();
			MemDC.SelectObject(pOldBitmap); 
			//释放资源   
			MemDC.DeleteDC();
			BmpMem.DeleteObject();
	}

 	//CBitmapButton::DrawItem(lpDrawItemStruct); 
	//显示按钮的文本 
	pDC-> SetTextColor(m_TextColor); 

	if(!m_strTitle.IsEmpty()) 
	{ 
		CFont*   hFont   =   GetFont(); 		
		CFont*   hOldFont  =NULL;

		if(m_pFont)
			hOldFont=   pDC-> SelectObject(m_pFont); 
		else
			hOldFont=   pDC-> SelectObject(&m_font); 

		int   nMode   =  pDC-> SetBkMode(TRANSPARENT); 

		pDC->DrawText(m_strTitle,rect,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		pDC-> SelectObject(hOldFont); 
		pDC-> SetBkMode(nMode); 
	} 

	pDC-> RestoreDC(nSaveDC);
}
BOOL CImageCtrl::OnEraseBkgnd(CDC* pDC)
{
	return true;
}