// ProDiscrete.cpp : 实现文件
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "ProDiscrete.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ProDiscrete 离散量协议处理类


ProDiscrete::ProDiscrete()
{
	m_nDlgId = 0;

	m_bExitSaveToDB = FALSE;
	m_bExitLoadIcd = false;
	m_bExitLoadDBIcd = false;
	m_bExitDeleteDB = false;

	m_vtTemp1.clear();
	m_vtTemp2.clear();
	m_vtTemp3.clear();
}

ProDiscrete::~ProDiscrete()
{

}


// ProDiscrete 处理程序


void ProDiscrete::LoadIcdFile(CString strFilePath)
{
	m_protocoNameVec.clear();

	//m_config_1553.OpenFile(strFilePath,m_protocoNameVec);

	m_excelFile.OpenFile(strFilePath);

	m_excelFile.LoadSheetName(m_protocoNameVec);
}

TREEDATA& ProDiscrete::BuildProtocolTree(TREEDATA& ParentTree,int nProType,int nDlgId,int& nTreeId)
{
	vector<vector<string>> vtTemp2Head;
	vector<vector<string>> vtTemp2Tail;
	vector<vector<string>> vtItemHead;
	vector<string> vtItemRow;

	GetTempHeadTail(m_vtTemp2,vtTemp2Head,vtTemp2Tail,2);
	// 构建ICD模板（去掉共享内存ID）
	if(vtTemp2Head.size() > 0)
	{
		int nEnd = vtTemp2Head[0].size();
		for(int i = 0; i < 10 && i < nEnd; ++ i)
		{
			vtItemRow.push_back(vtTemp2Head[0][i]);
		}
		vtItemHead.push_back(vtItemRow);
	}

	m_nDlgId = nDlgId;

	// 获得目录页数据（该页记录所有的协议块）
	TREEDATA TreeItem;
	TreeItem.lID = 0;
	TreeItem.nProType = nProType;
	TreeItem.nData = -1;
	TreeItem.nDlgId = m_nDlgId;
	TreeItem.nSysDriveId = -1;
	TreeItem.nRecSend = -1;
	TreeItem.sName = "";
	TreeItem.sProNo = "";
	TreeItem.sDataType = "";
	TreeItem.pParent = NULL;
	TreeItem.vtNodeData.clear();
	TreeItem.subNode.clear();

	m_treedata.lID = 0;
	m_treedata.nProType = nProType;
	m_treedata.nDlgId = m_nDlgId;
	m_treedata.nSysDriveId = -1;
	m_treedata.nRecSend = -1;
	m_treedata.nData = -1;
	m_treedata.sName = "";
	m_treedata.sProNo = "";
	m_treedata.sDataType = "PROOT";
	m_treedata.pParent = &ParentTree;
	m_treedata.vtNodeData.clear();
	m_treedata.subNode.clear();

	// debug
	//return m_treedata;

	int nSheetNum = m_protocoNameVec.size();
	if(nSheetNum <= 0)
		return m_treedata;

	int nTabIndex = 0;
	vector<vector<string>>vec;
	m_excelFile.ReadInfo_Sub_Item(nTabIndex+1,vec,10,-1);

	if(vec.size()==0)
	{
		//AfxMessageBox("该页为空！");
		return m_treedata;
	}

	m_treedata.lID = nTreeId ++;
	m_treedata.sProNo = "目录";
	m_treedata.nData = nTabIndex;
	m_treedata.sName = "mDAQ_ICD";

	vector<string> vtDirectory;
	vtDirectory.push_back("驱动板卡");
	m_treedata.vtNodeData.push_back(vtDirectory);

	for(int i = 1,end = vec.size(); i < end; ++ i)
	{
		if(m_bExitLoadIcd)
		{
			//m_bExitLoadIcd = false;

			m_treedata.lID = 0;
			m_treedata.nProType = nProType;
			m_treedata.nData = -1;
			m_treedata.sName = "";
			m_treedata.sProNo = "";
			m_treedata.sDataType = "PROOT";
			m_treedata.pParent = &ParentTree;
			m_treedata.vtNodeData.clear();
			m_treedata.subNode.clear();

			return m_treedata;
		}

		vector<string>& RowDataItem = vec[i];
		CString strDriveName = RowDataItem[3].c_str();
		strDriveName.Trim();
		if(strDriveName.IsEmpty())
			continue;

		BOOL bFindSubNode = FALSE;
		int nSubNodeNum = m_treedata.subNode.size();
		int j = 0;
		for(; j < nSubNodeNum; ++ j)
		{
			TREEDATA& subNode = m_treedata.subNode[j];
			if(subNode.sName == strDriveName)
			{
				bFindSubNode = TRUE;
				break;
			}
		}

		// 如果找到节点则将数据插入到该节点
		if(bFindSubNode)
		{
			TREEDATA& subNode = m_treedata.subNode[j];
			subNode.vtNodeData.push_back(RowDataItem);
		}
		// 增加新节点
		else
		{
			// 从sstrDriveName可以获得nSysDriveId
			int nSysDriveId = -1;
			int nDBoardNum = m_DriveVec.size();
			for(int k = 0; k < nDBoardNum; ++ k)
			{
				__SYS_Drive& DriveItem = m_DriveVec[k];
				if(DriveItem.sName == strDriveName.GetBuffer())
				{
					nSysDriveId = DriveItem.nId;
					break;
				}
			}

			TreeItem.lID = nTreeId ++;
			TreeItem.nData = nTabIndex;
			TreeItem.nSysDriveId = nSysDriveId;
			TreeItem.sName = strDriveName/*RowDataItem[1].c_str()*/;
			TreeItem.sDataType = "ITEM";
			TreeItem.sProNo = "";
			TreeItem.pParent = &m_treedata;
			TreeItem.vtNodeData = vtItemHead;//vtTemp2Head;
			TreeItem.vtNodeData.push_back(RowDataItem);
			TreeItem.subNode.clear();

			vtDirectory.clear();
			vtDirectory.push_back(strDriveName.GetBuffer());
			m_treedata.vtNodeData.push_back(vtDirectory);

			m_treedata.subNode.push_back(TreeItem);
		}
	}


	vector<TREEDATA>::iterator it_pt = ParentTree.subNode.begin();
	for(; it_pt != ParentTree.subNode.end(); ++ it_pt)
	{
		// 删除同名节点
		if(it_pt->sName == m_treedata.sName)
		{
			ParentTree.subNode.erase(it_pt);
			break;
		}
	}
	ParentTree.subNode.push_back(m_treedata);

	return ParentTree.subNode[ParentTree.subNode.size()-1];
}

