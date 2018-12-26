#include "StdAfx.h"
#include "XWnd_Image_Rotate_V02.h"


XWnd_Image_Rotate_V02::XWnd_Image_Rotate_V02(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo =info;
	m_fScale =fScale;
}


XWnd_Image_Rotate_V02::~XWnd_Image_Rotate_V02(void)
{

}
  void XWnd_Image_Rotate_V02::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow)
{
	CRect rt;
	rt.left = m_ctrlInfo.nX*m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	BOOL B = Create(0,"", WS_CHILD | WS_VISIBLE  ,rt,pParentWnd,m_nCtrlId);
}

void XWnd_Image_Rotate_V02::DecodeXml()
{
	CMarkup xml(m_ctrlInfo.sAttr.c_str());

	char lpbuf[64];
	if(xml.FindElem("Attr"))
	{
		
		//是否背景色
		m_Attr.bShowBackColor = atoi(xml.GetAttrib("bBackColor"));
		

		//背景色
		m_Attr.colorBack = atoi(xml.GetAttrib("ColorBack"));
		if(m_Attr.colorBack==0)
			m_Attr.colorBack =RGB(28,28,28);
		
		//边线色
		m_Attr.colorLine = atoi(xml.GetAttrib("ColorLine"));		
		if(m_Attr.colorLine==0)
			m_Attr.colorLine =RGB(28,28,28);
	 
		//最小、大值
		m_Attr.fValueMin = atof(xml.GetAttrib("fValueMin"));
		m_Attr.fValueMax = atof(xml.GetAttrib("fValueMax"));


		//显示共享内存值，是否显示，字体，字号，字体颜色，位置X,Y
		m_Attr.bShowText = atoi(xml.GetAttrib("bShowText"));
		if(m_Attr.bShowText)
		{
			m_Attr.sFontName = xml.GetAttrib("FontName");
			m_sFontName= m_Attr.sFontName.empty()?m_Attr.sFontName:"黑体";
			
			m_Attr.nFontSize  = atoi(xml.GetAttrib("FontSize"));
			m_nFontSize = m_Attr.nFontSize ==0? 16:m_Attr.nFontSize;

			m_Attr.colorFont =atoi(xml.GetAttrib("ColorFont"));			
			m_ColorFont =m_Attr.colorFont==0?RGB(28,28,28):m_Attr.colorFont;

			m_Attr.nFontX =atoi(xml.GetAttrib("nFontX"));	
			m_Attr.nFontY =atoi(xml.GetAttrib("nFontY"));

			m_Attr.nFontW =atoi(xml.GetAttrib("nFontW"));	
			m_Attr.nFontH =atoi(xml.GetAttrib("nFontH"));

			if(m_Attr.nFontX<0)
				m_nAlign =DT_CENTER;
			sprintf(lpbuf,"%.2f",m_Attr.fValueMin);
			m_strOutValue = lpbuf;			
		}
		
		//背景图，开始、结束角度，位置X,Y
		m_Attr.sPicName =xml.GetAttrib("sPicName");
		m_Attr.nAngleStart = atoi(xml.GetAttrib("nAngleStart"));
		m_Attr.nAngleEnd = atoi(xml.GetAttrib("nAngleEnd"));
		
		m_Attr.nPicX = atoi(xml.GetAttrib("nPicX"));
		m_Attr.nPicY = atoi(xml.GetAttrib("nPicY"));
		

	}
	
	m_bBackColor = m_Attr.bShowBackColor ;
	m_ColorBack = m_Attr.colorBack;
	m_ColorLine = m_Attr.colorLine;
	
	SetPicFileName(m_Attr.sPicName);
	SetRotate(m_Attr.nAngleStart);

	m_nBitX = m_Attr.nPicX;
	m_nBitY = m_Attr.nPicY;

	QueryTextOut();
}

 void XWnd_Image_Rotate_V02::SetSelect(bool bSel)
{
	m_bSel =bSel;
	this->Invalidate();
}
 
void XWnd_Image_Rotate_V02::MoveWnd()
{
	int nW = m_ctrlInfo.nW*m_fScale;
	int nH = m_ctrlInfo.nH*m_fScale;
	int x = m_ctrlInfo.nX*m_fScale;
	int y = m_ctrlInfo.nY*m_fScale;

	MoveWindow(x,y, nW, nH );
	this->Invalidate();

}
  void XWnd_Image_Rotate_V02::ReCreate()
{
	CManageDb_Sqllite::GetInstance().Query_CtrlInfo(m_ctrlInfo,m_ctrlInfo.nId);
	DecodeXml();
}

