#pragma once

#include "baseimage_rotate_v02.h"

/*
���ܣ�ģ��������(��ť),

1.������������ֵ����Сֵ��
2.����ͼƬ����
3.ͼƬ����ʼλ�á�
4.��������ֵ����ͼƬ����ʾ�Ƕ�
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

	//�麯��
	virtual void OnChildDrawInfo(CDC * pDc,CRect  rt);

	virtual void OnChildLButtonUp(UINT nFlags, CPoint point);

	virtual void OnChildTimer(UINT_PTR nIDEvent);
	
	void QueryTextOut();

	void QueryRefreshEvent();
public:
	
	vector<XTextOutColor*>m_TextOutVec;
	//����
	__RotateAttr_V02 m_Attr;

	string m_strOutValue;
};

