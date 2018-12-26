// XWnd_Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XWnd_Dlg.h"

#include "afxdialogex.h"


// XWnd_Dlg 对话框

IMPLEMENT_DYNAMIC(XWnd_Dlg, CDialogEx)

XWnd_Dlg::XWnd_Dlg(int nId,CWnd* pParent /*=NULL*/)
	: CBaseDialog( nId, pParent)
{
	m_nDutIndex=-1;
	m_nTimer =-1;
	m_nEventNo =0;
	m_pToolTip = NULL;
 
}
 

XWnd_Dlg::~XWnd_Dlg()
{
	ReleaseRec();

	if(m_pToolTip)
		m_pToolTip->DestroyWindow();
	delete_safe(m_pToolTip);
}

void XWnd_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(XWnd_Dlg, CBaseDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//ON_WM_NCHITTEST()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void XWnd_Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(CAppData::GetInstance().m_bBuilder)
	{
		CAppData::GetInstance().SetCurPoint(point.x,point.y);
	}

}
//LRESULT XWnd_Dlg::OnNcHitTest(CPoint point)
//{
//	UINT nHitTest = CBaseDialog::OnNcHitTest(point);
// 
//	if (nHitTest == HTCLIENT)
//    {
//       return HTCAPTION;
//    }
//    else
//       return nHitTest;
//}
void XWnd_Dlg::OnTimer(UINT_PTR nIDEvent)
{

	if(nIDEvent ==1)
	{
		KillTimer(1);
		DealTimer();

		SetTimer(1,m_nTimer,0);
	}

	CDialogEx::OnTimer(nIDEvent);
}
void XWnd_Dlg::DealTimer()
{
	//判断是否显示
	//if(m_ctrlInfo.nInterfaceId==-1)
	{		 
		ShowWnd(false);
		DealInitEvent();
		//刷新数据
		RefreshData();
	}
}
void XWnd_Dlg::RefreshData()
{
	if(!IsWindowVisible())
		return ;
	for(int i =0, end =m_CtrlVec.size();i<end;i++)
	{ 
		m_CtrlVec[i]->RefreshData();
	}
}

