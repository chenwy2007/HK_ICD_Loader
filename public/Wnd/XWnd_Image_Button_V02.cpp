#include "StdAfx.h"
#include "XWnd_Image_Button_V02.h"
XWnd_Image_Button_V02::XWnd_Image_Button_V02(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo =info;
	m_fScale =fScale;

	m_nCurIndex =0;
	m_pTopBase = NULL;
}
XWnd_Image_Button_V02::XWnd_Image_Button_V02(void)
{
	m_pTopBase = NULL;
}
 
XWnd_Image_Button_V02::~XWnd_Image_Button_V02(void)
{
	for(int i =0,end = m_TextOutVec.size();i<end;i++)
	{
		delete_safe(m_TextOutVec[i]);
		m_TextOutVec[i] =NULL;
	}
	m_TextOutVec.clear();
}


void XWnd_Image_Button_V02::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow )
{
	CRect rt;
	rt.left = m_ctrlInfo.nX*m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	BOOL B = Create(0,"", WS_CHILD | WS_VISIBLE  ,rt,pParentWnd,m_nCtrlId);

}

void XWnd_Image_Button_V02::DecodeXml()
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
		m_nFontSize_Define =m_nFontSize_Define==0?20:m_nFontSize_Define;
		 		
		m_nAlign = atoi(xml.GetAttrib("Align"));
		if(m_nAlign<0)
			m_nAlign =DT_CENTER;

		m_sFontName = xml.GetAttrib("FontName");
		m_sFontName= m_sFontName.empty()?m_sFontName:"黑体";

		//是否有背景图
		m_bBackBmp = atoi(xml.GetAttrib("bBackBmp"));
		m_nBitX = atoi(xml.GetAttrib("BackBmpX"));
		m_nBitY =atoi(xml.GetAttrib("BackBmpY"));

		m_nEType = atoi(xml.GetAttrib("EventType"));
		m_nEventType = m_nEType;
	}
	
	m_nFontSize = m_nFontSize_Define;

	m_ctrlInfo.sTitle ="";
	m_sTitle = m_ctrlInfo.sTitle;
	 
	QueryTextOut();

	CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_EventClickVec,m_ctrlInfo.nId,EVENT_TYPE_CLICK);
	CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_CloseEventVec,m_ctrlInfo.nId,EVENT_TYPE_CLOSE);

	CManageDb_Sqllite::GetInstance().Query_ImageCtrl_Image(m_ImagePathMap,m_ctrlInfo.nId);
	CManageDb_Sqllite::GetInstance().Query_Ctrl_FontColor(m_ColorFontMap,m_ctrlInfo.nId,0);


	if(m_ImagePathMap.size()>0)
		SetPicFileName(m_ImagePathMap[0]);
	if(m_ColorFontMap.size())
		m_ColorFont = m_ColorFontMap[0];
	m_nCurIndex =0;

}
void XWnd_Image_Button_V02::QueryTextOut()
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
void XWnd_Image_Button_V02::SetSelect(bool bSel)
{
	m_bSel =bSel;
	this->Invalidate();
}
void XWnd_Image_Button_V02::MoveWnd()
{
	int nW = m_ctrlInfo.nW*m_fScale;
	int nH = m_ctrlInfo.nH*m_fScale;
	int x = m_ctrlInfo.nX*m_fScale;
	int y = m_ctrlInfo.nY*m_fScale;

	MoveWindow(x,y, nW, nH );
	this->Invalidate();
}

void XWnd_Image_Button_V02::ReCreate()
{
	CManageDb_Sqllite::GetInstance().Query_CtrlInfo(m_ctrlInfo,m_ctrlInfo.nId);
	DecodeXml();

}
void XWnd_Image_Button_V02::OnChildDrawInfo(CDC * pDC,CRect  rt)
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

