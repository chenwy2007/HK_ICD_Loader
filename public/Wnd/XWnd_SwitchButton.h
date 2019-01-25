#pragma once
#include "switchbutton.h"
/*

*/
class XWnd_SwitchButton :public CSwitchButton ,public CBaseElemCtrl
{
public:
	XWnd_SwitchButton(__CtrlInfo &info,float fScale);
	XWnd_SwitchButton(void);
	~XWnd_SwitchButton(void);
	
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);
	virtual void DecodeXml();
	
	virtual void RefreshData();
	virtual void MoveWnd();

	virtual void OnButtonDown();

	virtual void SetSelect(bool bSel);

	virtual void UpdateAttr(__SwitchButtonAttr &attr);

	vector<__Ctrl_Event>m_EventClickVec;

	vector<__Ctrl_Event>m_CloseEventVec;

	 
	DECLARE_MESSAGE_MAP()
public:
 
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

};

