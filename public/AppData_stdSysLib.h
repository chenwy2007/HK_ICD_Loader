#pragma once


/*
ϵͳ����ʱ��ȫ�ֱ���
*/


#define g_AppLib  (CAppData_StdSys::GetInstance())
class CAppData_StdSys
{
public:
	CAppData_StdSys(void);
	~CAppData_StdSys(void);
public:
	static CAppData_StdSys &GetInstance();
public:	  
	//ϵͳID��ϵͳKEY
	int m_nTestCurSysId;
};

//void SendMsgToDataProcess(COPYDATASTRUCT& copyData );
//
//void SendMsgToDataProcess(int nCmdId,int nDataLen,void * lpData);
//
//void SendMsgToTTS(int nCmdId,int nDataLen,void * lpData);
//
//void SendMsgToTTS(CString& sInfo);
//
//int GetSharedValue(char * lpbuf,int nDataType);
// 
//void StartDataProcessSys();
//
//bool InitEnvi();