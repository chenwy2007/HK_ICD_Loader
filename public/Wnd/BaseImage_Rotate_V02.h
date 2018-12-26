#pragma once


// CBaseImage_Rotate_V02
//����ͼ�����ݸ����ĽǶ���ת



class CBaseImage_Rotate_V02 : public CWnd
{
	DECLARE_DYNAMIC(CBaseImage_Rotate_V02)

public:
	CBaseImage_Rotate_V02();
	virtual ~CBaseImage_Rotate_V02();


public:
	//�麯��
	virtual void OnChildDrawInfo(CDC * pDc,CRect  rt){};

	virtual void OnChildLButtonUp(UINT nFlags, CPoint point){};

	virtual void OnChildTimer(UINT_PTR nIDEvent){};
  
	//���ñ���ͼ·��
	bool SetPicFileName(string sPicFilePath);
	
	void SetRotate(float fRotate);

	void SetCurFontColor(COLORREF ref);

	void DrawInfo(CDC *pDC,CRect rt);
 
	void Rotate();
public:

	//�Ƿ��б���ɫ
	bool m_bBackColor;

	//�б���ɫ��������ɫ
	COLORREF m_ColorLine;

	//�б���ɫ��������ɫ
	COLORREF m_ColorBack;

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
	
	//��ʾ�Ƕ�
	float m_fRotate;

	CString m_strPicFilePath;
protected:
	CxImage * m_lpImage;

	DWORD m_nFileType;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


