#pragma once


// CImageCtrl

class CImageCtrl : public CBitmapButton
{
	DECLARE_DYNAMIC(CImageCtrl)

public:
	CImageCtrl();
	virtual ~CImageCtrl();

	void SetValue(int nValue,bool bInvalidate= false);
	void SetNextValue();
	
	void AddImagePath(int,string);

	void SetTextInfo(CString sTitle,int nFontSize=30,COLORREF  ref = RGB(255,255,255));

	bool GetCurImagePath(string &sPath);

	void CreateBmpInfo();

	void SetShowImage(string &sPath);

public:
	
	map<int,string>m_ImageMap;

	CFont* m_pFont;

	CFont m_font;
	
	COLORREF m_TextColor ;

	int m_nFontSize;
	CString m_strTitle;
	
	int m_nCurValue;

	HBITMAP   m_hBitMap;

	bool m_bEnable;

	string m_strBmpPath;
protected:
	DECLARE_MESSAGE_MAP()
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


