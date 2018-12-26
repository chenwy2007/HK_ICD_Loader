// DlgCommProMain.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "DlgCommProMain.h"
#include "afxdialogex.h"
#include "DlgStart.h"
#include "DlgLoadIcdFile.h"
#include "htmlhelp.h"

//#pragma commet(lib,"htmlhelp.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDlgCommProMain �Ի���

IMPLEMENT_DYNAMIC(CDlgCommProMain, CDialogEx)

CDlgCommProMain::CDlgCommProMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCommProMain::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nCurTabIndex = -1;
	m_nTotalProNum = 0;
	m_nCurPos = 0;
	m_vtSaveNode.clear();
	m_hSaveToDB_Thread = NULL;
	m_hLoadIcd_Thread = NULL;

	m_nCurProType = 0;
	m_sCurIcdFilePath = "";
	m_nTreeId = 0;
	m_nTreeDBId = 0;
	m_nTreeExcelId = 0;
	m_pdlgDBProData = NULL;
	m_bLoadDBICD = FALSE;
	m_lCurProType= -2;
}

CDlgCommProMain::~CDlgCommProMain()
{
	if (m_pdlgDBProData != NULL){

		m_pdlgDBProData->DestroyWindow();
		delete m_pdlgDBProData;

		m_pdlgDBProData = NULL;
	}
}

void CDlgCommProMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TAB_PROTYPE, m_tabCtrl);
	//DDX_Control(pDX, IDC_STATIC_CURSYS, m_static_cursys);
	//DDX_Control(pDX, IDC_COMBO_CURSYS, m_combox_cursys);
	DDX_Control(pDX, IDC_GRID_PROTOCOL, m_grid_protocol);
	DDX_Control(pDX, IDC_TREE_PROTOCOL, m_tree_protocol);
}


BEGIN_MESSAGE_MAP(CDlgCommProMain, CDialogEx)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PROTYPE, &CDlgCommProMain::OnTcnSelchangeTabProtype)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PROTOCOL, &CDlgCommProMain::OnTvnSelchangedTreeProtocol)
	ON_COMMAND(ID_LOADICDFILE, &CDlgCommProMain::OnLoadicdfile)
	ON_COMMAND(ID_SAVETODB, &CDlgCommProMain::OnSavetodb)
	ON_COMMAND(ID_SELECTSYS, &CDlgCommProMain::OnSelectsys)
	ON_COMMAND(ID_HELP, &CDlgCommProMain::OnHelp)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_TREE_PROTOCOL, &CDlgCommProMain::OnNMCustomdrawTreeProtocol)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify)
	ON_NOTIFY(NM_DBLCLK, IDC_GRID_PROTOCOL, OnGridDBClick)
	ON_COMMAND(ID_PROTOCOL_DB, &CDlgCommProMain::OnProtocolDb)
	ON_WM_PAINT()
	ON_COMMAND(ID_LOADCTRLFILE, &CDlgCommProMain::OnLoadctrlfile)
	ON_COMMAND(ID_ABOUT, &CDlgCommProMain::OnAbout)
END_MESSAGE_MAP()


// CDlgCommProMain ��Ϣ�������


void CDlgCommProMain::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(cx <= 100 || cy == 100)
		return;

	CRect rect = CRect(0,0,cx,cy);
	CRect tab_rect = rect;
	// TODO: �ڴ˴������Ϣ����������
	//if(m_tabCtrl.GetSafeHwnd())
	//{
	//	tab_rect.top = 5 + TITLE_HEIGHT + 26;
	//	tab_rect.left = 5;
	//	tab_rect.bottom = rect.bottom - STATE_HEIGHT - 5;
	//	tab_rect.right = rect.right - 5;

	//	m_tabCtrl.MoveWindow(&tab_rect);
	//}

	CRect comb_rect = rect;
	//if(m_combox_cursys.GetSafeHwnd())
	//{
	//	comb_rect.top = 5 + TITLE_HEIGHT;
	//	comb_rect.left = rect.right - 205;
	//	comb_rect.bottom = comb_rect.top + 20;
	//	comb_rect.right = rect.right - 5;
	//	m_combox_cursys.MoveWindow(&comb_rect);
	//}

	CRect st_rect = comb_rect;
	//if(m_static_cursys.GetSafeHwnd())
	//{
	//	st_rect.top += 5;
	//	st_rect.left = comb_rect.left - 155;
	//	st_rect.bottom += 5;
	//	st_rect.right = comb_rect.left - 5;
	//	m_static_cursys.MoveWindow(&st_rect);
	//}

	CRect tree_rect = rect;
	if(m_tree_protocol.GetSafeHwnd())
	{
		tree_rect.top = rect.top + TITLE_HEIGHT;
		tree_rect.left = 5;
		tree_rect.bottom = rect.bottom - STATE_HEIGHT;
		tree_rect.right = tree_rect.left + 280;
		m_tree_protocol.MoveWindow(&tree_rect);
	}

	CRect grid_rect = rect;
	if(m_grid_protocol.GetSafeHwnd())
	{
		grid_rect.left = tree_rect.right + 5;
		grid_rect.top = tree_rect.top;
		grid_rect.bottom = tree_rect.bottom;
		grid_rect.right = rect.right - 5;
		m_grid_protocol.MoveWindow(&grid_rect);

		int nColNum = m_grid_protocol.GetColumnCount();
		if(nColNum > 0)
		{
			m_grid_protocol.GetClientRect(&grid_rect);
			int nW = (grid_rect.Width()-GRID_SPARE)/nColNum;
			for(int i = 0; i < nColNum; ++ i)
			{
				if(i == nColNum-1)
					m_grid_protocol.SetColumnWidth(i,grid_rect.Width()-(nColNum-1)*nW);
				else
					m_grid_protocol.SetColumnWidth(i, nW);
			}
			m_grid_protocol.Invalidate();
		}
	}

	if(m_Statusbar.GetSafeHwnd())
	{
		UINT nId = 0;
		UINT nStyle = 0;
		int nWidth = 0;
		m_Statusbar.GetPaneInfo(0,nId,nStyle,nWidth);
		nWidth = cx/3;
		m_Statusbar.SetPaneInfo(0,nId,nStyle,nWidth);

		m_Statusbar.GetPaneInfo(1,nId,nStyle,nWidth);
		nWidth = cx*2/3-10;
		m_Statusbar.SetPaneInfo(1,nId,nStyle,nWidth);

		RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
	}
}


void CDlgCommProMain::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}