void XWnd_Image_Rotate_V02::OnChildDrawInfo(CDC * pDC,CRect  rt)
{
	for(int i=0,end =m_TextOutVec.size();i<end;i++)
	{
		m_TextOutVec[i]->DrawInfoEx(pDC,rt);
	}
	if(m_Attr.bShowText)
	{
		CFont font;
		CFont *pFont;
		font.CreateFont(m_nFontSize,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,m_sFontName.c_str());

		CFont * OldFont=pDC->SelectObject(&font);

		pDC->SetTextColor(m_ColorFont);
		pDC->SetBkMode(TRANSPARENT);

		CRect rtTxt(m_Attr.nFontX,m_Attr.nFontY,m_Attr.nFontX+m_Attr.nFontW,m_Attr.nFontY+m_Attr.nFontH);
		pDC->DrawText(m_strOutValue.c_str(),rtTxt,DT_VCENTER|DT_CENTER|DT_SINGLELINE);

		font.DeleteObject();	  
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
}

void XWnd_Image_Rotate_V02::OnChildLButtonUp(UINT nFlags, CPoint point)
{
	TRACE("%s name = %s\n",__FUNCTION__,m_ctrlInfo.sName.c_str());
	if(CAppData::GetInstance().m_bBuilder )	 
	{
		CAppData::GetInstance().SetCurSelectWnd(this);
	}

	CAppData::GetInstance().SetCurTipWnd(this);
}

void XWnd_Image_Rotate_V02::OnChildTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent ==1)
		RefreshData();
}
void XWnd_Image_Rotate_V02::QueryRefreshEvent()
{
}
void XWnd_Image_Rotate_V02::QueryTextOut()
{
	vector<__TextAttr_V02>vec;
	CManageDb_Sqllite::GetInstance().Query_CtrlText(vec,m_ctrlInfo.nId);

	for(int i =0,end = m_TextOutVec.size();i<end;i++)
	{
		delete_safe(m_TextOutVec[i]);
		m_TextOutVec[i] =NULL;
	}
	vector<XTextOutColor*>().swap(m_TextOutVec);

	for(int i=0,end =vec.size();i<end;i++)
	{
		XTextOutColor *  text =new XTextOutColor(m_fScale,vec[i]);
		text->InitData();
		m_TextOutVec.push_back(text);
	}
}

void XWnd_Image_Rotate_V02::RefreshData()
{
	bool bInvalidate = false;
	
	//共享内存值很多是浮点型，暂时不处理
	//for(int i =0,end = m_TextOutVec.size();i<end;i++)
	//{
	//	
	//	//bInvalidate  |=m_TextOutVec[i]->RefreshData();		
	//}

	if(m_ctrlInfo.nSharedId!=0)
	{	
		string sValue;
		int nDataType;
		int bUpdateFlag;
		DWORD dwUpdateTime;

		if(!g_XShared.ReadData_ToString(m_ctrlInfo.nSharedId,sValue,nDataType,bUpdateFlag,dwUpdateTime))
		{
			LOG_Out_3(LOGNAME_HK_DataProcessSys,__FUNCTION__,__LINE__,"ReadData_ToString(%d) 读取共享内置值失败",m_ctrlInfo.nSharedId);
		}
		else if(bUpdateFlag)
		{
			LOG_Out_4(LOGNAME_HK_DataProcessSys,__FUNCTION__,__LINE__,LOG_Level_general,"ReadData_ToString(%d) return true  sValue =%s,nDataType = %d",m_ctrlInfo.nSharedId,sValue.c_str(),nDataType);
			if(nDataType ==DATA_TYPE_INT || nDataType == DATA_TYPE_FLOAT ||nDataType ==DATA_TYPE_DOUBLE)
			{
				float fValue = atof(sValue.c_str());
				if(m_Attr.fValueMax-m_Attr.fValueMin >1)
				{
					float fRotate = m_Attr.nAngleStart+(m_Attr.nAngleEnd - m_Attr.nAngleStart) * fValue /(m_Attr.fValueMax-m_Attr.fValueMin);
					SetRotate(fRotate);	
					bInvalidate =true;
				}
				else
					LOG_Out_4(LOGNAME_HK_DataProcessSys,__FUNCTION__,__LINE__,LOG_Level_general,"m_Attr.fValueMax（%.2f）-m_Attr.fValueMin（%.2f） <=1 数据配置有误",m_Attr.fValueMax,m_Attr.fValueMin);
			}

			m_strOutValue = sValue;
		}
	}
	
	if(bInvalidate)
		Invalidate();
}