void ProDiscrete::GetTempData(vector<vector<string>>& vtTemp, CString csSheetNo)
{
	vtTemp.clear();

	if(csSheetNo == "PROOT")
		vtTemp = m_vtTemp1;
	else if(csSheetNo == "ITEM")
		vtTemp = m_vtTemp2;
	else if(csSheetNo == "DATA")
		vtTemp = m_vtTemp3;

	return;
}

void ProDiscrete::Initailize(void)
{
	// 获取协议模板
	CString strPath = GetExePath();
	CString strTempFile;
	strTempFile.Format("%s\\Template\\mDAQ_DISP.xlsx",strPath);

	vector<CString> vtSheetName;
	m_excelFile.OpenFile(strTempFile);
	m_excelFile.LoadSheetName(vtSheetName);

	vector<vector<string>> vtTemp1;
	vector<vector<string>> vtTemp2;
	vector<vector<string>> vtTemp3;

	int nTempNum = vtSheetName.size();
	for(int i = 0; i < nTempNum; ++ i)
	{
		if(vtSheetName[i] == "目录")
			m_excelFile.ReadInfo_Sub_Item(i+1,m_vtTemp1,1,-1);
		else if(vtSheetName[i] == "ITEM")
			m_excelFile.ReadInfo_Sub_Item(i+1,m_vtTemp2,1,-1,11);
		else if(vtSheetName[i] == "DATA")
			m_excelFile.ReadInfo_Sub_Item(i+1,m_vtTemp3,1,-1);
	}
	m_excelFile.close_excel();
}

