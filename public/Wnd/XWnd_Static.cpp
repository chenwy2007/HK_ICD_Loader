// XWnd_Static.cpp : 实现文件
//

#include "stdafx.h"
#include "XWnd_Static.h"


// XWnd_Static

IMPLEMENT_DYNAMIC(XWnd_Static, CStatic)

XWnd_Static::XWnd_Static(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo = info;
	m_fScale =fScale;

	m_strTitle ="";
	m_bBack =0;
	m_backColor=RGB(128,128,128);

	m_FontColor =RGB(0,64,128);
 
	m_sToolTipInfor="";
	m_nAlign = DT_LEFT;

	m_sFontName = "黑体";
	 
	m_nFontSize =20;

	m_hAlerthIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

	m_bIsAlert = false;
	m_bIsDrawTip = false;
	m_pTopBase = NULL;

}
XWnd_Static::XWnd_Static()
{
	m_strTitle ="";
	 
	m_bBack =0;
	m_backColor=RGB(128,128,128);

	m_FontColor =RGB(0,64,128);
	 
	
	m_sToolTipInfor="";
	m_nAlign = DT_CENTER;	

	m_hAlerthIcon = AfxGetApp()->LoadIcon(IDI_ICON_ALERT);

	m_bIsAlert = false;
	m_bIsDrawTip = false;
	m_pTopBase = NULL;
}

XWnd_Static::~XWnd_Static()
{
	DestroyIcon(m_hAlerthIcon);
}


BEGIN_MESSAGE_MAP(XWnd_Static, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// XWnd_Static 消息处理程序
 
void XWnd_Static::OnPaint()
{
	CPaintDC dc(this);  
	
	CDC *pDC = &dc;
	CRect rt;
	GetClientRect(rt);
	if(m_bBack)
	{

		CDC MemDC;

		MemDC.CreateCompatibleDC(pDC); 

		CBitmap BmpMem;	
		BmpMem.CreateCompatibleBitmap(pDC,rt.Width(),rt.Height());
		CBitmap* pOldBitmap= MemDC.SelectObject(&BmpMem); 

		MemDC.SetTextColor(m_FontColor);
		MemDC.SetBkMode(TRANSPARENT);

		DrawInfo(&MemDC,rt);

		dc.BitBlt(0, 0, rt.Width(), rt.Height(), &MemDC, 0, 0, SRCCOPY);

		MemDC.SelectObject(pOldBitmap); 
		//释放资源   
		MemDC.DeleteDC();
		BmpMem.DeleteObject();
	}
	else 
	{
		DrawInfo(pDC,rt);
	}
 
}
void XWnd_Static::DrawInfo(CDC *pDC,CRect rt)
{
	if(m_strTitle.IsEmpty())
		GetWindowText(m_strTitle);
	if(m_bBack)
	{
		CBrush brush(m_backColor);
		CPen pen(PS_SOLID,0,RGB(255,255,255));
		CBrush *oldBrush = pDC->SelectObject(&brush);
		CPen *oldPen = pDC->SelectObject(&pen);
		pDC->Rectangle(rt);
		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);
	}

	CFont font;
	CFont *pFont;
	font.CreateFont(m_nFontSize*m_fScale,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,m_sFontName.c_str());
	 	
	
	CFont * OldFont=pDC->SelectObject(&font);

	pDC->SetTextColor(m_FontColor);
	pDC->SetBkMode(TRANSPARENT);
	//pDC->SetBkColor(m_backColor);

	pDC->DrawText(m_strTitle,rt,m_nAlign|DT_VCENTER|DT_SINGLELINE);
	 
	if(m_bIsAlert)
	{
		//DrawIconEx(pDC->m_hDC,0,0,m_hAlerthIcon,32,32,0,NULL,DI_NORMAL|DI_COMPAT);
		DrawIcon(pDC->m_hDC,0,-10,m_hAlerthIcon);
	}

	if(CAppData::GetInstance().m_bBuilder  && m_bSel)	 
	{ 
		CPen pen;
		pen.CreatePen(PS_DASHDOTDOT,0,RGB(255,0,0));
		
		CPen *oldPen =pDC->SelectObject(&pen);
		pDC->MoveTo(0,0);
		pDC->LineTo(0,rt.bottom-1);
		pDC->LineTo(rt.right-1,rt.bottom-1);
		pDC->LineTo(rt.right-1,0);
		pDC->LineTo(0,0);
		pDC->SelectObject(oldPen);
	}
	 
	pDC->SelectObject(OldFont);
	font.DeleteObject();

}
void XWnd_Static::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow )
{
	m_nCtrlId = nCtrlId;
	CRect rt;
	rt.left = m_ctrlInfo.nX * m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	Create(m_strTitle, WS_VISIBLE|WS_CHILD|SS_NOTIFY,rt,pParentWnd,m_nCtrlId);
}
void XWnd_Static::SetSelect(bool bSel)
{
	m_bSel =bSel;
	this->Invalidate();
}
void XWnd_Static::DecodeXml()
{
	m_strTitle = m_ctrlInfo.sTitle.c_str();

	CMarkup xml(m_ctrlInfo.sAttr.c_str());
	CString str;
	if(xml.FindElem("Attr"))
	{
		m_bBack = atoi(xml.GetAttrib("Back"));
		if(m_bBack ==1)
		{
			str =xml.GetAttrib("BackColor");
			sscanf(str,"%d",&m_backColor);
		}
		str  = xml.GetAttrib("Align");		
		m_nAlign = atoi(str);
		if(m_nAlign<0)
			m_nAlign =0;

		m_nFontSize = atoi(xml.GetAttrib("FontSize"));
		m_nFontSize =m_nFontSize==0?20:m_nFontSize;

		m_sFontName =xml.GetAttrib("FontName");
		m_sFontName= m_sFontName.empty()?m_sFontName:"黑体";

		m_FontColor =atoi(xml.GetAttrib("FontColor"));
		m_FontColor =m_FontColor==0?RGB(255,255,255):m_FontColor;
	}

//	CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_RefreshEventVec,m_ctrlInfo.nId,EVENT_TYPE_REFRESH,-1);

}
void XWnd_Static::MoveWnd()
{ 
	int nW = m_ctrlInfo.nW*m_fScale;
	int nH = m_ctrlInfo.nH*m_fScale;
	int x = m_ctrlInfo.nX*m_fScale;
	int y = m_ctrlInfo.nY*m_fScale;

	MoveWindow(x,y, nW, nH );
	this->Invalidate();
}

