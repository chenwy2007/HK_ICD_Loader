#pragma once


class CBaseElemCtrl
{
public:
	CBaseElemCtrl(void);
	~CBaseElemCtrl(void);

public:
	virtual void CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd=NULL,int nShow =SW_SHOW )=0;
	virtual void DecodeXml()=0;
	 
	//设置是否选中
	virtual void SetSelect(bool bSel){m_bSel = bSel;};
	
	//设置显示比例
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

	// 输出提示信息
	virtual void DrawToolTip(RECT rect,CString sInfor,int nShow=SW_SHOW) {};

	// 响应外部命令，改变控件显示样式
	virtual void AlterDispStyle(int nStatus) {};

public:
	DWORD m_nCtrlId;

	__CtrlInfo m_ctrlInfo;

	//显示比例
	float m_fScale;
	
	//是否选中
	bool m_bSel;
	
	int m_nEventType;

//protected:
//	CBaseElemCtrl* m_pTopWnd;
};