BOOL CDlgCommProMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//SetSideWidth(TITLE_HEIGHT,STATE_HEIGHT,3);
	CString strFrmTitle = "ͨ��Э�飨ICD������ϵͳ";
	//SetTitleTxt(strFrmTitle);
	//SetStatusTxt(1,"�Ϻ����յ������޹�˾--����");
	//SetBackColor(RGB(28,28,28),RGB(70,70,70));
	//SetEnableBt(true,false);
	SetWindowText(strFrmTitle);
	//SetDialogBkColor(RGB(0,0,255),RGB(255,0,0));

	m_menu_main.LoadMenu(IDR_MENU_MAIN);
	SetMenu(&m_menu_main);

	CMenu* submenu =m_menu_main.GetSubMenu(0);
	submenu->EnableMenuItem(ID_LOADCTRLFILE,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	//CBitmap bmp;
	//bmp.LoadBitmap(IDB_BMP_BK);
	//m_BKBrush.CreatePatternBrush(&bmp);//����λͼ��ˢ
	//MENUINFO mnInfo;
	//memset(&mnInfo,0,sizeof(MENUINFO));
	//mnInfo.cbSize=sizeof(MENUINFO);
	//mnInfo.fMask=MIM_BACKGROUND;
	//mnInfo.hbrBack=m_BKBrush;
	//CMenu *pSubMenu=m_menu_main.GetSubMenu(0);//����Ӳ˵�������У�0��ʾ��������Ӧ���ļ����˵�
	//::SetMenuInfo(pSubMenu->m_hMenu,&mnInfo);
	//m_BKBrush.Detach();

	//m_ImageList.Create(16, 16, ILC_COLOR32 ,10,CLR_NONE);
	m_ImageList.Create(16, 16,  ILC_COLOR32| ILC_MASK, 4, 2);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_MTS));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_MTS));
	
	m_ToolBarImage.Create(16, 16,  ILC_COLOR32| ILC_MASK, 4, 2);
	m_ToolBarImage.Add(AfxGetApp()->LoadIcon(IDI_ICON_SYSTEM));
	m_ToolBarImage.Add(AfxGetApp()->LoadIcon(IDI_ICON_OPEN));
	m_ToolBarImage.Add(AfxGetApp()->LoadIcon(IDI_ICON_SAVE));
	m_ToolBarImage.Add(AfxGetApp()->LoadIcon(IDI_ICON_DBICD));

	// ������������������Դ
	if(!m_wndToolBar.Create(this))
	{
		TRACE0( "Failed to Create Dialog Toolbar\n ");
		EndDialog(IDCANCEL);
	}
	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
	//	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}
	//m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);

	UINT nArray[4] = {ID_SELECTSYS,ID_LOADICDFILE,ID_SAVETODB,ID_PROTOCOL_DB};
	m_wndToolBar.SetButtons(nArray,4);
	m_wndToolBar.SetSizes(CSize(24,24),CSize(16,16));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ToolBarImage);
	//m_wndToolBar.SetButtonText(0,"ѡ��ϵͳ");
	//m_wndToolBar.SetButtonText(1,"����ICD�ļ�");
	//m_wndToolBar.SetButtonText(2,"���浽���ݿ�");
	m_wndToolBar.EnableToolTips();

	//static CBitmap  myBitmap;
	//myBitmap.LoadBitmap( IDB_BMP_TOOL);
	//if ( !m_wndReBar.Create( this) || 
	//	!m_wndReBar.AddBar(&m_wndToolBar, NULL, &myBitmap))
	//{
	//	TRACE0("Failed to create rebar\n");
	//	return -1;      // fail to create
	//}
	//m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle()|CBRS_TOOLTIPS | CBRS_FLYBY|CBRS_ALIGN_ANY);

	if(!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_MAIN))
	{
		TRACE0( "Failed to Create Dialog Toolbar\n ");
		EndDialog(IDCANCEL);
	}

	CTime time;
	time=time.GetCurrentTime();
	CString stime;
	stime=time.Format("%y-%m-%d %H:%M:%S"); 
	// ���״̬��
	UINT array[2]={12301,12302};
	m_Statusbar.Create(this);
	m_Statusbar.SetIndicators(array,sizeof(array)/sizeof(UINT));

	//��ʾ״̬��
	CRect rect;
	GetWindowRect(rect);
	m_Statusbar.SetPaneInfo(0,array[0],0,rect.Width()/3);
	m_Statusbar.SetPaneInfo(1,array[1],0,rect.Width()/3*2-10);
	m_Statusbar.SetPaneText(0,stime);
	m_Statusbar.SetPaneText(1,_T("����..."));

	m_tree_protocol.SetImageList(&m_ImageList,TVSIL_STATE);//TVSIL_NORMAL
	//m_tree_protocol.SetImageList(&m_ImageList,TVSIL_NORMAL);//

	//m_ocx.Set_Product(1);
	//m_ocx.ShowDlg(m_nDlgId);
	//m_ocx.StartReadData(1);

	//m_btnStart.SetBkColor(RGB(70,70,70));
	//m_btnStart.SetForeColor(RGB(255,255,255));

	//m_btnStop.SetBkColor(RGB(70,70,70));
	//m_btnStop.SetForeColor(RGB(255,255,255));

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	int full_x = GetSystemMetrics(SM_CXSCREEN); 
	int full_y = GetSystemMetrics(SM_CYSCREEN); 
	//int cx = GetSystemMetrics(SM_CXFULLSCREEN);  
	//int cy = GetSystemMetrics(SM_CYFULLSCREEN);

	//InitialCombox();
	//InitTabCtrl();
	//ShowTab(0);

	full_x -= 100;
	full_y -= 100;
	MoveWindow(50,50,full_x,full_y);

	//CRect proRect;
	////GetClientRect(&rect);
	//proRect.left = 50 + (full_x-50)/2 - 150;
	//proRect.top = 50 + (full_y-50)/2 - 15;
	//proRect.right = full_x - (full_x-50)/2 + 150;
	//proRect.bottom = full_y - (full_y-50)/2 + 15;
	////WS_CHILD|WS_VISIBLE|PBS_SMOOTHREVERSE 
	//m_myProCtrl.Create(WS_VISIBLE,proRect,this,20000); //����λ�á���С
	//m_myProCtrl.ShowWindow(SW_HIDE);

	//RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);

	m_pdlgDBProData = new CDlgDBProtocolICD;
	m_pdlgDBProData->SetTreeData(&m_DBIcdTree);
	m_pdlgDBProData->Create(IDD_DLG_DBVIEW,this);
	m_pdlgDBProData->ShowWindow(SW_HIDE);

	::CoInitialize(NULL);
	m_Pro1553B.Initailize();
	m_ProDiscrete.Initailize();
	m_ProCan.Initailize();
	m_ProSerial.Initailize();
	m_ProArinc429.Initailize();
	::CoUninitialize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


//void CDlgCommProMain::OnTcnSelchangeTabProtype(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//}

void CDlgCommProMain::ShowTab(int nIndex)
{
	if(m_nCurTabIndex == nIndex)
		return;
	m_nCurTabIndex = nIndex;
	//CDialogEx *pDlg[] ={m_pDlgAnalogSignal,/*m_pDlgSysTblData,*/m_pDlgCanMain,m_pDlg1553BSim,/*m_pDlgCanDashBoard,*/m_pDlgPwmInfor};
	//int nCount=sizeof(pDlg)/sizeof(CDialogEx*);
	//for(int i =0;i<nCount;i++)
	//{
	//	if(pDlg[i]==NULL)
	//		continue;
	//	if(nIndex == i)
	//	{
	//		pDlg[i]->ShowWindow(SW_SHOW);
	//	}
	//	else 
	//		pDlg[i]->ShowWindow(SW_HIDE);
	//}
}

//void CDlgCommProMain::InitTabCtrl()
//{
//	CString strTitle[]={"1553B","CAN","422��������)","429","��ɢ��"};
//	int nCount =5;
//	//m_tabCtrl.SetItemTabSize(100,20);
//	m_tabCtrl.SetFontInfo(16);
//	m_tabCtrl.SetItemSize(CSize(m_tabCtrl.m_nTabWidth,m_tabCtrl.m_nTabHight));
//	m_tabCtrl.SetMinTabWidth(m_tabCtrl.m_nTabWidth);
//
//	for(int i =0;i<nCount;i++)
//		m_tabCtrl.InsertItem(i,strTitle[i]);
//
//
//
//}

//void CDlgCommProMain::InitialCombox()
//{
//	vector<__DLGInfo>vec;
//	CManageDb_Sqllite::GetInstance().Query_DlgInfo(vec,m_nCurSysId);
//
//	m_combox_cursys.ResetContent();
//	for(int i =0,end = vec.size();i<end;i++)
//	{
//		m_combox_cursys.AddString(vec[i].sInfo.c_str());
//	}
//	m_combox_cursys.SetCurSel(0);
//}



void CDlgCommProMain::OnTvnSelchangedTreeProtocol(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	HTREEITEM hItem=m_tree_protocol.GetSelectedItem();  
	if(hItem==NULL)
		return ;

	TREEDATA& TreeNodeItem = *(TREEDATA*)m_tree_protocol.GetItemData(hItem);
	m_nCurDlgId = TreeNodeItem.nDlgId;
	m_sCurDlgName = m_tree_protocol.GetItemText(hItem);
	m_lCurProType = TreeNodeItem.nProType;

	DisplayGridData(TreeNodeItem);

	m_tree_protocol.Select(hItem, TVGN_DROPHILITE);

	*pResult = 0;
}

void CDlgCommProMain::OnGridDBClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNMHDR;

	if(pItem)
	{
		long nTreeId = (long)m_grid_protocol.GetItemData(pItem->iRow,pItem->iColumn);
		// �ҵ���ӦnTreeId�Ľڵ㣬չ�����ڵ㣬����ʾ�ڵ�����
		if(nTreeId != 0)
		{
			HTREEITEM hItem=m_tree_protocol.GetSelectedItem();  
			if(hItem==NULL)
				return ;

			HTREEITEM hFItem = FindTreeNode(hItem,nTreeId,0);
			if(hFItem != 0)
			{
				TREEDATA& TreeItem = *(TREEDATA*)m_tree_protocol.GetItemData(hFItem);
				// ��ʾ�������
				DisplayGridData(TreeItem);

				// ѡ�����ڵ�
				m_tree_protocol.SelectItem(hFItem); 
				m_tree_protocol.Select(hFItem, TVGN_DROPHILITE);

				// չ��ѡ��Ľڵ�
				m_tree_protocol.Expand(hItem, TVE_EXPAND);
			}
		}
	}
}

HTREEITEM CDlgCommProMain::FindTreeNode(HTREEITEM hItem,int nTreeId,int nNodeNo)
{
	HTREEITEM hFind;
	HTREEITEM hChildItem;
	++ nNodeNo;

	while(hItem != NULL)   
	{   
		TREEDATA& TreeItem = *(TREEDATA*)m_tree_protocol.GetItemData(hItem);
		if(TreeItem.lID == nTreeId)
		{
			return hItem;
		}

		if(m_tree_protocol.ItemHasChildren(hItem))   
		{   
			hChildItem = m_tree_protocol.GetChildItem(hItem);   
			hFind = FindTreeNode(hChildItem,nTreeId,nNodeNo);   
			if(hFind)   
			{   
				return hFind;   
			}   
			else   
				hItem= m_tree_protocol.GetNextSiblingItem(hItem);   
		}   
		else if(nNodeNo != 1)
		{   
			hItem= m_tree_protocol.GetNextSiblingItem(hItem);   
		}
		else
		{
			hItem = NULL;
		}
	}  

	return NULL;   
}

