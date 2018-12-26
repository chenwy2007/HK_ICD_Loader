// DlgDiscrete.cpp : 实现文件
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "DlgDiscrete.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlgDiscrete 对话框

IMPLEMENT_DYNAMIC(CDlgDiscrete, CDialogEx)

CDlgDiscrete::CDlgDiscrete(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDiscrete::IDD, pParent)
{

}

CDlgDiscrete::~CDlgDiscrete()
{
}

void CDlgDiscrete::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_protocol, m_combo_protocol);
	DDX_Control(pDX, IDC_CUSTOM1, m_grid);
	DDX_Control(pDX, IDC_COMBOX_TYPE, m_combox_type);
}


BEGIN_MESSAGE_MAP(CDlgDiscrete, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Read, &CDlgDiscrete::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_SelectFile, &CDlgDiscrete::OnBnClickedButtonSelectfile)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgDiscrete::OnBnClickedButtonSave)

END_MESSAGE_MAP()


// CDlgDiscrete 消息处理程序


BOOL CDlgDiscrete::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	m_combox_type.SetCurSel(0);

	return TRUE;  
	
}


void CDlgDiscrete::OnBnClickedButtonRead()
{
	int nSel = m_combo_protocol.GetCurSel();
	if(nSel==-1)
		return;

	vector<vector<string>>vec;
	m_excelFile.ReadInfo_Sub_Item(nSel+1,vec,10,-1,13,2);

	int nCount =10;
	int nSel_Type = m_combox_type.GetCurSel();
	if(nSel_Type == 1)
		nCount = 13;

	CRect rt;
	m_grid.GetClientRect(rt);
	int nW =rt.Width()/nCount;

	//CString strItem[]={"名称","数据类型","方向","板卡名","通道号/端口","系数","补充值","属性","备注"};
	m_grid.DeleteAllItems();
	m_grid.SetEditable(TRUE);
	m_grid.SetDefCellHeight(20);
	m_grid.SetColumnCount(nCount+1);
	m_grid.SetRowCount(vec.size());
	for(int i =0;i<nCount+1;i++)
	{
		//SetGirdItemText_string(m_grid,0,i,strItem[i]);
		m_grid.SetColumnWidth(i,nW-2);
	}
	 
	 for(int i=0,end = vec.size();i<end;i++)
	 {
		 vector<string>&info =vec[i];

		 for(int k=0,end1 =info.size();k<end1 && k<nCount;k++)
		 {			 
			 SetGirdItemText_string(m_grid,i,k,info[k].c_str());
		 }
	 }
}


void CDlgDiscrete::OnBnClickedButtonSelectfile()
{
	CString strFilePath;
	CFileDialog dlg(TRUE);

	if(dlg.DoModal()!=IDOK)
		return;
	strFilePath = dlg.GetPathName(); 
	GetDlgItem(IDC_EDIT_file_path)->SetWindowText(strFilePath);

	m_combo_protocol.ResetContent();

	m_protocoNameVec.clear();

	m_excelFile.OpenFile(strFilePath,m_protocoNameVec);

	for(int i =0,end =m_protocoNameVec.size();i<end;i++)
		m_combo_protocol.AddString(m_protocoNameVec[i]);

	m_combo_protocol.SetCurSel(0);

}


