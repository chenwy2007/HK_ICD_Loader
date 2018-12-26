#pragma once

///////////////////////////////////////////////////////////////////////////////
//// CDigitStatic window
//class CMemDC : public CDC 
//{
//private:
//	CBitmap*	m_bmp;
//	CBitmap*	m_oldbmp;
//	CDC*		m_pDC;
//	CRect		m_Rect;
//public:
//	CMemDC(CDC* pDC, const CRect& rect) : CDC()
//	{
//		CreateCompatibleDC(pDC);
//		m_bmp = new CBitmap;
//		m_bmp->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
//		m_oldbmp = SelectObject(m_bmp);
//		m_pDC = pDC;
//		m_Rect = rect;
//	}
//	~CMemDC() 
//	{
//		m_pDC->BitBlt(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height(), 
//			this, m_Rect.left, m_Rect.top, SRCCOPY);
//		SelectObject(m_oldbmp);
//		if (m_bmp != NULL) 
//			delete m_bmp;
//	}
//};

// XWnd_DigitalStatic
/*
�Ƿ��б��� Back :0���ޱ�����1���б���
����ɫ  BackColor: 
�����С��FontSize:
������ɫ:FontColor:
*/
typedef struct tagDISPINFOR
{
	LPCTSTR lpDisp;		// ������ʾ���ַ�
	int nxPosi;			// �ַ���ͼƬ�е�λ�ã������ؼƣ�
	int nWidth;			// �ַ����
}DISPINFOR,*pDISPINFOR;

class XWnd_DigitalStatic : public CStatic,public CBaseElemCtrl
{
	DECLARE_DYNAMIC(XWnd_DigitalStatic)

public:
	XWnd_DigitalStatic(__CtrlInfo &info,float fScale);
	
	XWnd_DigitalStatic();
	
	virtual ~XWnd_DigitalStatic();

	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);
	
	virtual void DecodeXml();
	
	virtual void SetSelect(bool bSel);
	
	virtual void MoveWnd();
	
	virtual void RefreshData();
	
	void UpdateAttr(__DigitalStaticAttr & attrr);

	void SubControl(UINT uID,CWnd* pParent);
	LPTSTR GetText();
	void SetText(LPCTSTR text);

	CString m_strTitle;		
	
	COLORREF m_FontColor;
		
	string m_sFontName;

	int m_nFontSize; 

	int m_DigitalLen;  //���ֳ���

	int m_nAlign; //DT_LEFT DT_CENTER, DT_RIGHT

	COLORREF m_backColor;
	
	int m_bBack; //1���б���ɫ	
	
	//�����ڴ�ֵ����Ӧ����ʾ��ɫ
	__Value_Color m_ValueColor[2];
	
	map<int ,string>m_valueShowInfo;

	int m_nDigitalFontType;

	int m_nCtrlType;
	CString m_sDispContend;
	CString m_sRange;

protected:
	UINT  m_PerHeight;
	UINT  m_PerWidth;

private:
	void DrawInfo(CDC *pDC,CRect rt);
	bool IsNoDispCharacter(int nDispCharacterSize,int nDispLen,WCHAR* wDispValue);
	void MByte2Unicode(char *lpData,WCHAR  * lpToData);
	void Unicode2MByte(WCHAR * lpData,char *lpToData,int nsize=2);
	bool SetCurValue(int nValue);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

private:
	DISPINFOR m_DispInfor[28];

};