void CDlgCommProMain::DisplayGridData(TREEDATA& TreeNode)
{
	// ��ʾ���ڵ�����
	if(TreeNode.nProType == -2)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		vector<vector<string>> vtTemp;
		vector<string> vtTData;
		vtTData.push_back("���ڵ�");
		vtTData.push_back("�ӽڵ�");
		vtTemp.push_back(vtTData);
		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
		//DispRootInfor();
	}
	// ��ʾ����ϵͳ�½ڵ�����
	else if(TreeNode.nProType == -1)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		vector<vector<string>> vtTemp;
		vector<string> vtTData;
		vtTData.push_back("���ڵ�");
		vtTData.push_back("�ӽڵ�");
		vtTemp.push_back(vtTData);
		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
		//DispSystemInfor();
	}
	// ��ʾ1553BЭ��ڵ�����
	else if(TreeNode.nProType == 0)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		//m_Pro1553B.ReadSheetData(TreeNode.nData,vtSheetData);
		vector<vector<string>> vtTemp;
		m_Pro1553B.GetTempData(vtTemp,TreeNode.sDataType);

		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
	}
	// ��ʾ����Э��ڵ�����
	else if(TreeNode.nProType == 1)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		vector<vector<string>> vtTemp;
		m_ProSerial.GetTempData(vtTemp,TreeNode.sDataType);

		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
	}
	// ��ʾ429Э��ڵ�����
	else if(TreeNode.nProType == 2)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		vector<vector<string>> vtTemp;
		m_ProArinc429.GetTempData(vtTemp,TreeNode.sDataType);

		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
	}
	// ��ʾCANЭ��ڵ�����
	else if(TreeNode.nProType == 3)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		//m_ProCan.ReadSheetData(TreeNode.nData,vtSheetData);
		vector<vector<string>> vtTemp;
		m_ProCan.GetTempData(vtTemp,TreeNode.sDataType);

		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
	}
	// ��ʾ��ɢ���ڵ�����
	else if(TreeNode.nProType == 4)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		vector<vector<string>> vtTemp;
		m_ProDiscrete.GetTempData(vtTemp,TreeNode.sDataType);

		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
	}
}

void CDlgCommProMain::DispNodeInfor(vector<vector<string>>& vtInfor, 
	vector<vector<string>>& vtTemp,TREEDATA& TreeItem)
{
	int nRow_Count =vtInfor.size();
	if(nRow_Count <= 0) return;
	int nCol_Count =vtInfor[0].size();
	if(nCol_Count <= 0) return;
	int nRow_DispCount = nRow_Count;
	if(nRow_DispCount < 40)
		nRow_DispCount = 40;

	int nSubItemNum = TreeItem.subNode.size();
	int nSubIdx = 0;

	CRect rt;
	m_grid_protocol.DeleteAllItems();
	//m_grid_protocol.Invalidate(FALSE);

	m_grid_protocol.SetEditable(false);
	m_grid_protocol.SetDefCellHeight(30);
	m_grid_protocol.SetColumnCount(nCol_Count);
	m_grid_protocol.SetRowCount(nRow_DispCount);
	m_grid_protocol.SetFixedRowCount();
	m_grid_protocol.GetClientRect(&rt);

	int nW = (rt.Width()-GRID_SPARE)/nCol_Count;
	vector<string>& TitleItem = vtInfor[0];
	for(int i =0;i<nCol_Count;i++)		
	{
		SetGirdItemText(m_grid_protocol,0,i,TitleItem[i].c_str(),GVNI_READONLY,TITLE_HEAD_COLOR,COLOR_BK_JPG,DT_CENTER);	
		if(i == nCol_Count-1)
			m_grid_protocol.SetColumnWidth(i,rt.Width()-(nCol_Count-1)*nW);
		else
			m_grid_protocol.SetColumnWidth(i,nW);
	}

	int nColIdx = 0;
	int nRowIdx = 1;
	CString sTemp = "";
	for(int i = 1; i < nRow_Count; ++ i)
	{
		vector<string>& RowItem = vtInfor[i];
		//// ���ΪBLOCK���ݣ�ֻ��ʾBLOCK��¼
		//if(TreeItem.sDataType == "PROOT")
		//{
		//	if(RowItem[2] != "BLOCK")
		//		continue;
		//}
		for(int j = 0; j < nCol_Count; ++ j)
		{
			SetGirdItemText_string(m_grid_protocol,i,j,RowItem[j].c_str(),GVNI_READONLY);
		}
		//++ nRealRow_Count;
	}

	// �������Ӧ�и�
	for (int nRow = 0; nRow < nRow_Count; nRow++)
	{
		//  Skip hidden rows when autosizing
		if( m_grid_protocol.GetRowHeight( nRow) > 0 )
			m_grid_protocol.AutoSizeRow(nRow);
	}

	// ����ģ�����ñ������
	long lTemp_RowNum = vtTemp.size();
	if(lTemp_RowNum <= 0) return;
	long lTemp_ColumNum = vtTemp[0].size();
	if(lTemp_ColumNum <= 0) return;

	long lTblTailNum = 2;
	long lTblHeadNum = 0;
	long lSpaceNum = 0;
	long lStartColum = 0;

	int n = 0;
	bool bTblHead = true;
	bool bTblTail = false;
	for(long i = 0; i < nRow_Count; ++ i)
	{
		lSpaceNum = 0;

		// ��ͷģ��
		if(i < lTemp_RowNum && bTblHead)
		{
			vector<string>& vtTempRowItem = vtTemp[i];
			for(long j = 0; j < nCol_Count; ++ j)
			{
				if(j >= lTemp_ColumNum || j >= nCol_Count) break;
				if(vtTempRowItem[j] != "")
				{
					const LOGFONT* lpFont = m_grid_protocol.GetItemFont(i,j);
					LOGFONT ItemFont;
					memcpy(&ItemFont,lpFont,sizeof(LOGFONT));
					ItemFont.lfWeight = 700;	// ���óɺ���
					m_grid_protocol.SetItemFont(i,j,&ItemFont);
				}
				else
				{
					++ lSpaceNum;
				}
			}
			// ģ������пո��ʾ��ͷ�Ľ���
			if(lSpaceNum == lTemp_ColumNum || lSpaceNum == nCol_Count)
			{
				bTblHead = false;
			}
			lTblHeadNum = i;
		}
		else
		{
			bTblHead = false;
		}

		// ��βģ��
		lSpaceNum = 0;
		if(i >= nRow_Count-lTblTailNum && !bTblHead)
		{
			n = lTemp_RowNum-(nRow_Count-i);
			if(lTblHeadNum < n && n < lTemp_RowNum)
			{
				vector<string>& vtTempRowItem = vtTemp[n];

				for(long j = 0; j < nCol_Count; ++ j)
				{
					if(j >= lTemp_ColumNum) break;
					if(vtTempRowItem[j] != "")
					{
						const LOGFONT* lpFont = m_grid_protocol.GetItemFont(i,j);
						LOGFONT ItemFont;
						memcpy(&ItemFont,lpFont,sizeof(LOGFONT));
						ItemFont.lfWeight = 700;	// ���óɺ���
						m_grid_protocol.SetItemFont(i,j,&ItemFont);
					}
					else
					{
						++ lSpaceNum;
					}
				}

				if(lSpaceNum != lTemp_ColumNum)
					bTblTail = true;
			}
		}

		// ����������������PROOT��BLOCK���ݣ������ù������ݼ���ɫ
		if(TreeItem.sDataType == "PROOT" || TreeItem.sDataType == "ITEM" || 
			TreeItem.sDataType == "ROOT" || TreeItem.sDataType == "SYSTEM")
		{
			// ��ɢ��BLOCK���ݲ�������ʾ
			if(TreeItem.nProType == 4 && TreeItem.sDataType == "ITEM")
				continue;
			// ��ͷ����β���ݲ���ʾ����
			if(!bTblHead && !bTblTail)
			{
				if(TreeItem.sDataType == "PROOT" || TreeItem.sDataType == "ITEM")
					lStartColum = 0;
				else
					lStartColum = 1;

				// ����淶�Ż��ź�����Ϊ�������ø�������������
				if(TreeItem.sDataType == "ITEM")
				{
					vector<string>& RowItem = vtInfor[i];
					if(RowItem[lStartColum] == "" || RowItem[lStartColum+1] == "")
						continue;
				}

				const LOGFONT* lpFont = m_grid_protocol.GetItemFont(i,lStartColum);
				LOGFONT ItemFont;
				memcpy(&ItemFont,lpFont,sizeof(LOGFONT));
				ItemFont.lfWeight = 700;	// ���óɺ���
				m_grid_protocol.SetItemFont(i,lStartColum,&ItemFont);
				m_grid_protocol.SetItemFgColour(i,lStartColum,RGB(0x1C,0x86,0xEE));//#1C86EE
				if(nSubIdx < nSubItemNum)
					m_grid_protocol.SetItemData(i,lStartColum,TreeItem.subNode[nSubIdx].lID);
				++ nSubIdx;
				// ������������
			}
		}
	}

	m_grid_protocol.Invalidate();
}

