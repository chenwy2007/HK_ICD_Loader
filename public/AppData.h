#pragma once


/*
--1.是否为多单元测试
--2.是否通过modbus识别产品
--3.识别产品，对应的共享内存
--4.如果是单一产品，则直接显示产品
*/

class CBaseElemCtrl;
class XWnd_Dlg_Builder;
class XWnd_Dlg;

#define g_App  (CAppData::GetInstance())
class CAppData
{
public:
	CAppData(void);
	~CAppData(void);
public:
	void SetMainWnd(CWnd * pMainWnd){m_pMainWnd = pMainWnd;};
	
	static CAppData &GetInstance();

	void SetCurSelectWnd(CBaseElemCtrl * pCtrl);

	void SetCurTipWnd(CBaseElemCtrl * pCtrl);

	void SetCurPoint(int x,int y);
	void NewSubCtrlToDlg(XWnd_Dlg_Builder * pdlg,int nCtrlType);
	
	void NewSubCtrlToDlg(int nCtrlType);

	bool IsDutShow(int nIndex);

	void CopyCtrl();

	void PasteCtrl();


	CString GetDataTypeName(int nDataType);
	int GetDataType(CString sDataTypeName);

	CString GetCtrlName(int nCtrlType);
	 
public:

	BOOL m_bTipShow;

	BOOL m_bSpeakTip;

	//是否为构件系统
	bool m_bBuilder; 
	
	//系统ID、系统KEY
	int m_nCurSysId;

	string m_sSysName;
	string m_strCurAppKey;
	
	//当前打开窗口的ID
	int m_nDlgId;
	
	int m_nDutShow[5];

	//当前选中的控件
	CBaseElemCtrl * m_pCurCtrl;
	
	//增加子窗口到该窗口下
	XWnd_Dlg * m_pParentDlg;
	
	CBaseElemCtrl * m_pCtrl_Copy;

	//复制到指定的窗口下
	int  m_nCopyToParentId;
	
	//主窗口
	CWnd * m_pMainWnd;
	
	//窗口、标题默认背景色
	COLORREF m_colorDefaultDlgBackColor;
	COLORREF m_colorDefaultDlgTitle;
	
	//标题默认高度
	int m_nDefualtTitleHeight;

	//按钮：背景默认颜色，周边颜色 ,字体大小、字体颜色
	COLORREF m_colorDefaultBt;
	COLORREF m_colorDefaultBtLine;
	COLORREF m_colorDefaultFont;
	int m_nFontSize;

	//数据类型
	CStringArray m_arrayDataType;

	CStringArray m_arrayCtrlName;
	
	map<int,string>m_ctrlNameMap;
	//字体
	CStringArray m_arrayFontName;

	///LIB

	int m_nCurTestSysId;

};

void SendMsgToDataProcess(COPYDATASTRUCT& copyData );

void SendMsgToDataProcess(int nCmdId,int nDataLen,void * lpData);

void SendMsgToTTS(int nCmdId,int nDataLen,void * lpData);

void SendMsgToTTS(CString& sInfo);

int GetSharedValue(char * lpbuf,int nDataType);



void StartDataProcessSys();

bool InitEnvi();