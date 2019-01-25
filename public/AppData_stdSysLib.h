#pragma once


/*
系统运行时的全局变量
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
	//系统ID、系统KEY
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