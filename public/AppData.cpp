#include "StdAfx.h"
#include "AppData.h"

 

CAppData::CAppData(void)
{
	m_bBuilder = false;
	m_pCurCtrl=NULL;

	m_colorDefaultDlgBackColor = RGB(156,170,193);
	m_colorDefaultDlgTitle = RGB(205,220,239);
	m_nDefualtTitleHeight =50;

	m_colorDefaultBt = RGB(243,243,243);
	m_colorDefaultBtLine = RGB(255,255,255);
	m_colorDefaultFont =RGB(0,0,255);
	m_nFontSize = 100;



	memset(&m_nDutShow,0,sizeof(int)*5);
	m_pCtrl_Copy =NULL;
	m_nCopyToParentId =0;
	m_bSpeakTip = false;
	m_bTipShow =true;
 
	//全局：数据类型名称
	m_arrayDataType.RemoveAll();
	m_arrayDataType.Add(DATA_TYPE_INT_NAME);
	m_arrayDataType.Add(DATA_TYPE_FLOAT_NAME);
	m_arrayDataType.Add(DATA_TYPE_STRING_NAME);
	m_arrayDataType.Add(DATA_TYPE_DOUBLE_NAME);
	m_arrayDataType.Add(DATA_TYPE_CHAR_NAME);
	m_arrayDataType.Add(DATA_TYPE_PROTOCL_NAME);

	CString strCtrlName[]={"窗口","按钮","文本框","分组框","编辑框",
							"LED指示灯","开关","分页框","LED指示灯(图片)","开关(图片)",
							"矩形控件","区级LED指示灯(图片)",WND_type_Rectangle_Name,WND_type_Circle_Name,WND_type_Line_Name,
							"","","","","",
							//V2.0
							WND_type_image_button_V02_Name,WND_type_image_Led_V02_Name,WND_type_image_Rotate_V02_Name,WND_type_digital_Name};

	int nCountCtrl =24;
	m_arrayCtrlName.RemoveAll();
	for(int i =0;i<nCountCtrl;i++)
	{
		if(strCtrlName[i].IsEmpty())
			continue;
		m_arrayCtrlName.Add(strCtrlName[i]);
	}
	m_ctrlNameMap.clear();
	for(int i =0;i<nCountCtrl;i++)
	{
		if(strCtrlName[i].IsEmpty())
			continue;
		m_ctrlNameMap[i] = strCtrlName[i];
	}
	m_arrayFontName.RemoveAll();
	m_arrayFontName.Add("黑体");
	m_arrayFontName.Add("宋体");
	
}


CAppData::~CAppData(void)
{
	
	 
}
CAppData &CAppData::GetInstance()
{
	static CAppData inst;
	return inst;
}
void CAppData::SetCurSelectWnd(CBaseElemCtrl * pCtrl)
{
	if(m_pCurCtrl)
	{
		m_pCurCtrl->SetSelect(false);			
	}
	 
	m_pCurCtrl =pCtrl;
	if(m_pCurCtrl)
	{
		m_pCurCtrl->SetSelect(true);
		if(m_pMainWnd)
			m_pMainWnd->PostMessage(WM_MSG_WND_ATTR,(long)pCtrl,pCtrl->m_ctrlInfo.nType);
	}
	if(pCtrl && pCtrl->m_ctrlInfo.nType == WND_type_dlg)
		m_pParentDlg = (XWnd_Dlg * )pCtrl;

	if(pCtrl && (pCtrl->m_ctrlInfo.nType ==WND_type_dlg || pCtrl->m_ctrlInfo.nType == WND_type_tab))
	{
	//	m_pParentDlg = pCtrl;

		m_nCopyToParentId  =pCtrl->m_ctrlInfo.nId;
	}
	else
		m_nCopyToParentId =0;

}
void CAppData::SetCurTipWnd(CBaseElemCtrl * pCtrl)
{
	if(m_pMainWnd && pCtrl)
	{
	 	m_pMainWnd->PostMessage(WM_MSG_TEST_TIP,(long)pCtrl,pCtrl->m_ctrlInfo.nId);
	}


}
void CAppData::SetCurPoint(int x,int y)
{
	static long lr = 0;
	if(lr==0)
	{
		lr=GetTickCount();
	}
	else 
	{
		if(GetTickCount()-lr<50)
			return;
		lr = GetTickCount();
	}

	if(m_pMainWnd)
	{
		m_pMainWnd->PostMessage(WM_MSG_CUR_POINT,x,y);
	}
}

