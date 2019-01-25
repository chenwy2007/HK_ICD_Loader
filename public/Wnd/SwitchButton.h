#pragma once


// CSwitchButton
/*

*/

class CSwitchButton : public CButton
{
	DECLARE_DYNAMIC(CSwitchButton)

public:
	CSwitchButton();
	virtual ~CSwitchButton();

	void SetValue(int nValue);
	virtual void OnButtonDown(){};
	bool GetReadWriteState(){return m_bEnable;};
	
public:

	void DrawButton(CDC * pDC,CRect rt);
	
	int  m_nDefaultValue;


	bool m_bHorizontal;//false;垂直 true，水平
	
	bool m_bsequence;//false,默认，从上向下(从左向右)，0，1...，true::从下向上(从右向左)，0，1 

	bool m_bEnable;//false,只读，true：读写

	//
	int m_nValues[2];//1:关，0：开，默认为1，0
	int m_nItemCount; //默认2个,目前只有2个，后期扩展
	void OnMyPaint(CDC * pDC);

protected:
	int m_nIndex;
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


