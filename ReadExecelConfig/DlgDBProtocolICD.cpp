// DlgDBProtocolICD.cpp : 实现文件
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "DlgDBProtocolICD.h"
#include "afxdialogex.h"


// CDlgDBProtocolICD 对话框

IMPLEMENT_DYNAMIC(CDlgDBProtocolICD, CDialogEx)

CDlgDBProtocolICD::CDlgDBProtocolICD(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDBProtocolICD::IDD, pParent),m_pPro1553B(NULL),m_pProDiscrete(NULL),m_pProCan(NULL)
	,m_pProSerial(NULL),m_pProArinc429(NULL)
{
	m_pdlgProgress = NULL;
	m_nExitCode = 0;
	m_pTreeData = NULL;
}

CDlgDBProtocolICD::~CDlgDBProtocolICD()
{
}

void CDlgDBProtocolICD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PROTOCOL, m_tree_protocol);
	DDX_Control(pDX, IDC_GRID_PRODATA, m_grid_protocol);
}


BEGIN_MESSAGE_MAP(CDlgDBProtocolICD, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PROTOCOL, &CDlgDBProtocolICD::OnTvnSelchangedTreeProtocol)
	ON_NOTIFY(NM_DBLCLK, IDC_GRID_PRODATA, OnGridDBClick)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_PROTOCOL, OnRBClick)
	ON_COMMAND(ID_DELETE_DBICD, &CDlgDBProtocolICD::OnDeleteDbicd)
END_MESSAGE_MAP()


// CDlgDBProtocolICD 消息处理程序


BOOL CDlgDBProtocolICD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_ImageList.Create(16, 16, ILC_COLOR32 ,10,CLR_NONE);
	m_ImageList.Create(16, 16,  ILC_COLOR32| ILC_MASK, 4, 2);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_MTS));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_MTS));

	m_tree_protocol.SetImageList(&m_ImageList,TVSIL_STATE);//TVSIL_NORMAL

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDBProtocolICD::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CDlgDBProtocolICD::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);

	//CDialogEx::OnClose();
}

void CDlgDBProtocolICD::BuilProTree(TREEDATA& PTData, BOOL bSelectRoot)
{
	m_tree_protocol.Invalidate(false);
	m_tree_protocol.DeleteAllItems();

	// 如果树为空的话，不构建树
	if(PTData.sName == "" && PTData.nProType == -2)
	{
		m_tree_protocol.Invalidate();
		return;
	}

	HTREEITEM rootItem = m_tree_protocol.GetRootItem();
	int nNodeNo = 0;
	InsertTreeNode(PTData,rootItem,nNodeNo);

	HTREEITEM hChildItem = m_tree_protocol.GetChildItem(rootItem);
	if(hChildItem && bSelectRoot)
	{
		//TREEDATA& TreeNodeItem = *(TREEDATA*)m_tree_protocol.GetItemData(hChildItem);

		//DisplayGridData(TreeNodeItem);

		m_tree_protocol.SelectItem(hChildItem);
		m_tree_protocol.Select(hChildItem, TVGN_DROPHILITE);
	}

	m_tree_protocol.Invalidate();
}

void CDlgDBProtocolICD::InsertTreeNode(TREEDATA& ProTree,HTREEITEM hParentItem,int nNodeNo)
{
	// 遍历该节点下所有子节点，将要插入的同名节点删除
	HTREEITEM hChildItem = m_tree_protocol.GetChildItem(hParentItem);

	// ITEM项允许重复 nNodeNo=1:块协议，=2:Item项
	while(hChildItem != NULL && nNodeNo != 4)
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

	// 协议节点设置成粗体
	if((nNodeNo == 2 || nNodeNo == 0))
	{
		m_tree_protocol.SetItemState(hItem, TRUE ? TVIS_BOLD: 0, TVIS_BOLD );
	}

	if(nNodeNo == 1)
	{
		m_tree_protocol.SetItemState(hItem, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK); 
	}

	int nCurNodeNo = nNodeNo + 1;
	int nSubItemNum = ProTree.subNode.size();
	for(int i = 0; i < nSubItemNum; ++ i)
	{
		InsertTreeNode(ProTree.subNode[i],hItem,nCurNodeNo);
	}

	// 折叠协议的ITEM项
	if(nNodeNo == 4)
		m_tree_protocol.Expand(hParentItem,TVE_COLLAPSE);
	else
		m_tree_protocol.Expand(hParentItem,TVE_EXPAND);
}

