#pragma once
 
class CBaseDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CBaseDialog)

public:
	CBaseDialog(int nId,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBaseDialog();
	 
	//���ڵĶ������ײ������ұߵĸ߿�
	void SetSideWidth(int nTop =50,int nBottomH=30,int nLeftRight=3);
	
	//��������
	void SetTitleTxt(CString strTille) ;
	
	//״̬���� ,���2����nIndex��0����������1���Ҳ����
	void SetStatusTxt(int nIndex,CString sTxt);
	
	void SetBackColor(COLORREF colorBack,COLORREF colorTitle,COLORREF colorfont =RGB(255,255,255));

	void InvalidateStatusTxt(int nIndex);

	void SetEnableBt(bool bCloseBtn,bool bMaxBtn);

	virtual void OnExit();
	 
	virtual void DrawCustom(CDC *pDC,CRect rt){};
	 
//protected:

	COLORREF m_colorBack;
	COLORREF m_colorTitle;
	  
	 
	COLORREF m_colorFont;
	int m_nFontSize;
	
	//����ĸ߶�
	int m_nTopHeight;
	//�ײ��ĸ߶�
	int m_nBottomHeight;
	int m_nBorderWidth;
	
	CRect m_RectTitle;
	CString m_strTitle;

	CString m_strLeftState;
	CString m_strRigthState;

//	CFont m_FontTitle;
	CFont m_FontState;

	CRect m_RectLeftState;
	CRect m_RectRightState;

	//top ��buttom �������ʱ������Ԥ�����
	int m_nSideWidth;

	//�رհ�ť�����ť�Ƿ���ʾ��m_nTopHeight>0 m_bHasCloseBtn��m_bHasMaxBtn��Ч����m_bHasCloseBtn:TRUE,m_bHasMaxBtn��Ч
	bool m_bHasCloseBtn;
	int m_nCloseBtState; //0 :Ĭ�ϣ�1������
	
	//�Ƿ���ʾͼ��LOG
	bool m_bHasTitleLog;

	CRect m_RectCloseBtn;

	bool m_bSideLine;

	bool m_bHasMaxBtn;
	int m_nMaxBtState; //0 :Ĭ�ϣ�1������
	CRect m_RectMaxBtn;
	  
	int m_nCmdShow ;

	

protected:
	//��LOG��λ��Ϊ���ڵ����Ͻ�,������LOG��������Ҳ�λ��
	int  DrawTopLog(CDC * pDC);
	void DrawCloseBtn(CDC * pDC,CRect rt);
	void DrawTopTxt(CDC * pDC,CRect rt,int nLeft);
	void DrawBottomTxt(CDC * pDC,CRect rt);
	void OnMyPaint(CDC * pDC,CRect rt);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
 virtual BOOL OnInitDialog();
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