void CAppData::NewSubCtrlToDlg(int nCtrlType)
{
	if(m_pMainWnd)
		m_pMainWnd->PostMessage(WM_MSG_WND_ATTR,0,nCtrlType);
}
void CAppData::NewSubCtrlToDlg(XWnd_Dlg_Builder * pDlg,int nCtrlType)
{
	//m_pParentDlg =pDlg;
	if(m_pMainWnd)
		m_pMainWnd->PostMessage(WM_MSG_WND_ATTR,0,nCtrlType);

}

bool CAppData::IsDutShow(int nIndex)
{
	if(nIndex<0 ||nIndex>=5)
		return true;
	return m_nDutShow[nIndex];
}

void CAppData::CopyCtrl()
{
	m_pCtrl_Copy = m_pCurCtrl;
}
void CAppData::PasteCtrl()
{
	CWaitCursor wait;
	CBaseElemCtrl * pCtrl = CAppData::GetInstance().m_pCtrl_Copy;
	if(pCtrl ==NULL)
		return;
	//相关表,t_ctrl,t_ctrl_event,t_ctrl_image,

	int nParentId = CAppData::GetInstance().m_nCopyToParentId;
	if(nParentId==0)
		return;

	CManageDb_Sqllite::GetInstance().Copy_Ctrl(pCtrl->m_ctrlInfo.nId,pCtrl->m_ctrlInfo.nType,nParentId); 
}

CString CAppData::GetDataTypeName(int nDataType)
{
	if(nDataType==DATA_TYPE_INT)
		return DATA_TYPE_INT_NAME;
	else if(nDataType == DATA_TYPE_FLOAT)
		return DATA_TYPE_FLOAT_NAME;
	else if(nDataType == DATA_TYPE_STRING)
		return DATA_TYPE_STRING_NAME;
	else if(nDataType == DATA_TYPE_DOUBLE)
		return DATA_TYPE_DOUBLE_NAME;
	else if(nDataType == DATA_TYPE_CHAR)
		return DATA_TYPE_CHAR_NAME;
	else if(nDataType == DATA_TYPE_PROTOCL)
		return DATA_TYPE_PROTOCL_NAME;
	
	return "";
}
int CAppData::GetDataType(CString sDataTypeName)
{
	if(sDataTypeName==DATA_TYPE_INT_NAME )
		return DATA_TYPE_INT;
	else if(sDataTypeName == DATA_TYPE_FLOAT_NAME)
		return DATA_TYPE_FLOAT ;
	else if(sDataTypeName == DATA_TYPE_STRING_NAME )
		return DATA_TYPE_STRING;
	else if(sDataTypeName == DATA_TYPE_DOUBLE_NAME )
		return DATA_TYPE_DOUBLE;
	else if(sDataTypeName == DATA_TYPE_CHAR_NAME )
		return DATA_TYPE_CHAR;
	else if(sDataTypeName == DATA_TYPE_PROTOCL_NAME)
		return DATA_TYPE_PROTOCL ;

	return -1;
}
CString CAppData::GetCtrlName(int nCtrlType)
{
	map<int,string>::iterator it =m_ctrlNameMap.find(nCtrlType);
	if(it!=m_ctrlNameMap.end())
	{
		return it->second.c_str();
	}
	return "";
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
	if(!CAppData::GetInstance().m_bSpeakTip)
		return ;
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

//
//bool GetBmpSize(CString sFilePath,int &nWidth, int &nHigth)
//{
//	nWidth =0;
//	nHigth=0;
//	HANDLE hFile;
//	BITMAPFILEHEADER bmpfheader;
//	BITMAPINFOHEADER bmpiheader;
//	DWORD nBytesRead;
//
//	hFile=CreateFile(sFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
//	if(hFile && hFile!=INVALID_HANDLE_VALUE)
//	{ 
//		ReadFile(hFile,&bmpfheader,sizeof(BITMAPFILEHEADER),&nBytesRead,NULL);
//		ReadFile(hFile,&bmpiheader,sizeof(BITMAPINFOHEADER),&nBytesRead,NULL);
//		nWidth = bmpiheader.biWidth;
//		nHigth =bmpiheader.biHeight;
//		CloseHandle(hFile);
//		return true;
//	}
//	
//	return false;
//}

void StartDataProcessSys()
{
	CString strTitle="HK_DATA_PROCESS";
	HWND hwnd  =::FindWindow(NULL,strTitle);
	int n=0;
	 if(hwnd==NULL)
	{	CString strPath = GetExecFilePath()+"HKDataProcessSys.exe";
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