void CDlgDBProtocolICD::OnTvnSelchangedTreeProtocol(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	HTREEITEM hItem=m_tree_protocol.GetSelectedItem();  
	if(hItem==NULL)
		return ;

	TREEDATA& TreeNodeItem = *(TREEDATA*)m_tree_protocol.GetItemData(hItem);
	//m_nCurDlgId = TreeNodeItem.nData;
	//m_sCurDlgName =m_tree_protocol.GetItemText(hItem);

	DisplayGridData(TreeNodeItem);

	m_tree_protocol.Select(hItem, TVGN_DROPHILITE);

	*pResult = 0;
}

void CDlgDBProtocolICD::OnRBClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint point;
	GetCursorPos(&point);
	CPoint pointInTree = point;
	m_tree_protocol.ScreenToClient(&pointInTree);
	HTREEITEM hItem;
	UINT flag = TVHT_ONITEM;
	hItem = m_tree_protocol.HitTest(pointInTree, &flag);
	if(hItem != NULL)
	{
		m_tree_protocol.SelectItem(hItem);
		TREEDATA& TreeNodeItem = *(TREEDATA*)m_tree_protocol.GetItemData(hItem);
		CString sTxt = "删除";
		if(TreeNodeItem.sDataType == "ROOT")
			sTxt = "删除系统全部协议";
		else if(TreeNodeItem.sDataType == "SYSTEM")
			sTxt = "删除当前测试系统协议";
		else if(TreeNodeItem.sDataType == "PROOT")
			sTxt = "删除当前协议类";
		else if(TreeNodeItem.sDataType == "ITEM")
			sTxt = "删除"+TreeNodeItem.sName;
		// 协议ITEM项不能单独删除
		else
			return;

		CMenu menu;
		menu.LoadMenu(IDR_MENU_DBICD);
		CMenu *subMenu= menu.GetSubMenu(0);     //【功能】对应菜单栏第5列，索引值为5，获得子菜单的句柄
		BOOL MFlag=subMenu->ModifyMenuA(ID_DELETE_DBICD,MF_BYCOMMAND|MF_STRING,ID_DELETE_DBICD,sTxt);
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN |
			TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
	}
}

void CDlgDBProtocolICD::OnGridDBClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNMHDR;

	if(pItem)
	{
		long nTreeId = (long)m_grid_protocol.GetItemData(pItem->iRow,pItem->iColumn);
		// 找到对应nTreeId的节点，展开树节点，并显示节点数据
		if(nTreeId != 0)
		{
			HTREEITEM hItem=m_tree_protocol.GetSelectedItem();  
			if(hItem==NULL)
				return ;

			HTREEITEM hFItem = FindTreeNode(hItem,nTreeId,0);
			if(hFItem != 0)
			{
				TREEDATA& TreeItem = *(TREEDATA*)m_tree_protocol.GetItemData(hFItem);
				// 显示表格数据
				DisplayGridData(TreeItem);

				// 选择树节点
				m_tree_protocol.SelectItem(hFItem); 
				m_tree_protocol.Select(hFItem, TVGN_DROPHILITE);

				// 展开选择的节点
				m_tree_protocol.Expand(hItem, TVE_EXPAND);
			}
		}
	}
}

HTREEITEM CDlgDBProtocolICD::FindTreeNode(HTREEITEM hItem,int nTreeId,int nNodeNo)
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

void CDlgDBProtocolICD::DisplayGridData(TREEDATA& TreeNode)
{
	// 显示根节点数据
	if(TreeNode.nProType == -2)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		vector<vector<string>> vtTemp;
		vector<string> vtTData;
		vtTData.push_back("父节点");
		vtTData.push_back("子节点");
		vtTemp.push_back(vtTData);
		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
		//DispRootInfor();
	}
	// 显示测试系统下节点数据
	else if(TreeNode.nProType == -1)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		vector<vector<string>> vtTemp;
		vector<string> vtTData;
		vtTData.push_back("父节点");
		vtTData.push_back("子节点");
		vtTemp.push_back(vtTData);
		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
		//DispSystemInfor();
	}
	// 显示1553B协议节点数据
	else if(TreeNode.nProType == 0)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		//m_Pro1553B.ReadSheetData(TreeNode.nData,vtSheetData);
		vector<vector<string>> vtTemp;
		if(m_pPro1553B)
			m_pPro1553B->GetTempData(vtTemp,TreeNode.sDataType);

		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
	}
	// 显示串口协议节点数据
	else if(TreeNode.nProType == 1)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		vector<vector<string>> vtTemp;
		if(m_pProSerial)
			m_pProSerial->GetTempData(vtTemp,TreeNode.sDataType);

		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
	}
	// 显示429协议节点数据
	else if(TreeNode.nProType == 2)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		vector<vector<string>> vtTemp;
		if(m_pProArinc429)
			m_pProArinc429->GetTempData(vtTemp,TreeNode.sDataType);

		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
	}
	// 显示CAN协议节点数据
	else if(TreeNode.nProType == 3)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		//m_pProCan.ReadSheetData(TreeNode.nData,vtSheetData);
		vector<vector<string>> vtTemp;
		if(m_pProCan)
			m_pProCan->GetTempData(vtTemp,TreeNode.sDataType);

		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
	}
	// 显示离散量节点数据
	else if(TreeNode.nProType == 4)
	{
		vector<vector<string>>& vtSheetData = TreeNode.vtNodeData;
		//m_pProDiscrete.ReadSheetData(TreeNode.nData,vtSheetData);
		vector<vector<string>> vtTemp;
		if(m_pProDiscrete)
			m_pProDiscrete->GetTempData(vtTemp,TreeNode.sDataType);

		DispNodeInfor(vtSheetData, vtTemp, TreeNode);
	}
}

