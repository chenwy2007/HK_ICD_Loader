// DlgStart.cpp : 实现文件
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "DlgStart.h"
#include "afxdialogex.h"
#include <shlwapi.h>

#pragma comment(lib,"Shlwapi.lib") //如果没有这行，会出现link错误

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlgSysStart 对话框

IMPLEMENT_DYNAMIC(CDlgSysStart, CDialogEx)

CDlgSysStart::CDlgSysStart(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSysStart::IDD, pParent)
{

}

CDlgSysStart::~CDlgSysStart()
{
}

void CDlgSysStart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_CUSTOM1, m_grid);
	DDX_Control(pDX, IDC_STATIC_SELECTSYS, m_static_selectsys);
	DDX_Control(pDX, IDC_GRID_SELECTSYS, m_grid_selectsys);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDlgSysStart, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSysStart::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK,IDC_CUSTOM1,OnDblClk)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgSysStart 消息处理程序

void  CDlgSysStart::OnDblClk(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnBnClickedOk();

}
BOOL CDlgSysStart::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//SetSideWidth(TITLE_HEIGHT,STATE_HEIGHT,3);
	//SetTitleTxt("通用协议（ICD）导入系统");
	//SetStatusTxt(1,"上海航空电器有限公司--研制");
	//SetBackColor(RGB(28,28,28),RGB(70,70,70));
	//SetEnableBt(true,false);
	CManageDb_Sqllite_Sys db;
	// 判断库表是否存在及合法性
	string sTblName = "t_sys";
	string sFieldName[] = {"nId","name","key"};
	int nFieldNum = 3;
	int nErr = 0;

	db.TableIsValid(sTblName,sFieldName,nFieldNum,nErr);
	if(nErr == 1)
	{
		CString sTxt = "";
		sTxt.Format("数据库表%s不存在！",sTblName);
		AfxMessageBox(sTxt);
	}
	else if(nErr == 2)
	{
		CString sTxt = "";
		sTxt.Format("数据库表%s非法！",sTblName);
		AfxMessageBox(sTxt);
	}

	if(nErr == 0)
		db.Query_Sys(m_Sys_vec);
	else
		m_Sys_vec.clear();

	CString strTitle[]={"系统名称","关键字"};
	CRect rt;	

	m_grid_selectsys.DeleteAllItems();
	m_grid_selectsys.SetEditable(TRUE);
	m_grid_selectsys.SetDefCellHeight(30);
	m_grid_selectsys.SetColumnCount(2);
	int nRow_Cnt = 1+m_Sys_vec.size();
	if(nRow_Cnt < 15)
		nRow_Cnt = 15;
	m_grid_selectsys.SetRowCount(nRow_Cnt);
	m_grid_selectsys.SetFixedRowCount();
	m_grid_selectsys.GetClientRect(&rt);

	int nW = (rt.Width()-GRID_SPARE)/2;
	for(int i =0;i<2;i++)
	{
		SetGirdItemText(m_grid_selectsys,0,i,strTitle[i],GVNI_READONLY,TITLE_HEAD_COLOR,COLOR_BK_JPG,DT_CENTER);  	 
		if(i == 1)
			m_grid_selectsys.SetColumnWidth(i,rt.Width()-nW);
		else
			m_grid_selectsys.SetColumnWidth(i,nW);
	}

	for(int i =0,end = m_Sys_vec.size();i<end;i++)
	{
		SetGirdItemText(m_grid_selectsys,1+i,0,m_Sys_vec[i].sName.c_str());  
		SetGirdItemText(m_grid_selectsys,1+i,1,m_Sys_vec[i].sKey.c_str() ); 

		m_grid_selectsys.SetItemData(1+i,0,m_Sys_vec[i].nId);
	}

	//CRect rect;
	//GetClientRect(&rect);
	//rect.bottom += 1;
	//rect.right += 1;
	//MoveWindow(&rect);

	//ShowWindow(SW_NORMAL);
	CRect rtDesk;
	CRect rtDlg;
	::GetWindowRect(::GetDesktopWindow(), &rtDesk);
	GetWindowRect(&rtDlg);
	int iXpos = rtDesk.Width() / 2 - rtDlg.Width() / 2;
	int iYpos = rtDesk.Height() / 2 - rtDlg.Height() / 2;
	SetWindowPos(NULL, iXpos, iYpos, 0, 0, SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);

	return TRUE;  
}


