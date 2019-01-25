#pragma once
#include "BaseImage_V02.h"

#include "XTextOutColor.h"
/*
根据共享内存值
*/

class XWnd_Image_Button_V02:public CBaseElemCtrl,public CBaseImage_V02
{
public:
	XWnd_Image_Button_V02(__CtrlInfo &info,float fScale);

	XWnd_Image_Button_V02();

	~XWnd_Image_Button_V02(void);

public:
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);

	virtual void DecodeXml();

	virtual void SetSelect(bool bSel);

	virtual void MoveWnd();

	virtual void ReCreate();

	virtual void DealCloseEvent();

	virtual void OnChildDrawInfo(CDC * pDc,CRect  rt);

	virtual void OnChildLButtonUp(UINT nFlags, CPoint point);

	virtual void OnChildTimer(UINT_PTR nIDEvent);

	void SetValue(int nIndex);

	void SetParentBase(CBaseElemCtrl* pBase) {m_pTopBase = pBase;}

protected:
	virtual void AlterDispStyle(int nStatus);

private:

	void QueryTextOut();
	CBaseElemCtrl* m_pTopBase;
public :
	vector<__Ctrl_Event>m_EventClickVec;
	
	vector<__Ctrl_Event>m_CloseEventVec;
	 
	vector<XTextOutColor *>m_TextOutVec;
	
	//字体颜色
	map<int,COLORREF>m_ColorFontMap;
	
	map<int,string>m_ImagePathMap;

	int m_nCurIndex;

	int m_nFontSize_Define;

};