void CDlgDBProtocolICD::DispNodeInfor(vector<vector<string>>& vtInfor, 
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
	m_grid_protocol.GetClientRect(rt);

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
		//// 如果为BLOCK数据，只显示BLOCK记录
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

	// 表格自适应行高
	for (int nRow = 0; nRow < nRow_Count; nRow++)
	{
		//  Skip hidden rows when autosizing
		if( m_grid_protocol.GetRowHeight( nRow) > 0 )
			m_grid_protocol.AutoSizeRow(nRow);
	}

	// 根据模板设置表格字体
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

		// 表头模板
		if(i < lTemp_RowNum && bTblHead)
		{
			vector<string>& vtTempRowItem = vtTemp[i];
			for(long j = 0; j < nCol_Count; ++ j)
			{
				if(j >= lTemp_ColumNum) break;
				if(vtTempRowItem[j] != "")
				{
					const LOGFONT* lpFont = m_grid_protocol.GetItemFont(i,j);
					LOGFONT ItemFont;
					memcpy(&ItemFont,lpFont,sizeof(LOGFONT));
					ItemFont.lfWeight = 700;	// 设置成黑体
					m_grid_protocol.SetItemFont(i,j,&ItemFont);
				}
				else
				{
					++ lSpaceNum;
				}
			}
			// 模板的整行空格表示表头的结束
			if(lSpaceNum == lTemp_ColumNum)
			{
				bTblHead = false;
			}
			lTblHeadNum = i;
		}
		else
		{
			bTblHead = false;
		}

		// 表尾模板
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
						ItemFont.lfWeight = 700;	// 设置成黑体
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

		// 表格数据区，如果是PROOT及BLOCK数据，则设置关联数据及颜色
		if(TreeItem.sDataType == "PROOT" || TreeItem.sDataType == "ITEM" || 
			TreeItem.sDataType == "ROOT" || TreeItem.sDataType == "SYSTEM")
		{
			// 离散量BLOCK数据不高亮显示
			if(TreeItem.nProType == 4 && TreeItem.sDataType == "ITEM")
				continue;

			if(!bTblHead && !bTblTail)
			{
				if(TreeItem.sDataType == "PROOT" || TreeItem.sDataType == "ITEM")
					lStartColum = 0;
				else
					lStartColum = 1;
				
				// 如果规范号或信号名称为空则不设置高亮及关联属性
				if(TreeItem.sDataType == "ITEM")
				{
					vector<string>& RowItem = vtInfor[i];
					if(RowItem[lStartColum] == "" || RowItem[lStartColum+1] == "")
						continue;
				}

				const LOGFONT* lpFont = m_grid_protocol.GetItemFont(i,lStartColum);
				LOGFONT ItemFont;
				memcpy(&ItemFont,lpFont,sizeof(LOGFONT));
				ItemFont.lfWeight = 700;	// 设置成黑体
				m_grid_protocol.SetItemFont(i,lStartColum,&ItemFont);
				m_grid_protocol.SetItemFgColour(i,lStartColum,RGB(0x1C,0x86,0xEE));//#1C86EE
				if(nSubIdx < nSubItemNum)
					m_grid_protocol.SetItemData(i,lStartColum,TreeItem.subNode[nSubIdx].lID);
				++ nSubIdx;
				// 设置数据索引
			}
		}
	}

	m_grid_protocol.Invalidate();
}

void CDlgDBProtocolICD::ClearGridData(void)
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

