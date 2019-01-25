// BaseDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "BaseDialog.h"
#include "afxdialogex.h"


// CBaseDialog 对话框

IMPLEMENT_DYNAMIC(CBaseDialog, CDialogEx)

CBaseDialog::CBaseDialog(int nId,CWnd* pParent)
	: CDialogEx(nId, pParent)
{
	m_colorTitle  = RGB(174,200,237); 
	m_colorBack = RGB(205,220,239);
	m_nTopHeight = 50;
	m_nBottomHeight = 30;

	m_colorFont = RGB(0,64,128);
	m_nFontSize =120;

	m_strLeftState ="";
	m_strRigthState ="";
	m_nSideWidth = 10;
	m_nCloseBtState =0;
	m_nMaxBtState =0;
	m_bHasCloseBtn =false;
	m_bHasMaxBtn =false;
	m_nCmdShow  =SW_NORMAL;
	m_RectRightState.left=-1;
	m_RectLeftState.left=-1;
	m_bHasTitleLog =false;

m_bSideLine =false;

	
}

CBaseDialog::~CBaseDialog()
{
}

void CBaseDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBaseDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
//	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



HBRUSH CBaseDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_STATIC)
	{

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		return (HBRUSH)GetStockObject(NULL_BRUSH);; 
	}
	//else if(nCtlColor == CTLCOLOR_BTN)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(RGB(255,255,255));
	//	 hbr = CreateSolidBrush(RGB(112,146,190));
	//}
	return hbr;
}

