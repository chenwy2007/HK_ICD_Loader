
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#ifndef _AFXDLL
#define _AFXDLL
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

// �޳���intsafe.hͷ�ļ���������stdint.h�ĺ��ض���
#define _INTSAFE_H_INCLUDED_

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <string>
#include <vector>
#include <map>
using namespace std;


#include "excel9.h"

#include "XReadConfig.h"

#include "sqlite3\\sqlite3\\sqlite3.h"

#include "GridCtrl\\NewCellTypes\\GridCellCombo.h"
#include "GridCtrl\\NewCellTypes\\GridCellCheck.h"
#include "GridCtrl\\GridCtrl.h"

#include "function.h"

#include "../public/AppData.h"
#include "../public/define_comm_protocol.h"
#include "../public/define.h"
#include "../public/CppSQLite3.h"
#include "../public/ManageDb.h"
#include "../public/Markup.h"

#include "../public/define_comm_protocol.h"

#include "ProtocolDefine.h"

#include "Execel_Base.h"

#include "XProtocol_Funtion.h"
#ifdef _DEBUG
#pragma comment (lib, "../ICD_Tool/ICD_bin_debug/sqlite3.lib") 
#else
#pragma comment (lib, "../ICD_Tool/ICD_bin_release/sqlite3.lib")
#endif
//void SetGirdItemText(CGridCtrl &grid,int nRow,int nCol,CString stTxt);



extern int m_nCurSysId;
extern CString m_sSysName;
extern int m_nCurDlgId;
extern CString m_sCurDlgName;