#pragma once
class XImageStatic: public XBaseWnd 
{
public:
	XImageStatic(void);
	~XImageStatic(void);
public:
	virtual void CreateCtrl(CString sCtrlName,CString sTitle,DWORD  nCtrlId,CRect rect,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);

	virtual void OnPaintEx(CDC *pDC,CRect rt);

	virtual void RefreshData();	

	void SetDefaultImageFile(CString sFileName);
	void SetImageFile(CString sFileName);
	 void CreateBmp();
public:
	int m_nbmpHigth;
	int m_nbmpWidth;
	CBitmap m_bitmap;  

	CString m_sDefaultImageFile;
	CString m_sCurImageFile;
	map<int,CString>m_imageFileMap;
	COLORREF m_ColorBack;
};

