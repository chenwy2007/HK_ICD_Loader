#pragma once


// CBaseImage_V02

/*
������

1.�Ƿ��б���ɫ
2.�߿�ɫ������ɫ
3.����ͼƬ
4�����ֵ���ʾ������ʵ��

��ť
1.����ɫ����������ɫ
2.����ɫ����������ɫ�ı仯
3.ȫ����ͼƬ���л�

led
1.����ɫ�������ֵı仯,�����֡�������
2.����ͼ�� ������ɫ�仯�������֡�������
3.����ͼ�ı仯��������ɫ�仯�������֡�������

*/
class CBaseImage_V02 : public CWnd
{
	DECLARE_DYNAMIC(CBaseImage_V02)

public:
	CBaseImage_V02();
	virtual ~CBaseImage_V02();


public:
	//�麯��
	virtual void OnChildDrawInfo(CDC * pDc,CRect  rt){};

	virtual void OnChildLButtonUp(UINT nFlags, CPoint point){};

	virtual void OnChildTimer(UINT_PTR nIDEvent){};
	 
	void SetDefulatPicFileName(string sPicFilePath);
	 
	//���ñ���ͼ·��
	bool SetPicFileName(string sPicFilePath);
	
	void SetCurFontColor(COLORREF ref);

	void DrawInfo(CDC *pDC,CRect rt);

	void DrawText(CDC  * pDC,CRect rt);
public:

	//�Ƿ��б���ɫ
	bool m_bBackColor;
	
	//�б���ɫ��������ɫ
	COLORREF m_ColorLine;
	
	//�б���ɫ��������ɫ
	COLORREF m_ColorBack;

	//�Ƿ񱳾�ͼ
	bool m_bBackBmp;

	//ͼƬ·��
	string m_sPicFileName;

	string m_sDefaultPicFileName;
	//������ɫ
	COLORREF m_ColorFont;
	
	string m_sFontName;

	int m_nFontSize;
		
	int m_nAlign;

	//��ʾ����
	string m_sTitle;
	 
	//ͼƬ����ʾ�Ŀ�ʼλ�ã�����λ��
	int m_nBitX;

	int m_nBitY;

	int m_nEType;

protected:

	CBitmap m_bitmap;  
	int m_nbmpWidth;
	int m_nbmpHigth;

	void CreateBmp(bool bReCreate = false);
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