void CDlgCommProMain::ClearGridData(void)
{
	int nCol_Count = 2;
	int nRow_DispCount = 40;

	CRect rt;
	m_grid_protocol.DeleteAllItems();

	m_grid_protocol.SetEditable(false);
	m_grid_protocol.SetDefCellHeight(30);
	m_grid_protocol.SetColumnCount(nCol_Count);
	m_grid_protocol.SetRowCount(nRow_DispCount);
	m_grid_protocol.SetFixedRowCount();
	m_grid_protocol.GetClientRect(&rt);

	int nW = (rt.Width()-GRID_SPARE)/nCol_Count;
	for(int i =0;i<nCol_Count;i++)		
	{
		SetGirdItemText(m_grid_protocol,0,i,"",GVNI_READONLY,TITLE_HEAD_COLOR,COLOR_BK_JPG,DT_CENTER);	
		if(i == nCol_Count-1)
			m_grid_protocol.SetColumnWidth(i,rt.Width()-(nCol_Count-1)*nW);
		else
			m_grid_protocol.SetColumnWidth(i,nW);
	}
}

//void CDlgCommProMain::OnNMCustomdrawTreeProtocol(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//
//	LPNMTVCUSTOMDRAW pDraw=(LPNMTVCUSTOMDRAW)pNMHDR;
//	DWORD dwDrawStage=pDraw->nmcd.dwDrawStage;
//	UINT uItemState =pDraw->nmcd.uItemState;
//	//
//	*pResult=CDRF_NOTIFYITEMDRAW;
//	//|CDRF_NOTIFYPOSTPAINT|CDRF_NOTIFYSUBITEMDRAW|CDRF_NOTIFYPOSTERASE;
//	CDC* pdc=CDC::FromHandle(pDraw->nmcd.hdc);
//	CRect rc;
//	HTREEITEM hItem=(HTREEITEM) pDraw->nmcd.dwItemSpec;
//	m_tree_protocol.GetItemRect(hItem,&rc,TRUE);//FALSE);text only
//	CString txt=m_tree_protocol.GetItemText(hItem);
//	if((dwDrawStage & CDDS_ITEM) && (uItemState & CDIS_SELECTED))
//	{// 
//		//pdc->FillSolidRect(&rc,RGB(49,106,197));//clr);
//		//
//		pdc->SetTextColor(RGB(255,255,255));//white
//		pdc->SetBkColor(RGB(49,106,197));//clr);
//		//
//
//		CFont* pfnt=pdc->GetCurrentFont();
//		//
//		pdc->TextOut(rc.left+2,rc.top+2,txt);
//		pdc->SelectObject(pfnt);
//
//		*pResult |= CDRF_SKIPDEFAULT;
//		// afxDump << "1\n";
//	}
//	else // without these ,1st blue !
//	{
//		//pdc->FillSolidRect(&rc, GetSysColor(COLOR_WINDOW));
//		pdc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
//		pdc->TextOut(rc.left+2, rc.top+2, txt);
//		// afxDump << "2\n";
//	}
//}

void CDlgCommProMain::OnLoadicdfile()
{
	// TODO: �ڴ���������������
	if(m_sSysName.IsEmpty())
	{
		AfxMessageBox("��ѡ������ϵͳ��");
		return;
	}
	int nSubSystemNum = m_vtSubSystem.size();
	bool bFindCurSystem = false;
	for(int i = 0; i < nSubSystemNum; ++ i)
	{
		if(m_sCurDlgName == m_vtSubSystem[i])
		{
			bFindCurSystem = true;
			break;
		}
	}
	if(!bFindCurSystem || m_lCurProType != -1)
	{
		AfxMessageBox("��ѡ�����ϵͳ��");
		return;
	}

	CDlgLoadIcdFile dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_sCurIcdFilePath = dlg.GetIdcFilePath();
		m_nCurComChannelNo = dlg.GetCurComChannelNo();
		vector<__SYS_Drive>& vtDriveInfor = dlg.getCurDriveInfor();
		m_nCurProType = dlg.GetCurProtocolType();
		int nCurDriveNo = dlg.GetCurDriveNo();
		if(m_nCurProType == 0)		// 1553BЭ��
		{
			m_Pro1553B.SetDriveInfor(vtDriveInfor,nCurDriveNo);
		}
		else if(m_nCurProType == 1)	// ����Э��
		{
			m_ProSerial.SetChannelNo(m_nCurComChannelNo);
			m_ProSerial.SetDriveInfor(vtDriveInfor,nCurDriveNo);
		}
		else if(m_nCurProType == 2)	// 429Э��
		{
			m_ProArinc429.SetDriveInfor(vtDriveInfor,nCurDriveNo);

		}
		else if(m_nCurProType == 3)	// CAN Э��
		{
			m_ProCan.SetDriveInfor(vtDriveInfor,nCurDriveNo);
		}
		else if(m_nCurProType == 4)	// ��ɢ��
		{
			m_ProDiscrete.SetDriveInfor(vtDriveInfor);
		}

		m_sCurIcdFilePath.Trim();
		if(m_sCurIcdFilePath.IsEmpty())
		{
			AfxMessageBox("��ѡ��ICD�ļ���");
			return;
		}
		// ���ý���������
		m_pdlgProgress = new CDlgProgress;
		m_pdlgProgress->SetProgressType(1);

		m_ProDiscrete.StopLoadIcd(false);
		m_Pro1553B.StopLoadIcd(FALSE);		// ������ICD
		m_ProCan.StopLoadIcd(false);
		m_ProSerial.StopLoadIcd(false);
		m_ProArinc429.StopLoadIcd(false);

		m_nTotalProNum = 100;;
		m_nCurPos = 0;
		// ��������ICD
		unsigned int dwThreadID = 0;
		_beginthreadex(NULL, 0, LoadDataFile, this, 0, &dwThreadID);

		if(m_pdlgProgress->DoModal() != IDOK)
		{
			m_Pro1553B.StopLoadIcd(TRUE);
			m_ProDiscrete.StopLoadIcd(true);
			m_ProCan.StopLoadIcd(true);
			m_ProSerial.StopLoadIcd(true);
			m_ProArinc429.StopLoadIcd(true);
		}

		int nExiteCode = m_pdlgProgress->GetExitCode();
		
		m_pdlgProgress->DestroyWindow();
		delete m_pdlgProgress;
		m_pdlgProgress = NULL;

		if(nExiteCode != 2)
		{
			// �������ڵ�
			HTREEITEM rootItem = m_tree_protocol.GetRootItem();
			HTREEITEM hChildItem = m_tree_protocol.GetChildItem(rootItem);
			while(hChildItem != NULL)
			{
				CString sProName = m_tree_protocol.GetItemText(hChildItem);
				if(sProName == m_sCurDlgName)
					break;

				hChildItem = m_tree_protocol.GetNextSiblingItem(hChildItem);
			}
			// �������ڵ�����
			bool bFindTreeNode = false;
			int n = 0;
			for(int end = m_ParentTree.subNode.size(); n < end; ++ n)
			{
				TREEDATA& TdItem = m_ParentTree.subNode[n];
				if(TdItem.sName == m_sCurDlgName)
				{
					bFindTreeNode = true;
					break;
				}
			}
			if(bFindTreeNode)
			{
				vector<TREEDATA>& vtProTree = m_ParentTree.subNode[n].subNode;
				int nProNum = vtProTree.size();
				//if(nProNum > 0)
				//{
				//	m_tree_protocol.Invalidate(false);
				//	int nNodeNo = 0;
				//	InsertTreeNode(vtProTree[nProNum-1],hChildItem,nNodeNo);
				//	m_tree_protocol.Invalidate();
				//}
				// ��ϵͳ�µ���ȫ���ع�
				m_tree_protocol.Invalidate(false);
				for(int i = 0; i < nProNum; ++ i)
				{
					int nNodeNo = 0;
					InsertTreeNode(vtProTree[i],hChildItem,nNodeNo);
				}
				m_tree_protocol.Invalidate();
			}

			// ˢ�µ�ǰ�ڵ����ݱ�
			HTREEITEM hItem = m_tree_protocol.GetSelectedItem();
			if(hItem != 0)
			{
				TREEDATA& TreeItem = *(TREEDATA*)m_tree_protocol.GetItemData(hItem);
				DisplayGridData(TreeItem);
			}
			else
			{
				ClearGridData();
			}
		}
	}
}

