#pragma once


// CBaseStatic

class CBaseStatic : public CStatic
{
	DECLARE_DYNAMIC(CBaseStatic)

public:
	CBaseStatic();
	virtual ~CBaseStatic();

	virtual void DrawInfoEx(CDC *pDC,CRect rt){};

public:
	void DrawInfo(CDC *pDC,CRect rt);
	
	void SetColor(COLORREF ref);

public:
	
	string m_strTitle;		//��ʾ����	

 	COLORREF m_colorFont;	//������ɫ
	
	int  m_nSizeFont;		//�����С

	int m_nAlign;			//Ĭ�ϣ�DT_VCENTER|DT_CENTER

   	COLORREF m_colorBack;	//������ɫ

	COLORREF m_colorLine;	//������ɫ

	int m_nType;		//0:Բ��1�������Σ�2��û�б���ɫ��ֻ������	

	COLORREF m_colorCircle ;//Բ������ɫ

private:

	void DrawInfo_Circle(CDC *pDC,CRect rt);
	void DrawInfo_Rect(CDC *pDC,CRect rt);
	void DrawInfo_Text(CDC *pDc,CRect rt);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