CString ProDiscrete::GetExePath(void)  
{  
	CString strExePath;  
	GetModuleFileName(NULL, strExePath.GetBuffer(MAX_PATH), MAX_PATH);  
	strExePath.ReleaseBuffer();  
	strExePath = strExePath.Left(strExePath.ReverseFind(_T('\\')));  
	return strExePath;  
}

// 获得模板的头部和尾部数据
void ProDiscrete::GetTempHeadTail(vector<vector<string>>& vtTemp,vector<vector<string>>& vtTHead,
	vector<vector<string>>& vtTTail,int nType)
{
	// 获得模板2的头部和尾部
	int nTRows = vtTemp.size();
	bool bFindAllPace = false;
	bool bFindHead = false;
	bool bFindTail = false;
	int nTHeadRows = 0;
	int nTTailRows = 0;
	for(int i = 0; i < nTRows; ++ i)
	{
		int nSpaceNum = 0;
		vector<string>& vtTRowData = vtTemp[i];
		int nTColums = vtTRowData.size();

		for(int j = 0; j < nTColums; ++ j)
		{
			if(vtTRowData[j] == "")
			{
				++ nSpaceNum;
			}
		}

		if((!bFindAllPace || !bFindHead) && !bFindTail)
		{
			if(nSpaceNum != nTColums)
			{
				vtTHead.push_back(vtTemp[i]);
				++ nTHeadRows;
				bFindHead = true;
				bFindAllPace = false;
			}
			else
			{
				bFindAllPace = true;
			}
		}
		else if(!bFindAllPace || !bFindTail)
		{
			if(nSpaceNum != nTColums)
			{
				vtTTail.push_back(vtTemp[i]);
				++ nTTailRows;
				bFindTail = true;
				bFindAllPace = false;
			}
			else
			{
				bFindAllPace = true;
			}
		}
		else
		{
			break;
		}
	}

	// 检验模板的合法性
	bool bIsValid = true;
	if(nType == 1)
	{
		if(nTHeadRows < 1)
			bIsValid = false;
		else if(vtTHead[0].size() < 1)
			bIsValid = false;
	}
	else if(nType == 2)
	{
		if(nTHeadRows < 1)
			bIsValid = false;
		else if(vtTHead[0].size() < 11)
			bIsValid = false;
	}

	if(!bIsValid)
	{
		int nColNum = 0;
		vtTHead.clear();
		vtTTail.clear();
		if(nType == 1)
		{
			nTHeadRows = 1;
			nTTailRows = 0;
			nColNum = 1;
		}
		else if(nType == 2)
		{
			nTHeadRows = 1;
			nTTailRows = 0;
			nColNum = 11;
		}

		for(int i = 0; i < nTHeadRows; ++ i)
		{
			vector<string> vtRowData;
			for(int j = 0; j < nColNum; ++ j)
			{
				vtRowData.push_back("");
			}
			vtTHead.push_back(vtRowData);
		}

		for(int i = 0; i < nTTailRows; ++ i)
		{
			vector<string> vtRowData;
			for(int j = 0; j < nColNum; ++ j)
			{
				vtRowData.push_back("");
			}
			vtTHead.push_back(vtRowData);
		}
	}
}

