// XWnd_DigitalStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "XWnd_DigitalStatic.h"
//#include "../../../commonLib/GridCtrl/MemDC_Grid.h"
//#include "dstring.h"
#include "boost/range/as_literal.hpp"

// XWnd_DigitalStatic

IMPLEMENT_DYNAMIC(XWnd_DigitalStatic, CStatic)

XWnd_DigitalStatic::XWnd_DigitalStatic(__CtrlInfo &info,float fScale):m_nDigitalFontType(0)
{
	DISPINFOR tDispInfor[] = {
		{"1",10,16},
		{"2",30,16},
		{"3",50,16},
		{"4",70,16},
		{"5",91,16},
		{"6",111,16},
		{"7",131,16},
		{"8",152,16},
		{"9",172,16},
		{"0",193,16},
		{"e",212,16},
		{"r",232,16},
		{".",252,7},
		{"-",264,16},
		{" ",282,14},
		{"日",299,18},
		{"夜",322,20},
		{"强",348,20},
		{"光",374,20},
		{"正",400,20},
		{"常",427,20},
		{"不",453,18},
		{"检",474,22},
		{"灯",505,19},
		{"视",531,21},
		{"x",557,16},
		{"、",579,16},
		{"=",604,16}
	};

	memcpy(&m_DispInfor,&tDispInfor,sizeof(DISPINFOR)*28);

	m_ctrlInfo = info;
	m_fScale =fScale;

	m_strTitle ="";
	m_bBack =0;
	m_backColor=RGB(128,128,128);

	m_FontColor =RGB(0,64,128);
 
	m_strTitle="";
	m_nAlign = DT_LEFT;

	m_sFontName = "黑体";
	 
	m_nFontSize =20;

	m_DigitalLen = 6;
	m_PerHeight = 33;
	m_PerWidth = 14;
	m_sRange = "";
}
XWnd_DigitalStatic::XWnd_DigitalStatic():m_nDigitalFontType(0)
{
	m_strTitle ="";
	 
	m_bBack =0;
	m_backColor=RGB(128,128,128);

	m_FontColor =RGB(0,64,128);
	 
	
	m_strTitle="";
	m_nAlign = DT_CENTER;	

	m_DigitalLen = 6;
	m_PerHeight = 33;
	m_PerWidth = 12;
	m_sRange = "";
}

XWnd_DigitalStatic::~XWnd_DigitalStatic()
{
}


BEGIN_MESSAGE_MAP(XWnd_DigitalStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


bool XWnd_DigitalStatic::IsNoDispCharacter(int nDispCharacterSize,int nDispLen,WCHAR* wDispValue)
{
	//nDispCharacterSize = sizeof(m_DispInfor)/sizeof(DISPINFOR);
	//int nDispLen = m_strTitle.GetLength();
	char lpData[4] = {0};
	WCHAR wCh = 0;
	for(int i = 0; i < nDispLen; ++ i)
	{
		int nCmpLen = 1;
		bool bFind = false;
		lpData[0] = 0;
		wCh = wDispValue[i];
		Unicode2MByte(&wCh,lpData);
		if(lpData[0]&0x80)
			nCmpLen = 2;
		for(int j = 0; j < nDispCharacterSize; ++ j)
		{
			if(strncmp(lpData,m_DispInfor[j].lpDisp,nCmpLen) == 0)
			//if(stricmp())
			{
				bFind = true;
				break;
			}
		}
		if(!bFind) return false;
	}

	return true;
}

void XWnd_DigitalStatic::MByte2Unicode(char *lpData,WCHAR  * lpToData)
{
	//获取持转换的字符个数
	int unicodeLen = MultiByteToWideChar(CP_ACP,0,lpData,-1,NULL,0); 

	//分配宽字符内存
	wchar_t * pUnicode = new wchar_t[unicodeLen]; 

	//多字节->宽字符
	MultiByteToWideChar(CP_ACP,0,lpData,-1,pUnicode,unicodeLen); 

	memcpy(lpToData,pUnicode,sizeof(wchar_t) * wcslen(pUnicode));

	delete []pUnicode; 
}

void XWnd_DigitalStatic::Unicode2MByte(WCHAR * lpData,char *lpToData,int nsize)
{ 
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpData,-1,NULL,0,NULL,FALSE);
	WideCharToMultiByte(CP_OEMCP,NULL,lpData,-1,lpToData,nsize,NULL,FALSE);
}



// XWnd_DigitalStatic 消息处理程序
 
