#include "StdAfx.h"
#include "XImageStatic.h"


XImageStatic::XImageStatic(void)
{
	m_nbmpWidth =NULL;
	m_nbmpHigth =NULL;
	m_ColorBack =RGB(28,28,28);
}


XImageStatic::~XImageStatic(void)
{
}

 void XImageStatic::CreateCtrl(CString sCtrlName,CString sTitle,DWORD  nCtrlId,CRect rect,CWnd * pParentWnd,int nShow )
 {

 }
 void XImageStatic::OnPaintEx(CDC *pDC,CRect rt)
 {
	 if(!m_bitmap.m_hObject)
	 {
		pDC->DrawText("未发现资源",rt,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		return;
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
	 
	 CBrush brush(m_ColorBack);
	 CPen   pen(0,0,m_ColorBack);

	 int nX = (rt.Width()-m_nbmpWidth)/2;
	 int nY = (rt.Height()-m_nbmpHigth)/2;

	 CBitmap BmpMem;	
	 BmpMem.CreateCompatibleBitmap(pDC,m_nbmpWidth,m_nbmpHigth);	

	 CBitmap*  pOldDrawbmp = DisplayDC.SelectObject(&BmpMem);
	 CBrush *pOldBrush =DisplayDC.SelectObject(&brush);
	 CPen *pOldPen =DisplayDC.SelectObject(&pen);

	 DisplayDC.Rectangle(rt);

	 DisplayDC.BitBlt(0,0, m_nbmpWidth,m_nbmpHigth ,&MemDC,0,0,SRCCOPY);	

	 pDC->StretchBlt(nX,nY,m_nbmpWidth,m_nbmpHigth ,&DisplayDC,0,0,m_nbmpWidth,m_nbmpHigth ,SRCCOPY);
	
	 DisplayDC.SelectObject(pen);
	 DisplayDC.SelectObject(pOldBrush);
	 DisplayDC.SelectObject(pOldDrawbmp);
	 DisplayDC.DeleteDC();
	 MemDC.SelectObject(pOldBitmap); 
	 //释放资源   
	 MemDC.DeleteDC();
	 BmpMem.DeleteObject();

 }
 void XImageStatic::RefreshData()
 {
	 if(m_nSharedId==0)
		 return; 
	 int nValue;
	 int nUpdateFlag;
	 unsigned long nUpdateTime;
	 if(HK_GetValue_Int_1(m_nSharedId,&nValue,&nUpdateFlag,&nUpdateTime)==0)
	 {
		 //未更新
		 if( !nUpdateFlag )
			 return;

		 map<int,CString>::iterator it =m_imageFileMap.find(nValue);
		 if(it!=m_imageFileMap.end())
			 SetImageFile(it->second);			 
		 else
			 SetImageFile(m_sDefaultImageFile);			 
	 }
	 else
		SetImageFile(m_sDefaultImageFile);

	 Invalidate(); 
 }

 void XImageStatic::SetDefaultImageFile(CString sFileName)
 {
	 m_sDefaultImageFile = sFileName;
	 m_sCurImageFile= m_sDefaultImageFile;	 
	 CreateBmp();
}
void XImageStatic::SetImageFile(CString sFileName)
{
	if(m_sCurImageFile == sFileName)
		return;

	m_sCurImageFile = sFileName;
	CreateBmp();
}

 void XImageStatic::CreateBmp()
 { 
	 CString strPath;	 
	 HBITMAP	hBitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),m_sCurImageFile,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE); 
	 if(hBitMap)
	 { 
		 m_bitmap.DeleteObject();
		 m_bitmap.Attach(hBitMap);
	 } 
	 
	 if(m_nbmpWidth==0 || m_nbmpHigth ==0)
		 GetBmpSize(strPath,m_nbmpWidth,m_nbmpHigth);

 }