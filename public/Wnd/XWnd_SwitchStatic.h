#pragma once
#include "switchstatic.h"
class XWnd_SwitchStatic : public CSwitchStatic ,public CBaseElemCtrl
{
public:
	XWnd_SwitchStatic(__CtrlInfo &info,float fScale);
	XWnd_SwitchStatic(void);
	~XWnd_SwitchStatic(void);

public:

	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);
	virtual void DecodeXml();
	virtual void SetSelect(bool bSel);
	
	virtual void MoveWnd();
	virtual void RefreshData();
	virtual void UpdateAttr(__SwitchStaticAttr &attr);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