void CDlgCommProMain::LoadIcdFile()
{
	// ����1553BЭ�鵽��ǰ����ϵͳ
	bool bFindSystem = false;
	int n = 0;
	for(int end = m_ParentTree.subNode.size(); n < end; ++ n)
	{
		TREEDATA& TdItem = m_ParentTree.subNode[n];
		if(TdItem.sName == m_sCurDlgName)
		{
			bFindSystem = true;
			break;
		}
	}
	if(!bFindSystem) return;

	if(m_nCurProType == 0)
	{
		// ɾ��ϵͳ��ͬ���ڵ�
		vector<vector<string>>::iterator it = m_ParentTree.subNode[n].vtNodeData.begin();
		while(it != m_ParentTree.subNode[n].vtNodeData.end())
		{
			if((*it)[1] == "1553B_ICD")
				it = m_ParentTree.subNode[n].vtNodeData.erase(it);
			else
				++ it;
		}

		vector<string> vtNodeRowData;

		vtNodeRowData.push_back(m_sCurDlgName.GetBuffer());
		vtNodeRowData.push_back("1553B_ICD");
		m_ParentTree.subNode[n].vtNodeData.push_back(vtNodeRowData);

		m_Pro1553B.LoadIcdFile(m_sCurIcdFilePath);

		int nTreeId = m_nTreeExcelId;			// ��¼��ǰ�����ڵ�ID
		TREEDATA& ProTree = m_Pro1553B.BuildProtocolTree(m_ParentTree.subNode[n],m_nCurProType,
			m_nCurDlgId,m_nTreeExcelId);
		m_Pro1553B.m_config_1553.close_excel();

		// δ�ҵ�Э����
		if(ProTree.nData == -1)
		{
			m_nTreeExcelId = nTreeId;
			return;
		}
	}
	// ���봮��ͨ��Э�鵽��ǰ����ϵͳ
	else if(m_nCurProType == 1)
	{
		// ɾ��ϵͳ��ͬ���ڵ�
		vector<vector<string>>::iterator it = m_ParentTree.subNode[n].vtNodeData.begin();
		while(it != m_ParentTree.subNode[n].vtNodeData.end())
		{
			if((*it)[1] == "Serial_ICD")
				it = m_ParentTree.subNode[n].vtNodeData.erase(it);
			else
				++ it;
		}

		vector<string> vtNodeRowData;

		vtNodeRowData.push_back(m_sCurDlgName.GetBuffer());
		vtNodeRowData.push_back("Serial_ICD");
		m_ParentTree.subNode[n].vtNodeData.push_back(vtNodeRowData);

		m_ProSerial.LoadIcdFile(m_sCurIcdFilePath);

		int nTreeId = m_nTreeExcelId;			// ��¼��ǰ�����ڵ�ID
		TREEDATA& ProTree = m_ProSerial.BuildProtocolTree(m_ParentTree.subNode[n],m_nCurProType,
			m_nCurDlgId,m_nTreeExcelId);
		m_ProSerial.m_excelFile.close_excel();

		// δ�ҵ�Э����
		if(ProTree.nData == -1)
		{
			m_nTreeExcelId = nTreeId;
			return;
		}
	}
	// ����429ͨ��Э�鵽��ǰ����ϵͳ
	else if(m_nCurProType == 2)
	{
		// ɾ��ϵͳ��ͬ���ڵ�
		vector<vector<string>>::iterator it = m_ParentTree.subNode[n].vtNodeData.begin();
		while(it != m_ParentTree.subNode[n].vtNodeData.end())
		{
			if((*it)[1] == "Arinc429_ICD")
				it = m_ParentTree.subNode[n].vtNodeData.erase(it);
			else
				++ it;
		}

		vector<string> vtNodeRowData;

		vtNodeRowData.push_back(m_sCurDlgName.GetBuffer());
		vtNodeRowData.push_back("Arinc429_ICD");
		m_ParentTree.subNode[n].vtNodeData.push_back(vtNodeRowData);

		m_ProArinc429.LoadIcdFile(m_sCurIcdFilePath);

		int nTreeId = m_nTreeExcelId;			// ��¼��ǰ�����ڵ�ID
		TREEDATA& ProTree = m_ProArinc429.BuildProtocolTree(m_ParentTree.subNode[n],m_nCurProType,
			m_nCurDlgId,m_nTreeExcelId);
		m_ProArinc429.m_excelFile.close_excel();

		// δ�ҵ�Э����
		if(ProTree.nData == -1)
		{
			m_nTreeExcelId = nTreeId;
			return;
		}
	}
	// ����CANͨ��Э�鵽��ǰ����ϵͳ
	else if(m_nCurProType == 3)
	{
		// ɾ��ϵͳ��ͬ���ڵ�
		vector<vector<string>>::iterator it = m_ParentTree.subNode[n].vtNodeData.begin();
		while(it != m_ParentTree.subNode[n].vtNodeData.end())
		{
			if((*it)[1] == "CAN_ICD")
				it = m_ParentTree.subNode[n].vtNodeData.erase(it);
			else
				++ it;
		}

		vector<string> vtNodeRowData;

		vtNodeRowData.push_back(m_sCurDlgName.GetBuffer());
		vtNodeRowData.push_back("CAN_ICD");
		m_ParentTree.subNode[n].vtNodeData.push_back(vtNodeRowData);

		m_ProCan.LoadIcdFile(m_sCurIcdFilePath);

		int nTreeId = m_nTreeExcelId;			// ��¼��ǰ�����ڵ�ID
		TREEDATA& ProTree = m_ProCan.BuildProtocolTree(m_ParentTree.subNode[n],m_nCurProType,
			m_nCurDlgId,m_nTreeExcelId);
		m_ProCan.m_excelFile.close_excel();

		// δ�ҵ�Э����
		if(ProTree.nData == -1)
		{
			m_nTreeExcelId = nTreeId;
			return;
		}
	}
	// ������ɢ��ͨ��Э�鵽��ǰ����ϵͳ
	else if(m_nCurProType == 4)
	{
		// ɾ��ϵͳ��ͬ���ڵ�
		vector<vector<string>>::iterator it = m_ParentTree.subNode[n].vtNodeData.begin();
		while(it != m_ParentTree.subNode[n].vtNodeData.end())
		{
			if((*it)[1] == "mDAQ_ICD")
				it = m_ParentTree.subNode[n].vtNodeData.erase(it);
			else
				++ it;
		}

		vector<string> vtNodeRowData;

		vtNodeRowData.push_back(m_sCurDlgName.GetBuffer());
		vtNodeRowData.push_back("mDAQ_ICD");
		m_ParentTree.subNode[n].vtNodeData.push_back(vtNodeRowData);

		m_ProDiscrete.LoadIcdFile(m_sCurIcdFilePath);

		int nTreeId = m_nTreeExcelId;			// ��¼��ǰ�����ڵ�ID
		TREEDATA& ProTree = m_ProDiscrete.BuildProtocolTree(m_ParentTree.subNode[n],m_nCurProType,
			m_nCurDlgId,m_nTreeExcelId);
		m_ProDiscrete.m_excelFile.close_excel();

		// δ�ҵ�Э����
		if(ProTree.nData == -1)
		{
			m_nTreeExcelId = nTreeId;
			return;
		}
	}

	// �˳�����������
	if(m_pdlgProgress && m_pdlgProgress->GetSafeHwnd())
	{
		//pUser->m_pdlgProgress->ExitWindow();
		HWND hProgress = m_pdlgProgress->GetSafeHwnd();
		::PostMessage(hProgress,EXIT_PROGRESS,0,0);
	}
}

void CDlgCommProMain::InsertTreeNode(TREEDATA& ProTree,HTREEITEM hParentItem,int nNodeNo)
{
	// �����ýڵ��������ӽڵ㣬��Ҫ�����ͬ���ڵ�ɾ��
	HTREEITEM hChildItem = m_tree_protocol.GetChildItem(hParentItem);

	// ITEM�������ظ� nNodeNo=1:��Э�飬=2:Item��
	while(hChildItem != NULL && nNodeNo != 2)
	{
		CString sProName = m_tree_protocol.GetItemText(hChildItem);
		if(sProName == ProTree.sName)
		{
			m_tree_protocol.DeleteItem(hChildItem);
			break;
		}

		hChildItem = m_tree_protocol.GetNextSiblingItem(hChildItem);
	}

	HTREEITEM hItem = m_tree_protocol.InsertItem(ProTree.sName,hParentItem);
	m_tree_protocol.SetItemData(hItem,(DWORD)&ProTree);

	int nCurNodeNo = nNodeNo + 1;
	int nSubItemNum = ProTree.subNode.size();
	for(int i = 0; i < nSubItemNum; ++ i)
	{
		InsertTreeNode(ProTree.subNode[i],hItem,nCurNodeNo);
	}
	
	if(nNodeNo == 2)
		m_tree_protocol.Expand(hParentItem,TVE_COLLAPSE);
	else
		m_tree_protocol.Expand(hParentItem,TVE_EXPAND);

	if(nNodeNo == 1)
	{
		//m_tree_protocol.SetItemState( hParentItem, true ? TVIS_BOLD: 0, TVIS_BOLD );

		m_tree_protocol.SetItemBold(hParentItem,TRUE);
		//m_tree_protocol.SetItemColor(hParentItem,RGB(0,255,0));
	}
}


