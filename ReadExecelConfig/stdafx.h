
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#ifndef _AFXDLL
#define _AFXDLL
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

// 剔除掉intsafe.h头文件，避免与stdint.h的宏重定义
#define _INTSAFE_H_INCLUDED_

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









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