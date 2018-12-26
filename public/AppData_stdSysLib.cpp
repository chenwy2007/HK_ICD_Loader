#include "StdAfx.h"
#include "AppData_stdSysLib.h"

 

CAppData_StdSys::CAppData_StdSys(void)
{
	 
}


CAppData_StdSys::~CAppData_StdSys(void)
{
	
	 
}
CAppData_StdSys &CAppData_StdSys::GetInstance()
{
	static CAppData_StdSys inst;
	return inst;
} 

void SendMsgToDataProcess(COPYDATASTRUCT& copyData )
{
	CString strTitle="HK_DATA_PROCESS";
	HWND hwnd  =::FindWindow(NULL,strTitle);
	if(hwnd)
	{
		::SendMessage(hwnd,WM_COPYDATA,0,(LPARAM)&copyData);
	}
}
void SendMsgToDataProcess(int nCmdId,int nDataLen,void * lpData)
{
	COPYDATASTRUCT copyData;
	copyData.dwData =nCmdId;
	copyData.cbData =nDataLen;
	copyData.lpData = lpData;

	CString strTitle="HK_DATA_PROCESS";
	HWND hwnd  =::FindWindow(NULL,strTitle);
	if(hwnd)
	{
		::SendMessage(hwnd,WM_COPYDATA,0,(LPARAM)&copyData);
	}
}
void SendMsgToTTS(int nCmdId,int nDataLen,void * lpData)
{
	COPYDATASTRUCT copyData;
	copyData.dwData =nCmdId;
	copyData.cbData =nDataLen;
	copyData.lpData = lpData;

	CString strTitle="HK_TTS";
	HWND hwnd  =::FindWindow(NULL,strTitle);
	if(hwnd)
	{
		::SendMessage(hwnd,WM_COPYDATA,0,(LPARAM)&copyData);
	}
}
void SendMsgToTTS(CString& sInfo)
{ 
	CString strPath ="c:\\tts.ini";
	WritePrivateProfileString("TTS","index_1",sInfo,strPath);

	CString strTitle="HK_TTS";
	HWND hwnd  =::FindWindow(NULL,strTitle);
	
	COPYDATASTRUCT copyData;
	copyData.dwData =10;	
	copyData.cbData =0;
	copyData.lpData = 0;
	if(hwnd)
	{
		::SendMessage(hwnd,WM_COPYDATA,0,(LPARAM)&copyData);
	}

}
int GetSharedValue(char * lpbuf,int nDataType)
{
	int nValue=0;
	if(nDataType ==DATA_TYPE_INT)
	{
		memcpy(&nValue,lpbuf,sizeof(int)); 
		nValue *=BASE_NUM;		
	}
	else if(nDataType ==DATA_TYPE_DOUBLE)
	{
		double dValue;
		memcpy(&dValue,lpbuf,sizeof(double));
		nValue = dValue*BASE_NUM;					
	} 
	return nValue;
}
 
void StartDataProcessSys()
{
		CString strTitle="HK_DATA_PROCESS";
		HWND hwnd  =::FindWindow(NULL,strTitle);
		if(!hwnd)
		{
			CString strPath = GetExecFilePath()+"HKDataProcessSys.exe";
			ShellExecute(NULL,"open",strPath,NULL,NULL,SW_SHOWNORMAL); 
		}
	
		strTitle="HK_TTS";
		  hwnd  =::FindWindow(NULL,strTitle);
		if(!hwnd)
		{
			CString strPath = GetExecFilePath()+"TTS.exe";
			ShellExecute(NULL,"open",strPath,NULL,NULL,SW_SHOWNORMAL); 
		}

}

bool InitEnvi()
{
	CString strIni = GetExecFilePath()+"hkstdsys.ini";
	char lpFileName[512]={0};
	GetPrivateProfileString("CONFIG","DB_NAME","",lpFileName,256,strIni);
	if(strlen(lpFileName)==0)
		return false;
	CManageDb_Sqllite::GetInstance().SetDbFileName(lpFileName);
	
	return true;

}

