#include "StdAfx.h"
#include "XWnd_SwitchButton.h"
 

XWnd_SwitchButton::XWnd_SwitchButton(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo = info;
	m_fScale = fScale;
}
XWnd_SwitchButton::XWnd_SwitchButton(void)
{
}

XWnd_SwitchButton::~XWnd_SwitchButton(void)
{
	for(int i=0,end = m_CloseEventVec.size();i<end;i++)
	{
 		__Copy_Data_Info info;
		info.nShareId = m_CloseEventVec[i].nSharedId;
		info.nValue = m_CloseEventVec[i].nValue1 ;
		SendMsgToDataProcess(COPY_DATA_CMD_Event,sizeof(__Copy_Data_Info),(void*)&info);
	}

}

BEGIN_MESSAGE_MAP(XWnd_SwitchButton, CSwitchButton)
 	ON_WM_LBUTTONUP()	
END_MESSAGE_MAP()
void XWnd_SwitchButton::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow )
{
	m_nCtrlId = nCtrlId;
	
	CRect rt;
	rt.left = m_ctrlInfo.nX*m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;

	Create("", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON| BS_OWNERDRAW ,rt,pParentWnd,m_nCtrlId);

	if(!CAppData::GetInstance().m_bBuilder)
	{
		CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_EventClickVec,m_ctrlInfo.nId,EVENT_TYPE_CLICK);
		CManageDb_Sqllite::GetInstance().Query_CtrlEvent(m_CloseEventVec,m_ctrlInfo.nId,EVENT_TYPE_CLOSE);
	}
}
void XWnd_SwitchButton::DecodeXml()
{
	CMarkup xml(m_ctrlInfo.sAttr.c_str());
	if(!xml.FindElem("Attr"))
		return;
	//Enable
	CString  str;
	str = xml.GetAttrib("Enable");
	if(str.IsEmpty())
		m_bEnable = false;
	else 
		m_bEnable = atoi(str);
	str = xml.GetAttrib("DefualtValue");

	if(str.IsEmpty())
		m_nDefaultValue=0;
	else
		m_nDefaultValue =atoi(str);
	
	int nIndex;
	 while(xml.FindChildElem("State"))
	 {
		 str = xml.GetChildAttrib("Index");
		 if(str.IsEmpty())
			 continue;
		 nIndex  =atoi(str);
		 if(nIndex<0 ||nIndex>1)
			 continue;
		 str = xml.GetChildAttrib("Value");
		 m_nValues[nIndex] =atoi(str);
	 }

	 SetValue(m_nDefaultValue);

}

void XWnd_SwitchButton::OnButtonDown()
{

}
void XWnd_SwitchButton::MoveWnd()
{
	MoveWindow(m_ctrlInfo.nX *m_fScale,m_ctrlInfo.nY * m_fScale,m_ctrlInfo.nW * m_fScale,m_ctrlInfo.nH*m_fScale);	
}

void XWnd_SwitchButton::SetSelect(bool bSel)
{
	m_bSel = bSel;
	Invalidate(FALSE);
}
void XWnd_SwitchButton::UpdateAttr(__SwitchButtonAttr &attr)
{
	m_ctrlInfo.sName = attr.sName;
	m_ctrlInfo.sTitle = attr.sName;
	m_ctrlInfo.nH = attr.nH;
	m_ctrlInfo.nW = attr.nW;
	m_ctrlInfo.nY = attr.nY;
	m_ctrlInfo.nX = attr.nX;
	m_ctrlInfo.nSharedId = attr.nSharedId;
	m_nValues[0]= attr.nValues[0];
	m_nValues[1]= attr.nValues[1];
	m_bEnable = attr.bEnable;
	SetValue(attr.nDefualtValue);
}
void XWnd_SwitchButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(CAppData::GetInstance().m_bBuilder )	 
	{
		CAppData::GetInstance().SetCurSelectWnd(this);	
		CButton::OnLButtonUp(nFlags, point);
	}
	else if(m_bEnable )  //ÔÊÐí²Ù×÷
	{ 
	
		for(int i=0,end = m_EventClickVec.size();i<end;i++)
		{
		 	__Copy_Data_Info info;
			info.nShareId = m_EventClickVec[i].nSharedId;
			info.nValue = m_nValues[m_nIndex] ;
			SendMsgToDataProcess(COPY_DATA_CMD_Event,sizeof(__Copy_Data_Info),(void*)&info);
		}
		
		//if(m_ctrlInfo.nSharedId>0)
		//{ 
		//	COPYDATASTRUCT data;
		//	data.dwData = m_ctrlInfo.nSharedId; 
		//	//data.lpData =m_nValues[m_nIndex];
		//	data.lpData =NULL;
		//	data.cbData =m_nValues[m_nIndex];
		//	SendMsgToDataProcess(data);
		//}
	}
	CSwitchButton::OnLButtonUp(nFlags, point);
	CAppData::GetInstance().SetCurTipWnd(this);

}

void XWnd_SwitchButton:: DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
void XWnd_SwitchButton::RefreshData()
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
		SetValue(nValue);
		Invalidate();
	}
		 
}