void CDlgDBProtocolICD::OnDeleteDbicd()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM hItem=m_tree_protocol.GetSelectedItem();  
	if(hItem==NULL)
		return ;

	TREEDATA& TreeNodeItem = *(TREEDATA*)m_tree_protocol.GetItemData(hItem);
	
	CString sTxt = "";
	sTxt.Format("是否删除%s！", TreeNodeItem.sName);
	if(AfxMessageBox(sTxt,MB_YESNO) == IDNO)
		return;

	// 启动进度条动画
	m_pdlgProgress = new CDlgProgress;
	m_pdlgProgress->SetInfor("删除数据中...");
	m_pdlgProgress->SetProgressType(1);

	m_pPro1553B->StopDeleteDB(FALSE);		// 允许删除数据库ICD
	m_pProDiscrete->StopDeleteDB(false);
	m_pProCan->StopDeleteDB(false);
	m_pProSerial->StopDeleteDB(false);
	m_pProArinc429->StopDeleteDB(false);

	m_nExitCode = 0;

	// 重置ID
	XProtocol_Funtion::ResetDataId();
	XProtocol_Funtion::ResetProtocolId();

	// 启动删除数据库ICD的线程
	unsigned int dwThreadID = 0;
	_beginthreadex(NULL, 0, DeleteDBData, this, 0, &dwThreadID);

	if(m_pdlgProgress->DoModal() != IDOK)
	{
		m_pPro1553B->StopDeleteDB(TRUE);
		m_pProDiscrete->StopDeleteDB(true);
		m_pProCan->StopDeleteDB(true);
		m_pProSerial->StopDeleteDB(true);
		m_pProArinc429->StopDeleteDB(true);
	}

	m_nExitCode = m_pdlgProgress->GetExitCode();

	m_pdlgProgress->DestroyWindow();
	delete m_pdlgProgress;
	m_pdlgProgress = NULL;

	//// 删除数据库ICD记录
	//DeleteDBICD(TreeNodeItem);

	// 删除树节
	if(m_nExitCode != 2)
		DeleteTreeData(TreeNodeItem,hItem);
}

BOOL CDlgDBProtocolICD::DeleteDBICD(void)
{
	HTREEITEM hItem=m_tree_protocol.GetSelectedItem();  
	if(hItem==NULL)
		return FALSE;

	TREEDATA& TreeNodeItem = *(TREEDATA*)m_tree_protocol.GetItemData(hItem);

	BOOL bSuccess = DeleteDBICD(TreeNodeItem);

	// 退出进度条动画
	if(m_pdlgProgress && m_pdlgProgress->GetSafeHwnd())
	{
		//pUser->m_pdlgProgress->ExitWindow();
		HWND hProgress = m_pdlgProgress->GetSafeHwnd();
		::PostMessage(hProgress,EXIT_PROGRESS,0,0);
	}

	return bSuccess;
}

BOOL CDlgDBProtocolICD::DeleteDBICD(TREEDATA& TreeNodeItem)
{
	if(m_nExitCode == 2)
		return FALSE;

	int nTest = 0;
	if(TreeNodeItem.sDataType == "ITEM")
	{
		// 删除1553B协议块
		if(TreeNodeItem.nProType == 0)
			m_pPro1553B->DeleteBLOCKFromDB(TreeNodeItem);
		// 删除串口类协议块
		else if(TreeNodeItem.nProType == 1)
			m_pProSerial->DeleteBLOCKFromDB(TreeNodeItem);
		// 删除429协议块
		else if(TreeNodeItem.nProType == 2)
			m_pProArinc429->DeleteBLOCKFromDB(TreeNodeItem);
		// 删除CAN协议块
		else if(TreeNodeItem.nProType == 3)
			m_pProCan->DeleteBLOCKFromDB(TreeNodeItem);
		// 删除离散量协议
		else if(TreeNodeItem.nProType == 4)
			m_pProDiscrete->DeleteBLOCKFromDB(TreeNodeItem);
	}

	int nNodeNum = TreeNodeItem.subNode.size();
	for(int i = 0; i < nNodeNum; ++ i)
	{
		if(m_nExitCode == 2)
			return FALSE;

		DeleteDBICD(TreeNodeItem.subNode[i]);
	}

	return TRUE;
}

unsigned WINAPI DeleteDBData(LPVOID lpParam)
{
	CDlgDBProtocolICD* pUser = (CDlgDBProtocolICD*)lpParam;
	if(pUser)
	{
		pUser->DeleteDBICD();
	}

	return 0;
}

