#pragma once


class CBaseElemCtrl
{
public:
	CBaseElemCtrl(void);
	~CBaseElemCtrl(void);

public:
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW )=0;
	virtual void DecodeXml()=0;
	 
	//�����Ƿ�ѡ��
	virtual void SetSelect(bool bSel){m_bSel = bSel;};
	
	//������ʾ����
	virtual void  SetScale(float fScale);

	virtual void ReSizeCtrl(CBaseElemCtrl * pCtrl){};
	//
	virtual CWnd * GetParentWnd(){return NULL;};
	
	virtual void MoveWnd(){};
	
	virtual void ShowWnd(int bShow){};

	virtual void RefreshData(){};

	virtual void DealCloseEvent(){};

	virtual void DealInitEvent(){};

	virtual void ReCreate();

	virtual	CBaseElemCtrl * FindElemCtrlByEventType(int nEventType) { return NULL;};

	// �����ʾ��Ϣ
	virtual void DrawToolTip(RECT rect,CString sInfor,int nShow=SW_SHOW) {};

	// ��Ӧ�ⲿ����ı�ؼ���ʾ��ʽ
	virtual void AlterDispStyle(int nStatus) {};

public:
	DWORD m_nCtrlId;

	__CtrlInfo m_ctrlInfo;

	//��ʾ����
	float m_fScale;
	
	//�Ƿ�ѡ��
	bool m_bSel;
	
	int m_nEventType;

//protected:
//	CBaseElemCtrl* m_pTopWnd;
};