void XWnd_Dlg::DecodeXml()
{
	SetTitleTxt(m_ctrlInfo.sTitle.c_str());
	m_nDutIndex = m_ctrlInfo.nIndex-1;

	CMarkup xml(m_ctrlInfo.sAttr.c_str());

	CString str;
	if(xml.FindElem("Attr"))
	{
		str = xml.GetAttrib("BackColor");		
		if(!str.IsEmpty())
			sscanf(str,"%d",&m_colorBack);		 
		str = xml.GetAttrib("DutIndex");
	 
		
		str =xml.GetAttrib("Timer");
		if(!str.IsEmpty())
			m_nTimer = atoi(str);

		if(xml.FindChildElem("Title"))
		{

			str=xml.GetChildAttrib("Height");
			if(!str.IsEmpty())
				m_nTopHeight= atoi(str);
			
			str=xml.GetChildAttrib("Color");
			if(!str.IsEmpty())
				sscanf(str,"%d",&m_colorTitle);						

			str=xml.GetChildAttrib("FontColor");
			if(!str.IsEmpty())
				sscanf(str,"%d",&m_colorFont);						

			str=xml.GetChildAttrib("FontSize");
			if(!str.IsEmpty())
				sscanf(str,"%d",&m_nFontSize);		
		}		 
	}

}
void  XWnd_Dlg::SetScale(float fScale)
{	
	 m_nTopHeight *=fScale;
	 m_nBottomHeight*=fScale;
	CBaseElemCtrl::SetScale(fScale);	

}
void XWnd_Dlg::SetSelect(bool bSel)
{
	CBaseElemCtrl::SetSelect(bSel);
	this->Invalidate();

}
 void XWnd_Dlg::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow  )
{	  
	m_nCtrlId = nCtrlId;
	Create(nCtrlId,pParentWnd);

	// debug
	//Create(IDD_ABOUTBOX_HKSTDSYSOCX,pParentWnd);
	 
	MoveWindow(m_ctrlInfo.nX,m_ctrlInfo.nY, m_ctrlInfo.nW, m_ctrlInfo.nH );

	if(nShow==SW_SHOW)
	{
		bool b = CAppData::GetInstance().IsDutShow(m_nDutIndex);
		ShowWindow(b ?SW_SHOW:SW_HIDE);
	}
	else 
		ShowWindow(nShow);

 }

 BOOL XWnd_Dlg::OnInitDialog()
 {
	 __super::OnInitDialog();
	 
	 SetTitleTxt(m_ctrlInfo.sTitle.c_str());
	 
	 //m_tooltip.Create(this);

	 ////设定文字的颜色
	 //m_tooltip.SetTipTextColor(RGB(0,0,255));
	 //m_tooltip.SetTipBkColor( RGB(255,255,255)); //设置提示框的背景颜色

	 ////设定提示文字在控件上停留的时间
	 //m_tooltip.SetDelayTime(150);             
	 //m_tooltip.Activate(TRUE);	//设置是否启用提示

	 MoveCtrl();
	 if(!CAppData::GetInstance().m_bBuilder)
 	{
		 if(m_ctrlInfo.nInterfaceId ==-1)	 
			InitCtrl();

		if(m_nTimer>0 && g_App_StdSys.m_nStartTest==1)
			 SetTimer(1,m_nTimer,0);
	 }

	 return TRUE;  
 }
 void XWnd_Dlg::MoveCtrl()
 {
	 //-1：弹出窗口，-2，独立子窗口
	 if(m_ctrlInfo.nInterfaceId ==-1 || m_ctrlInfo.nInterfaceId==-2)	 
	 {
		 if(m_ctrlInfo.nInterfaceId==-1)
		 {
			SetSideWidth(30,20,5);
			SetEnableBt(TRUE,FALSE);
		 }
		 else 
		 {
			 SetSideWidth(0,0,0);
			 SetEnableBt(false,FALSE);
		 }
		 int cx = GetSystemMetrics(SM_CXFULLSCREEN);  
		 int cy = GetSystemMetrics(SM_CYFULLSCREEN);
		 int x = (cx-m_ctrlInfo.nW)/2;
		 int y = (cy-m_ctrlInfo.nH)/2;
		 x=x>0?x:0;
		 y =y>0?y:0;
		 MoveWindow(x,y,m_ctrlInfo.nW,m_ctrlInfo.nH);
	 }	
 }
 void XWnd_Dlg::MoveWnd()
 {

	 int nW = m_ctrlInfo.nW*m_fScale;
	 int nH = m_ctrlInfo.nH*m_fScale;
	 int x = m_ctrlInfo.nX*m_fScale;
	 int y = m_ctrlInfo.nY*m_fScale;
	 
	 MoveWindow(x,y, nW, nH );
	 this->Invalidate();
 }
 void XWnd_Dlg::MoveWnd(CRect clientRect)
 {
	 int nW = m_ctrlInfo.nW*m_fScale;
	 int nH = m_ctrlInfo.nH*m_fScale;
	 int x = clientRect.left;
	 int y = clientRect.top;
	 if(clientRect.Width()>nW || clientRect.Height()>nH)
	 {
		x = (clientRect.Width()-nW)/2 +clientRect.left;
		y = (clientRect.Height()-nH)/2 +clientRect.top;
	 }
	 MoveWindow(x,y, nW, nH );
	 this->Invalidate();
 }
 void XWnd_Dlg::ReSizeCtrl(CBaseElemCtrl * pCtrl)
 {
	 bool bFind =false;
	 if(pCtrl->m_ctrlInfo.nInterfaceId != m_ctrlInfo.nId)
	 {
		 for(int i=0,end = m_CtrlVec.size();i<end;i++)
		 {
			 if(m_CtrlVec[i]->m_ctrlInfo.nType == WND_type_tab || m_CtrlVec[i]->m_ctrlInfo.nId == pCtrl->m_ctrlInfo.nInterfaceId)
			 { 
				 m_CtrlVec[i]->ReSizeCtrl(pCtrl);
			 }			 
		 }
		 return;
	 }
	 pCtrl->ReCreate();
	 pCtrl->MoveWnd();
	 this->Invalidate();

	  
 }
 void XWnd_Dlg::InitCtrl()
 {
	 m_subCtrlVec.clear();
	 m_InitEventVec.clear();
	 m_ColseEventVec.clear();
	 m_nEventNo=0;

	 CManageDb_Sqllite::GetInstance().Query_SubCtrlInfo(m_subCtrlVec,m_ctrlInfo.nId);
	
	 CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_InitEventVec,m_ctrlInfo.nId,EVENT_TYPE_INIT);
	 CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_ColseEventVec,m_ctrlInfo.nId,EVENT_TYPE_CLOSE);

	 for(int i =0,end = m_subCtrlVec.size();i<end;i++)
	 {
		AddChildCtrl(m_subCtrlVec[i]);
	 }
	 
 }
  

 void XWnd_Dlg::OnPaint()
 {
	 CPaintDC dc(this); 
	 CDC *pDC = &dc;
	 CRect rt;
	 GetClientRect(rt);
	 OnMyPaint(&dc,rt);	 
	 
	 if(!CAppData::GetInstance().m_bBuilder )	 
		 return;
	 //弹出窗口，不画周边虚线
	 if(m_ctrlInfo.nInterfaceId ==-1)
		 return ;
	 CPen pen;
	 if(m_bSel)
		pen.CreatePen(PS_DASHDOTDOT,0,RGB(255,0,0));
	 else
		pen.CreatePen(PS_DASH,0,RGB(0,128,192));
	 CPen *oldPen =pDC->SelectObject(&pen);

	 pDC->MoveTo(0,0);
	 pDC->LineTo(0,rt.bottom-1);
	 pDC->LineTo(rt.right-1,rt.bottom-1);
	 pDC->LineTo(rt.right-1,0);
	 pDC->LineTo(0,0);
	 pDC->SelectObject(oldPen); 
 }


 void XWnd_Dlg::OnLButtonUp(UINT nFlags, CPoint point)
 { 
	 if(CAppData::GetInstance().m_bBuilder )	 
	 {
		 CAppData::GetInstance().SetCurSelectWnd(this);		  
	 }
	 else 
	 {
		 if(m_ctrlInfo.nInterfaceId !=-1)
			CAppData::GetInstance().SetCurTipWnd(this);
	 }

	 CBaseDialog::OnLButtonUp(nFlags, point);
 }

 void XWnd_Dlg::AddChildCtrl_ID(int nWndId)
 {
	 __CtrlInfo info;
	 CManageDb_Sqllite::GetInstance().Query_CtrlInfo(info,nWndId);
	 AddChildCtrl(info);
	 m_subCtrlVec.push_back(info);	 

 }

 // Stoopid win95 accelerator key problem workaround - Matt Weagle.
 //BOOL XWnd_Dlg::PreTranslateMessage(MSG* pMsg) 
 //{
	// if(pMsg->message==WM_MOUSEMOVE)
	//	 m_tooltip.RelayEvent(pMsg);

	// return __super::PreTranslateMessage(pMsg);
 //}
 
 void XWnd_Dlg::AddChildCtrl(__CtrlInfo&info)
 {	 
 	 if(info.nType ==0)
	 {
		 XWnd_Dlg *pCtrl = new XWnd_Dlg(IDD_DIALOG_BASE_CHILD);
		  pCtrl->m_ctrlInfo = info;
		 pCtrl->m_fScale =m_fScale;
		 pCtrl->SetSideWidth(0,0,0);		 
		 pCtrl->DecodeXml();
		 pCtrl->CreateCtrl(IDD_DIALOG_BASE_CHILD,this);
		 pCtrl->MoveWnd();
		 pCtrl->InitCtrl();

		 if(CAppData::GetInstance().IsDutShow(pCtrl->m_nDutIndex))
 			 pCtrl->ShowWindow(SW_SHOW);
	
		 
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);	

	 } else if(info.nType ==1 )
	 { 
		 XWnd_Button * pCtrl = new XWnd_Button(info,m_fScale);		 
		 pCtrl->DecodeXml();
		 pCtrl->CreateCtrl(GetCtrlId(),this);		 
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);	  
		 //m_tooltip.AddTool(pCtrl, "很好");		
	 } 
	 else if(info.nType  == 2)
	 {
		 XWnd_Static *pCtrl = new XWnd_Static(info,m_fScale);		 
		 pCtrl->DecodeXml();		 
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 //pCtrl->SetParentBase(this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);
	 }
	 else if(info.nType == WMD_type_digital)
	 {
		 XWnd_DigitalStatic *pCtrl = new XWnd_DigitalStatic(info,m_fScale);		 
		 pCtrl->DecodeXml();		 
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 //pCtrl->SubControl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);
	 }
	 else if(info.nType  ==WND_type_group)
	 {
		 XWnd_Group * pCtrl  =new XWnd_Group(info,m_fScale);		  
		 pCtrl->DecodeXml();		 
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);	
	 }
	 else  if(info.nType  ==WND_type_edit)
	 {
		 XWnd_Edit *pCtrl = new XWnd_Edit(info,m_fScale);		 
		 pCtrl->DecodeXml();		 
		 pCtrl->SetParentBase(this);
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);
	 }
	 else if(info.nType  ==WND_type_switch_static)
	 { 

		 XWnd_SwitchStatic *pCtrl = new XWnd_SwitchStatic(info,m_fScale);		 		 
		 pCtrl->DecodeXml();
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);	

	 }
	 else if(info.nType  ==WND_type_swtich_button)
	 {
		 XWnd_SwitchButton * pCtrl  =new XWnd_SwitchButton(info,m_fScale);

		 pCtrl->DecodeXml();
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);	
	 }
	 else if(info.nType  == WND_type_tab)
	 { 
		 XWnd_TabCtrl * pCtrl  =new XWnd_TabCtrl(info,m_fScale);		 
		 pCtrl->DecodeXml();					 
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);	
	 }
	 else if(info.nType  == WND_type_switch_static_image)
	 {
		 XWnd_ImageCtrl * pCtrl  =new XWnd_ImageCtrl(info,m_fScale);

		 pCtrl->DecodeXml();
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);	
	 }
	 else if(info.nType == WND_type_switch_button_image)
	 {
		 XWnd_ImageCtrl * pCtrl  =new XWnd_ImageCtrl(info,m_fScale);

		 pCtrl->DecodeXml();
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);	
	 } 
	 else if(info.nType == WND_type_switch_static_rectangle )
	 {
		 Wnd_Static_Rectangle *pCtrl = new Wnd_Static_Rectangle(info,m_fScale);
		 pCtrl->DecodeXml();
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);	
	 }
	 else if(info.nType == WND_type_Image_Interval)
	 {
		 Wnd_Image_Control *pCtrl = new Wnd_Image_Control(info,m_fScale);
		 pCtrl->DecodeXml();
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);	
	 } 
	 else if(info.nType == WND_type_Line)
	 {
		 Wnd_Line *pCtrl = new Wnd_Line(info,m_fScale);
		 pCtrl->DecodeXml();
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);	
	 } 
	 else if(info.nType == WND_type_image_button_V02)
	 {
		 XWnd_Image_Button_V02 *pCtrl = new XWnd_Image_Button_V02(info,m_fScale);
		 pCtrl->DecodeXml();
		 pCtrl->SetParentBase(this);
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);
	 }
	 else if(info.nType == WND_type_image_Led_V02)
	 {
		 XWnd_Image_Led_V02 * pCtrl = new XWnd_Image_Led_V02(info,m_fScale);
		 pCtrl->DecodeXml();
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);
	 }
	 else if(info.nType == WND_type_image_Rotate_V02)
	 {
		 XWnd_Image_Rotate_V02 * pCtrl = new XWnd_Image_Rotate_V02(info,m_fScale);
		 pCtrl->DecodeXml();
		 pCtrl->CreateCtrl(GetCtrlId(),this);
		 m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl);
		 
	 }
	 else
		 return ;

	 
	 CBaseElemCtrl *pCtrl = m_CtrlVec[m_CtrlVec.size()-1];
	 pCtrl->MoveWnd();
 }

 void XWnd_Dlg::DrawToolTip(RECT rect,CString sInfor,int nShow)
 {
	 ScreenToClient(&rect);

	 if(!m_pToolTip)
	 {
		 __CtrlInfo ToolTipInfo;

		 ToolTipInfo.nH = rect.bottom-rect.top;
		 ToolTipInfo.nId = GetCtrlId();
		 ToolTipInfo.nIndex = 0;
		 ToolTipInfo.nInterfaceId = 0;
		 ToolTipInfo.nSharedId = 0;
		 ToolTipInfo.nType = 2;
		 ToolTipInfo.nW = rect.right-rect.left;
		 ToolTipInfo.nX = rect.left;
		 ToolTipInfo.nY = rect.top;
		 ToolTipInfo.sAttr = "";
		 ToolTipInfo.sName = "";
		 ToolTipInfo.sReserve1 = "";
		 ToolTipInfo.sReserve2 = "";
		 ToolTipInfo.sTitle = "";

		 XWnd_Static* pToolTip = new XWnd_Static(ToolTipInfo,m_fScale);
		 pToolTip->m_strTitle = sInfor;
		 pToolTip->m_bBack = 1;
		 pToolTip->m_sFontName = "黑体";
		 pToolTip->m_nFontSize = 14;
		 pToolTip->m_backColor = RGB(0xb0,0xc0,0x82);
		 pToolTip->m_FontColor = RGB(5,5,5);
		 pToolTip->CreateCtrl(ToolTipInfo.nId,this);

		 m_pToolTip = pToolTip;
	 }
	 else
	 {
		  XWnd_Static* pToolTip = (XWnd_Static*)m_pToolTip;
		  pToolTip->m_strTitle = sInfor;
		  pToolTip->MoveWindow(&rect);
	 }

	 m_pToolTip->ShowWindow(nShow);
 }

 CBaseElemCtrl * XWnd_Dlg::FindElemCtrlByEventType(int nEventType)
 {
	 for(int i =0,end = m_CtrlVec.size();i<end;i++)
	 {
		 if(m_CtrlVec[i]->m_nEventType == nEventType)
			 return m_CtrlVec[i];
	 }

	 return NULL;
 }
 //
 //CBaseElemCtrl * XWnd_Dlg::FindElemCtrl(int nId)
 //{
	// for(int i =0,end = m_CtrlVec.size();i<end;i++)
	// {
	//	 if(m_CtrlVec[i]->m_ctrlInfo.nId == nId)
	//		 return m_CtrlVec[i];
	//	 if(m_CtrlVec[i]->m_ctrlInfo.nType == WND_type_dlg )
	//	 {
	//		 CBaseElemCtrl * pCtrl =((XWnd_Dlg*)m_CtrlVec[i])->FindElemCtrl(nId);
	//		 if(pCtrl!=NULL)
	//			 return pCtrl;
	//	 }
	//	 else if( m_CtrlVec[i]->m_ctrlInfo.nType == WND_type_tab)
	//	 {
	//		 CBaseElemCtrl * pCtrl =((XWnd_TabCtrl*)m_CtrlVec[i])->FindElemCtrl(nId);
	//		 if(pCtrl!=NULL)
	//			 return pCtrl;
	//	 }
	// }

	// return NULL;
 //}

 void XWnd_Dlg::ShowWnd(int bShow)
 {	
	 if(m_ctrlInfo.nInterfaceId==-1)
	 {
		 for(int i =0, end =m_CtrlVec.size();i<end;i++)
		 { 
			 if(m_CtrlVec[i]->m_ctrlInfo.nType == WND_type_dlg || m_CtrlVec[i]->m_ctrlInfo.nType == WND_type_tab)
				 m_CtrlVec[i]->ShowWnd(bShow);
		 }
		 return ;
	 }

	 ////子窗口，
	 if(m_nDutIndex <0)
		 return ;

	 bool b = CAppData::GetInstance().IsDutShow(m_nDutIndex);

	 if(b==IsWindowVisible())
		 return ;

	 ShowWindow(b?SW_SHOW:SW_HIDE);

	 //if(b)	 
		// DealInitEvent();		 
	 //else 
		// DealCloseEvent();
 }

 
 void XWnd_Dlg::DealInitEvent()
 { 

	 if(m_nEventNo ==1)
		 return ;
	 if(!CAppData::GetInstance().IsDutShow(m_nDutIndex))
	 {
		 return ;

	 }
	 TRACE("[%s:%d],%s,m_nDutIndex =%d \n",__FUNCTION__,__LINE__,m_ctrlInfo.sName.c_str(),m_nDutIndex);
	 m_nEventNo =1;

	 for(int i =0,end = m_InitEventVec.size();i<end;i++)
	 {
		  
		__Copy_Data_Info info;
		info.nShareId = m_InitEventVec[i].nSharedId;
		info.nValue = m_InitEventVec[i].nValue1;
		 SendMsgToDataProcess(COPY_DATA_CMD_Event,sizeof(__Copy_Data_Info),(void*)&info);

	 }
	 for(int i =0, end =m_CtrlVec.size();i<end;i++)
	 {  
			 m_CtrlVec[i]->DealInitEvent();;
	 }
 }
 void XWnd_Dlg::DealCloseEvent()
 {
	 if(m_nEventNo==0)
		 return ;

	 bool b = CAppData::GetInstance().IsDutShow(m_nDutIndex);
	 if(b )
		 return ;
	  
	  TRACE("[%s:%d],%s,m_nDutIndex =%d \n ",__FUNCTION__,__LINE__,m_ctrlInfo.sName.c_str(),m_nDutIndex);
	 m_nEventNo =0;

	 for(int i =0,end = m_ColseEventVec.size();i<end;i++)
	 {
		 __Copy_Data_Info info;
		 info.nShareId = m_ColseEventVec[i].nSharedId;
		 info.nValue = m_ColseEventVec[i].nValue1;
		 SendMsgToDataProcess(COPY_DATA_CMD_Event,sizeof(__Copy_Data_Info),(void*)&info);
	 }	 
	 for(int i =0,end =m_InitEventVec.size();i<end;i++)
	 { 		 
		 __Copy_Data_Info info;
		 info.nShareId = m_InitEventVec[i].nSharedId;
		 info.nValue = 0;
		 SendMsgToDataProcess(COPY_DATA_CMD_Event,sizeof(__Copy_Data_Info),(void*)&info);

	 }
	 for(int i =0,end =m_CtrlVec.size();i<end;i++)
	 {
		 m_CtrlVec[i]->DealCloseEvent();
	 }
	 
 }

 BOOL XWnd_Dlg::DestroyWindow() 
 { 	 
	 ReleaseRec();
	 return CWnd::DestroyWindow();
 }
 void XWnd_Dlg::OnOK()
 {
	// CBaseDialog::OnOK();
 }


 void XWnd_Dlg::OnCancel()
 {

	// CBaseDialog::OnCancel();
 }