void ProDiscrete::SaveDataToDB(TREEDATA& TreeData, int nCurDrive)
{
	int nItem_ConfigId = 0;
	int nItem_SharedId = 0;

	int nDBoardNum = m_DriveVec.size();
	if(nDBoardNum <= 0)
	{
		//AfxMessageBox("没有获得驱动板卡的信息！");
		return;
	}

	if(m_protocoNameVec.size() <= 0)
		return;

	m_nDlgId = TreeData.nDlgId;
	CString sPageName = m_protocoNameVec[0];

	int nBlockNum = TreeData.subNode.size();
	for(int i = 0; i < nBlockNum; ++ i)
	{
		if(m_bExitSaveToDB)
		{
			//m_bExitSaveToDB = FALSE;
			return;
		}

		TREEDATA& TreeSubItem = TreeData.subNode[i];

		int nSysDriveId = 0;
		int nDriveid = 0;
		bool bFindDBoard = false;
		for(int j = 0; j < nDBoardNum; ++ j)
		{
			__SYS_Drive& DriveInfor = m_DriveVec[j];
			// ICD文件中的板卡是否可以从驱动板卡信息中找到
			if(TreeSubItem.sName == DriveInfor.sName.c_str())
			{
				nSysDriveId = DriveInfor.nId;
				nDriveid = DriveInfor.nDriveId;

				bFindDBoard = true;
				break;
			}
		}

		// 如果驱动板卡信息中找不到该驱动板卡，则当前ICD不保存
		if(!bFindDBoard)
		{
			//// 进度条回调
			//m_func(m_lpParam);

			continue;
		}

		// ICD记录存入数据库
		int nDataNum = TreeSubItem.vtNodeData.size();
		for(int j = 1; j < nDataNum; ++ j)
		{
			if(m_bExitSaveToDB)
			{
				//m_bExitSaveToDB = FALSE;
				return;
			}

			vector<string>& vtRowData = TreeSubItem.vtNodeData[j];

			CString sName = vtRowData[0].c_str();
			CString sInOut = vtRowData[2].c_str();
			CString sDataType = vtRowData[1].c_str();
			CString sSysDriveName = vtRowData[3].c_str();	
			CString sChannelNo = vtRowData[4].c_str();
			CString sMark ="";		 
			CString sDealMode = vtRowData[9].c_str();

			sName.Trim();
			sDataType.Trim();
			sInOut.Trim();
			sSysDriveName.Trim();
			sChannelNo.Trim();
			if(sName.IsEmpty() || sDataType.IsEmpty() || sInOut.IsEmpty()||sSysDriveName.IsEmpty() || sChannelNo.IsEmpty())
				continue;

			sName =sPageName+"."+sName;

			int  nDealMode = _tstoi(sDealMode);			// 处理方式
			int nChannelNo = _tstoi(sChannelNo);		// 通道号

			if(sInOut == _T("输入"))
				sInOut = PROTOCOL_RecSend_Rec;
			else
				sInOut = PROTOCOL_RecSend_Send;
			//构建共享内存ID
			XProtocol_Funtion::Save_ProtoclData_toConfigData(m_nDlgId,sInOut,sName,
				sDataType,0,nDealMode,nChannelNo,
				nDriveid,nSysDriveId,"",1.0,0.0,sMark,
				nItem_ConfigId,nItem_SharedId,9,nDealMode);
		}
	
		// 进度条回调
		m_func(m_lpParam);
	}
}