void XWnd_DigitalStatic::OnPaint()
{
	CPaintDC dc(this);  

	CDC *pDC = &dc;
	CRect rt;
	GetClientRect(rt);

	SetWindowText("");
	CBitmap bmp;
	if(m_nDigitalFontType == 0)
		bmp.LoadBitmap(IDB_BMP_HOLLOW);		// 空心字体
	else if(m_nDigitalFontType == 1)
		bmp.LoadBitmap(IDB_BMP_HOLLOWRED);	// 红色空心字体（用于报警数据）
	else if(m_nDigitalFontType == 2)
		bmp.LoadBitmap(IDB_BMP_SOLID);		// 实心字体
	else
		bmp.LoadBitmap(IDB_BMP_WHITE);		// 反显字体
	
	CDC MemDC;

	MemDC.CreateCompatibleDC(pDC); 

	CBitmap* pOldBitmap= MemDC.SelectObject(&bmp); 

	MemDC.SetTextColor(m_FontColor);
	MemDC.SetBkMode(TRANSPARENT);

	BITMAP bInfo;
	bmp.GetBitmap(&bInfo);
	int x = bInfo.bmWidth;
	int y = bInfo.bmHeight;

	if(y > rt.Height()) y = rt.Height();// 以当前设置的高度绘置显示区

	DrawInfo(&MemDC,rt);

	char lpData[4] = {0};
	WCHAR wDispValue[20] = {0};
	MByte2Unicode(m_strTitle.GetBuffer(),wDispValue);
	int nDispLen = wcslen(wDispValue);

	m_DigitalLen = (int)(rt.Width()*1.0/m_PerWidth+0.5);
	
	int nDispCharacterSize = sizeof(m_DispInfor)/sizeof(DISPINFOR);

	// 超出显示范围
	if(m_DigitalLen < nDispLen)
	{
		m_strTitle = "err";
		nDispLen = 3;
	}
	else
	{
		// 判断是否有非显字符
		bool bNoDispCharacter = IsNoDispCharacter(nDispCharacterSize,nDispLen,wDispValue);
		if(!bNoDispCharacter)
		{
			m_strTitle = "err";
			nDispLen = 3;
		}
	}

	//pDISPINFOR pCurDisp = NULL;

	//if (len>0 && len<=m_DigitalLen)
	//{
	//	pCurDisp = new DISPINFOR[len];
	//	
	//	for (int n = 0; n < len; n ++)
	//	{
	//		char a=  m_strTitle[len-n-1];
	//		int b = 0;
	//		bool bIsFind = false;

	//		for(int j = 0; j < 14; ++ j)
	//		{
	//			if(m_DispInfor[j].strDisp == a)
	//			{
	//				b = m_DispInfor[j].nPosi;
	//				bIsFind = true;
	//				break;
	//			}
	//		}
	//		if(bIsFind)
	//		{
	//			pCurDisp[n].strDisp = a;
	//			pCurDisp[n].nPosi = b;
	//		}
	//		else
	//		{
	//			delete_safe(pCurDisp);
	//			pCurDisp = new DISPINFOR[3];
	//			memcpy(pCurDisp,&tErrInfor,3*sizeof(DISPINFOR));
	//			len = 3;
	//			break;
	//		}
	//	}

	//}

	// 清除显示字符
	int nAccxPosi = 0;
	for (int i = 0; i< m_DigitalLen; i++)
	{
		nAccxPosi += m_DispInfor[14].nWidth;
		dc.BitBlt(rt.Width()-nAccxPosi,0,m_DispInfor[14].nWidth,y,&MemDC,m_DispInfor[14].nxPosi,7,SRCCOPY);
	}

	wDispValue[0] = 0;
	WCHAR wCh = 0;
	MByte2Unicode(m_strTitle.GetBuffer(),wDispValue);

	nAccxPosi = 0;
	for (int n = 0 ; n < nDispLen; n++)
	{
		int nCmpLen = 1;
		lpData[0] = 0;
		wCh = wDispValue[nDispLen-n-1];
		Unicode2MByte(&wCh,lpData);
		if(lpData[0]&0x80)
			nCmpLen = 2;
		for(int j = 0; j < nDispCharacterSize; ++ j)
		{
			if(strncmp(lpData,m_DispInfor[j].lpDisp,nCmpLen) == 0)
			{
				nAccxPosi += m_DispInfor[j].nWidth;
				dc.BitBlt(rt.Width()-nAccxPosi,0,m_DispInfor[j].nWidth,y,&MemDC,m_DispInfor[j].nxPosi,7,SRCCOPY);
				break;
			}
		}
	}

	//delete_safe(pCurDisp);
	MemDC.SelectObject(pOldBitmap); 
	//释放资源   
	bmp.DeleteObject();
	MemDC.DeleteDC();
}
void XWnd_DigitalStatic::DrawInfo(CDC *pDC,CRect rt)
{
	//if(m_strTitle.IsEmpty())
	//	GetWindowText(m_strTitle);
	//if(m_bBack)
	//{
	//	CBrush brush(m_backColor);
	//	CPen pen(PS_SOLID,0,RGB(255,255,255));
	//	CBrush *oldBrush = pDC->SelectObject(&brush);
	//	CPen *oldPen = pDC->SelectObject(&pen);
	//	pDC->Rectangle(rt);
	//	pDC->SelectObject(oldPen);
	//	pDC->SelectObject(oldBrush);
	//}

	//CFont font;
	//CFont *pFont;
	//font.CreateFont(m_nFontSize*m_fScale,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
	//	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,m_sFontName.c_str());
	// 	
	//
	//CFont * OldFont=pDC->SelectObject(&font);

	//pDC->SetTextColor(m_FontColor);
	//pDC->SetBkMode(TRANSPARENT);

	//pDC->DrawText(m_strTitle,rt,m_nAlign|DT_VCENTER|DT_SINGLELINE);
	 
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
	 
	//pDC->SelectObject(OldFont);
	//font.DeleteObject();

}
void XWnd_DigitalStatic::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow )
{
	m_nCtrlId = nCtrlId;
	CRect rt;
	rt.left = m_ctrlInfo.nX * m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	Create(m_strTitle, WS_VISIBLE|WS_CHILD|SS_NOTIFY,rt,pParentWnd,m_nCtrlId);
}
void XWnd_DigitalStatic::SetSelect(bool bSel)
{
	m_bSel =bSel;
	this->Invalidate();
}
void XWnd_DigitalStatic::DecodeXml()
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

		// 获取控件属性值
		m_nCtrlType = atoi(xml.GetAttrib("CtrlType"));
		m_sDispContend = xml.GetAttrib("DispContend");
		m_sRange = xml.GetAttrib("ValueRange");
	}

