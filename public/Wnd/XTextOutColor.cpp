#include "StdAfx.h"
#include "XTextOutColor.h"


XTextOutColor::XTextOutColor(float fScale,__TextAttr_V02 &attr)
{
	m_fScale =fScale;

	m_nId = attr.nId;
	m_nCtrlId = attr.nCtrlId;

	m_sTitle = attr.sTitle;
	m_ColorDefaultFont = attr.colorFont;
	m_ColorCurFont = attr.colorFont;
	
	m_sFontName =attr.sFontName;
	m_nFontSize =attr.nFontSize * m_fScale;
	
	m_nAlign =attr.nAlign;	 
	m_nX =attr.nX;
	m_nY =attr.nY;	
}


XTextOutColor::~XTextOutColor(void)
{
}

void XTextOutColor::InitData()
{
	//刷新事件
	CManageDb_Sqllite::GetInstance().Query_CtrlEvent_Text(m_RefreshEventVec,m_nId,m_nCtrlId);
}
 
bool XTextOutColor::RefreshData()
{
	if(m_RefreshEventVec.size()==0)
		return false;
 
	int nValue =0; 
	int bUpdateFlag;
	DWORD dwUpdateTime;
	for(int i =0,end =m_RefreshEventVec.size();i<end;i++)
	{
		if(m_RefreshEventVec[i].nSharedId==0)
			continue;

		if(  g_XShared.ReadData_Int(m_RefreshEventVec[i].nSharedId,nValue,bUpdateFlag,dwUpdateTime))
		{
			if(!bUpdateFlag)
				continue;
			//区间值，或者等于某一个值
			if(nValue>=m_RefreshEventVec[i].nValue1 && nValue<=m_RefreshEventVec[i].nValue2)			
			{
				if(m_nCurValue ==nValue )
					return false;
				m_ColorCurFont = m_RefreshEventVec[i].color;
				return true;
			}
		}
	}
	//表示没有变化
	if(m_nCurValue == nValue)
		return false;

	m_ColorCurFont = m_ColorDefaultFont;	 
	return true;
}
void XTextOutColor::SetCurTextColor(COLORREF ref)
{
	m_ColorCurFont =ref;
}
void XTextOutColor::ReSetTextColor()
{
	m_ColorCurFont = m_ColorDefaultFont;
}

void XTextOutColor::DrawInfoEx(CDC * pDC ,CRect rt)
{
	if(m_sTitle.empty()) 
		return;

	CFont font;
	
	font.CreateFont(m_nFontSize,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,m_sFontName.c_str());

	CFont * OldFont=pDC->SelectObject(&font);
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_ColorCurFont);
	
	if(m_nAlign>=0)
		pDC->DrawText(m_sTitle.c_str(),rt,m_nAlign|DT_VCENTER|DT_SINGLELINE);
	else
		pDC->TextOut(m_nX,m_nY,m_sTitle.c_str(),m_sTitle.size());

	pDC->SelectObject(OldFont);
	font.DeleteObject();	  
}