BOOL ProDiscrete::BuildDBTree(TREEDATA& DBTree, __SYS_Drive& DInforItem, int nProType, int& nTreeId,int& nStatus)
{
	// 获取该系统编号及板卡编号下的所有记录
	CString strWhere = "";
	strWhere.Format("and a.nSysDriveId=%d",DInforItem.nId);
	vector<__DATA_CONFIG> vtDisProtocol;
	CManageDb_Sqllite::GetInstance().Query_DataConfig(vtDisProtocol,DBTree.nDlgId,strWhere.GetBuffer());

	int nProNum = vtDisProtocol.size();
	if(nProNum <= 0)
		return FALSE;

	// 获取数据显示模板
	vector<vector<string>> vtTemp1Head;
	vector<vector<string>> vtTemp1Tail;
	vector<vector<string>> vtTemp2Head;
	vector<vector<string>> vtTemp2Tail;
	GetTempHeadTail(m_vtTemp1,vtTemp1Head,vtTemp1Tail,1);
	GetTempHeadTail(m_vtTemp2,vtTemp2Head,vtTemp2Tail,2);

	TREEDATA TreeSubItem;
	TreeSubItem.lID = nTreeId ++;
	TreeSubItem.nProType = nProType;
	TreeSubItem.nData = 0;
	TreeSubItem.nDlgId = DBTree.nDlgId;
	TreeSubItem.nSysDriveId = DInforItem.nId;
	TreeSubItem.nRecSend = -1;
	TreeSubItem.sName = vtDisProtocol[0].sSysDriveName.c_str();
	TreeSubItem.sProNo = "";
	TreeSubItem.sDataType = "ITEM";
	TreeSubItem.pParent = NULL;
	TreeSubItem.vtNodeData = vtTemp2Head;
	TreeSubItem.subNode.clear();

	CString sTxt = "";
	for(int i = 0; i < nProNum; ++ i)
	{
		if(m_bExitLoadDBIcd)
		{
			nStatus = 1;
			//m_bExitLoadDBIcd = false;
			return false;
		}

		__DATA_CONFIG& ProItem = vtDisProtocol[i];

		vector<string> vtRowData;

		// 名称
		vtRowData.push_back(ProItem.sName);
		// 数据类型
		sTxt = GetDataType(ProItem.nDataType);
		vtRowData.push_back(sTxt.GetBuffer());
		// 方向
		sTxt = ProItem.nInOut==1?"输入":"输出";
		vtRowData.push_back(sTxt.GetBuffer());
		// 板卡名称
		vtRowData.push_back(ProItem.sSysDriveName);
		// 通道号
		sTxt.Format("%d",ProItem.nChannelIndex);
		vtRowData.push_back(sTxt.GetBuffer());
		// 系数
		sTxt.Format("%.2f",ProItem.fCoefficient);
		vtRowData.push_back(sTxt.GetBuffer());
		// 补偿值
		sTxt.Format("%.2f",ProItem.fOffset);
		vtRowData.push_back(sTxt.GetBuffer());
		// 属性
		vtRowData.push_back(ProItem.sExtArr);
		// 备注
		vtRowData.push_back(ProItem.sMark);
		// 处理方式
		sTxt.Format("%d",ProItem.nDealMode);
		vtRowData.push_back(sTxt.GetBuffer());
		// 共享内存ID
		sTxt.Format("%d",ProItem.nSharedID);
		vtRowData.push_back(sTxt.GetBuffer());

		TreeSubItem.vtNodeData.push_back(vtRowData);
	}

	// 判断该节点下是否有mDAQ_ICD节点
	int nSubNodeNum = DBTree.subNode.size();
	BOOL bFindSubNode = FALSE;
	int i = 0;
	for(; i < nSubNodeNum; ++ i)
	{
		TREEDATA& SubItem = DBTree.subNode[i];
		if(SubItem.sName == "mDAQ_ICD")
		{
			bFindSubNode = TRUE;
			break;
		}
	}

	// 如果没有该节点，则创建新节点
	if(!bFindSubNode)
	{
		TREEDATA TreeItem;

		TreeSubItem.pParent = &TreeItem;

		TreeItem.lID = nTreeId ++;
		TreeItem.nProType = nProType;
		TreeItem.nData = 0;
		TreeItem.nDlgId = DBTree.nDlgId;
		TreeItem.nSysDriveId = DInforItem.nId;
		TreeItem.nRecSend = -1;
		TreeItem.sProNo = "目录";
		TreeItem.sDataType = "PROOT";
		TreeItem.sName = "mDAQ_ICD";
		TreeItem.vtNodeData.clear();
		TreeItem.pParent = &DBTree;
		TreeItem.subNode.push_back(TreeSubItem);

		// 数据列表中插入一条记录
		vector<string> vtRowData;
		//vtRowData.push_back("驱动板卡");
		//TreeItem.vtNodeData.push_back(vtRowData);
		TreeItem.vtNodeData = vtTemp1Head;

		vtRowData.clear();
		vtRowData.push_back(TreeSubItem.sName.GetBuffer());
		TreeItem.vtNodeData.push_back(vtRowData);

		// 父节点表格数据
		vector<string> vtParentNode;
		vtParentNode.push_back(DBTree.sName.GetBuffer());
		vtParentNode.push_back("mDAQ_ICD");
		DBTree.vtNodeData.push_back(vtParentNode);

		// 插入数据节点
		DBTree.subNode.push_back(TreeItem);
	}
	else
	{
		TREEDATA& TreeSubNode = DBTree.subNode[i];

		// 清除树中对应节点以前的数据
		vector<vector<string>>::iterator it_tb = TreeSubNode.vtNodeData.begin();
		for(; it_tb != TreeSubNode.vtNodeData.end(); ++ it_tb)
		{
			vector<string>& vtTbData = *it_tb;
			if(vtTbData[0] == DInforItem.sName)
			{
				TreeSubNode.vtNodeData.erase(it_tb);
				break;
			}
		}

		// 删除同名节点
		vector<TREEDATA>::iterator it_pt = TreeSubNode.subNode.begin();
		for(; it_pt != TreeSubNode.subNode.end(); ++ it_pt)
		{
			// 删除同名节点
			if(it_pt->sName == TreeSubItem.sName)
			{
				TreeSubNode.subNode.erase(it_pt);
				break;
			}
		}	
		
		TreeSubItem.pParent = &TreeSubNode;

		TreeSubNode.subNode.push_back(TreeSubItem);

		vector<string> vtRowData;
		vtRowData.push_back(TreeSubItem.sName.GetBuffer());
		TreeSubNode.vtNodeData.push_back(vtRowData);
	}

	return TRUE;
}

