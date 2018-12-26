
// ReadExecelConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "ReadExecelConfigDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// CReadExecelConfigDlg �Ի���




CReadExecelConfigDlg::CReadExecelConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReadExecelConfigDlg::IDD, pParent)
	, m_nUnitNums(5)
	 
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReadExecelConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_GOODS, m_comboGoods);
	DDX_Control(pDX, IDC_LIST_DATA, m_list);
	DDX_Control(pDX, IDC_LIST_DATA2, m_list_1);
	DDX_Control(pDX, IDC_COMBO_GOODS2, m_comboBoxGoods_1);
	DDX_Control(pDX, IDC_COMBO_SYS, m_comboSys);
	DDX_Text(pDX, IDC_EDIT_UNIT_NUM, m_nUnitNums);
	 
}

BEGIN_MESSAGE_MAP(CReadExecelConfigDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SelectFile, &CReadExecelConfigDlg::OnBnClickedButtonSelectfile)
	ON_BN_CLICKED(IDC_BUTTON_Read, &CReadExecelConfigDlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_CHANAGE, &CReadExecelConfigDlg::OnBnClickedButtonChanage)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CReadExecelConfigDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_RADIO_Mode, &CReadExecelConfigDlg::OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_RADIO_Mode2, &CReadExecelConfigDlg::OnBnClickedRadioMode2)
END_MESSAGE_MAP()


// CReadExecelConfigDlg ��Ϣ�������

BOOL CReadExecelConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
 
	//InitList_2();
	 InitList(0);
	InitList(1);
	ReadSysGoods();
	return TRUE;  
}


void CReadExecelConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CReadExecelConfigDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CReadExecelConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CReadExecelConfigDlg::OnBnClickedButtonSelectfile()
{
	UpdateData();
	CString strFilePath;

	CFileDialog dlg(TRUE);

	if(dlg.DoModal()!=IDOK)
		return;
	strFilePath = dlg.GetPathName();

	//#ifdef _DEBUG
	////E:\work\prj\HKStdSys\doc
	//	strFilePath = "e:\\work\\prj\\HKStdSys\\doc\\16034-G1ģ�����˿�����_V0.0.xlsx";
	//#else
	//;
	//#endif 

	GetDlgItem(IDC_EDIT_file_path)->SetWindowText(strFilePath);
	m_comboGoods.ResetContent();

	vector<CString>vec;
  
	m_config.OpenFile(strFilePath,vec);
	
	for(int i =0,end =vec.size();i<end;i++)
		m_comboGoods.AddString(vec[i]);
		
	 
 

	m_comboGoods.SetCurSel(0);
}

void CReadExecelConfigDlg::InitList(int nType)
{
	CString strTitle[2][9]=
					{{"���ػ�һ��Ԫ","���ػ�����Ԫ","���ػ�����Ԫ","���ػ��ĵ�Ԫ","���ػ��嵥Ԫ","�������","����","��������",""},
						{"��Ԫ","����","�豸��","�忨","ͨ��","����","��������","��ע","����ʽ"}};

	CListCtrl * pCtrl[]={&m_list,&m_list_1};
	int nCol[]={8,9};

	CRect rt;
	m_list.GetClientRect(rt);
	int nW = rt.Width()/nCol[nType];

	for(int i =0;i<nCol[nType];i++)
	{
		pCtrl[nType]->InsertColumn(i,strTitle[nType][i],LVCFMT_CENTER,nW);
	}

	DWORD dwStyle = pCtrl[nType]->GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	pCtrl[nType]->SetExtendedStyle(dwStyle); 
	 
}

void CReadExecelConfigDlg::OnBnClickedButtonRead()
{ 
 		
		UpdateData();
		m_list.DeleteAllItems();
		m_list_1.DeleteAllItems();

		//InitListData_2();

		//return;
		int nSel =m_comboGoods.GetCurSel();
		if(nSel ==-1)
			return ;

		vector<__ConfigInfo>().swap(m_configVec);
		m_nUnitNum = m_nUnitNums;

		m_config.ReadInfo(nSel+1,m_configVec,m_nUnitNum);

		int nIndex;
	 
		for(int i=0,end = m_configVec.size();i<end;i++)
		{
			nIndex = m_list.InsertItem(1000,m_configVec[i].sInfo[0]);
			for(int k=1;k<m_nUnitNum;k++)
			{
				m_list.SetItemText(nIndex,k,m_configVec[i].sInfo[k]);
			}

			for(int k=m_nUnitNum;k<8;k++ )
			{
				m_list.SetItemText(nIndex,k+(5-m_nUnitNum),m_configVec[i].sInfo[k]);
			}
		}
	 
	
}


