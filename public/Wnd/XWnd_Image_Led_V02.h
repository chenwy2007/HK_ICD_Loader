#pragma once

class XWnd_Image_Led_V02:public CBaseElemCtrl,public CBaseImage_V02
{
public:
	XWnd_Image_Led_V02(__CtrlInfo &info,float fScale);

	XWnd_Image_Led_V02(void);

	~XWnd_Image_Led_V02(void);

public:
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);

	virtual void DecodeXml();

	virtual void SetSelect(bool bSel);

	virtual void MoveWnd();

	virtual void ReCreate();

	virtual void RefreshData();

	virtual void OnChildDrawInfo(CDC * pDc,CRect  rt);

	virtual void OnChildLButtonUp(UINT nFlags, CPoint point);

	virtual void OnChildTimer(UINT_PTR nIDEvent);

	virtual void DealCloseEvent();

public:
	
	void QueryRefreshEvent();

	void QueryGroupRefreshEvent(int nGroupId);

	bool RefreshData_group();

	void QueryTextOut();
public:

	int m_nFontSize_Define;
	vector<__Ctrl_Event>m_RefreshEventVec;

	map<int,vector<__Ctrl_Event>>m_GroupRefreshEventMap;

	vector<XTextOutColor*>m_TextOutVec;
};

