// XWnd_TabCtrl.cpp : 实现文件
//

#include "stdafx.h"

#include "XWnd_TabCtrl.h"
 

// XWnd_TabCtrl

IMPLEMENT_DYNAMIC(XWnd_TabCtrl, CTabCtrl)

XWnd_TabCtrl::XWnd_TabCtrl(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo = info;
	m_fScale =fScale;
	m_nShowTab =0;
	m_nfontSize_define = 20;
	m_nTabHight_define =30;
	m_nTabWidth_define  = 200;
	m_lpLastShowDlg =NULL;
}

XWnd_TabCtrl::~XWnd_TabCtrl()
{
	ReleaseRec();
}
void XWnd_TabCtrl::ReleaseRec()
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
				//	pCtrl->ReleaseRec();
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

		}

		//delete_safe(pCtrl);
	}
	vector<CBaseElemCtrl *>().swap(m_CtrlVec);
	vector<CBaseElemCtrl *>().swap(m_CtrlVec_Show);
		vector<__CtrlInfo>().swap(m_subCtrlVec);
}

BEGIN_MESSAGE_MAP(XWnd_TabCtrl, CTabCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, &XWnd_TabCtrl::OnNMClick)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &XWnd_TabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()



// XWnd_TabCtrl 消息处理程序



void XWnd_TabCtrl::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow )
{
	m_nCtrlId = nCtrlId;

	CRect rt;
	rt.left = m_ctrlInfo.nX*m_fScale;
	rt.right = (rt.left+m_ctrlInfo.nW)*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = (rt.top +m_ctrlInfo.nH)*m_fScale;

 	bool b = Create(TCS_TABS|WS_CHILD|WS_VISIBLE|TCS_OWNERDRAWFIXED,rt,pParentWnd,nCtrlId);
	/*m_nTabHight =30 *m_fScale;
	m_nTabWidth = 100*m_fScale;*/
	SetItemSize(CSize(m_nTabWidth,m_nTabHight));
	SetMinTabWidth(m_nTabWidth);
	//bool b = Create(TCS_TABS|WS_CHILD|WS_VISIBLE,rt,pParentWnd,nCtrlId);
  
	 InitCtrl();
}

void XWnd_TabCtrl::DecodeXml()
{
	m_nfontSize_define = 20;
	m_nTabHight_define =30;
	m_nTabWidth_define  = 200;


	CMarkup xml(m_ctrlInfo.sAttr.c_str());
	CString str;
	if(xml.FindElem("Attr"))
	{
		m_backColor = atoi(xml.GetAttrib("colorBack"));
		m_backColor = m_backColor==0?RGB(28,28,28):m_backColor;

		m_LineColor = atoi(xml.GetAttrib("colorLine"));
		m_LineColor = m_LineColor==0?RGB(28,28,28):m_LineColor;

		m_nTabWidth_define =atoi(xml.GetAttrib("TabWidth"));
		m_nTabWidth_define = m_nTabWidth_define==0?200:m_nTabWidth_define;

		m_nTabHight_define =atoi(xml.GetAttrib("TabHeight"));
		m_nTabHight_define = m_nTabHight_define==0?50:m_nTabHight_define;

		m_sFontName = xml.GetAttrib("FontName");
		m_sFontName= m_sFontName.empty()?m_sFontName:"黑体";

		m_colorText = atoi(xml.GetAttrib("FontColor"));
		m_colorText = m_colorText==0?RGB(255,255,255):m_colorText;

		m_nfontSize_define = atoi(xml.GetAttrib("FontSize"));
		m_nfontSize_define = m_nfontSize_define==0?20:m_nfontSize_define;

		m_selectTabColor = atoi(xml.GetAttrib("colorSelTab"));
		m_selectTabColor = m_selectTabColor==0?RGB(28,28,28):m_selectTabColor;

		m_unSelectTabColor = atoi(xml.GetAttrib("colorUnSelTab"));
		m_unSelectTabColor = m_unSelectTabColor==0?RGB(179,179,179):m_unSelectTabColor;

	}

	m_nFontSize = m_nfontSize_define *m_fScale;
	m_nTabHight = m_nTabHight_define*m_fScale;
	m_nTabWidth =m_nTabWidth_define *m_fScale;

}

void XWnd_TabCtrl::InitCtrl()
{
	CManageDb_Sqllite::GetInstance().Query_SubCtrlInfo(m_subCtrlVec,m_ctrlInfo.nId);

	 
	int nType;
	CRect rt;
	GetClientRect(rt);
	for(int i =0,end = m_subCtrlVec.size();i<end;i++)
	{
		if(m_subCtrlVec[i].nType !=WND_type_dlg)
			continue;
	 
		nType = m_subCtrlVec[i].nType;
		 
		int  nDis =10;

		m_subCtrlVec[i].nX = nDis;
		m_subCtrlVec[i].nY = m_nTabHight+nDis*2;
		m_subCtrlVec[i].nW =m_ctrlInfo.nW -nDis*2;
		m_subCtrlVec[i].nH = m_ctrlInfo.nH-m_subCtrlVec[i].nY-nDis;
		
		 
		XWnd_Dlg *pCtrl = new XWnd_Dlg(IDD_DIALOG_BASE_CHILD,this);
		pCtrl->SetSideWidth(0,0,0);
	 	pCtrl->m_ctrlInfo = m_subCtrlVec[i];
		pCtrl->DecodeXml();
		pCtrl->CreateCtrl(IDD_DIALOG_BASE_CHILD,this,SW_HIDE);
		pCtrl->MoveWnd();
		pCtrl->InitCtrl();		
		m_CtrlVec.push_back((CBaseElemCtrl*)pCtrl) ;		  
		 
		if(CAppData::GetInstance().m_bBuilder || CAppData::GetInstance().IsDutShow(pCtrl->m_nDutIndex))
		{
			InsertItem(i,m_subCtrlVec[i].sName.c_str());
			m_CtrlVec_Show.push_back((CBaseElemCtrl*)pCtrl);
		//	pCtrl->DealInitEvent();

		}
	}
	showTab(0);
	SetCurSel(0);
}

