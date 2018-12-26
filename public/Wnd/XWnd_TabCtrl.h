#pragma once


// XWnd_TabCtrl
/*	
	TABCTRL,子窗体一定是一个DLG
	属性，
	TAB页，宽度、高度、背景色、选中颜色
	主题背景色
*/
#include "OwnerDrawTabCtrl.h"
#include "XWnd_Dlg.h"
class XWnd_TabCtrl : public COwnerDrawTabCtrl,public CBaseElemCtrl
{
	DECLARE_DYNAMIC(XWnd_TabCtrl)

public:
	XWnd_TabCtrl(__CtrlInfo &info,float fScale);
	virtual ~XWnd_TabCtrl();

	void ReleaseRec();

	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);
	virtual void DecodeXml();
	
	virtual void RefreshData();

	virtual void DealCloseEvent();

	virtual void DealInitEvent();

	virtual void ShowWnd(int bShow);

	void InitCtrl();

	void showTab(int nIndex);

	void UpdateAttr(__TabAttr & attr);

	CBaseElemCtrl * FindElemCtrl(int nId);

	void StartReadData(int nStartFlag);
public:
	vector<CBaseElemCtrl *>m_CtrlVec;
	vector<__CtrlInfo>m_subCtrlVec;

	vector<CBaseElemCtrl *>m_CtrlVec_Show;

	XWnd_Dlg * m_lpLastShowDlg;
 
	int m_nShowTab;
	//定义字号、标签的高度、宽度
	int m_nfontSize_define;

	int m_nTabHight_define;

	int m_nTabWidth_define;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


