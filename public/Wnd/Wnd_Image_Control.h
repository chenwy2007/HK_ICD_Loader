#pragma once
#include "baseimage.h"
/*
支持控件：
	1.按钮
	2.分组显示，根据共享内存具体值，显示图片
	3.分组显示，根据共享内存值具体在某一个区间值显示图片
*/

class Wnd_Image_Control : 	public CBaseImage,public CBaseElemCtrl
{
public:
	Wnd_Image_Control(__CtrlInfo &info,float fScale);
	~Wnd_Image_Control(void);

	virtual void DrawInfoEx(CDC *pDC,CRect rt);
	virtual void  OnMyLButtonUp(UINT nFlags, CPoint point);

public:
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_HIDE);
	
	virtual void DecodeXml();
	
	virtual void MoveWnd();
  
	virtual void RefreshData();
 
	virtual void SetSelect(bool bSel);

	virtual void UpdateAttr(__ImageCtrlAttr &attr);

	void QueryRefreshEvent();

	void QueryGroupRefreshEvent(int nGroupId);

	void RefreshData_group();
	void RefreshData_group_switch_static_image();
	void RefreshData_group_Image_Interval();
public:

public:
	vector<string>m_sBtPicPathVec;

	vector<__Ctrl_Event>m_EventClickVec;

	vector<__Ctrl_Event>m_CloseEventVec;

	vector<__Ctrl_Event>m_RefreshEventVec;

	map<int,vector<__Ctrl_Event>>m_GroupRefreshEventMap;

	bool m_bEnable;
public:
	int m_nSizeFont_Define;
protected:
	DECLARE_MESSAGE_MAP()
 
};

