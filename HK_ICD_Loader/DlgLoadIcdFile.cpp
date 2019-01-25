// DlgLoadIcdFile.cpp : 实现文件
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "DlgLoadIcdFile.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DISP_INFOR WM_USER+0x100
// CDlgLoadIcdFile 对话框

IMPLEMENT_DYNAMIC(CDlgLoadIcdFile, CDialogEx)

CDlgLoadIcdFile::CDlgLoadIcdFile(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLoadIcdFile::IDD, pParent)
	, m_nCurProtocolType(-1),m_nCurComChannelNo(0)
{
	m_sIdcFilePath = "";
	m_nCurDriveIdx = -1;
}

CDlgLoadIcdFile::~CDlgLoadIcdFile()
{
}

void CDlgLoadIcdFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_1553B, m_nCurProtocolType);
	DDX_Control(pDX, IDC_EDIT_CHANELNO, m_edit_comchannelno);
	//DDX_Control(pDX, IDC_EDIT_DRIVETYPE, m_edit_drivetype);
	DDX_Control(pDX, IDC_EDIT_ICDFILE, m_edit_idcfile);
	DDX_Control(pDX, IDC_COMBO_DRIVETYPE, m_comboDrive);
	DDX_Control(pDX, IDC_STATIC_INFOR, m_static_infor);
}


BEGIN_MESSAGE_MAP(CDlgLoadIcdFile, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_1553B, &CDlgLoadIcdFile::OnBnClickedRadio1553b)
	ON_BN_CLICKED(IDC_RADIO_SCOM, &CDlgLoadIcdFile::OnBnClickedRadioScom)
	ON_BN_CLICKED(IDC_RADIO_ARINC429, &CDlgLoadIcdFile::OnBnClickedRadioArinc429)
	ON_BN_CLICKED(IDC_RADIO_CAN, &CDlgLoadIcdFile::OnBnClickedRadioCan)
	ON_BN_CLICKED(IDC_RADIO_mDAQ, &CDlgLoadIcdFile::OnBnClickedRadioDiscrete)
	ON_BN_CLICKED(IDC_BTN_SELECTFILE, &CDlgLoadIcdFile::OnBnClickedBtnSelectfile)
	ON_CBN_SELCHANGE(IDC_COMBO_DRIVETYPE, &CDlgLoadIcdFile::OnCbnSelchangeComboDrivetype)
	ON_MESSAGE(DISP_INFOR, OnDispInfor)
END_MESSAGE_MAP()


// CDlgLoadIcdFile 消息处理程序


BOOL CDlgLoadIcdFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_edit_comchannelno.SetWindowText("0");

	// TODO:  在此添加额外的初始化
	InitialRadio(m_nCurProtocolType);
	InitialDriveInfor(m_nCurProtocolType);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgLoadIcdFile::InitialDriveInfor(int nCurProtocolType)
{
	// 未找到系统驱动
	// 串口协议通道编号使能
	if(nCurProtocolType == 1)
		m_edit_comchannelno.EnableWindow(TRUE);
	else
		m_edit_comchannelno.EnableWindow(FALSE);

	if(nCurProtocolType == -1)
	{
		//AfxMessageBox("请添加系统设备驱动！");
		//PostMessage(DISP_INFOR,0,0);
		m_static_infor.ShowWindow(SW_SHOW);
		return;
	}

	CString strWhere = "";

	m_DriveVec.clear();
	m_comboDrive.ResetContent();

	CString sDriveType = "";
	if(nCurProtocolType == 0)
		sDriveType = "1553B";
	else if(nCurProtocolType == 1)
		sDriveType = "Serial";
	else if(nCurProtocolType == 2)
		sDriveType = "429";
	else if(nCurProtocolType == 3)
		sDriveType = "CAN";
	else
		sDriveType = "mDAQ";

	if(sDriveType == "mDAQ")
		strWhere = "and nDriveId in (select nid from t_drive where smark not in('1553B','Serial','429','CAN'))";
	else
		strWhere.Format("and nDriveId in (select nid from t_drive where smark ='%s')",sDriveType);

	CManageDb_Sqllite::GetInstance().Query_SysDrive(m_DriveVec,m_nCurSysId,strWhere.GetBuffer());

	int nIndex;
	for(int i =0,end =m_DriveVec.size();i<end;i++)
	{
		nIndex =m_comboDrive.AddString(m_DriveVec[i].sName.c_str());		

	}
	m_comboDrive.SetCurSel(0);
	m_nCurDriveIdx = 0;
}

void CDlgLoadIcdFile::InitialRadio(int& nCurProtocolType)
{
	CString strWhere = "";
	bool bFirstValidDrive = true;

	const TCHAR szDriveType[][16] = {"1553B","Serial","429","CAN","mDAQ"};
	const long lDriveTId[] = {IDC_RADIO_1553B,IDC_RADIO_SCOM,IDC_RADIO_ARINC429,IDC_RADIO_CAN,IDC_RADIO_mDAQ};
	for(int i = 0; i < 5; ++ i)
	{
		vector<__SYS_Drive> vtDriveInfor;

		if(lstrcmp(szDriveType[i],"mDAQ") == 0)
			strWhere = "and nDriveId in (select nid from t_drive where smark not in('1553B','Serial','429','CAN'))";
		else
			strWhere.Format("and nDriveId in (select nid from t_drive where smark ='%s')",szDriveType[i]);

		CManageDb_Sqllite::GetInstance().Query_SysDrive(vtDriveInfor,m_nCurSysId,strWhere.GetBuffer());

		int nDriveBoardNum = vtDriveInfor.size();

		if(nDriveBoardNum <= 0)
		{
			// 禁止单选按钮
			CButton* pBtn = (CButton*)GetDlgItem(lDriveTId[i]);
			if(pBtn)
				pBtn->EnableWindow(FALSE);
		}
		else
		{
			// 设置第一块有效板卡的编号
			if(bFirstValidDrive)
			{
				nCurProtocolType = i;
				bFirstValidDrive = false;
			}

			// 使能单选按钮
			CButton* pBtn = (CButton*)GetDlgItem(lDriveTId[i]);
			if(pBtn)
				pBtn->EnableWindow(TRUE);
		}
	}

	UpdateData(FALSE);
}