int CBaseDialog::DrawTopLog(CDC * pDC)
{

	if(!m_bHasTitleLog)
		return m_nSideWidth;

	CDC dcimage;
	dcimage.CreateCompatibleDC(pDC);  

	//标题
	CString strpath = GetExecFilePath()+"res\\Title.bmp";
	HBITMAP hBitmap=(HBITMAP)LoadImage(NULL,strpath , IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	
	CBitmap bitmap; 
	bitmap.Attach(hBitmap);   
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);  
	dcimage.SelectObject(&bitmap);  
	pDC->BitBlt(m_nSideWidth,0,bmp.bmWidth,bmp.bmHeight,&dcimage,0,0,SRCCOPY); //显示 

	return m_nSideWidth+bmp.bmWidth; 

}
void CBaseDialog::DrawCloseBtn(CDC * pDC,CRect rt)
{
	if(!m_bHasCloseBtn)
		return ;

	CDC dcimage;
	dcimage.CreateCompatibleDC(pDC);  

	//CLOSE按钮
	CString strpath = GetExecFilePath()+"res\\"+(m_nCloseBtState==0 ?"exit_Normal.bmp":"exit_Press.bmp");
	
	HBITMAP hBitmap=(HBITMAP)LoadImage(NULL,strpath , IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	CBitmap bitmap; 
	bitmap.Attach(hBitmap);   
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);  
	dcimage.SelectObject(&bitmap);

	int y =(m_nTopHeight - bmp.bmHeight)/2;
	int x = rt.Width()-bmp.bmWidth- m_nSideWidth;
	m_RectCloseBtn=CRect(x,y,x+bmp.bmWidth,y+bmp.bmHeight);
	pDC->BitBlt(x,y,bmp.bmWidth,bmp.bmHeight,&dcimage,0,0,SRCCOPY); //显示 
 
	//最大化按钮
	if(!m_bHasMaxBtn)
		return;

	strpath = GetExecFilePath()+"res\\"+(m_nMaxBtState==0 ?"Max_Normal.bmp":"Max_Press.bmp");
	hBitmap=(HBITMAP)LoadImage(NULL,strpath , IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	CBitmap bitmapMax; 
	bitmapMax.Attach(hBitmap);   	 
	dcimage.SelectObject(&bitmapMax);
	x = x-bmp.bmWidth -5;
	m_RectMaxBtn=CRect(x,y,x+bmp.bmWidth,y+bmp.bmHeight);
	pDC->BitBlt(x,y,bmp.bmWidth,bmp.bmHeight,&dcimage,0,0,SRCCOPY); //显示 

	 
}
void CBaseDialog::OnMyPaint(CDC * pDC,CRect rt)
{
	CDC MemDC;

	MemDC.CreateCompatibleDC(pDC); 

	CBitmap BmpMem;	
	BmpMem.CreateCompatibleBitmap(pDC,rt.Width(),rt.Height());
	CBitmap* pOldBitmap= MemDC.SelectObject(&BmpMem); 
	CFont * OldFont=NULL;


	MemDC.SetTextColor(m_colorFont);
	MemDC.SetBkMode(TRANSPARENT);
	int y =0;
;

	if(m_nTopHeight>0)
	{ 
		MemDC.FillSolidRect(0,y,rt.Width(),m_nTopHeight,m_colorTitle );
		y += m_nTopHeight;
		MemDC.FillSolidRect(0,y,rt.Width(),1,RGB(255,255,255) );
		y += 1;
	} 
	MemDC.FillSolidRect(0,y,rt.Width(),rt.Height()-m_nBottomHeight-m_nTopHeight,m_colorBack);
	if(m_nBottomHeight>0)
	{
		y += rt.Height()-m_nBottomHeight-m_nTopHeight;
		MemDC.FillSolidRect(0,y,rt.Width(), 1,RGB(255,255,255));
		y +=1;
		MemDC.FillSolidRect(0,y,rt.Width(), m_nBottomHeight,m_colorTitle);
	}

	if(m_nTopHeight>0)
	{
		int nLeft  = DrawTopLog(&MemDC);
		DrawTopTxt(&MemDC,rt,nLeft);
		DrawCloseBtn(&MemDC,rt);
	}
	m_RectTitle =CRect(0,0,rt.Width(),m_nTopHeight);
	if(m_nBottomHeight>0)
		DrawBottomTxt(&MemDC,rt);
	if(m_bSideLine)
	{
		CPen pen;
		 pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
		 CPen *oldPen =MemDC.SelectObject(&pen);
		 MemDC.MoveTo(0,0);
		 MemDC.LineTo(0,rt.Height()-1);
		 MemDC.LineTo(rt.Width()-1,rt.Height()-1);
		 MemDC.LineTo(rt.Width()-1,0);
		 MemDC.LineTo(0,0);
		 MemDC.SelectObject(oldPen);
	}

	DrawCustom( &MemDC,rt);
	 

	pDC->BitBlt(0, 0, rt.Width(), rt.Height(), &MemDC, 0, 0, SRCCOPY);

	if(!OldFont)
		MemDC.SelectObject(OldFont);

	MemDC.SelectObject(pOldBitmap); 
	//释放资源   
	MemDC.DeleteDC();
	BmpMem.DeleteObject();
}
void CBaseDialog::OnPaint()
{
	CPaintDC dc(this); 

	CDC *pDC = &dc;
	CRect rt;
	GetClientRect(rt);

	OnMyPaint(pDC,rt);
	
 
}
BOOL CBaseDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	
	return TRUE;
}
//
LRESULT CBaseDialog::OnNcHitTest(CPoint point)
{
	 if(m_nBottomHeight>0)
		 return HTCAPTION;  
	 return HTCLIENT;
	//return CDialogEx::OnNcHitTest(point);
}


