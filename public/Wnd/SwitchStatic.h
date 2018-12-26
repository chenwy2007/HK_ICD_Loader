#pragma once
#include "afxwin.h"
 
/*
���ԣ�
 ԰����ʼֵ
 ����ɫ
 ������ɫ��

*/
 
class CSwitchStatic :
	public CStatic
{
public:
	CSwitchStatic(void);
	~CSwitchStatic(void);

public: //run
	bool SetCurValue(int nValue);
	 
public: //��ʼ��
	void SetValueColor(int nIndex,COLORREF ref);
 	void SetValueColor(map<int ,COLORREF>&ref);
	void SetSideColor(COLORREF ref);

	void SetTitleInfo(CString sInfo, int nAlign =1, int nSize = 80 );

	void SetBkColor(COLORREF ref){m_BackColor =ref;};


	void OnMyPaint(CDC * pDC ,CRect rt);
private:
	void DrawInfo(CDC *pDC ,CRect rt);
	 
public:
	
	//�Ƿ��б���ɫ
	bool m_bBackColor;

	//����ɫ
	COLORREF m_BackColor;
	
	//��ǰԲ����ɫ
	COLORREF m_CurCircleColor;
	
	//Բ����ɫ
	COLORREF m_lineColor;

	//����ֵ��Ӧ����ɫ��Ĭ��2��
	map<int ,COLORREF>m_colorMap;

	//Ĭ��ֵ
	int m_nCurValue;


	CFont m_fontText;
	int m_fontSize;
	
	COLORREF m_FontColor;
	CString  m_strTitle;

	int m_nAlign ;//0:����ʾ���⣬1����࣬2���Ҳ࣬3���ϲ࣬4���²�
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};