void XWnd_Static::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(CAppData::GetInstance().m_bBuilder )	 
	{
		CAppData::GetInstance().SetCurSelectWnd(this);
	}

	__super::OnLButtonUp(nFlags, point);
}

void XWnd_Static::OnMouseMove(UINT /*nFlags*/, CPoint point)
{
	if(!m_bIsAlert || !m_pTopBase)
		return;

	CRect rect;
	GetClientRect(rect);

	// If outside client area, return (unless we are drag n dropping)
	if (!rect.PtInRect(point))
		return;

	if(!m_bIsDrawTip)
	{
		m_bIsDrawTip = true;

		CRect rectscreen = rect;
		ClientToScreen(&rectscreen);
		rectscreen.left = rectscreen.right;
		rectscreen.right = rectscreen.left + 180;
		rectscreen.top -= 20;
		rectscreen.bottom -= 20;

		m_pTopBase->DrawToolTip(rectscreen,m_sToolTipInfor);
		SetTimer(1,200,0);
	}
}

void XWnd_Static::OnTimer(UINT nIDEvent)
{
	if(m_bIsDrawTip && nIDEvent == 1)
	{
		CRect rect,rectscreen;
		GetClientRect(&rect);

		rectscreen = rect;
		ClientToScreen(&rectscreen);
		rectscreen.left = rectscreen.right;
		rectscreen.right = rectscreen.left + 180;
		rectscreen.top -= 20;
		rectscreen.bottom -= 20;

		CPoint   curPt;
		GetCursorPos(&curPt);
		ScreenToClient(&curPt);
		if(!rect.PtInRect(curPt))
		{
			//m_bIsAlert = FALSE;
			m_bIsDrawTip = false;
			m_pTopBase->DrawToolTip(rectscreen,"",SW_HIDE);
			KillTimer(1);
		}
	}
}

void XWnd_Static::OnDestroy()
{
	__super::OnDestroy();
	// TODO: 在此处添加消息处理程序代码

	KillTimer(1);
}

void XWnd_Static::UpdateAttr(__StaticAttr & attr)
{
	m_ctrlInfo.sTitle = attr.sTitle;
	m_ctrlInfo.sName  = attr.sName;
	m_strTitle = attr.sTitle.c_str();

	m_nAlign = attr.nAlign;
	m_sFontName = attr.sFontName;
	m_nFontSize = attr.nFontSize;

	m_FontColor = attr.fontColor;
	m_backColor = attr.backColor;
	m_bBack = attr.bBackColor;

	m_ctrlInfo.nH = attr.nH;
	m_ctrlInfo.nW = attr.nW;
	m_ctrlInfo.nX = attr.nX;
	m_ctrlInfo.nY = attr.nY;

}
 void XWnd_Static::RefreshData()
 {

 }