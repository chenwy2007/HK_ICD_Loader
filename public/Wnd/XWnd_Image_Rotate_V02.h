#pragma once

#include "baseimage_rotate_v02.h"

/*
功能：模拟量输入(旋钮),

1.定义输入的最大值、最小值、
2.定义图片名称
3.图片的起始位置、
4.根据输入值，调图片的显示角度
*/

class XWnd_Image_Rotate_V02 :public CBaseElemCtrl,public CBaseImage_Rotate_V02
{
public:
	XWnd_Image_Rotate_V02(__CtrlInfo &info,float fScale);
	~XWnd_Image_Rotate_V02(void);

public:
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);

	virtual void DecodeXml();

	virtual void SetSelect(bool bSel);

	virtual void MoveWnd();

	virtual void ReCreate();
		
	virtual void RefreshData();

	//虚函数
	virtual void OnChildDrawInfo(CDC * pDc,CRect  rt);

	virtual void OnChildLButtonUp(UINT nFlags, CPoint point);

	virtual void OnChildTimer(UINT_PTR nIDEvent);
	
	void QueryTextOut();

	void QueryRefreshEvent();
public:
	
	vector<XTextOutColor*>m_TextOutVec;
	//属性
	__RotateAttr_V02 m_Attr;

	string m_strOutValue;
};

