#pragma once
#include "baseimage.h"
/*
֧�ֿؼ���
	1.��ť
	2.������ʾ�����ݹ����ڴ����ֵ����ʾͼƬ
	3.������ʾ�����ݹ����ڴ�ֵ������ĳһ������ֵ��ʾͼƬ
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

