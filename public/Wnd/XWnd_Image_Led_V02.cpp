#include "StdAfx.h"
#include "XWnd_Image_Led_V02.h"


XWnd_Image_Led_V02::XWnd_Image_Led_V02(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo =info;
	m_fScale =fScale;

}
XWnd_Image_Led_V02::XWnd_Image_Led_V02(void)
{
}


XWnd_Image_Led_V02::~XWnd_Image_Led_V02(void)
{
	for(int i =0,end = m_TextOutVec.size();i<end;i++)
	{
		delete_safe(m_TextOutVec[i]);
		m_TextOutVec[i] =NULL;
	}
	m_TextOutVec.clear();
}


void XWnd_Image_Led_V02::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow )
{
	CRect rt;
	rt.left = m_ctrlInfo.nX*m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	BOOL B = Create(0,"", WS_CHILD | WS_VISIBLE  ,rt,pParentWnd,m_nCtrlId);

}

void XWnd_Image_Led_V02::DecodeXml()
{  

	CMarkup xml(m_ctrlInfo.sAttr.c_str());

	if(xml.FindElem("Attr"))
	{
		//是否背景色
		m_bBackColor = atoi(xml.GetAttrib("bBackColor"));

		m_ColorBack = atoi(xml.GetAttrib("ColorBack"));
		if(m_ColorBack==0)
			m_ColorBack =RGB(28,28,28);

		m_ColorLine = atoi(xml.GetAttrib("ColorLine"));
		if(m_ColorLine==0)
			m_ColorLine=RGB(255,255,255);

		m_ColorFont = atoi(xml.GetAttrib("ColorFont"));

		m_nFontSize_Define = atoi(xml.GetAttrib("FontSize"));
		if(m_nFontSize_Define>100 || m_nFontSize_Define<0)
			m_nFontSize_Define =20;
		
		m_nAlign = atoi(xml.GetAttrib("Align"));
		if(m_nAlign<0)
			m_nAlign =DT_CENTER;

		m_sFontName = xml.GetAttrib("FontName");
		m_sFontName= m_sFontName.empty()?m_sFontName:"黑体";

		//是否有背景图
		m_bBackBmp = atoi(xml.GetAttrib("bBackBmp"));
		m_nBitX = atoi(xml.GetAttrib("BackBmpX"));
		m_nBitY =atoi(xml.GetAttrib("BackBmpY"));		 
	}

	m_sTitle = m_ctrlInfo.sTitle;
	
	m_nFontSize = m_nFontSize_Define *m_fScale;

	QueryRefreshEvent();
	QueryTextOut();
 
}

void XWnd_Image_Led_V02::SetSelect(bool bSel)
{
	m_bSel =bSel;
	this->Invalidate();
}
void XWnd_Image_Led_V02::MoveWnd()
{
	int nW = m_ctrlInfo.nW*m_fScale;
	int nH = m_ctrlInfo.nH*m_fScale;
	int x = m_ctrlInfo.nX*m_fScale;
	int y = m_ctrlInfo.nY*m_fScale;

	MoveWindow(x,y, nW, nH );
	this->Invalidate();
}

void XWnd_Image_Led_V02::ReCreate()
{
	CManageDb_Sqllite::GetInstance().Query_CtrlInfo(m_ctrlInfo,m_ctrlInfo.nId);
	DecodeXml();

}
void XWnd_Image_Led_V02::OnChildDrawInfo(CDC * pDC,CRect  rt)
{
	for(int i=0,end =m_TextOutVec.size();i<end;i++)
	{
		m_TextOutVec[i]->DrawInfoEx(pDC,rt);
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

void XWnd_Image_Led_V02::OnChildLButtonUp(UINT nFlags, CPoint point)
{
	TRACE("%s name = %s\n",__FUNCTION__,m_ctrlInfo.sName.c_str());
	if(CAppData::GetInstance().m_bBuilder )	 
	{
		CAppData::GetInstance().SetCurSelectWnd(this);
	}

	CAppData::GetInstance().SetCurTipWnd(this);
}

void XWnd_Image_Led_V02::OnChildTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent ==1)
		RefreshData();
}