void XWnd_Image_Button_V02::OnChildLButtonUp(UINT nFlags, CPoint point)
{
	TRACE("%s name = %s\n",__FUNCTION__,m_ctrlInfo.sName.c_str());

	if(CAppData::GetInstance().m_bBuilder )	 
	{
		CAppData::GetInstance().SetCurSelectWnd(this);
	}
	else if(m_nEType == 1)
	{
		if(g_App_StdSys.m_nStartTest==0)
		{
			AfxMessageBox("请启动测试！");
			return;
		}
		// 启动发送
		g_App_StdSys.m_bStartWrite = true;
		__Copy_Data_Info info;
		if(m_EventClickVec.size() > 0) 
		{
			info.nShareId = m_EventClickVec[0].nSharedId;
			info.nValue =1;
			SendMsgToDataProcess(COPY_DATA_CMD_Start_Write_data,sizeof(__Copy_Data_Info),(void*)&info);
		}
		SetValue(1);
	}
	// 点击停止发送
	else if(m_nEType == 2)
	{
		// 改变发送按钮背景颜色
		if(m_pTopBase)
		{
			CBaseElemCtrl* pSendButton = m_pTopBase->FindElemCtrlByEventType(1);
			if(pSendButton)
			{
				XWnd_Image_Button_V02* pBtn = (XWnd_Image_Button_V02*)pSendButton;
				pBtn->SetValue(0);
			}
		}
		// 停止发送
		g_App_StdSys.m_bStartWrite = false;
		__Copy_Data_Info info;
		if(m_EventClickVec.size() > 0) 
		{
			info.nShareId = m_EventClickVec[0].nSharedId;
			info.nValue =0;
			SendMsgToDataProcess(COPY_DATA_CMD_Start_Write_data,sizeof(__Copy_Data_Info),(void*)&info);
		}
	}
	else 
	{ 	
		CHECK_START_READ;

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
		 m_nCurIndex++;
		 SetValue(m_nCurIndex);
	}
	 
	CAppData::GetInstance().SetCurTipWnd(this);
}

void XWnd_Image_Button_V02::OnChildTimer(UINT_PTR nIDEvent)
{

}
void XWnd_Image_Button_V02::SetValue(int nIndex)
{
	 if(m_ImagePathMap.size()>1)
	 {
		 map<int,string>::iterator it = m_ImagePathMap.find(nIndex);

		 if(it==m_ImagePathMap.end())
		 { 
			 m_nCurIndex =0;
			 SetValue(m_nCurIndex);
			 return;
		 }
		 else
		 {
			SetPicFileName(it->second);
		 }
	 }

	 if(m_ColorFontMap.size()>1)
	 {
		 map<int,COLORREF>::iterator it = m_ColorFontMap.find(nIndex);

		 if(it==m_ColorFontMap.end())
		 { 
			 m_nCurIndex =0;
			 SetValue(m_nCurIndex);
			 return;
		 }
		 else
		 {
			 for(int i =0,end = m_TextOutVec.size();i<end;i++)
			 {
				 m_TextOutVec[i]->SetCurTextColor(it->second);
			 }
			 SetCurFontColor(it->second);			 
		 }
		  
	 }
	 Invalidate();
}
 void XWnd_Image_Button_V02::DealCloseEvent()
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
	// SetValue(0);

	 //for(int i =0,end = m_TextOutVec.size();i<end;i++)		 
	 // m_TextOutVec[i]->ReSetTextColor();	
 }

 void XWnd_Image_Button_V02::AlterDispStyle(int nStatus)
 {
	 // 外部停止测试时，关闭输出
	 if(nStatus == 0 && m_nEType == 2)
	 {
		 // 改变发送按钮背景颜色
		 if(m_pTopBase)
		 {
			 CBaseElemCtrl* pSendButton = m_pTopBase->FindElemCtrlByEventType(1);
			 if(pSendButton)
			 {
				 XWnd_Image_Button_V02* pBtn = (XWnd_Image_Button_V02*)pSendButton;
				 pBtn->SetValue(0);
			 }
		 }
		 // 停止发送
		 g_App_StdSys.m_bStartWrite = false;
		 __Copy_Data_Info info;
		 if(m_EventClickVec.size() > 0) 
		 {
			 info.nShareId = m_EventClickVec[0].nSharedId;
			 info.nValue =0;
			 SendMsgToDataProcess(COPY_DATA_CMD_Start_Write_data,sizeof(__Copy_Data_Info),(void*)&info);
		 }
	 }
 }
