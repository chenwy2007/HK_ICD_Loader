#pragma once


// XBaseWnd

class XBaseWnd : public CStatic
{
	DECLARE_DYNAMIC(XBaseWnd)
public:
	XBaseWnd();
	virtual ~XBaseWnd();

public:
	virtual void CreateCtrl(CString sCtrlName,CString sTitle,DWORD  nCtrlId,CRect rect,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);

	virtual void OnLButtonUpEx(UINT nFlags, CPoint point){};

	virtual void OnLButtonDownEx(UINT nFlags, CPoint point){};

	virtual void OnPaintEx(CDC *pDC,CRect rt){};

	virtual void RefreshData(){};

public:
	void SetShared(int nShareId){m_nSharedId = nShareId;};

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:	
	//控件名称、共享内存，用于读取数据
	CString m_sCtrlName;
	int m_nSharedId;

	//显示内容
	CString m_sTitle;
	CRect m_Rect;
protected:
	int m_nCtrlId;
	CWnd * m_pParentWnd;

};