CString ProDiscrete::GetDataType(int nDataType)
{
	//数据类型
	if(nDataType == DATA_TYPE_INT)
	{
		return DATA_TYPE_INT_NAME;
	}
	else if(nDataType==DATA_TYPE_FLOAT)
	{
		return DATA_TYPE_FLOAT_NAME;
	}
	else if(nDataType==DATA_TYPE_STRING)
	{
		return DATA_TYPE_STRING_NAME;
	}
	else if(nDataType==DATA_TYPE_CHAR)
	{
		return DATA_TYPE_CHAR_NAME;
	}
	else if(nDataType==DATA_TYPE_DOUBLE)
	{
		return DATA_TYPE_DOUBLE_NAME;
	} 
	else if(nDataType ==DATA_TYPE_STRING_UNICODE)
	{
		return DATA_TYPE_STRING_UNICODE_NAME;
	}
	else if(nDataType == DATA_TYPE_BUFFER)
	{
		return DATA_TYPE_BUFFER_NAME;
	}
	else 
		return "";

	return "";
}

void ProDiscrete::DeleteBLOCKFromDB(TREEDATA& TreeNode)
{
	if(TreeNode.sDataType != "ITEM")
		return;

	CString sCondition = "";
	CString sName = "";
	CString sInOut = "";

	int nDataNum = TreeNode.vtNodeData.size();
	for(int i = 1; i < nDataNum; ++ i)
	{
		if(m_bExitDeleteDB)
			return;

		vector<string>& vtRowDataItem = TreeNode.vtNodeData[i];

		sName = vtRowDataItem[0].c_str();
		sInOut = vtRowDataItem[2].c_str();
		int nInOut = sInOut == "输入"?1:2;

		sCondition.Format("nDlgId=%d and nSysDriveId=%d and sName='%s' and nInOut=%d",
			TreeNode.nDlgId,TreeNode.nSysDriveId,sName,nInOut);
		CManageDb_Sqllite::GetInstance().Delete_DataConfig(sCondition);
	}
}

/*void ProDiscrete::OnBnClickedButtonSave()
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


void ProDiscrete::SaveTblData(CString& sPageName)
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
*/

void ProDiscrete::SetProgressFunc(CallFunc func,LPVOID lpParam)
{
	m_lpParam = lpParam;
	m_func = func;
}
