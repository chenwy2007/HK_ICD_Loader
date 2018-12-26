#pragma once

/*
���ԣ�
�������ͣ� 
Act:
	EXITSYS,�˳�ϵͳ
	EXIT:�˳���ǰ����
	ShowDlg ,�����´���

param1
Exit :0,�˳�ϵͳ��1���˳���ǰ����


*/
class XWnd_Button :
	public CColorButton,public CBaseElemCtrl
{
public:
	XWnd_Button(__CtrlInfo &info,float fScale);
	XWnd_Button(void);
	~XWnd_Button(void);

	CString  m_strAct;
	CString  m_strParam;
	int m_bExit;
	vector<__BT_ACT>m_actVec;
	void ShowDlg();
	void UpdateAttr(__ButtonAttr & attr);
	

public:
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW);
	virtual void DecodeXml();
	virtual void SetSelect(bool bSel);
	virtual void MoveWnd();
	

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};