void CBaseDialog::SetSideWidth(int nTop ,int nBottom,int nLeftRight)
{
	m_nTopHeight = nTop;
	m_nBottomHeight = nBottom;
	m_nBorderWidth = nLeftRight;
}
void CBaseDialog::SetBackColor(COLORREF colorBack,COLORREF colorTitle,COLORREF colorfont)
{
	m_colorBack	 = colorBack;
	m_colorTitle = colorTitle; 
	m_colorFont = colorfont;
}
void CBaseDialog::DrawTopTxt(CDC * pDC,CRect rt,int nLeft)
{
	CRect textRt;
	textRt.left =nLeft+10;
	textRt.top =0;
	textRt.right = rt.Width()-2*m_nSideWidth;
	textRt.bottom = m_nTopHeight;
	
	CFont font;
	font.CreatePointFont(m_nFontSize,"黑体",NULL);

	CFont * OldFont = pDC->SelectObject(&font);

	pDC->DrawText(m_strTitle,textRt,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	pDC->SelectObject(OldFont);
}
void CBaseDialog::DrawBottomTxt(CDC * pDC,CRect rt)
{
	CRect textRt;
	textRt.left =m_nSideWidth;
	textRt.top =rt.Height()-m_nBottomHeight;
	textRt.right = rt.Width()-m_nSideWidth;
	textRt.bottom = rt.Height();

	CFont FontState;
	FontState.CreatePointFont(100,"宋体",NULL);
	CFont * OldFont = pDC->SelectObject(&FontState);
	
	if(!m_strLeftState.IsEmpty())
	{
		pDC->DrawText(m_strLeftState,textRt,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		 CSize size = pDC->GetTextExtent(m_strLeftState);
		m_RectLeftState.left = textRt.left;
		m_RectLeftState.right = textRt.right-100;
		m_RectLeftState.top = textRt.top+textRt.Height()/2 - size.cy/2;
		m_RectLeftState.bottom = m_RectLeftState.top+size.cy;
	}

	if(!m_strRigthState.IsEmpty())
	{
		 pDC->DrawText(m_strRigthState,textRt,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		 CSize size = pDC->GetTextExtent(m_strRigthState);

		 m_RectRightState.left = textRt.right - size.cx-10;
		 m_RectRightState.right = textRt.right;
		 m_RectRightState.top = textRt.top+textRt.Height()/2 - size.cy/2;
		 m_RectRightState.bottom = m_RectRightState.top+size.cy;
	}
	
	pDC->SelectObject(OldFont);
	FontState.DeleteObject();
}

//标题内容
void CBaseDialog::SetTitleTxt(CString strTille)
{
	m_strTitle = strTille;
};
//标题内容
void CBaseDialog::SetStatusTxt(int nIndex,CString sTxt)
{
	if(nIndex==0)
		m_strLeftState = sTxt;
	else if(nIndex ==1)
		m_strRigthState = sTxt;


	 
};

void CBaseDialog::InvalidateStatusTxt(int nIndex)
{
	if(nIndex==0)
	{
		if(m_RectLeftState.left!=-1)
			InvalidateRect(m_RectLeftState,0);
		else
			Invalidate();
	}
	else if(nIndex==1)
	{ 
		if(m_RectRightState.left!=-1)
			InvalidateRect(m_RectRightState,0);
		else
			Invalidate();
	}
}
void CBaseDialog::SetEnableBt(bool bCloseBtn,bool bMaxBtn)
{
	m_bHasCloseBtn = bCloseBtn;
	m_bHasMaxBtn = bMaxBtn;
}

void CBaseDialog::OnLButtonDown(UINT nFlags, CPoint point)
{

	CRect rectMain;
	GetClientRect(rectMain);
	CRect rectTitle(0,0,rectMain.Width(),m_nTopHeight);

	if (m_RectCloseBtn.PtInRect(point) && m_bHasCloseBtn)
	{
		m_nCloseBtState = 1;
		InvalidateRect(&m_RectCloseBtn,FALSE);
	}
	else if (m_RectMaxBtn.PtInRect(point) && m_bHasMaxBtn)
	{
		m_nMaxBtState = 1;
		InvalidateRect(&m_RectMaxBtn,FALSE);
	}
	else   if (m_RectTitle.PtInRect(point) && m_nCmdShow == SW_NORMAL)
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	}


	CDialogEx::OnLButtonDown(nFlags, point);
}


void CBaseDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	 
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_RectCloseBtn.PtInRect(point) &&  m_bHasCloseBtn)
	{  
		OnExit();
	}
	else  if(m_RectMaxBtn.PtInRect(point) &&  m_bHasMaxBtn)
	{  
		m_nMaxBtState = 0;
		if(m_nCmdShow ==SW_NORMAL) 
			m_nCmdShow = SW_SHOWMAXIMIZED;
		else
			m_nCmdShow = SW_NORMAL;

			ShowWindow(m_nCmdShow);
		 this->Invalidate();
		 
	}
 
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CBaseDialog::OnExit()
{
	CBaseDialog::OnCancel();
}

void CBaseDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if((m_RectTitle.PtInRect(point)  &&m_bHasMaxBtn)  && !(m_RectCloseBtn.PtInRect(point) &&  m_bHasCloseBtn))
	{  
		m_nMaxBtState = 0;
		if(m_nCmdShow ==SW_NORMAL) 
			m_nCmdShow = SW_SHOWMAXIMIZED;
		else
			m_nCmdShow = SW_NORMAL;

		ShowWindow(m_nCmdShow);
		this->Invalidate();

	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