void CReadExecelConfigDlg::OnBnClickedButtonChanage()
{
//Analysis_2();
	//return;
/*
	CString sDut[]={"���ػ�һ��Ԫ","���ػ�����Ԫ","���ػ�����Ԫ","���ػ��ĵ�Ԫ","���ػ��嵥Ԫ"};
	CString sInfo[8];

	for(int i =0,end = m_list.GetItemCount();i<end;i++)
	{ 
		sInfo[5] = m_list.GetItemText(i,2);
		sInfo[1] = m_list.GetItemText(i,1);
		sInfo[6] = m_list.GetItemText(i,3);


		for(int  k =0;k<m_nUnitNum;k++)
		{
			sInfo[0]=sDut[k];
			sInfo[7] = m_list.GetItemText(i,k);

			if(Analysis(sInfo[7],sInfo[2],sInfo[3],sInfo[4]))
			{
				InsertData(k,sInfo);
			}

		}
	}
	*/
 
	CString sDut[]={"���ػ�һ��Ԫ","���ػ�����Ԫ","���ػ�����Ԫ","���ػ��ĵ�Ԫ","���ػ��嵥Ԫ"};
	CString sInfo[9];
 //  ��Ԫ���� ������������ƣ��豸���ƣ��忨

	for(int i =0,end = m_list.GetItemCount();i<end;i++)
	{ 
		sInfo[5] = m_list.GetItemText(i,6);
		sInfo[1] = m_list.GetItemText(i,5);
		sInfo[6] = m_list.GetItemText(i,7);
		sInfo[6].MakeUpper();
		if(sInfo[6].Find("PWM")!=-1)
			sInfo[8]="PWM";
		else
			sInfo[8]="";


		for(int  k =0;k<m_nUnitNum;k++)
		{
			sInfo[0]=sDut[k];
			sInfo[7] = m_list.GetItemText(i,k);
			if(sInfo[7].Find("5410")!=-1 || sInfo[7].Find("5141")!=-1 || sInfo[7].Find("5142")!=-1 || sInfo[7].Find("5121")!=-1 ) 
			{
				sInfo[6]="����";
			}
			else if(sInfo[7].Find("5033")!=-1)
			{
				sInfo[6]="������";
			}
			if(	Analysis(sInfo[7],sInfo[2],sInfo[3],sInfo[4]))
			{ 
				InsertData(k,sInfo);
			}

		}

	}
}


void CReadExecelConfigDlg::InsertData(int nDut,CString strInfo[9])
{ 
	int nIndex =m_list_1.InsertItem(1000,strInfo[0]);
	m_list_1.SetItemData(nIndex,nDut);

	for(int i =1;i<9;i++)
		m_list_1.SetItemText(nIndex,i,strInfo[i]);	

}
bool CReadExecelConfigDlg::Analysis(CString &sInfo,CString& sSysName,CString &sDriveName,CString& sChannelIndex)
{
	sInfo.MakeUpper();
	int nPos = sInfo.Find("-");
	if(nPos ==-1)
		return false;
	CString str = sInfo.Left(nPos);
	str.Trim();

	sSysName = str;
	sDriveName = sSysName.Left(4);

	nPos= sInfo.Find("CH");
	if(nPos==-1)
		return false;

	sChannelIndex = sInfo.Right(sInfo.GetLength() - nPos-2);
	return true;
}

