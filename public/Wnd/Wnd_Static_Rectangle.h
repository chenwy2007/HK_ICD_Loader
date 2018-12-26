#pragma once
#include "basestatic.h"
class Wnd_Static_Rectangle :public CBaseStatic,public CBaseElemCtrl
{
public:

	Wnd_Static_Rectangle(__CtrlInfo &info,float fScale);
	~Wnd_Static_Rectangle(void);

public:

	virtual void DrawInfoEx(CDC *pDC,CRect rt);

public:
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_HIDE);
	virtual void DecodeXml();
	virtual void MoveWnd();

public:
	void UpdateAttr(__RectangleAttr &attr);
	//void  UpdateAttr(__RectangleAttr &attr);
public:
	int m_nSizeFont_Define;

	__Value_Color m_ValueColor[2];
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

};