//	CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_RefreshEventVec,m_ctrlInfo.nId,EVENT_TYPE_REFRESH,-1);

}
void XWnd_DigitalStatic::MoveWnd()
{ 
	int nW = m_ctrlInfo.nW*m_fScale;
	int nH = m_ctrlInfo.nH*m_fScale;
	int x = m_ctrlInfo.nX*m_fScale;
	int y = m_ctrlInfo.nY*m_fScale;

	MoveWindow(x,y, nW, nH );
	this->Invalidate();
}

void XWnd_DigitalStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(CAppData::GetInstance().m_bBuilder )	 
	{
		CAppData::GetInstance().SetCurSelectWnd(this);
	}

	__super::OnLButtonUp(nFlags, point);
}

void XWnd_DigitalStatic::UpdateAttr(__DigitalStaticAttr & attr)
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

	m_ctrlInfo.nSharedId = attr.nSharedId;
}

 void XWnd_DigitalStatic::RefreshData()
 {
	 if(m_ctrlInfo.nSharedId<=0)
		 return ;
	 int nValue =0;
	 int bUpdateFlag;
	 DWORD dwUpdateTime;
	 
	 if(g_XShared.ReadData_Int(m_ctrlInfo.nSharedId,nValue,bUpdateFlag,dwUpdateTime))
	 {
		 if(!bUpdateFlag)
			 return;		  
		 SetCurValue(nValue);
		 Invalidate();
	 }
 }

 bool XWnd_DigitalStatic::SetCurValue(int nValue)
 {
	 m_nDigitalFontType = 0;

	 // 普通文本框
	 if(m_nCtrlType == 4)
	 {
		 m_strTitle.Format("%d",nValue);
	 }
	 // 带基数的文本框
	 else if(m_nCtrlType == 10)
	 {
		 int nBaseValue = _tstoi(m_sDispContend);
		 m_strTitle.Format("%d",nValue+nBaseValue);
	 }
	 // 组合选择框
	 else if(m_nCtrlType == 5)
	 {
		 vector<string> vtContent;
		 string strContent = m_sDispContend.GetBuffer();
		 SplitString(strContent,vtContent,"，");
		 int nDispColNum = vtContent.size();
		 if(0 <= nValue && nValue < nDispColNum)
		 {
			 m_strTitle = vtContent[nValue].c_str();
		 }
		 else
		 {
			 m_strTitle = " ";
		 }
	 }

	 // 如果数据超出范围，用红色空心字体
	 if(!m_sRange.IsEmpty())
	 {
		 int nPos = m_sRange.Find("~");
		 if(nPos > 0)
		 {
			 int nMin = _tstoi(m_sRange.Left(nPos));
			 int nMax = _tstoi(m_sRange.Right(m_sRange.GetLength()-nPos-1));
			 if(nValue < nMin || nValue > nMax)
				 m_nDigitalFontType = 1;
		 }
	 }

	 // 如果无效，用黑色字体
	 if((nValue&0xFFFF0000) == 0)
		m_nDigitalFontType = 2;

	 return true;
 }


 void XWnd_DigitalStatic::SetText(LPCTSTR text)
 {
	 m_strTitle = text;
	 RedrawWindow();
 }

 LPTSTR XWnd_DigitalStatic::GetText()
 {
	 return m_strTitle.GetBuffer(0);
 }

 void XWnd_DigitalStatic::SubControl(UINT uID,CWnd* pParent)
 {
	 SubclassDlgItem(uID,pParent);
	 CRect wRc;
	 GetWindowRect(wRc);
	 ScreenToClient(wRc);

	 MapWindowPoints(pParent,wRc);
	 wRc.right = wRc.left+m_DigitalLen*m_PerWidth;
	 wRc.bottom = wRc.top+m_PerHeight;
	 int width = m_DigitalLen*m_PerWidth;
	 MoveWindow(wRc);
 }
