#pragma once

/*������� �ؼ��ϵ�����

1.���ֲ���

2.��ɫ��Ĭ��ֵ�����ݹ����ڴ�ֵ������
 
*/

class XTextOutColor
{
public:
	XTextOutColor(float fScale,__TextAttr_V02 &attr);
	~XTextOutColor(void);

public:
	void InitData();
	bool RefreshData();
	
	void SetCurTextColor(COLORREF ref);
	void ReSetTextColor();
public:
	void DrawInfoEx(CDC * pDC ,CRect rt);
	 
public:
	int m_nId;
	int m_nCtrlId;

	float m_fScale;

	string m_sTitle;  
	
	COLORREF m_ColorCurFont;

	COLORREF m_ColorDefaultFont;
	
	//����	
	string m_sFontName;
	
	//�ֺ�
	int m_nFontSize;

	//�Ƿ����,��ʱ����
	bool m_bBold;	
	 
	//��ʾλ��
	int m_nAlign; //>=0,��ʾʹ��m_nAlign��-1��λ�õ����·��Ϊm_nX��m_nY
	int m_nX;
	int m_nY;

	vector<__Ctrl_Event_Text>m_RefreshEventVec;


	int m_nCurValue;
};
 