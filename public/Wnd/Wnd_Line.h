#pragma once


// Wnd_Line


class Wnd_Line : public CStatic,public CBaseElemCtrl
{
	DECLARE_DYNAMIC(Wnd_Line)

public:
	Wnd_Line(__CtrlInfo &info,float fScale);
	virtual ~Wnd_Line();
public:
	
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW );
	
	virtual void DecodeXml();
	 
	virtual void MoveWnd();
	virtual void ReCreate();
public:
	void DrawInfo(CDC * pDC,CRect);
	void UpdateAttr(__LineAttr &attr);
public:
	
	//线型 、线宽、实线长度、虚线长度、线色
	int m_nLineType;
		
	int m_nLineWidth;

	int m_nRealLineLength;

	int m_nDottedLineLength;

	COLORREF m_colorLine;
	
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