void XWnd_Dlg:: OnExit()
 {
	 DealCloseEvent();
	 CBaseDialog::OnCancel();
 }
void XWnd_Dlg::ReleaseRec()
{
	DealCloseEvent();
	for(int i =0,end =m_CtrlVec.size();i<end;i++)
	{
		CBaseElemCtrl * pCtrl_1 = m_CtrlVec[i];
		if(pCtrl_1)
		{
			int nType = pCtrl_1->m_ctrlInfo.nType;

			if(nType ==0)
			{
				XWnd_Dlg *pCtrl =  (XWnd_Dlg *)pCtrl_1;		
				delete pCtrl;
				pCtrl =NULL;

			} 
			else if(nType ==1 )
			{ 
				XWnd_Button *pCtrl =  (XWnd_Button *)pCtrl_1;
				delete pCtrl;
				pCtrl =NULL;			   
			} 
			else if(nType == 2)
			{
				XWnd_Static *pCtrl =  (XWnd_Static *)pCtrl_1;
				delete pCtrl;
				pCtrl =NULL;	
			}
			else if(nType == WMD_type_digital)
			{
				XWnd_DigitalStatic *pCtrl =  (XWnd_DigitalStatic *)pCtrl_1;
				delete pCtrl;
				pCtrl =NULL;	
			}
			else if(nType ==WND_type_group)
			{			
				XWnd_Group *pCtrl =  (XWnd_Group *)pCtrl_1;
				delete pCtrl;
				pCtrl =NULL;	
			}
			else  if(nType ==WND_type_edit)
			{
				XWnd_Edit *pCtrl =  (XWnd_Edit *)pCtrl_1;
				delete pCtrl;
				pCtrl =NULL;				
			}
			else if(nType ==WND_type_switch_static)
			{   
				XWnd_SwitchStatic *pCtrl =  (XWnd_SwitchStatic *)pCtrl_1;
				delete pCtrl;
				pCtrl =NULL;	
			}
			else if(nType ==WND_type_swtich_button)
			{
				XWnd_SwitchButton *pCtrl =  (XWnd_SwitchButton *)pCtrl_1;
				delete pCtrl;
				pCtrl =NULL;	
			}
			else if(nType == WND_type_tab)
			{ 

				XWnd_TabCtrl *pCtrl =  (XWnd_TabCtrl *)pCtrl_1;
				delete pCtrl;
				pCtrl =NULL;	
			}
			else if(nType == WND_type_switch_static_image || nType == WND_type_switch_button_image)
			{
				XWnd_ImageCtrl * pCtrl  =(XWnd_ImageCtrl *)pCtrl_1;
				delete pCtrl;
				pCtrl =NULL; 
				 
			}
			else if(nType == WND_type_Line)
			{
				 Wnd_Line *  pCtrl  =(Wnd_Line *)pCtrl_1;
				 delete pCtrl;
				 pCtrl =NULL; 
			}
			else if(nType == WND_type_image_button_V02)
			{
				XWnd_Image_Button_V02 * pCtrl = (XWnd_Image_Button_V02 *)pCtrl_1;
				delete_safe(pCtrl);
			}
			else if(nType == WND_type_image_Led_V02)
			{
				XWnd_Image_Led_V02 * pCtrl = (XWnd_Image_Led_V02 *)pCtrl_1;
				delete_safe(pCtrl); 
			}
			 else if(nType == WND_type_image_Rotate_V02)
			{
				XWnd_Image_Rotate_V02 * pCtrl = (XWnd_Image_Rotate_V02*)pCtrl_1;
				delete_safe(pCtrl);
			}
			 else
			 {
				 TRACE(" %s:%d\r\n",__FUNCTION__,__LINE__);
			 }
			pCtrl_1 =NULL;
		}
		 
	}
	vector<CBaseElemCtrl *>().swap(m_CtrlVec);
}

void XWnd_Dlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	if(bShow)	
		DealInitEvent();	
	else 
		DealCloseEvent();
}
void XWnd_Dlg::StartReadData(int nStartFlag)
{
	if(nStartFlag==1)
	{
		if(m_nTimer>0)
		SetTimer(1,m_nTimer,0);
	}
	else
	{  
		DealCloseEvent();
		KillTimer(1);
	}

	for(int i =0,end =m_CtrlVec.size();i<end;i++)
	{
		CBaseElemCtrl * pCtrl_1 = m_CtrlVec[i];
		if(pCtrl_1)
		{
			int nType = pCtrl_1->m_ctrlInfo.nType;
			if(nType ==0)
			{
				XWnd_Dlg *pCtrl =  (XWnd_Dlg *)pCtrl_1;
				pCtrl->StartReadData(nStartFlag);
			} 			
			else if(nType == WND_type_tab)
			{  
				XWnd_TabCtrl *pCtrl =  (XWnd_TabCtrl *)pCtrl_1;
				pCtrl->StartReadData(nStartFlag);				 
			}
			else if(nType == WND_type_image_button_V02)
			{
				pCtrl_1->AlterDispStyle(nStartFlag);
			}
		}
	}
}