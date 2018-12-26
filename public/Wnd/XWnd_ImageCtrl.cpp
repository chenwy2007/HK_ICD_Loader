#include "StdAfx.h"
#include "XWnd_ImageCtrl.h"


XWnd_ImageCtrl::XWnd_ImageCtrl(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo =info;
	m_fScale =fScale;
}


XWnd_ImageCtrl::~XWnd_ImageCtrl(void)
{
}
BEGIN_MESSAGE_MAP(XWnd_ImageCtrl, CImageCtrl)
	ON_WM_LBUTTONUP()	
END_MESSAGE_MAP()

void XWnd_ImageCtrl::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow )
{

	CRect rt;
	rt.left = m_ctrlInfo.nX*m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	Create("", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW ,rt,pParentWnd,m_nCtrlId);

	
}
  void XWnd_ImageCtrl::DecodeXml()
  {
	  if(m_ctrlInfo.nType == WND_type_switch_static_image)
	  {
		  m_bEnable =false;
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
			  m_TextColor = atoi(str);
		  }

		  str = xml.GetAttrib("FontSize");

		  if(!str.IsEmpty())
			  m_nFontSize =atoi(str);
	   }
	  

	
	CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_EventClickVec,m_ctrlInfo.nId,EVENT_TYPE_CLICK);
	CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_CloseEventVec,m_ctrlInfo.nId,EVENT_TYPE_CLOSE);

	QueryRefreshEvent();
	//CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_RefreshEventVec,m_ctrlInfo.nId,EVENT_TYPE_REFRESH);
	//

	//if(m_ctrlInfo.nType == WND_type_switch_static_image)
	//{
	//	/*for(int i =0,end = m_RefreshEventVec.size();i<end;i++)
	//	{
	//		AddImagePath(i,m_RefreshEventVec[i].sPath);
	//	}
	//	*/

	//	SetValue(0);
	//}
	//else if(m_ctrlInfo.nType == WND_type_switch_button_image)
	//{
	//	vector<string>vec;
	//	CManageDb_Sqllite::GetInstance().Query_ImageCtrl_Image(vec,m_ctrlInfo.nId);
	//	for(int i =0,end = vec.size();i<end;i++)
	//	{
	//		AddImagePath(i,vec[i]);
	//	}
	//	SetValue(0);
	//}
  }
 
  void XWnd_ImageCtrl::RefreshData()
  {
	  if(m_RefreshEventVec.size()==0)
		  return ;
	   int nValue =0;
	   
	 if(m_GroupRefreshEventMap.size()>0)
	 {
		 RefreshData_group();
	 }

	 BOOL bAdd =false;

	  for(int i =0,end =m_RefreshEventVec.size();i<end;i++)
	  {
		  if(m_RefreshEventVec[i].nSharedId==0)
			  continue;
		  nValue =0;
		  int bUpdateFlag;
		  DWORD dwUpdateTime;

		  if(!bAdd && m_RefreshEventVec[i].nValue1 ==-1)
		  {
				g_XShared.ReadData_Int(m_RefreshEventVec[i].nSharedId,nValue,bUpdateFlag,dwUpdateTime);
				if(nValue ==1)
				{
					bAdd = true;
					continue;
				}
				break;
		  }

		  if(m_RefreshEventVec[i].nSharedId!=0 && g_XShared.ReadData_Int(m_RefreshEventVec[i].nSharedId,nValue,bUpdateFlag,dwUpdateTime))
		  {
			  if(nValue ==1)
			  {
				  SetValue(i);
				  return  ;				  
			  }
		  }
	  }
	  
	 SetValue(0);	  
  }
  void XWnd_ImageCtrl::RefreshData_group()
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
					  SetShowImage(it->second[k].sPath);
					  return ;
				  }
			  }
			  SetShowImage(it->second[0].sPath);
		  }
		  
	  }
  }
  void XWnd_ImageCtrl::MoveWnd()
  {
	MoveWindow(m_ctrlInfo.nX *m_fScale,m_ctrlInfo.nY * m_fScale,m_ctrlInfo.nW * m_fScale,m_ctrlInfo.nH*m_fScale);
	Invalidate();
  }
 
  void XWnd_ImageCtrl::OnLButtonUp(UINT nFlags, CPoint point)
  {
	  TRACE("%s name = %s\n",__FUNCTION__,m_ctrlInfo.sName.c_str());
	  if(CAppData::GetInstance().m_bBuilder )	 
	  {
		  CAppData::GetInstance().SetCurSelectWnd(this);	
		  CButton::OnLButtonUp(nFlags, point);
	  }
	  else if(m_bEnable )  //允许操作
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
		   SetNextValue();
		 
	  }
	  CBitmapButton::OnLButtonUp(nFlags, point);
	  CAppData::GetInstance().SetCurTipWnd(this);
  }
 

  void XWnd_ImageCtrl:: DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
  {
	  __super::DrawItem(lpDrawItemStruct);

	  CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);

	  int nSavedDC=pDC->SaveDC();
	  if(!CAppData::GetInstance().m_bBuilder  || !m_bSel)	 
		  return;

	  CPen pen(PS_DASHDOTDOT,0,RGB(255,0,0)); 
	  pDC->SelectObject(&pen);

	  pDC->MoveTo(0,0);
	  pDC->LineTo(0,lpDrawItemStruct->rcItem.bottom-1);
	  pDC->LineTo(lpDrawItemStruct->rcItem.right-1,lpDrawItemStruct->rcItem.bottom-1);
	  pDC->LineTo(lpDrawItemStruct->rcItem.right-1,0);
	  pDC->LineTo(0,0);

	  pDC->RestoreDC(nSavedDC);

  }
  void XWnd_ImageCtrl::SetSelect(bool bSel)
  {
	  m_bSel = bSel;
	  Invalidate(FALSE);
  }
  void XWnd_ImageCtrl::UpdateAttr(__ImageCtrlAttr &attr)
  {
	  m_ctrlInfo.nType = attr.nCtrlType;
	  m_ctrlInfo.sName = attr.sName;
	  m_ctrlInfo.sTitle = attr.sTitle;
	  m_ctrlInfo.nX =attr.nX;
	  m_ctrlInfo.nY = attr.nY;
	  m_ctrlInfo.nW = attr.nW;
	  m_ctrlInfo.nH= attr.nH;
	  m_nFontSize = attr.nFontSize;
	  m_TextColor = attr.fontColor;
	  m_strTitle = attr.sTitle.c_str();

	  QueryRefreshEvent();
	  SetValue(0,true);
 	 // CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_RefreshEventVec,m_ctrlInfo.nId,EVENT_TYPE_REFRESH);

	  //for(int i =0,end = m_RefreshEventVec.size();i<end;i++)
	  //{
		 // AddImagePath(i,m_RefreshEventVec[i].sPath);
	  //}
	  //SetValue(0);
  }
  void XWnd_ImageCtrl::QueryRefreshEvent()
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
		if(m_RefreshEventVec.size()>0)
		{	
			if( m_RefreshEventVec[0].nGroup ==-1)
				{
					for(int i =0,end = m_RefreshEventVec.size();i<end;i++)
						AddImagePath(i,m_RefreshEventVec[i].sPath);
				}
			else  
			{
				map<int,vector<__Ctrl_Event>>::iterator it = m_GroupRefreshEventMap.find( m_RefreshEventVec[0].nId);
				if(it != m_GroupRefreshEventMap.end())
				{
					if(it->second.size()>0)
						SetShowImage(it->second[0].sPath);
				}
			}
		}


	}
	else if(m_ctrlInfo.nType == WND_type_switch_button_image)
	{
		vector<string>vec;
		CManageDb_Sqllite::GetInstance().Query_ImageCtrl_Image(vec,m_ctrlInfo.nId);
		for(int i =0,end = vec.size();i<end;i++)
		{
			AddImagePath(i,vec[i]);
		}
		SetValue(0);
	}

  }
  
  void XWnd_ImageCtrl::QueryGroupRefreshEvent(int nGroupId)
  {
	  vector<__Ctrl_Event>vec;
	  CManageDb_Sqllite::GetInstance().Query_CtrlEvent(vec,m_ctrlInfo.nId,EVENT_TYPE_REFRESH,nGroupId);
	  if(vec.size()>0)
		  m_GroupRefreshEventMap[nGroupId].swap(vec);
  }


