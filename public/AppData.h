#pragma once


/*
--1.�Ƿ�Ϊ�൥Ԫ����
--2.�Ƿ�ͨ��modbusʶ���Ʒ
--3.ʶ���Ʒ����Ӧ�Ĺ����ڴ�
--4.����ǵ�һ��Ʒ����ֱ����ʾ��Ʒ
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

	//�Ƿ�Ϊ����ϵͳ
	bool m_bBuilder; 
	
	//ϵͳID��ϵͳKEY
	int m_nCurSysId;

	string m_sSysName;
	string m_strCurAppKey;
	
	//��ǰ�򿪴��ڵ�ID
	int m_nDlgId;
	
	int m_nDutShow[5];

	//��ǰѡ�еĿؼ�
	CBaseElemCtrl * m_pCurCtrl;
	
	//�����Ӵ��ڵ��ô�����
	XWnd_Dlg * m_pParentDlg;
	
	CBaseElemCtrl * m_pCtrl_Copy;

	//���Ƶ�ָ���Ĵ�����
	int  m_nCopyToParentId;
	
	//������
	CWnd * m_pMainWnd;
	
	//���ڡ�����Ĭ�ϱ���ɫ
	COLORREF m_colorDefaultDlgBackColor;
	COLORREF m_colorDefaultDlgTitle;
	
	//����Ĭ�ϸ߶�
	int m_nDefualtTitleHeight;

	//��ť������Ĭ����ɫ���ܱ���ɫ ,�����С��������ɫ
	COLORREF m_colorDefaultBt;
	COLORREF m_colorDefaultBtLine;
	COLORREF m_colorDefaultFont;
	int m_nFontSize;

	//��������
	CStringArray m_arrayDataType;

	CStringArray m_arrayCtrlName;
	
	map<int,string>m_ctrlNameMap;
	//����
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