void CDlgLoadIcdFile::OnBnClickedRadio1553b()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	InitialDriveInfor(m_nCurProtocolType);
}



void CDlgLoadIcdFile::OnBnClickedRadioScom()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	InitialDriveInfor(m_nCurProtocolType);
}


void CDlgLoadIcdFile::OnBnClickedRadioArinc429()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	InitialDriveInfor(m_nCurProtocolType);
}


void CDlgLoadIcdFile::OnBnClickedRadioCan()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	InitialDriveInfor(m_nCurProtocolType);
}


void CDlgLoadIcdFile::OnBnClickedRadioDiscrete()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	InitialDriveInfor(m_nCurProtocolType);
}

LRESULT CDlgLoadIcdFile::OnDispInfor(WPARAM wParam, LPARAM lParam)
{
	//Sleep(1000);
	AfxMessageBox("请添加系统设备驱动！");
	return 0;
}

void CDlgLoadIcdFile::OnBnClickedBtnSelectfile()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	CString strFilePath;

	// szFilters is a text string that includes two file name filters:
	// "*.my" for "MyType Files" and "*.*' for "All Files."
	//TCHAR szFilters[]= _T("ICD文件 (*.xlsx)|*.xlsx|All Files (*.*)|*.*||");
	TCHAR szFilters[256] = {0};
	CString strFilterFile = "";
	CString strFilterEx = "";
	// 1553B文件类型
	if(m_nCurProtocolType == 0)
	{
		TCHAR szFilterBuf[] = _T("1553B ICD文件 (*.1553B)|*.1553B||");
		lstrcpy(szFilters,szFilterBuf);
		strFilterFile = "*.1553B";
		strFilterEx = "1553B";
	}
	// 串口文件类型
	else if(m_nCurProtocolType == 1)
	{
		TCHAR szFilterBuf[] = _T("COM ICD文件 (*.Serial)|*.Serial||");
		lstrcpy(szFilters,szFilterBuf);
		strFilterFile = "*.Serial";
		strFilterEx = "Serial";
	}
	// 429文件类型
	else if(m_nCurProtocolType == 2)
	{
		TCHAR szFilterBuf[] = _T("Arinc429 ICD文件 (*.Arinc429)|*.Arinc429||");
		lstrcpy(szFilters,szFilterBuf);
		strFilterFile = "*.Arinc429";
		strFilterEx = "Arinc429";
	}
	// CAN文件类型
	else if(m_nCurProtocolType == 3)
	{
		TCHAR szFilterBuf[] = _T("CAN ICD文件 (*.CAN)|*.CAN||");
		lstrcpy(szFilters,szFilterBuf);
		strFilterFile = "*.CAN";
		strFilterEx = "CAN";
	}
	// 离散量文件类型
	else if(m_nCurProtocolType == 4)
	{
		TCHAR szFilterBuf[] = _T("mDAQ ICD文件 (*.mDAQ)|*.mDAQ||");
		lstrcpy(szFilters,szFilterBuf);
		strFilterFile = "*.mDAQ";
		strFilterEx = "mDAQ";
	}
	else
	{
		AfxMessageBox("请选择协议类型！");
		return;
	}

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog dlg(TRUE, strFilterEx, strFilterFile,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);


	//CFileDialog dlg(TRUE);
	//CFileDialog dlg(TRUE,"xlsx",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"(*.xls;*.xlsx)|*.xls;*.xlsx||", this, 0, FALSE);

	if(dlg.DoModal()!=IDOK)
		return;
	strFilePath = dlg.GetPathName(); 
	//strFilePath = "G:\\Non-standardTEQ-HK\\trunk\\Hk18007\\通用协议\\1553B通用协议_1.xlsx";
	m_edit_idcfile.SetWindowText(strFilePath);

	//m_combo_protocol.ResetContent();

	//m_protocoNameVec.clear();

	//m_config_1553.OpenFile(strFilePath,m_protocoNameVec);

	//for(int i =0,end =m_protocoNameVec.size();i<end;i++)
	//	m_combo_protocol.AddString(m_protocoNameVec[i]);

	//m_combo_protocol.SetCurSel(0);
}


void CDlgLoadIcdFile::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData();
	CString sTxt = "";
	// 获取串口的通道号
	m_edit_comchannelno.GetWindowText(sTxt);
	m_nCurComChannelNo = _tstoi(sTxt);

	// 获取IDC文件路径
	m_edit_idcfile.GetWindowText(m_sIdcFilePath);

	if(m_sIdcFilePath == "")
	{
		AfxMessageBox("请选择ICD文件！");
		return;
	}

	CDialogEx::OnOK();
}


void CDlgLoadIcdFile::OnCbnSelchangeComboDrivetype()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurDriveIdx = m_comboDrive.GetCurSel();
}