void CReadExecelConfigDlg::OnBnClickedButtonSave()
{

	CWaitCursor wait;
	int nSel = m_comboBoxGoods_1.GetCurSel();
	if(nSel ==-1)
	{
		AfxMessageBox("δѡ���Ʒ");
		return ;
	}
	CString strToName ;
	m_comboBoxGoods_1.GetWindowText(strToName);
	CString strSrcName;
	m_comboGoods.GetWindowText(strSrcName);
	if(strToName!=strSrcName)
	{
		int  nRe = AfxMessageBox("���Ʋ���ͬ,�Ƿ����",MB_YESNO);
		if(nRe == IDNO)
			return;
	}
	int nDlgId = m_comboBoxGoods_1.GetItemData(nSel);
	int nDutIndex;
	int nChannelIndex;
	CString sName;
	int nInOut; //1���룬2�����

	int nDataType;
	CString sSysDriveName;
	CString sMark;
	int  nDealMode;//����ʽ
	  
	CString str;
	

	int nMaxId = CManageDb_Sqllite::GetInstance().GetMaxId(" select max(nId) from  t_data_config ")+1;
	
	
	CString strSql;
	strSql.Format("delete from t_data_config_1 ");
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	for(int i =0,end =m_list_1.GetItemCount();i<end;i++)
	{
		nDutIndex = m_list_1.GetItemData(i);
		sName = m_list_1.GetItemText(i,1);
		sSysDriveName = m_list_1.GetItemText(i,2);
		nChannelIndex=atoi(m_list_1.GetItemText(i,4));
		
		str = m_list_1.GetItemText(i,5);
		if(str =="���")
			nInOut =2;
		else if(str =="����")
			nInOut =1;
		else
			nInOut =0; 

		str = m_list_1.GetItemText(i,6);
		if(str =="����")
			nDataType =0;
		else if(str =="������")
			nDataType =1;
		else if(str.Find("ģ����")!=-1  )
			nDataType =1;
		else
			nDataType =0;
		sMark =m_list_1.GetItemText(i,7);

		str = m_list_1.GetItemText(i,8);
		if(str=="PWM")
			nDealMode = 1;
		else
			nDealMode =0;

		strSql.Format("insert into t_data_config_1 (nId,nDlgId,sName,nDutIndex,nSysDriveId,nDriveid,nChannelIndex,nInOut,mark,nSharedId,nDataType,nDataSize,fTimes,nDealMode) values (%d ,\
					  %d,'%s',%d,(select nId from t_sys_drive where sName ='%s'),( select nDriveId from t_sys_drive where sName ='%s'),%d,%d,'%s',0,%d,4,1.0,%d)",
					nMaxId++,  nDlgId,sName,nDutIndex,sSysDriveName,sSysDriveName,nChannelIndex,nInOut,sMark,nDataType,nDealMode);
		 BOOL b= CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
		 if(b)
		 {
			 m_list_1.SetItemText(i,8,"д��ɹ�");
		 }
		 else
			  m_list_1.SetItemText(i,8,"д��ʧ��");

		 if(b && nDealMode==1)
		 {
			 strSql.Format("insert into t_data_config_1 (nId,nDlgId,sName,nDutIndex,nSysDriveId,nDriveid,nChannelIndex,nInOut,mark,nSharedId,nDataType,nDataSize,fTimes,nDealMode) values (%d ,\
					  %d,'%s',%d,(select nId from t_sys_drive where sName ='%s'),( select nDriveId from t_sys_drive where sName ='%s'),%d,%d,'%s',0,%d,4,1.0,%d)",
					nMaxId++,  nDlgId,sName,nDutIndex,sSysDriveName,sSysDriveName,nChannelIndex,nInOut,sMark,nDataType,2);
			 BOOL b= CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
			 if(b)
			 {
				 m_list_1.SetItemText(i,8,"д��ɹ�_1");
			 }
			 else
				  m_list_1.SetItemText(i,8,"д��ʧ��_1");
		 }
	}

	//:nSharedID= �豸id<<24 +ϵͳ�豸ID<<16+ͨ����
	strSql.Format("update t_data_config_1 set nSharedId = (nDriveid<<24)+(nSysDriveId<<16)+nChannelIndex where nDlgId = %d and nDealMode!=2 " ,nDlgId);
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
	
	//PWN��ѹֵ PWM��ѹֵ:nSharedID =�豸id<<24 +ϵͳ�豸ID<<16+(ͨ����+1)<<8+0 
	strSql.Format("update t_data_config_1 set nSharedId = (nDriveid<<24)+(nSysDriveId<<16)+((nChannelIndex+1)<<8) where nDlgId = %d and nDealMode=2" ,nDlgId);
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	strSql.Format("delete from t_data_config where nDlgId = %d and nDriveid in (select nId from t_drive)",nDlgId);
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
	
	strSql.Format("insert into t_data_config select * from t_data_config_1 ");
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
		 
}

