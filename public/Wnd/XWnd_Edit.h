#pragma once


// XWnd_Edit

class XWnd_Edit : public CEdit,public CBaseElemCtrl
{
	DECLARE_DYNAMIC(XWnd_Edit)

public:
	XWnd_Edit(__CtrlInfo &info,float fScale);
	XWnd_Edit();
	virtual ~XWnd_Edit();


	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);
	virtual void DecodeXml();
	virtual void SetSelect(bool bSel);
	virtual void MoveWnd();

	virtual void RefreshData();

	void UpdateAttr(__EditAttr & attr);
	void SetParentBase(CBaseElemCtrl* pBase) {m_pTopBase = pBase;}

	void  Analysis_Expression(string sData,int nDataType);
	int m_nDataType;
	__DATA_CONFIG m_data_Config;

	vector<string>m_sExpressionVec;

	CString m_sRange;
	CString m_sDefaultValue;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* pDc,UINT nCtlColor);
    //afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
	//HICON m_hAlerthIcon;
	//bool m_bIsAlert;
	CBaseElemCtrl* m_pTopBase;
	XWnd_Static* m_pAlertIcon;
};