void XWnd_TabCtrl::showTab(int nIndex)
{	
	if(m_lpLastShowDlg)
		m_lpLastShowDlg->ShowWindow(SW_HIDE);

 	if(nIndex >=m_CtrlVec_Show.size())
		return ;
 
	m_nShowTab = nIndex;
	m_lpLastShowDlg =(XWnd_Dlg *)m_CtrlVec_Show[m_nShowTab];
	m_lpLastShowDlg->ShowWindow(SW_SHOW);
	
}

void XWnd_TabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	 
	__super::OnLButtonDown(nFlags, point);
}


void XWnd_TabCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void XWnd_TabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nCurSel = GetCurSel(); 
	if(nCurSel !=m_nShowTab)
		showTab(nCurSel);

	*pResult = 0;
}

void XWnd_TabCtrl::RefreshData()
{
	for(int i =0,end =m_CtrlVec.size();i<end;i++)
	{
		m_CtrlVec[i]->RefreshData();
	}	
}
//需要改进
 void XWnd_TabCtrl::ShowWnd(int bShow)
 { 
	vector<CBaseElemCtrl *>showCtrl;	
	 for(int i =0,end =m_CtrlVec.size();i<end;i++)
	 {
		 if(m_CtrlVec[i]->m_ctrlInfo.nType!=WND_type_dlg)
			 continue;

		 XWnd_Dlg * pCtrl = (XWnd_Dlg*) m_CtrlVec[i]; 
		 if(CAppData::GetInstance().IsDutShow(pCtrl->m_nDutIndex))	
		 { 	  
			showCtrl.push_back(pCtrl);	
			if(!pCtrl->IsWindowVisible())
			{
				pCtrl->DealInitEvent();
			}			
		 }
		 else
		 {
			 pCtrl->DealCloseEvent();
		 }
		  
	 }

	  bool bDif =false;
	  if(showCtrl.size() !=m_CtrlVec_Show.size())
		  bDif = true;
	  else
	  {
		  for(int i =0,end = showCtrl.size();i<end;i++)
		  {
			  if(showCtrl[i] !=m_CtrlVec_Show[i])				
				{
					  bDif =true;
				 } 
		  }
	  }
	  // 相同，不处理
	  if(!bDif)
		  return  ;

	 DeleteAllItems();
	 for(int i =0,end = showCtrl.size();i<end;i++)
	 {
		 InsertItem(i,showCtrl[i]->m_ctrlInfo.sName.c_str());
	 }
	 showCtrl.swap(m_CtrlVec_Show);

	 showTab(0);
	 SetCurSel(0);

	this->Invalidate();
 }
  void XWnd_TabCtrl:: DealCloseEvent()
{
	for(int i =0,end =m_CtrlVec.size();i<end;i++)
	{
		m_CtrlVec[i]->DealCloseEvent();
	}
  }

  void XWnd_TabCtrl:: DealInitEvent()
 {
	 for(int i =0,end =m_CtrlVec.size();i<end;i++)
	 {
		 m_CtrlVec[i]->DealInitEvent();
	 }
  }

void XWnd_TabCtrl::StartReadData(int nStartFlag)
{
	for(int i =0,end = m_CtrlVec.size();i<end;i++)
	{
		CBaseElemCtrl * pCtrl_1 = m_CtrlVec[i];
		if(pCtrl_1==NULL)
			continue;

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
	}
}
void XWnd_TabCtrl::UpdateAttr(__TabAttr & attr)
{
	m_ctrlInfo.nX = attr.nX;
	m_ctrlInfo.nY =attr.nY;
	m_ctrlInfo.nW = attr.nW;
	m_ctrlInfo.nH = attr.nH;
	m_ctrlInfo.sTitle = attr.sTitle;
	m_ctrlInfo.sName = attr.sName;

	m_sFontName = attr.sFontName;
	m_nfontSize_define = attr.nFontSize;

	m_nTabWidth_define = attr.nTabWidth;
	m_nTabHight_define = attr.nTabHeight;

	m_colorText = attr.colorFont;
	m_backColor = attr.colorBack;
	m_LineColor =attr.colorFont;

	m_selectTabColor = attr.colorSelTab;
	m_unSelectTabColor = attr.colorUnSelTab;

	m_nFontSize = m_nfontSize_define *m_fScale;
	m_nTabHight = m_nTabHight_define*m_fScale;
	m_nTabWidth =m_nTabWidth_define *m_fScale;


	SetItemSize(CSize(m_nTabWidth,m_nTabHight));

	Invalidate();
}
CBaseElemCtrl * XWnd_TabCtrl::FindElemCtrl(int nId)
{
	for(int i =0,end = m_CtrlVec.size();i<end;i++)
	{
		if(m_CtrlVec[i]->m_ctrlInfo.nId == nId)
			return m_CtrlVec[i];
		if(m_CtrlVec[i]->m_ctrlInfo.nType == WND_type_dlg )
		{
			CBaseElemCtrl * pCtrl =((XWnd_Dlg_Builder*)m_CtrlVec[i])->FindElemCtrl(nId);
			if(pCtrl!=NULL)
				return pCtrl;
		}		 
	}

	return NULL;
}