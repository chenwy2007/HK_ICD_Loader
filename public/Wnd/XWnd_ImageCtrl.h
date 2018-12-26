#pragma once
#include "imagectrl.h"
/*

1.LED
2.°´Å¥

*/
class XWnd_ImageCtrl :	public CImageCtrl ,public CBaseElemCtrl
{
public:
	XWnd_ImageCtrl(__CtrlInfo &info,float fScale);
	~XWnd_ImageCtrl(void);

public:
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);

	virtual void DecodeXml();

	virtual void RefreshData();
	
	virtual void MoveWnd();
 
	virtual void SetSelect(bool bSel);

	virtual void UpdateAttr(__ImageCtrlAttr &attr);

	void QueryRefreshEvent();

	void QueryGroupRefreshEvent(int nGroupId);

	void RefreshData_group();

	virtual void DealCloseEvent();

	virtual void DealInitEvent();
public:
	vector<__Ctrl_Event>m_EventClickVec;

	vector<__Ctrl_Event>m_CloseEventVec;

	vector<__Ctrl_Event>m_RefreshEventVec;

	map<int,vector<__Ctrl_Event>>m_GroupRefreshEventMap;

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};