void CReadExecelConfigDlg::ReadSysGoods()
{
	//int nIndex ;
	//vector<__SysInfo>vecsys;
	//CManageDb_Sqllite::GetInstance().Query_Sys(vecsys);
	//for(int i =0,end = vecsys.size();i<end;i++)
	//{
	//	nIndex =m_comboSys.AddString(vecsys[i].sName.c_str());
	//	m_comboSys.SetItemData(nIndex,vecsys[i].nId);

	//	vector<__DLGInfo>vec;
	//	CManageDb_Sqllite::GetInstance().Query_DlgInfo(vec,vecsys[i].nId);

	//	for(int k_cnt=0,end1 = vec.size();k_cnt<end1;k_cnt++)
	//	{
	//		int nIndex1 =m_comboBoxGoods_1.AddString(vec[k_cnt].sInfo.c_str());
	//		m_comboBoxGoods_1.SetItemData(nIndex1,vec[k_cnt].nId);
	//	}

	//}
	//m_comboSys.SetCurSel(0);

	 
	
	vector<__DLGInfo>vec;
	CManageDb_Sqllite::GetInstance().Query_DlgInfo(vec,-1);

	for(int k_cnt=0,end1 = vec.size();k_cnt<end1;k_cnt++)
	{
		int nIndex1 =m_comboBoxGoods_1.AddString(vec[k_cnt].sInfo.c_str());
		m_comboBoxGoods_1.SetItemData(nIndex1,vec[k_cnt].nId);
	}
}

void CReadExecelConfigDlg::InitList_2()
{
	CString strTitle[]={"�������","����"};
	CRect rt;
	m_list.GetClientRect(rt);
	int nW = 200;//rt.Width()/nCol[nType];

	for(int i =0;i<2;i++)
	{
		m_list.InsertColumn(i,strTitle[i],LVCFMT_CENTER,nW);
	}

	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_list.SetExtendedStyle(dwStyle); 

}
void CReadExecelConfigDlg::InitListData_2()
{
	int nSel =m_comboGoods.GetCurSel();
	if(nSel ==-1)
		return ;

	vector<__ConfigInfo>().swap(m_configVec);

	 m_config.ReadInfo_2(nSel+1,m_configVec);

	 int nIndex;
	 for(int i =0,end = m_configVec.size();i<end;i++)
	 {
		nIndex =  m_list.InsertItem(1000,m_configVec[i].sInfo[0]);
		m_list.SetItemText(nIndex,1,m_configVec[i].sInfo[1]);
	 }
}
void CReadExecelConfigDlg::Analysis_2()
{
	CString strInfo;
	CString strName;

	CString sSysName,sDriveName,sChannelIndex;
	int nIndex;
	for(int i=0,end = m_list.GetItemCount();i<end;i++)
	{

		strInfo =m_list.GetItemText(i,0);
		if(!Analysis(strInfo,sSysName,sDriveName,sChannelIndex))
			continue;
		nIndex = m_list_1.InsertItem(1000,"");
		m_list_1.SetItemText(nIndex,1,strInfo);
		m_list_1.SetItemText(nIndex,2,sSysName);
		m_list_1.SetItemText(nIndex,3,sDriveName);
		m_list_1.SetItemText(nIndex,4,sChannelIndex);
		m_list_1.SetItemText(nIndex,7,strInfo);

		strInfo =m_list.GetItemText(i,1);

		if(strInfo =="ָʾ��")
		{
		 	m_list_1.SetItemText(nIndex,5,"����"); 
			m_list_1.SetItemText(nIndex,6,"����");
		}
	 	else if(strInfo =="����")
		{
			m_list_1.SetItemText(nIndex,5,"���");
			 m_list_1.SetItemText(nIndex,6,"����");
		}
		else if(strInfo =="��ѹ")
		{
			m_list_1.SetItemText(nIndex,5,"����");
			m_list_1.SetItemText(nIndex,6,"������");
		}
		m_list_1.SetItemData(nIndex,0);

	}

}

void CReadExecelConfigDlg::OnBnClickedRadioMode()
{
 
}


void CReadExecelConfigDlg::OnBnClickedRadioMode2()
{
	 
}


