// DlgMain.cpp : 实现文件
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "DlgMain.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlgMain 对话框

IMPLEMENT_DYNAMIC(CDlgMain, CDialogEx)

CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMain::IDD, pParent)
{
	m_nCurDlgId =0;
}

CDlgMain::~CDlgMain()
{

}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}


BEGIN_MESSAGE_MAP(CDlgMain, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CDlgMain::OnNMClickTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CDlgMain::OnTvnSelchangedTree1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_1553, &CDlgMain::OnBnClickedButton1553)
	ON_BN_CLICKED(IDC_BUTTON_429, &CDlgMain::OnBnClickedButton429)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL, &CDlgMain::OnBnClickedButtonSerial)
	ON_BN_CLICKED(IDC_BUTTON_signal, &CDlgMain::OnBnClickedButtonsignal)
	ON_BN_CLICKED(IDC_BUTTON_CAN, &CDlgMain::OnBnClickedButtonCan)
END_MESSAGE_MAP()


// CDlgMain 消息处理程序


BOOL CDlgMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitCtrl();
	InitTree();

	ShowDlg(0);

 
	return TRUE;  
}

void CDlgMain::InitTree()
{
	HTREEITEM rootItem = m_tree.GetRootItem();
 
	HTREEITEM hItem = m_tree.InsertItem(m_sSysName,rootItem);
	m_tree.SetItemData(hItem,0);

	vector<__DLGInfo>vec;
	CManageDb_Sqllite::GetInstance().Query_DlgInfo(vec,m_nCurSysId);

	HTREEITEM hChildItem;
	 
	for(int i =0,end = vec.size();i<end;i++)
	{
		hChildItem = m_tree.InsertItem(vec[i].sInfo.c_str(),hItem);
		m_tree.SetItemData(hChildItem,vec[i].nId); 
	}
	m_tree.Expand(hItem,TVE_EXPAND);

}
void CDlgMain::InitCtrl()
{
	
	int cx = GetSystemMetrics(SM_CXFULLSCREEN);  
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	MoveWindow(0,0,cx,cy); 
	
	CRect rt;
	GetClientRect(rt);
	int nLeft =180;
	int nTop = 80;

	m_tree.MoveWindow(10,20,150,cy-80);
	GetDlgItem(IDC_STATIC_CLIENT)->MoveWindow(nLeft,nTop,rt.Width()-nLeft-10,rt.Height()-nTop-30);
	 
	
	nLeft+=10;
	nTop+=10;

	m_pDlg1553 = new CDlg1553();
	m_pDlg1553->Create(CDlg1553::IDD,this);
	m_pDlg1553->MoveWindow(nLeft,nTop,rt.Width()-nLeft-20,rt.Height()-nTop-50);

	m_pDlg429 = new CDlg429();
	m_pDlg429->Create(CDlg429::IDD,this);
	m_pDlg429->MoveWindow(nLeft,nTop,rt.Width()-nLeft-20,rt.Height()-nTop-50);

	m_pDlgSerial = new CDlgSerial();
	m_pDlgSerial->Create(CDlgSerial::IDD,this);
	m_pDlgSerial->MoveWindow(nLeft,nTop,rt.Width()-nLeft-20,rt.Height()-nTop-50);

	 m_pDlgDiscrete = new CDlgDiscrete();
	 m_pDlgDiscrete->Create(CDlgDiscrete::IDD,this);
	 m_pDlgDiscrete->MoveWindow(nLeft,nTop,rt.Width()-nLeft-20,rt.Height()-nTop-50);

	 m_pDlgCanPro = new CDlgCANProtocol();
	 m_pDlgCanPro->Create(CDlgCANProtocol::IDD,this);
	 m_pDlgCanPro->MoveWindow(nLeft,nTop,rt.Width()-nLeft-20,rt.Height()-nTop-50);
}
void CDlgMain::ShowDlg(int nIndex)
{
	CDialogEx * pDlg[]={m_pDlg1553,m_pDlg429,m_pDlgCanPro,m_pDlgSerial,m_pDlgDiscrete};
	int nCount=5;
	for(int i =0;i<nCount;i++)
	{
		if(nIndex==i)
			pDlg[i]->ShowWindow(SW_SHOW);
		else
			pDlg[i]->ShowWindow(SW_HIDE);
	}
}


void CDlgMain::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{


	*pResult = 0;
}


void CDlgMain::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	HTREEITEM hItem=m_tree.GetSelectedItem();  
	if(hItem==NULL)
		return ;

	m_nCurDlgId = m_tree.GetItemData(hItem);
	m_sCurDlgName =m_tree.GetItemText(hItem);

	*pResult = 0;
}


void CDlgMain::OnDestroy()
{
	CDialogEx::OnDestroy();

	delete_safe(m_pDlg1553);
	delete_safe(m_pDlg429);
	delete_safe(m_pDlgSerial);
	delete_safe(m_pDlgDiscrete);
	delete_safe(m_pDlgCanPro);
}


void CDlgMain::OnBnClickedButton1553()
{
	ShowDlg(0);
}


void CDlgMain::OnBnClickedButton429()
{
	ShowDlg(1);
}


void CDlgMain::OnBnClickedButtonSerial()
{
	ShowDlg(3);
}


void CDlgMain::OnBnClickedButtonsignal()
{
	 ShowDlg(4);
}


void CDlgMain::OnBnClickedButtonCan()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowDlg(2);
}