void CDlgSysStart::OnBnClickedOk()
{
	CCellID  cell= m_grid_selectsys.GetFocusCell();
	if(cell.row == -1 || cell.col == -1)
	{
		AfxMessageBox("未选择系统,请重新选择!");
		return ;
	}

	int  nId  =m_grid_selectsys.GetItemData(cell.row,cell.col);
	if(nId ==0)
	{
		AfxMessageBox("选择错误,请重新选择!");
		return ;
	}

	CManageDb_Sqllite::GetInstance().Close();

	CManageDb_Sqllite::GetInstance().SetDbFileName(m_Sys_vec[cell.row-1].sKey+".db");
	if(!CManageDb_Sqllite::GetInstance().ConnectDb())
	{
		
		AfxMessageBox("连接数据库失败");
		return  ;
	}

	// 判断库表是否存在及合法性
	BOOL bTableValid = TRUE;

	do 
	{
		// t_Test_Sys
		string sTblName = "t_Test_Sys";
		string sFieldName1[] = {"nId","name","sMark"};
		int nFieldNum = 3;
		int nErr = 0;
		CManageDb_Sqllite::GetInstance().TableIsValid(sTblName,sFieldName1,nFieldNum,nErr);
		if(nErr == 1)
		{
			CString sTxt = "";
			sTxt.Format("数据库表%s不存在！",sTblName.c_str());
			AfxMessageBox(sTxt);
		}
		else if(nErr == 2)
		{
			CString sTxt = "";
			sTxt.Format("数据库表%s非法！",sTblName.c_str());
			AfxMessageBox(sTxt);
		}
		if(nErr != 0)
		{	
			bTableValid = FALSE;
			break;
		}

		// t_sys_drive
		sTblName = "t_sys_drive";
		string sFieldName2[] = {"nId","nSysId","sName","sDriveName","nDriveId","nBusNumber",
		"nDeviceNumber","Attr","nAutoConnect"};
		nFieldNum = 9;
		nErr = 0;

		CManageDb_Sqllite::GetInstance().TableIsValid(sTblName,sFieldName2,nFieldNum,nErr);
		if(nErr == 1)
		{
			CString sTxt = "";
			sTxt.Format("数据库表%s不存在！",sTblName.c_str());
			AfxMessageBox(sTxt);
		}
		else if(nErr == 2)
		{
			CString sTxt = "";
			sTxt.Format("数据库表%s非法！",sTblName.c_str());
			AfxMessageBox(sTxt);
		}
		if(nErr != 0)
		{	
			bTableValid = FALSE;
			break;
		}

		// t_drive
		sTblName = "t_drive";
		string sFieldName3[] = {"nId","sName","nChannelNum","ndriveType","smark"};
		nFieldNum = 5;
		nErr = 0;

		CManageDb_Sqllite::GetInstance().TableIsValid(sTblName,sFieldName3,nFieldNum,nErr);
		if(nErr == 1)
		{
			CString sTxt = "";
			sTxt.Format("数据库表%s不存在！",sTblName.c_str());
			AfxMessageBox(sTxt);
		}
		else if(nErr == 2)
		{
			CString sTxt = "";
			sTxt.Format("数据库表%s非法！",sTblName.c_str());
			AfxMessageBox(sTxt);
		}
		if(nErr != 0)
		{	
			bTableValid = FALSE;
			break;
		}

		// t_data_config
		sTblName = "t_data_config";
		string sFieldName4[] = {"nId","nDlgId","sName","nDutIndex","nSysDriveId","nDriveid","nChannelIndex","nSharedId",
		"nDataType","nDataSize","fCoefficient","fOffset","nInOut","mark","nDealMode","sDefaultValue","ExtendedAttr"};
		nFieldNum = 17;
		nErr = 0;

		CManageDb_Sqllite::GetInstance().TableIsValid(sTblName,sFieldName4,nFieldNum,nErr);
		if(nErr == 1)
		{
			CString sTxt = "";
			sTxt.Format("数据库表%s不存在！",sTblName.c_str());
			AfxMessageBox(sTxt);
		}
		else if(nErr == 2)
		{
			CString sTxt = "";
			sTxt.Format("数据库表%s非法！",sTblName.c_str());
			AfxMessageBox(sTxt);
		}
		if(nErr != 0)
		{	
			bTableValid = FALSE;
			break;
		}

		// t_comm_protocol
		sTblName = "t_comm_protocol";
		string sFieldName5[] = {"nId","nDlgId","nSysDriveId","sName","nType","sProtocol","sMark"};
		nFieldNum = 7;
		nErr = 0;

		CManageDb_Sqllite::GetInstance().TableIsValid(sTblName,sFieldName5,nFieldNum,nErr);
		if(nErr == 1)
		{
			CString sTxt = "";
			sTxt.Format("数据库表%s不存在！",sTblName.c_str());
			AfxMessageBox(sTxt);
		}
		else if(nErr == 2)
		{
			CString sTxt = "";
			sTxt.Format("数据库表%s非法！",sTblName.c_str());
			AfxMessageBox(sTxt);
		}
		if(nErr != 0)
		{	
			bTableValid = FALSE;
			break;
		}

		break;
	} while (TRUE);



	if(bTableValid)
	{
		m_sSysName = m_Sys_vec[cell.row-1].sName.c_str();
		m_nCurSysId =nId;
	}
	else
	{
		m_sSysName = "";
		m_nCurSysId = -1;
		return;
	}

	CDialogEx::OnOK();
}


void CDlgSysStart::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(rect);

	CRect st_rect = rect;
	if(m_static_selectsys.GetSafeHwnd())
	{
		st_rect.top += TITLE_HEIGHT+5;
		st_rect.bottom = st_rect.top+20;
		st_rect.left = 10;
		st_rect.right = st_rect.left + 180;
		m_static_selectsys.MoveWindow(&st_rect);
	}

	CRect grid_rect = rect;
	if(m_grid_selectsys.GetSafeHwnd())
	{
		grid_rect.top = st_rect.bottom;
		grid_rect.bottom = rect.bottom-STATE_HEIGHT-CONTROL_HEIGHT;
		grid_rect.left = rect.left + 10;
		grid_rect.right = rect.right -10;
		m_grid_selectsys.MoveWindow(&grid_rect);
	}

	CRect btn_rect = rect;
	CButton* pbtn[] = {&m_btnCancel,&m_btnOk};
	btn_rect.top = grid_rect.bottom + 10;
	btn_rect.bottom = rect.bottom-STATE_HEIGHT-10;
	int nBtnWidth = 100;
	int nInterV = 10;
	for(int i = 0; i < 2; ++ i)
	{
		if(pbtn[i]->GetSafeHwnd())
		{
			btn_rect.left = rect.right-(i+1)*(nBtnWidth+nInterV);
			btn_rect.right = btn_rect.left + nBtnWidth;
			pbtn[i]->MoveWindow(&btn_rect);
		}
	}
}


void CDlgSysStart::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_Sys_vec.clear();
}