void CDlgCommProMain::OnSavetodb()
{
	// TODO: �ڴ���������������
	int nTotalPro = 0;
	int nSaveProNum = 0;
	int nTestSysNUm = m_ParentTree.subNode.size();
	if(nTestSysNUm <= 0)
	{
		AfxMessageBox("�뵼�����ϵͳ��");
		return;
	}

	// �������ݿ�Э��鿴����
	if(m_pdlgDBProData)
		m_pdlgDBProData->ShowWindow(SW_HIDE);

	// �����Ҫд�����ݿ��Э���������ûص�����
	m_vtSaveNode.clear();
	for(int i = 0; i < nTestSysNUm; ++ i)
	{
		TREEDATA& TreeItem = m_ParentTree.subNode[i];
		int nProtocolNum = TreeItem.subNode.size();
		if(nProtocolNum <= 0)
			continue;

		for(int j = 0; j < nProtocolNum; ++ j)
		{
			TREEDATA& TreeSubItem = TreeItem.subNode[j];
			nTotalPro += TreeSubItem.subNode.size();
		}

		CString sTxt;
		sTxt.Format("�Ƿ�Э�鱣�浽��%s����",TreeItem.sName);
		if(AfxMessageBox(sTxt,MB_YESNO) == IDNO)
		{
			// ���ý�����������λ��
			continue;
		}

		m_vtSaveNode.push_back(TreeItem.sName);

		for(int j = 0; j < nProtocolNum; ++ j)
		{
			TREEDATA& TreeSubItem = TreeItem.subNode[j];
			nSaveProNum += TreeSubItem.subNode.size();
		}
	}

	if(nTotalPro == 0)
	{
		AfxMessageBox("δ����Э�飡",MB_ICONWARNING);
	}
	else if(nSaveProNum > 0)
	{
		// ���ý���������
		//m_myProCtrl.SetRange(0,nTotalPro);
		m_pdlgProgress = new CDlgProgress;
		m_pdlgProgress->InitialProgress(0,0,nSaveProNum);

		m_nTotalProNum = nSaveProNum;
		m_nCurPos = 0;

		m_Pro1553B.StopSaveToDB(FALSE);
		m_ProDiscrete.StopSaveToDB(FALSE);
		m_ProCan.StopSaveToDB(FALSE);
		m_ProSerial.StopSaveToDB(FALSE);
		m_ProArinc429.StopSaveToDB(FALSE);
		// ���ý������ص�
		m_Pro1553B.SetProgressFunc(ProgressPos,this);
		m_ProDiscrete.SetProgressFunc(ProgressPos,this);
		m_ProCan.SetProgressFunc(ProgressPos,this);
		m_ProSerial.SetProgressFunc(ProgressPos,this);
		m_ProArinc429.SetProgressFunc(ProgressPos,this);

		// �������ݵ��뵽���ݿ��߳�
		DWORD dwThreadID  =0;
		m_hSaveToDB_Thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SaveDataToDB,(LPVOID)this,0,&dwThreadID);
		//_beginthreadex(NULL, 0, SaveDataToDB, this, 0, &dwThreadID);
		//SaveProtocolToDB();
		if(m_pdlgProgress->DoModal() != IDOK)
		{
			m_Pro1553B.StopSaveToDB(TRUE);
			m_ProDiscrete.StopSaveToDB(TRUE);
			m_ProCan.StopSaveToDB(TRUE);
			m_ProSerial.StopSaveToDB(TRUE);
			m_ProArinc429.StopSaveToDB(TRUE);
		}
		delete m_pdlgProgress;
		m_pdlgProgress = NULL;
	
		// ��ֹ��ǰ����
		//ReleaseThread(m_hSaveToDB_Thread);
		// ���ݿⱻ�Ķ�
		m_bLoadDBICD = FALSE;
	}
}

unsigned WINAPI SaveDataToDB(LPVOID lpParam)
{
	CDlgCommProMain* pUser=(CDlgCommProMain*)lpParam;
	if(pUser)
	{
		pUser->SaveProtocolToDB();
	}
	return 0;
}

unsigned WINAPI LoadDataFile(LPVOID lpParam)
{
	CDlgCommProMain* pUser = (CDlgCommProMain*)lpParam;
	if(pUser)
	{
		::CoInitialize(NULL);
		pUser->LoadIcdFile();
		::CoUninitialize();
	}

	return 0;
}

void ProgressPos(LPVOID lpParam)
{
	CDlgCommProMain* pUser=(CDlgCommProMain*)lpParam;
	if(pUser)
	{
		pUser->SetProgressPos();
	}
}

void CDlgCommProMain::ReleaseThread(HANDLE& hThread)
{
	if(hThread)
	{
		if(WaitForSingleObject(hThread,20000L) != WAIT_OBJECT_0)
		{
			DWORD dwExitCode = 0;
			TerminateThread(hThread,dwExitCode);
		}
		CloseHandle(hThread);
		hThread = NULL;
	}
}

void CDlgCommProMain::SetProgressPos(void)
{
	++ m_nCurPos;
	//m_myProCtrl.SetPos(m_nCurPos);
	//if(m_nCurPos >= m_nTotalProNum)
	//	m_myProCtrl.ShowWindow(SW_HIDE);

	if(m_pdlgProgress && m_pdlgProgress->GetSafeHwnd())
	{
		//m_pdlgProgress->SetProgressPos(m_nCurPos);
		HWND hProgress = m_pdlgProgress->GetSafeHwnd();
		::PostMessage(hProgress,PROGRESS_POS,(WPARAM)m_nCurPos,0);
	}
}

void CDlgCommProMain::SaveProtocolToDB()
{
	int nTestSysNUm = m_ParentTree.subNode.size();

	for(int i = 0; i < nTestSysNUm; ++ i)
	{
		TREEDATA& TreeItem = m_ParentTree.subNode[i];

		int nProtocolNum = TreeItem.subNode.size();
		if(nProtocolNum <= 0) continue;

		// ��⵱ǰϵͳ�Ƿ�������Э��
		BOOL bSaveToDB = FALSE;
		for(int j = 0,end = m_vtSaveNode.size(); j < end; ++ j)
		{
			if(m_vtSaveNode[j] == TreeItem.sName)
			{
				bSaveToDB = TRUE;
				break;
			}
		}
		if(!bSaveToDB)
			continue;

		for(int j = 0; j < nProtocolNum; ++ j)
		{
			TREEDATA& TreeSubItem = TreeItem.subNode[j];
			// ����1553BЭ����
			if(TreeSubItem.sName == "1553B_ICD")
			{
				m_Pro1553B.SaveDataToDB(TreeSubItem);
			}
			// ����CANЭ����
			else if(TreeSubItem.sName == "CAN_ICD")
			{
				m_ProCan.SaveDataToDB(TreeSubItem);
			}
			// ���洮��Э����
			else if(TreeSubItem.sName == "Serial_ICD")
			{
				m_ProSerial.SaveDataToDB(TreeSubItem);
			}
			// ����429Э����
			else if(TreeSubItem.sName == "Arinc429_ICD")
			{
				m_ProArinc429.SaveDataToDB(TreeSubItem);
			}
			// ������ɢ��Э����
			else if(TreeSubItem.sName == "mDAQ_ICD")
			{
				m_ProDiscrete.SaveDataToDB(TreeSubItem);
			}
		}
	}

	// �˳�����������
	if(m_pdlgProgress && m_pdlgProgress->GetSafeHwnd())
	{
		//pUser->m_pdlgProgress->ExitWindow();
		HWND hProgress = m_pdlgProgress->GetSafeHwnd();
		::PostMessage(hProgress,EXIT_PROGRESS,0,0);
	}
}

void CDlgCommProMain::OnSelectsys()
{
	// TODO: �ڴ���������������
	CDlgSysStart dlgSys;
	if(dlgSys.DoModal() == IDOK)
	{
		if(!m_sSysName.IsEmpty())
		{
			m_Statusbar.SetPaneText(1,_T("��ǰϵͳ��")+m_sSysName);

			InitTree();

			// ��ʾѡ��Ľڵ�����
			HTREEITEM hRootItem = m_tree_protocol.GetRootItem();
			//HTREEITEM hChildItem = m_tree_protocol.GetChildItem(hRootItem);
			if(hRootItem)
			{
				m_tree_protocol.SelectItem(hRootItem);
				m_tree_protocol.Select(hRootItem, TVGN_DROPHILITE);
			}
			else
			{
				ClearGridData();
			}

			// ����ID
			XProtocol_Funtion::ResetDataId();
			XProtocol_Funtion::ResetProtocolId();
		}
	}
}