void XWnd_Image_Led_V02::RefreshData()
{
	bool bInvalidate = false;
	for(int i =0,end = m_TextOutVec.size();i<end;i++)
	{
		bInvalidate  |=m_TextOutVec[i]->RefreshData();		
	}

	if(m_RefreshEventVec.size()==0)
	{
		if(bInvalidate)
			Invalidate();
		return  ;
	}

	bool bFind =false;
	int nValue =0; 
	int bUpdateFlag;
	DWORD dwUpdateTime;

	if(m_GroupRefreshEventMap.size()>0)
	{
		bFind = RefreshData_group();
	}
	 
	bool b =false;

	for(int i =0,end =m_RefreshEventVec.size();i<end;i++)
	{
		if(m_RefreshEventVec[i].nSharedId==0)
			continue;
		if(!b && m_RefreshEventVec[i].nValue1 ==-1)
		{
			if( g_XShared.ReadData_Int(m_RefreshEventVec[i].nSharedId,nValue,bUpdateFlag,dwUpdateTime))
			{
				if(nValue ==0)
					break;
				b=true;
				continue;
			}
			 
			break;
		}

		if(m_RefreshEventVec[i].nSharedId!=0 && g_XShared.ReadData_Int(m_RefreshEventVec[i].nSharedId,nValue,bUpdateFlag,dwUpdateTime))
		{
			if(nValue ==1)
			{
				bFind =true;
				SetPicFileName(m_RefreshEventVec[i].sPath.c_str());					  
				break;
			}
		}
	}

	if(!bFind)
		SetPicFileName(m_sDefaultPicFileName);
	if(bInvalidate)
		Invalidate();

	return  ;
}
bool XWnd_Image_Led_V02::RefreshData_group()
{
	int nValue =0;
	int bUpdateFlag;
	DWORD dwUpdateTime;

	for(int i =0,end =m_RefreshEventVec.size();i<end;i++)
	{
		if(m_RefreshEventVec[i].nSharedId==0) //配置错误
			continue;

		if(g_XShared.ReadData_Int(m_RefreshEventVec[i].nSharedId,nValue,bUpdateFlag,dwUpdateTime) &&nValue==m_RefreshEventVec[i].nValue1)
		{
			map<int,vector<__Ctrl_Event>>::iterator it = m_GroupRefreshEventMap.find(m_RefreshEventVec[i].nId);
			if(it==m_GroupRefreshEventMap.end() ||it->second.size()==0 ) //配置错误
				continue; 

			for(int k =0,end1 = it->second.size();k<end1;k++)
			{
				if(g_XShared.ReadData_Int(it->second[k].nSharedId,nValue,bUpdateFlag,dwUpdateTime) && nValue ==1)
				{
					SetPicFileName(it->second[k].sPath);				
					return true ;
				}
			}
			SetPicFileName(it->second[0].sPath);
			return true;
		}
	}

	return false;
}

void XWnd_Image_Led_V02::QueryRefreshEvent()
{
	m_GroupRefreshEventMap.clear();

	//获取非分组的ID
	CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_RefreshEventVec,m_ctrlInfo.nId,EVENT_TYPE_REFRESH,-1);
	if(m_RefreshEventVec.size() ==0)
	{ 
		//分组父信息
		CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_RefreshEventVec,m_ctrlInfo.nId,EVENT_TYPE_REFRESH,0);
		for(int i =0,end = m_RefreshEventVec.size();i<end;i++)
		{
			QueryGroupRefreshEvent(m_RefreshEventVec[i].nId);
		}
	}

 	if(m_RefreshEventVec.size()>0)
	{	
		if( m_RefreshEventVec[0].nGroup ==-1)
		{
			SetDefulatPicFileName(m_RefreshEventVec[0].sPath);				
		}
		else  
		{
			map<int,vector<__Ctrl_Event>>::iterator it = m_GroupRefreshEventMap.find( m_RefreshEventVec[0].nId);
			if(it != m_GroupRefreshEventMap.end())
			{					
				SetDefulatPicFileName(it->second[0].sPath);
			}
		}
	}

}

void XWnd_Image_Led_V02::QueryGroupRefreshEvent(int nGroupId)
{
	vector<__Ctrl_Event>vec;
	CManageDb_Sqllite::GetInstance().Query_CtrlEvent(vec,m_ctrlInfo.nId,EVENT_TYPE_REFRESH,nGroupId);
	if(vec.size()>0)
		m_GroupRefreshEventMap[nGroupId].swap(vec);
}

void XWnd_Image_Led_V02::QueryTextOut()
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

void XWnd_Image_Led_V02::DealCloseEvent()
{
	for(int i =0,end = m_TextOutVec.size();i<end;i++)		 
		m_TextOutVec[i]->ReSetTextColor();	
}