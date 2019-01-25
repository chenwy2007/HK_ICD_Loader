#pragma once

class XWnd_Group :
	public CGroupStatic,public CBaseElemCtrl
{
public:
	XWnd_Group(__CtrlInfo &info,float fScale);
	XWnd_Group(void);
	~XWnd_Group(void);

public:
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);
	
	virtual void DecodeXml(); 

	virtual void SetSelect(bool bSel);
	
	virtual void MoveWnd();

	void UpdateAttr(__GroupAttr& attr);

public:
	int m_nfontSize_define;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

