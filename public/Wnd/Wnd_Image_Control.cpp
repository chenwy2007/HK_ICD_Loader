#include "StdAfx.h"
#include "Wnd_Image_Control.h"


Wnd_Image_Control::Wnd_Image_Control(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo = info;
	m_nSizeFont_Define =80;
	m_fScale = fScale;

}


Wnd_Image_Control::~Wnd_Image_Control(void)
{
}
BEGIN_MESSAGE_MAP(Wnd_Image_Control, CBaseImage)
 
END_MESSAGE_MAP()

 
void Wnd_Image_Control::OnMyLButtonUp(UINT nFlags, CPoint point)
{
	if(CAppData::GetInstance().m_bBuilder )	 
	{
		CAppData::GetInstance().SetCurSelectWnd(this);
		//CBaseImage::OnLButtonUp(nFlags, point);
	} 
	else if(m_ctrlInfo.nType ==WND_type_switch_button_image )  //允许操作
	{  
		for(int i=0,end = m_EventClickVec.size();i<end;i++)
		{
			__Copy_Data_Info info;
			info.nShareId = m_EventClickVec[i].nSharedId;

			info.nValue =  m_EventClickVec[i].nValue1;
			m_EventClickVec[i].nValue1++;
			if(m_EventClickVec[i].nValue1>1)
				m_EventClickVec[i].nValue1=0;

			SendMsgToDataProcess(COPY_DATA_CMD_Event,sizeof(__Copy_Data_Info),(void*)&info);
		} 
	 
	 
	 }
  
	//CBaseImage::OnLButtonUp(nFlags, point);

	CAppData::GetInstance().SetCurTipWnd(this);
}
void Wnd_Image_Control::DrawInfoEx(CDC *pDC,CRect rt)
{
	if(!CAppData::GetInstance().m_bBuilder || !m_bSel )	 
		return;

	CPen pen(PS_DASHDOTDOT,0,RGB(255,0,0));

	CPen *oldPen =pDC->SelectObject(&pen);

	pDC->MoveTo(0,0);
	pDC->LineTo(0,rt.bottom-1);
	pDC->LineTo(rt.right-1,rt.bottom-1);
	pDC->LineTo(rt.right-1,0);
	pDC->LineTo(0,0);
	pDC->SelectObject(oldPen);
}
void Wnd_Image_Control::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow)
{
	m_nCtrlId = nCtrlId;
	CRect rt;
	rt.left = m_ctrlInfo.nX * m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	Create(m_strTitle.c_str(), WS_VISIBLE|WS_CHILD|SS_NOTIFY,rt,pParentWnd,m_nCtrlId);
}
void Wnd_Image_Control::MoveWnd()
{
	MoveWindow(m_ctrlInfo.nX *m_fScale,m_ctrlInfo.nY * m_fScale,m_ctrlInfo.nW * m_fScale,m_ctrlInfo.nH*m_fScale);
	Invalidate();
}
/*
void Wnd_Image_Control::DecodeXml()
{
	m_strTitle = m_ctrlInfo.sTitle;

	CMarkup xml(m_ctrlInfo.sAttr.c_str());
	CString str;

	if(xml.FindElem("Attr"))
	{  
		str = xml.GetAttrib("FontColor");
		if(!str.IsEmpty())
			m_colorFont = atoi(str);
		str = xml.GetAttrib("FontSize");
		if(!str.IsEmpty())
		{
			m_nSizeFont_Define =atoi(str);
			m_nSizeFont = m_nSizeFont_Define*m_fScale;
		}
		int n=0;
		while(xml.FindChildElem("Shared") && n<2)
		{
			str = xml.GetChildAttrib("Value");
			if(str.IsEmpty())
				continue;
			 
		}
	} 
}
*/


  void Wnd_Image_Control::DecodeXml()
  {
	 if(m_ctrlInfo.nType == WND_type_switch_static_image)
	  {
		//  m_bEnable =false;
	  }
	  else 
	  {
		m_strTitle = m_ctrlInfo.sTitle.c_str();	
	  }

	  CMarkup xml(m_ctrlInfo.sAttr.c_str());
	  if(xml.FindElem("Attr"))
	  { 
		  CString  str;
		  str = xml.GetAttrib("FontColor");
		  if(!str.IsEmpty())
		  {
			  m_colorFont = atoi(str);
		  }

		  str = xml.GetAttrib("FontSize");

		  if(!str.IsEmpty())
		  {
			  m_nSizeFont_Define =atoi(str);
			  m_nSizeFont = m_nSizeFont_Define*m_fScale;
		  }
	   }
	   
	
	CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_EventClickVec,m_ctrlInfo.nId,EVENT_TYPE_CLICK);
	CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_CloseEventVec,m_ctrlInfo.nId,EVENT_TYPE_CLOSE);
	QueryRefreshEvent();
 
	 
  }
 
  void Wnd_Image_Control::RefreshData()
  {
	  //如果没有共享内存值，则返回
	  if(m_RefreshEventVec.size()==0)
		  return ;
	   int nValue =0;
	   
	 //分组显示
	 if(m_GroupRefreshEventMap.size()>0)
	 {
		 RefreshData_group();
		 return ;
	 }
	 //led灯
	 if(m_ctrlInfo.nType == WND_type_switch_static_image)
	 {
		 int nValue;
		 int bUpdateFlag;
		 DWORD dwUpdateTime;

		 for(int i =0,end =m_RefreshEventVec.size();i<end;i++)
		 {
			 if(m_RefreshEventVec[i].nSharedId==0)
				 continue; 
			 if(g_XShared.ReadData_Int(m_RefreshEventVec[i].nSharedId,nValue,bUpdateFlag,dwUpdateTime))
			 {				 
				 if(nValue == m_RefreshEventVec[i].nValue1)
				 {
					 SetPicFileName(m_RefreshEventVec[i].sPath); 
					 return ;
				 } 
			 }
		 }

		 SetPicFileName(m_RefreshEventVec[0].sPath); 
		 return ;
	 }
	 else  if(m_ctrlInfo.nType == WND_type_Image_Interval)
	 { 
		//区级显示
		 char lpbuf[256];
		 int nDataType;
		 int nValue=0;
		 int bUpdateFlag;
		 DWORD dwUpdateTime;
		 int nDataSize;
		 for(int i =0,end =m_RefreshEventVec.size();i<end;i++)
		 {
			 if(m_RefreshEventVec[i].nSharedId==0)
				 continue;
			 memset(lpbuf,0,256);

			 if(g_XShared.ReadData_Info(m_RefreshEventVec[i].nSharedId,lpbuf,nDataType,nDataSize,bUpdateFlag,dwUpdateTime))
			 { 
				 if(!bUpdateFlag)
					 return;

				 if(nDataType ==DATA_TYPE_INT)
				 {
					 memcpy(&nValue,lpbuf,sizeof(int));  
					 nValue *=BASE_NUM;		
				 }
				 else if(nDataType ==DATA_TYPE_DOUBLE)
				 {
					 double dValue;
					 memcpy(&dValue,lpbuf,sizeof(double));
					 nValue = dValue*BASE_NUM;					
				 } 
				 if(m_RefreshEventVec[i].nValue1>=nValue && nValue<m_RefreshEventVec[i].nValue2)
				 {
					 SetPicFileName(m_RefreshEventVec[i].sPath); 
					 return ;
				 } 
			 }
		 }

		SetPicFileName(m_RefreshEventVec[0].sPath); 
	 }
  }
  void Wnd_Image_Control::RefreshData_group()
  {
	  if(m_ctrlInfo.nType == WND_type_switch_static_image)
	  {
		  RefreshData_group_switch_static_image();
	  }
	  else if(m_ctrlInfo.nType == WND_type_Image_Interval)
	  {
		  RefreshData_group_Image_Interval();
	  }
	
  }
  void Wnd_Image_Control::RefreshData_group_switch_static_image()
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
						if(!bUpdateFlag)
							return;
						SetPicFileName(it->second[k].sPath);
						return ;
					}
				}
				SetPicFileName(it->second[0].sPath);
			}	 
	  }	  
  }
  void Wnd_Image_Control::RefreshData_group_Image_Interval()
  {
	  int nValue =0;
	  int nDataType ;
	  char lpbuf[256];
	  int bUpdateFlag;
	  DWORD dwUpdateTime;
	  int nDataSize;
	  for(int i =0,end =m_RefreshEventVec.size();i<end;i++)
	  {
		  //配置错误
		  if(m_RefreshEventVec[i].nSharedId==0) 
			  continue;
		 
		  //读取失败
		  if(!g_XShared.ReadData_Info(m_RefreshEventVec[i].nSharedId,lpbuf,nDataType,nDataSize,bUpdateFlag,dwUpdateTime))
				continue;
		  if(!bUpdateFlag)
			  continue;

		 nValue =GetSharedValue(lpbuf,nDataType);

		  //不在区间内
		  if(nValue<m_RefreshEventVec[i].nValue1 || nValue>=m_RefreshEventVec[i].nValue1)
			  continue;
		   
			map<int,vector<__Ctrl_Event>>::iterator it = m_GroupRefreshEventMap.find(m_RefreshEventVec[i].nId);
			//配置错误
			if(it==m_GroupRefreshEventMap.end() ||it->second.size()==0 ) 
				continue; 

			for(int k =0,end1 = it->second.size();k<end1;k++)
			{ 
				 //读取失败
				if(!g_XShared.ReadData_Info(it->second[k].nSharedId,lpbuf,nDataType,nDataSize,bUpdateFlag,dwUpdateTime))
					continue;
				if(!bUpdateFlag)
					continue;

				 nValue =GetSharedValue(lpbuf,nDataType);

				 //不在区间内
				 if(nValue<it->second[k].nValue1 || nValue>=it->second[k].nValue1)
					 continue;
  
					SetPicFileName(it->second[k].sPath);
					return ;
				 
			}
			SetPicFileName(it->second[0].sPath); 
	  }	  
  }
  
   
  void Wnd_Image_Control::SetSelect(bool bSel)
  {
	  m_bSel = bSel;
	  Invalidate(FALSE);
  }
  void Wnd_Image_Control::UpdateAttr(__ImageCtrlAttr &attr)
  {
	  m_ctrlInfo.nType = attr.nCtrlType;
	  m_ctrlInfo.sName = attr.sName;
	  m_ctrlInfo.sTitle = attr.sTitle;
	  m_ctrlInfo.nX =attr.nX;
	  m_ctrlInfo.nY = attr.nY;
	  m_ctrlInfo.nW = attr.nW;
	  m_ctrlInfo.nH= attr.nH;
	  m_nSizeFont_Define =attr.nFontSize;
		m_colorFont  =attr.fontColor;
	  
	  m_strTitle = attr.sTitle.c_str();
  
	  QueryRefreshEvent();   
  }
  void Wnd_Image_Control::QueryRefreshEvent()
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

	if(m_ctrlInfo.nType == WND_type_switch_static_image)
	{
		if(m_RefreshEventVec.size()==0)
			 return; 
		 	
		if( m_RefreshEventVec[0].nGroup ==-1)
		{
			SetPicFileName(m_RefreshEventVec[0].sPath); 
		}
		else  
		{
			map<int,vector<__Ctrl_Event>>::iterator it = m_GroupRefreshEventMap.find( m_RefreshEventVec[0].nId);
			if(it != m_GroupRefreshEventMap.end())
			{
				if(it->second.size()>0)
					SetPicFileName(it->second[0].sPath); 
			}
		} 
	}
	else if(m_ctrlInfo.nType == WND_type_switch_button_image)
	{ 
		vector<string>().swap(m_sBtPicPathVec);
		CManageDb_Sqllite::GetInstance().Query_ImageCtrl_Image(m_sBtPicPathVec,m_ctrlInfo.nId);
		 if(m_sBtPicPathVec.size()>0)
			SetPicFileName(m_sBtPicPathVec[0]);
	}
  }
  
  void Wnd_Image_Control::QueryGroupRefreshEvent(int nGroupId)
  {
	  vector<__Ctrl_Event>vec;
	  CManageDb_Sqllite::GetInstance().Query_CtrlEvent(vec,m_ctrlInfo.nId,EVENT_TYPE_REFRESH,nGroupId);
	  if(vec.size()>0)
		  m_GroupRefreshEventMap[nGroupId].swap(vec);
  }