void CDlgCommProMain::OnHelp()
{
	// TODO: �ڴ���������������
	//CAboutDlg dlgAbout;
	//dlgAbout.DoModal();
	// TODO: Add your message handler code here and/or call default
}

void CDlgCommProMain::InitTree()
{
	m_nTreeId = 10;					// ��ID��10��ʼ���
	m_tree_protocol.DeleteAllItems();
	m_vtSubSystem.clear();

	// ���ڵ����ݳ�ʼ��
	m_ParentTree.lID = m_nTreeId ++;
	m_ParentTree.nData = -1;
	m_ParentTree.nProType = -2;
	m_ParentTree.sProNo = "";
	m_ParentTree.sDataType = "ROOT";
	m_ParentTree.sName = m_sSysName;
	m_ParentTree.pParent = NULL;
	m_ParentTree.subNode.clear();

	TREEDATA TreeDataItem;

	HTREEITEM rootItem = m_tree_protocol.GetRootItem();

	HTREEITEM hItem = m_tree_protocol.InsertItem(m_ParentTree.sName,rootItem);
	m_tree_protocol.SetItemData(hItem,(DWORD)&m_ParentTree);

	m_tree_protocol.SetItemBold(hItem,TRUE);

	//vector<__DLGInfo>vec;
	//CManageDb_Sqllite::GetInstance().Query_DlgInfo(vec,m_nCurSysId);

	vector<__TestSysInfo> vec;
	CManageDb_Sqllite::GetInstance().Query_TestSysInfo(vec);

	HTREEITEM hChildItem;
	vector<vector<string>> vtNodeData;
	vector<vector<string>> vtSubNodeData;
	vector<string> vtNodeRowData;

	vtNodeRowData.push_back("���ڵ�");
	vtNodeRowData.push_back("�ӽڵ�");
	vtNodeData.push_back(vtNodeRowData);
	vtSubNodeData.push_back(vtNodeRowData);

	// �������ڵ�����
	for(int i =0,end = vec.size();i<end;i++)
	{
		vtNodeRowData.clear();

		TreeDataItem.lID = m_nTreeId ++;
		TreeDataItem.nDlgId = vec[i].nId;
		TreeDataItem.nProType = -1;
		TreeDataItem.sName = vec[i].sName.c_str();
		TreeDataItem.sProNo = "";
		TreeDataItem.sDataType = "SYSTEM";
		TreeDataItem.pParent = &m_ParentTree;
		TreeDataItem.vtNodeData = vtSubNodeData;
		TreeDataItem.subNode.clear();
		m_ParentTree.subNode.push_back(TreeDataItem);

		vtNodeRowData.push_back(m_sSysName.GetBuffer());
		vtNodeRowData.push_back(vec[i].sName);

		vtNodeData.push_back(vtNodeRowData);
	}

	m_ParentTree.vtNodeData = vtNodeData;
	// �������Ϳؼ�
	for(int i = 0,end = m_ParentTree.subNode.size(); i < end; ++ i)
	{
		hChildItem = m_tree_protocol.InsertItem(m_ParentTree.subNode[i].sName,hItem);
		m_tree_protocol.SetItemData(hChildItem,(DWORD)&m_ParentTree.subNode[i]);
		//m_tree_protocol.SetItemImage(hChildItem,0,0);
		// ͼ��ģʽ����
		m_tree_protocol.SetItemState(hChildItem, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK); 
		m_vtSubSystem.push_back(m_ParentTree.subNode[i].sName);
	}

	m_tree_protocol.Expand(hItem,TVE_EXPAND);

	// ϵͳ�ڵ����ݴ���DBICD���ڵ�
	m_nTreeDBId = m_nTreeId;
	m_nTreeExcelId = m_nTreeId;
	m_DBIcdTree = m_ParentTree;
	m_BackupTree = m_ParentTree;

	// ���ز鿴���ݿ�Э��Ĵ���
	m_bLoadDBICD = FALSE;
	if(m_pdlgDBProData)
	{
		m_pdlgDBProData->ShowWindow(SW_HIDE);
	}
}

BOOL CDlgCommProMain::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	CString str;
	UINT nID =pNMHDR->idFrom; //��ȡ��������ťID
	if(nID)
	{
		nID = m_wndToolBar.CommandToIndex(nID); //����ID��ȡ��ť����
		if (nID != -1)
		{
			switch(nID)
			{
			case 0:
				pTTT->lpszText  = "ѡ��ϵͳ";
				break;
			case 1:
				pTTT->lpszText  = "����ICD�ļ�";
				break;
			case 2:
				pTTT->lpszText  ="���浽���ݿ�";
				break;
			case 3:
				pTTT->lpszText  ="�鿴���ݿ�Э��";
				break;
			default:
				pTTT->lpszText = " ";
				break;

			}

			//��ȡ�������ı�			
			pTTT->hinst = AfxGetResourceHandle();
			return(TRUE);
		}
	}
	return(FALSE);
}

void CDlgCommProMain::OnProtocolDb()
{
	// TODO: �ڴ���������������
	// ����Ѿ��������ݿ�ICD��ֻ��ʾ
	if(m_bLoadDBICD)
	{
		// ��ʾ���ݴ���
		if(m_pdlgDBProData)
		{
			m_pdlgDBProData->ShowWindow(SW_SHOW);
			return;
		}
	}

	// δ�������ݿ�����б䶯�����µ���
	// �˴�����һ��Bug����ɾ��ȫ�����ݿ��¼ʱ�ٵ����¼�����
	m_nTreeDBId = m_nTreeId;
	m_DBIcdTree = m_BackupTree;
	int nSystemNum = m_DBIcdTree.subNode.size();
	if(nSystemNum <= 0)
	{
		AfxMessageBox("�뵼�����ϵͳ!");
		return;
	}

	// ��ñ�ϵͳ�����������忨����Ϣ
	m_vtDriveInfor.clear();
	//CString strWhere = "and nDriveId in (select nid from t_drive)";
	//CManageDb_Sqllite::GetInstance().Query_SysDrive(m_vtDriveInfor,m_nCurSysId,strWhere.GetBuffer());
	CManageDb_Sqllite::GetInstance().Query_SysDrive(m_vtDriveInfor,-1);
	int nDriveNum = m_vtDriveInfor.size();
	if(nDriveNum <= 0)
	{
		AfxMessageBox("û���ҵ������忨����Ϣ��");
		return;
	}

	// ��������������
	m_pdlgProgress = new CDlgProgress;
	m_pdlgProgress->SetProgressType(1);

	m_Pro1553B.StopLoadDBIcd(FALSE);		// ������ICD
	m_ProDiscrete.StopLoadDBIcd(false);
	m_ProCan.StopLoadDBIcd(false);
	m_ProSerial.StopLoadDBIcd(false);
	m_ProArinc429.StopLoadDBIcd(false);

	// �����������ݿ�ICD
	unsigned int dwThreadID = 0;
	_beginthreadex(NULL, 0, LoadDBData, this, 0, &dwThreadID);

	if(m_pdlgProgress->DoModal() != IDOK)
	{
		m_Pro1553B.StopLoadDBIcd(TRUE);
		m_ProDiscrete.StopLoadDBIcd(true);
		m_ProCan.StopLoadDBIcd(true);
		m_ProSerial.StopLoadDBIcd(true);
		m_ProArinc429.StopLoadDBIcd(true);
	}

	int nExitCode = m_pdlgProgress->GetExitCode();

	m_pdlgProgress->DestroyWindow();
	delete m_pdlgProgress;
	m_pdlgProgress = NULL;

	// ��ʾ���ݴ���
	if(m_pdlgDBProData && nExitCode != 2)
	{
		m_pdlgDBProData->SetPro1553B(&m_Pro1553B);
		m_pdlgDBProData->SetProDiscrete(&m_ProDiscrete);
		m_pdlgDBProData->SetProCAN(&m_ProCan);
		m_pdlgDBProData->SetProSerial(&m_ProSerial);
		m_pdlgDBProData->SetProArinc429(&m_ProArinc429);

		m_pdlgDBProData->BuilProTree(m_DBIcdTree);
		m_pdlgDBProData->ShowWindow(SW_SHOW);
		m_bLoadDBICD = TRUE;
	}
}

unsigned WINAPI LoadDBData(LPVOID lpParam)
{
	CDlgCommProMain* pUser = (CDlgCommProMain*)lpParam;
	if(pUser)
	{
		::CoInitialize(NULL);
		pUser->LoadIcdFromDB();
		::CoUninitialize();
	}

	return 0;
}

