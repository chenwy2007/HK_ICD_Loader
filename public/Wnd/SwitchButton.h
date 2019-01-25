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


	bool m_bHorizontal;//false;��ֱ true��ˮƽ
	
	bool m_bsequence;//false,Ĭ�ϣ���������(��������)��0��1...��true::��������(��������)��0��1 

	bool m_bEnable;//false,ֻ����true����д

	//
	int m_nValues[2];//1:�أ�0������Ĭ��Ϊ1��0
	int m_nItemCount; //Ĭ��2��,Ŀǰֻ��2����������չ
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


