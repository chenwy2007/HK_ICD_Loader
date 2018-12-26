#pragma once


// CColorButton

class CColorButton : public CButton 
{
	DECLARE_DYNAMIC(CColorButton)

public:
	CColorButton();
	virtual ~CColorButton();
 

public:
	void SetText(CString str);
	void SetForeColor(COLORREF color);       //�����ı���ɫ
	void SetBkColor(COLORREF color);         //���ñ�����ɫ
	void SetLineColor(COLORREF color);
	COLORREF GetBkColor(){return m_BackColor;};
	void SetTextFont(int FontHight,LPCTSTR FontName);    //�������� 

public:
	int m_nFontSize;
	COLORREF	m_lineColor;
	COLORREF     m_ForeColor;         //�ı���ɫ
	COLORREF     m_BackColor;         //����ɫ
	CString      m_strText;           //��ť����

protected:
	void   DrawButton(CDC *pDC);
	DECLARE_MESSAGE_MAP()

private:
	int          m_Style;     //��ť��״��0-������1-��ǰ��2-���£�3-������
	BOOL         b_InRect;            //�������־
	
	
	
	COLORREF     m_LockForeColor;     //������ť��������ɫ
	CRect        m_ButRect;           //��ť�ߴ�
	 
	
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