void CDlgCommProMain::LoadIcdFromDB()
{
	int nDriveNum = m_vtDriveInfor.size();
	int nSysNum = m_DBIcdTree.subNode.size();
	BOOL bResult = TRUE;
	int nStatus = 0;

	for(int i = 0; i < nSysNum; ++ i)
	{
		TREEDATA& TSysItem = m_DBIcdTree.subNode[i];

		for(int j = 0; j < nDriveNum; ++ j)
		{
			__SYS_Drive& DInforItem = m_vtDriveInfor[j];
			// ����1553B����Э��
			if(DInforItem.sMark == "1553B")
			{
				int nProType = 0;
				int nTreeId = m_nTreeDBId;
				bResult = m_Pro1553B.BuildDBTree(TSysItem,DInforItem,nProType,m_nTreeDBId,nStatus);
				if(!bResult)
					m_nTreeDBId = nTreeId;
				// ��ֹ�����ݿ⵼��
				if(nStatus == 1)
					break;
			}
			// ����CAN����Э��
			else if(DInforItem.sMark == "CAN")
			{
				int nProType = 3;
				int nTreeId = m_nTreeDBId;
				bResult = m_ProCan.BuildDBTree(TSysItem,DInforItem,nProType,m_nTreeDBId,nStatus);
				if(!bResult)
					m_nTreeDBId = nTreeId;
				// ��ֹ�����ݿ⵼��
				if(nStatus == 1)
					break;
			}
			// ����429����Э��
			else if(DInforItem.sMark == "429")
			{
				int nProType = 2;
				int nTreeId = m_nTreeDBId;
				bResult = m_ProArinc429.BuildDBTree(TSysItem,DInforItem,nProType,m_nTreeDBId,nStatus);
				if(!bResult)
					m_nTreeDBId = nTreeId;
				// ��ֹ�����ݿ⵼��
				if(nStatus == 1)
					break;
			}
			// ������������Э��
			else if(DInforItem.sMark == "Serial")
			{
				int nProType = 1;
				int nTreeId = m_nTreeDBId;
				bResult = m_ProSerial.BuildDBTree(TSysItem,DInforItem,nProType,m_nTreeDBId,nStatus);
				if(!bResult)
					m_nTreeDBId = nTreeId;
				// ��ֹ�����ݿ⵼��
				if(nStatus == 1)
					break;
			}
			// ������ɢ������Э��
			else
			{
				int nProType = 4;
				int nTreeId = m_nTreeDBId;
				bResult = m_ProDiscrete.BuildDBTree(TSysItem,DInforItem,nProType,m_nTreeDBId,nStatus);
				if(!bResult)
					m_nTreeDBId = nTreeId;
				// ��ֹ�����ݿ⵼��
				if(nStatus == 1)
					break;
			}
		}
		// ��ֹ�����ݿ⵼��
		if(nStatus == 1)
			break;
	}

	// �˳�����������
	if(m_pdlgProgress && m_pdlgProgress->GetSafeHwnd())
	{
		//pUser->m_pdlgProgress->ExitWindow();
		HWND hProgress = m_pdlgProgress->GetSafeHwnd();
		::PostMessage(hProgress,EXIT_PROGRESS,0,0);
	}
}


void CDlgCommProMain::OnPaint()
{
	__super::OnPaint();
	return;

	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
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
		////
		//// ��������ӱ���
		////
		//CPaintDC dc(this);
		//CRect rc;
		//GetClientRect(&rc);
		//CDC dcMem;
		//dcMem.CreateCompatibleDC(&dc);
		//CBitmap bmpBackground;
		//bmpBackground.LoadBitmap(IDB_BMP_BK);

		//BITMAP bitmap;
		//bmpBackground.GetBitmap(&bitmap);
		//CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
		//dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	
		//CRect   rc;
		//GetClientRect( &rc );// ��ȡ�ͻ���
		//CPaintDC dc(this);
		//dc.FillSolidRect(&rc, RGB(0,160,0));   // ���ͻ�����ɫ
		//CDialog::OnPaint();

		CPaintDC dc(this); // device context for painting

		// TODO: Add your message handler code here
		CDC     memDC;
		CBitmap	bmp;
		bmp.LoadBitmap(IDB_BMP_BK1);
		CRect Rect;
		GetClientRect( Rect );
		memDC.CreateCompatibleDC (&dc);
		BITMAP bm;
		CBitmap* pOldBitmap =(CBitmap *) memDC.SelectObject(&bmp );
		bmp.GetObject(sizeof( BITMAP ), &bm );
		int x, y = 0;
		while( y < Rect.bottom ){
			x = 0;
			while( x < Rect.right ){
				dc.BitBlt( x, y, bm.bmWidth, bm.bmHeight, &memDC,
					0, 0, SRCCOPY );
				x += bm.bmWidth;}	
			y += bm.bmHeight;}
	}
}


void CDlgCommProMain::OnLoadctrlfile()
{
	// TODO: �ڴ���������������
	if(m_sSysName.IsEmpty())
	{
		AfxMessageBox("��ѡ������ϵͳ��");
		return;
	}
	int nSubSystemNum = m_vtSubSystem.size();
	bool bFindCurSystem = false;
	for(int i = 0; i < nSubSystemNum; ++ i)
	{
		if(m_sCurDlgName == m_vtSubSystem[i])
		{
			bFindCurSystem = true;
			break;
		}
	}
	if(!bFindCurSystem || m_lCurProType != -1)
	{
		AfxMessageBox("��ѡ�����ϵͳ��");
		return;
	}

	CDlgLoadIcdFile dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_sCurIcdFilePath = dlg.GetIdcFilePath();
		m_nCurComChannelNo = dlg.GetCurComChannelNo();
		vector<__SYS_Drive>& vtDriveInfor = dlg.getCurDriveInfor();
		m_nCurProType = dlg.GetCurProtocolType();
		int nCurDriveNo = dlg.GetCurDriveNo();

		m_ProCommCtrl.SetDriveInfor(vtDriveInfor,nCurDriveNo);

		m_sCurIcdFilePath.Trim();
		if(m_sCurIcdFilePath.IsEmpty())
		{
			AfxMessageBox("��ѡ��ICD�ļ���");
			return;
		}
		// ���ý���������
		m_pdlgProgress = new CDlgProgress;
		m_pdlgProgress->SetProgressType(1);

		m_ProCommCtrl.StopLoadIcd(false);			// ������CTRL�ļ�
		m_ProCommCtrl.StopSaveToDB(FALSE);			// ����洢CTRL�ļ�

		//m_nTotalProNum = 100;;
		//m_nCurPos = 0;
		// ��������ICD
		unsigned int dwThreadID = 0;
		_beginthreadex(NULL, 0, tLoadCtrlFile, this, 0, &dwThreadID);

		if(m_pdlgProgress->DoModal() != IDOK)
		{
			m_ProCommCtrl.StopLoadIcd(true);
			m_ProCommCtrl.StopSaveToDB(TRUE);
		}

		int nExiteCode = m_pdlgProgress->GetExitCode();

		m_pdlgProgress->DestroyWindow();
		delete m_pdlgProgress;
		m_pdlgProgress = NULL;
	}
}

unsigned WINAPI tLoadCtrlFile(LPVOID lpParam)
{
	CDlgCommProMain* pUser = (CDlgCommProMain*)lpParam;
	if(pUser)
	{
		::CoInitialize(NULL);
		pUser->LoadCtrlFile();
		::CoUninitialize();
	}

	return 0;
}

void CDlgCommProMain::LoadCtrlFile()
{
	// ����Э��ؼ�����ǰ����ϵͳ
	bool bFindSystem = false;
	int n = 0;
	for(int end = m_ParentTree.subNode.size(); n < end; ++ n)
	{
		TREEDATA& TdItem = m_ParentTree.subNode[n];
		if(TdItem.sName == m_sCurDlgName)
		{
			bFindSystem = true;
			break;
		}
	}
	if(!bFindSystem) return;

	m_ProCommCtrl.LoadIcdFile(m_sCurIcdFilePath);

	TREEDATA ProCommTree;
	int nTreeId = m_nTreeExcelId;			// ��¼��ǰ�����ڵ�ID
	TREEDATA& ProTree = m_ProCommCtrl.BuildProtocolTree(ProCommTree,m_nCurProType,
		m_nCurDlgId,nTreeId);
	m_ProCommCtrl.m_excelFile.close_excel();

	// δ�ҵ�Э����
	if(ProTree.nData == -1)
	{
		nTreeId = m_nTreeExcelId;
		return;
	}

	// �洢Э�鵽���ݿ�
	int nProtocolNum = ProCommTree.subNode.size();

	for(int j = 0; j < nProtocolNum; ++ j)
	{
		TREEDATA& TreeSubItem = ProCommTree.subNode[j];

		m_ProCommCtrl.SaveDataToDB(TreeSubItem);
	}

	// �˳�����������
	if(m_pdlgProgress && m_pdlgProgress->GetSafeHwnd())
	{
		//pUser->m_pdlgProgress->ExitWindow();
		HWND hProgress = m_pdlgProgress->GetSafeHwnd();
		::PostMessage(hProgress,EXIT_PROGRESS,0,0);
	}
}


void CDlgCommProMain::OnAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