BOOL CDlgDBProtocolICD::DeleteTreeData(TREEDATA& TreeNode, HTREEITEM hTreeItem)
{
	// 删除树节点
	HTREEITEM hParentItem = m_tree_protocol.GetParentItem(hTreeItem);
	if(hParentItem != NULL)
	{
		TREEDATA& TreeNodeItem = *(TREEDATA*)m_tree_protocol.GetItemData(hParentItem);

		// 记录被删节点的父节点，在重构时再选种该节点
		m_ParentTreeNode.nDlgId = TreeNodeItem.nDlgId;
		m_ParentTreeNode.nProType = TreeNodeItem.nProType;
		m_ParentTreeNode.sName = TreeNodeItem.sName;
		// 删除目录中的记录
		vector<vector<string>>& vtPData = TreeNodeItem.vtNodeData;
		vector<vector<string>>::iterator it = vtPData.begin();
		for(; it != vtPData.end(); ++ it)
		{
			if(TreeNodeItem.sDataType == "PROOT")
			{
				if(TreeNode.nProType == 0)	// 1553B
				{
					if(it->at(1) == TreeNode.vtNodeData[0][3] && it->at(3) == TreeNode.vtNodeData[10][5])
					{
						vtPData.erase(it);
						break;
					}
				}
				else if(TreeNode.nProType == 1) // 串口
				{
					if(it->at(1) == TreeNode.vtNodeData[0][3] && it->at(3) == TreeNode.vtNodeData[6][5])
					{
						vtPData.erase(it);
						break;
					}
				}
				else if(TreeNode.nProType == 2) // 429
				{
					if(it->at(1) == TreeNode.vtNodeData[0][3] && it->at(3) == TreeNode.vtNodeData[6][5])
					{
						vtPData.erase(it);
						break;
					}
				}
				else if(TreeNode.nProType == 3) // CAN
				{
					if(it->at(1) == TreeNode.vtNodeData[0][3] && it->at(3) == TreeNode.vtNodeData[8][5])
					{
						vtPData.erase(it);
						break;
					}
				}
				else if(TreeNode.nProType == 4) // 离散量
				{
					if(it->at(0) == TreeNode.vtNodeData[1][3])
					{
						vtPData.erase(it);
						break;
					}
				}
			}
			else
			{
				if(it->at(1) == TreeNode.sName.GetBuffer())
				{
					vtPData.erase(it);
					break;
				}
			}
		}
		// 删除树节点数据
		vector<TREEDATA>& vtTreeNode = TreeNodeItem.subNode;
		vector<TREEDATA>::iterator it_tr = vtTreeNode.begin();
		for(; it_tr != vtTreeNode.end(); ++ it_tr)
		{
			if((*it_tr).lID == TreeNode.lID)
			{
				vtTreeNode.erase(it_tr);
				break;
			}
		}
	}
	else
	{
		// 清除树节点数据
		TreeNode.lID = 0;
		TreeNode.nProType = -2;
		TreeNode.nData = -1;
		TreeNode.nDlgId = -1;
		TreeNode.nSysDriveId = -1;
		TreeNode.nRecSend = -1;
		TreeNode.sName = "";
		TreeNode.sProNo = "";
		TreeNode.sDataType = "";
		TreeNode.pParent = NULL;
		TreeNode.vtNodeData.clear();
		TreeNode.subNode.clear();
		
		ClearGridData();
	}

	// 重构树节点
	BuilProTree(*m_pTreeData,FALSE);

	// 选中被删除节点的父节点
	HTREEITEM hRoot = m_tree_protocol.GetRootItem();
	HTREEITEM hFind = TravelAll(hRoot);
	if(hFind)
	{
		m_tree_protocol.SelectItem(hFind);
		m_tree_protocol.Select(hFind, TVGN_DROPHILITE);
	}

	return TRUE;
}


HTREEITEM CDlgDBProtocolICD::TravelAll(HTREEITEM ht)
{
	HTREEITEM hResult = ht;
	if(!hResult)
		return NULL;

	TREEDATA& TreeItem = *(TREEDATA*)m_tree_protocol.GetItemData(ht);
	if(TreeItem.nDlgId == m_ParentTreeNode.nDlgId && 
		TreeItem.nProType == m_ParentTreeNode.nProType &&
		TreeItem.sName == m_ParentTreeNode.sName)
	{
		return hResult;
	}
	
	HTREEITEM hCurItem = m_tree_protocol.GetChildItem(ht);
	HTREEITEM hNextItem;
	while(hCurItem)
	{
		hNextItem = hCurItem;
		hResult = TravelAll(hNextItem);
		if(hResult)
			return hResult;
		hCurItem = m_tree_protocol.GetNextSiblingItem(hCurItem);
	}

	return NULL;
}