void CDlgDiscrete::OnBnClickedButtonSave()
{ 
	int nSel = m_combo_protocol.GetCurSel();
	if(nSel==-1)
		return;
	CString sPageName;	
	m_combo_protocol.GetLBText(nSel,sPageName);

	CString sTxt;
	sTxt.Format("是否将协议保存到\"%s\"",m_sCurDlgName);

	if(AfxMessageBox(sTxt,MB_YESNO) == IDNO)
		return;

	CWaitCursor wait;

	int nSel_Type = m_combox_type.GetCurSel();
	if(nSel_Type == 1)
	{
		SaveTblData(sPageName);
		return;
	}
 	
	int nMaxId = CManageDb_Sqllite::GetInstance().GetMaxId(" select max(nId) from  t_data_config ")+1;
	CString strSql;
	strSql.Format("delete from t_data_config_1 ");
 	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());


	CString sName;
	CString sInOut;
	CString sDataType;
	CString sSysDriveName;	
	CString sChannelNo;
	CString sMark ="";		 
	int nInOut; //1输入，2：输出
	int nDataType;	
	int  nDealMode =0;//处理方式
	int nDataSize; 
	int nChannelNo;
	CString str;
	int n=0;
	for(int i =1,end =m_grid.GetRowCount();i<end;i++) 
	{

		n=0;
		sName =m_grid.GetItemText(i,n++);
		sDataType=m_grid.GetItemText(i,n++);
		sInOut =m_grid.GetItemText(i,n++);		
		sSysDriveName =m_grid.GetItemText(i,n++);
		sChannelNo =m_grid.GetItemText(i,n++);
		++n;
		++n;
		++n;
		++n;
		nDealMode = _tstoi(m_grid.GetItemText(i,n++));
		
		sName.Trim();
		sDataType.Trim();
		sInOut.Trim();
		sSysDriveName.Trim();
		sChannelNo.Trim();
		if(sName.IsEmpty() ||sDataType.IsEmpty() || sInOut.IsEmpty()||sSysDriveName.IsEmpty() || sChannelNo.IsEmpty() )
			continue;

		sName =sPageName+"."+sName;

		if(sInOut =="输出")
			nInOut =2;
		else if(sInOut =="输入")
			nInOut =1;
		else
			nInOut =0; 

		if(sDataType == DATA_TYPE_INT_NAME)
		{
			nDataType =0;
			nDataSize = sizeof(int);
		}
		else if(sDataType == DATA_TYPE_FLOAT_NAME)
		{
			nDataType =1;
			nDataSize = sizeof(int);
		}
		// 加入缓存区类型
		else if(sDataType == DATA_TYPE_BUFFER_NAME)
		{
			nDataType = DATA_TYPE_BUFFER;
			nDataSize = DATA_TYPE_BUFFER_LEN;
		}
		else 
			continue;
		nChannelNo =atoi(sChannelNo);


		strSql.Format("insert into t_data_config_1 (nId,nDlgId,sName,nDutIndex,nSysDriveId,nDriveid,nChannelIndex,nInOut,mark,nSharedId,nDataType,nDataSize,fCoefficient,fOffset,nDealMode) values (%d ,\
					  %d,'%s',%d,(select nId from t_sys_drive where sName ='%s'),( select nDriveId from t_sys_drive where sName ='%s'),%d,%d,'%s',0,%d,%d,1.0,0,%d)",
					  nMaxId++,  m_nCurDlgId,sName,0,sSysDriveName,sSysDriveName,nChannelNo,nInOut,sMark,nDataType,nDataSize,nDealMode);
		BOOL b= CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
		if(b)
		{
			 SetGirdItemText_string(m_grid,i,9,"写入成功");
		}
		else
			SetGirdItemText_string(m_grid,i,9,"写入失败");

		//if(b && nDealMode==1)
		//{

		//	strSql.Format("insert into t_data_config_1 (nId,nDlgId,sName,nDutIndex,nSysDriveId,nDriveid,nChannelIndex,nInOut,mark,nSharedId,nDataType,nDataSize,fCoefficient,fOffset,nDealMode) values (%d ,\
		//				  %d,'%s',%d,(select nId from t_sys_drive where sName ='%s'),( select nDriveId from t_sys_drive where sName ='%s'),%d,%d,'%s',0,%d,4,1.0,0,%d)",
		//				  nMaxId++,  m_nCurDlgId,sName,0,sSysDriveName,sSysDriveName,nChannelNo,nInOut,sMark,nDataType,2);

		//	BOOL b= CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
		//	if(b)
		//	{
		//		 SetGirdItemText_string(m_grid,i,9,"写入成功_1");
		//	}
		//	else
		//		SetGirdItemText_string(m_grid,i,9,"写入失败_1");
		//}
	}

 
	//:nSharedID= 设备id<<24 +系统设备ID<<16+处理模式<<8+通道号
	strSql.Format("update t_data_config_1 set nSharedId = (nDriveid<<24)+(nSysDriveId<<16)+(nDealMode<<8)+nChannelIndex where nDlgId = %d;", m_nCurDlgId); //and nDealMode!=2 
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	////PWN电压值 PWM电压值:nSharedID =设备id<<24 +系统设备ID<<16+(通道号+1)<<8+0 
	//strSql.Format("update t_data_config_1 set nSharedId = (nDriveid<<24)+(nSysDriveId<<16)+((nChannelIndex+1)<<8) where nDlgId = %d and nDealMode=2" ,m_nCurDlgId);
	//CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	strSql.Format("delete from t_data_config where nDlgId = %d and nDriveid in (select nId from t_drive) and sName in (select sName from t_data_config_1) ",m_nCurDlgId);

	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	strSql.Format("insert into t_data_config select * from t_data_config_1 ");
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

}


void CDlgDiscrete::SaveTblData(CString& sPageName)
{
	CString sId;
	CString sName;
	CString sUnit;
	CString sValue;
	CString sPrecision;
	CString sRef;
	CString sStatus;
	CString sMinV;
	CString sMaxV;
	CString sPort;
	CString sSharedId;
	CString sDlgId;
	CString sAttr;

	int nId;
	float fValue;
	int nPrecision;
	float fMinV;
	float fMaxV;
	int nSharedId;
	int nDlgId;

	CString str;
	CString strSql;
	int n=0;
	for(int i =1,end =m_grid.GetRowCount();i<end;i++) 
	{
		n=0;
		sId = m_grid.GetItemText(i,n++);
		nId = _tstoi(sId);
		sName = m_grid.GetItemText(i,n++);
		sUnit = m_grid.GetItemText(i,n++);
		n ++; //fValue
		sPrecision = m_grid.GetItemText(i,n++);
		nPrecision = _tstoi(sPrecision);
		sRef = m_grid.GetItemText(i,n++);
		n ++; // sStatus
		sMinV = m_grid.GetItemText(i,n++);
		fMinV = _tstof(sMinV);
		sMaxV = m_grid.GetItemText(i,n++);
		fMaxV = _tstof(sMaxV);
		sPort = m_grid.GetItemText(i,n++);
		sPort = sPageName+"."+sPort;
		n ++; // nSharedId
		sDlgId = m_grid.GetItemText(i,n++);
		nDlgId = _tstoi(sDlgId);
		n ++; // Attr

		strSql.Format("insert into t_grid (nId,sName,sUnit,fValue,nPrecision,sRef,sStatus,fMinV,fMaxV,sPort,nSharedId,nDlgId,Attr) values \
			(%d ,'%s','%s',0.0,%d,'%s','',%f,%f,'%s',0,%d,'')"
			,nId, sName,sUnit,nPrecision,sRef,fMinV,fMaxV,sPort,nDlgId);
		BOOL b= CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
		if(b)
		{
			SetGirdItemText_string(m_grid,i,6,"写入成功");
		}
		else
			SetGirdItemText_string(m_grid,i,6,"写入失败");
	}

	//:nSharedID= 设备id<<24 +系统设备ID<<16+通道号
	strSql.Format("update t_grid set nSharedId = (select nSharedId from t_data_config b where sPort=b.sName) where nDlgId = %d;", m_nCurDlgId);
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
}

