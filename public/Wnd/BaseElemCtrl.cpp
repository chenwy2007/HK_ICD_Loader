#include "StdAfx.h"
#include "BaseElemCtrl.h"


CBaseElemCtrl::CBaseElemCtrl(void)
{
	m_fScale =1.0;
	m_ctrlInfo.nId =-1;
	m_ctrlInfo.nInterfaceId =-1;
	m_bSel =false;
	m_nEventType = 0;
	//m_pTopWnd = NULL;
}


CBaseElemCtrl::~CBaseElemCtrl(void)
{
}

void  CBaseElemCtrl::SetScale(float fScale)
{
	m_fScale = fScale;
}

 void CBaseElemCtrl::ReCreate()
 {
	 CManageDb_Sqllite::GetInstance().Query_CtrlInfo(m_ctrlInfo,m_ctrlInfo.nId);
	 DecodeXml();
 }