void XWnd_ImageCtrl::DealCloseEvent()
{
	for(int i =0,end = m_CloseEventVec.size();i<end;i++)
	{
		__Copy_Data_Info info;
		info.nShareId = m_CloseEventVec[i].nSharedId;
		info.nValue = m_CloseEventVec[i].nValue1;
		SendMsgToDataProcess(COPY_DATA_CMD_Event,sizeof(__Copy_Data_Info),(void*)&info);
	}	
	for(int i=0,end = m_EventClickVec.size();i<end;i++)
	{
		if(m_EventClickVec[i].nValue1 ==0)
		{ 
			__Copy_Data_Info info;
			info.nShareId = m_EventClickVec[i].nSharedId;
			info.nValue =0;
			SendMsgToDataProcess(COPY_DATA_CMD_Event,sizeof(__Copy_Data_Info),(void*)&info);
		}
	}
	SetValue(0);
}

void XWnd_ImageCtrl::DealInitEvent()
{
	/*int size = m_EventClickVec.size();
	for(int i =0,end = m_EventClickVec.size();i<end;i++)
	{

		__Copy_Data_Info info;
		info.nShareId = m_InitEventVec[i].nSharedId;
		info.nValue = m_InitEventVec[i].nValue1;
		SendMsgToDataProcess(COPY_DATA_CMD_Event,sizeof(__Copy_Data_Info),(void*)&info);

	}*/
}