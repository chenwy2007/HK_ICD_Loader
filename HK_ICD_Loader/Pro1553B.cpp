// Pro1553B.cpp : 实现文件
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "Pro1553B.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct tagSUBITEMATTR {
	tagSUBITEMATTR()
	{
		bIsSign = false;
		nStartPosi = 0;
		nItemLen = 0;
		sItemName = "";
		nInforType = 0;
		fOffset = 0.0;
		fCoef = 1.0;
		sDispValue = "";
		sRemark = "";
	}
	bool bIsSign;
	int nStartPosi;				//起始位
	int nItemLen;				//位长
	string sItemName;			//显示名称
	int nInforType;				//信号类型
	float fOffset;				//偏置补偿
	float fCoef;				//系数补偿
	string sDispValue;			//显示值（转义用）
	string sRemark;				//注释
}SUBITEMATTR,*pSUBITEMATTR;
// Pro1553B


Pro1553B::Pro1553B()
{
	m_nProtocolId = 0;
	m_nPulicProId=0xffff;
	m_nDlgId = 0;
	m_bExitSaveToDB = FALSE;
	m_bExitLoadIcd = false;
	m_bExitLoadDBIcd = false;
	m_bExitDeleteDB = false;

	m_nDriveIndex = -1;

	m_vtTemp1.clear();
	m_vtTemp2.clear();
	m_vtTemp3.clear();
}

Pro1553B::~Pro1553B()
{

}

// Pro1553B
void Pro1553B::LoadIcdFile(CString strFilePath)
{
	m_protocoNameVec.clear();

	//m_config_1553.OpenFile(strFilePath,m_protocoNameVec);

	m_config_1553.OpenFile(strFilePath);

	m_config_1553.LoadSheetName(m_protocoNameVec);
}

void Pro1553B::ReadSheetData(int nCurSheetNo,vector<vector<string>>& vtSheetData)
{
	if(nCurSheetNo ==-1)
		return;

	vtSheetData.clear() ;
	CString str;

	m_nGirdColCount =8;

	m_config_1553.ReadInfo_Sub_Item(nCurSheetNo+1,vtSheetData,1,-1,m_nGirdColCount);	 
}

void Pro1553B::GetTempData(vector<vector<string>>& vtTemp, CString csSheetNo)
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

void Pro1553B::Initailize(void)
{
	// 获取协议模板
	CString strPath = GetExePath();
	CString strTempFile;
	strTempFile.Format("%s\\Template\\1553B_DISP.xlsx",strPath);

	vector<CString> vtSheetName;
	m_config_1553.OpenFile(strTempFile);
	m_config_1553.LoadSheetName(vtSheetName);

	vector<vector<string>> vtTemp1;
	vector<vector<string>> vtTemp2;
	vector<vector<string>> vtTemp3;

	int nTempNum = vtSheetName.size();
	for(int i = 0; i < nTempNum; ++ i)
	{
		if(vtSheetName[i] == "目录")
			m_config_1553.ReadInfo_Sub_Item(i+1,m_vtTemp1,1,-1);
		else if(vtSheetName[i] == "ITEM")
			m_config_1553.ReadInfo_Sub_Item(i+1,m_vtTemp2,1,-1,11);
		else if(vtSheetName[i] == "DATA")
			m_config_1553.ReadInfo_Sub_Item(i+1,m_vtTemp3,1,-1,11);
	}
	m_config_1553.close_excel();
}

void Pro1553B::GetTemplate(vector<vector<string>>& vtTemp, CString csSheetNo)
{
	vector<CString>::iterator it = find(m_protocoNameVec.begin(),m_protocoNameVec.end(),csSheetNo);
	if(it == m_protocoNameVec.end())
	{
		return;
	}
	int nTabIndex =(int)(it-m_protocoNameVec.begin());

	m_config_1553.ReadInfo_Sub_Item(nTabIndex+1,vtTemp,1,-1);
}

TREEDATA& Pro1553B::BuildProtocolTree(TREEDATA& ParentTree,int nProType,int nDlgId,int& nTreeId)
{
	m_nSysDriveId = -1;
	if(-1 < m_nDriveIndex && m_nDriveIndex < m_DriveVec.size())
		m_nSysDriveId = m_DriveVec[m_nDriveIndex].nId;
	m_nDlgId = nDlgId;

	// 获得目录页数据（该页记录所有的协议块）
	TREEDATA TreeItem;
	TreeItem.nProType = nProType;
	TreeItem.nData = -1;
	TreeItem.nDlgId = m_nDlgId;
	TreeItem.nSysDriveId = m_nSysDriveId;
	TreeItem.sName = "";
	TreeItem.sProNo = "";
	TreeItem.sDataType = "";
	TreeItem.pParent = NULL;
	TreeItem.vtNodeData.clear();
	TreeItem.subNode.clear();

	m_treedata.nProType = nProType;
	m_treedata.nData = -1;
	m_treedata.nDlgId = m_nDlgId;
	m_treedata.nSysDriveId = m_nSysDriveId;
	m_treedata.sName = "";
	m_treedata.sProNo = "";
	m_treedata.sDataType = "PROOT";
	m_treedata.pParent = &ParentTree;
	m_treedata.vtNodeData.clear();
	m_treedata.subNode.clear();

	// debug
	//return m_treedata;


	vector<CString>::iterator it = find(m_protocoNameVec.begin(),m_protocoNameVec.end(),"目录");
	if(it == m_protocoNameVec.end())
	{
		//AfxMessageBox("未找到协议目录！");
		return m_treedata;
	}
	int nTabIndex =(int)(it-m_protocoNameVec.begin());

	vector<vector<string>>vec;
	m_config_1553.ReadInfo_Sub_Item(nTabIndex+1,vec,1,-1);

	if(vec.size()==0)
	{
		//AfxMessageBox("该页为空！");
		return m_treedata;
	}

	// 剔除目录页中属性为DATA的记录
	vector<vector<string>>::iterator it_dir = vec.begin();
	while(it_dir != vec.end())
	{
		vector<string>& vtRowItem = *it_dir;
		if(vtRowItem[2] == "DATA")
		{
			it_dir = vec.erase(it_dir);
		}
		else
		{
			++ it_dir;
		}
	}

	//// 获得模板页数据
	//GetTemplate(m_vtTemp1,"Temp1");
	//GetTemplate(m_vtTemp2,"Temp2");
	//GetTemplate(m_vtTemp3,"Temp3");

	m_treedata.lID = nTreeId ++;
	m_treedata.sProNo = "目录";
	m_treedata.nData = nTabIndex;
	m_treedata.sName = "1553B_ICD";
	m_treedata.vtNodeData = vec;

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

		int nProSheetNo = -1;
		CString sDataType = RowDataItem[2].c_str();
		// 只有块协议加入树节点
		if(sDataType != "ITEM")
			continue;

		vector<vector<string>> vtBlockData;
		CString sProSheet = RowDataItem[0].c_str();
		vector<CString>::iterator it_pro = find(m_protocoNameVec.begin(),m_protocoNameVec.end(),sProSheet);
		if(it_pro != m_protocoNameVec.end())
		{
			//CString sInfor = "";
			//sInfor.Format("未找到%s协议页！",sProSheet);
			//AfxMessageBox(sInfor);
			//return;
			nProSheetNo =(int)(it_pro-m_protocoNameVec.begin());

			m_config_1553.ReadInfo_Sub_Item(nProSheetNo+1,vtBlockData,1,-1);
		}
		string sRecSend = RowDataItem[3];
		int nRecSend = sRecSend == "接收" ? 1:2;

		string sBlockName = RowDataItem[1]+"-"+sRecSend;
		TreeItem.lID = nTreeId ++;
		TreeItem.nData = nProSheetNo;
		TreeItem.nRecSend = nRecSend;
		TreeItem.sName = sBlockName.c_str()/*RowDataItem[1].c_str()*/;
		TreeItem.sDataType = "ITEM";
		TreeItem.sProNo = RowDataItem[0].c_str();
		TreeItem.pParent = &m_treedata;
		TreeItem.vtNodeData = vtBlockData;
		TreeItem.subNode.clear();

		BuilSubProTree(TreeItem,nProSheetNo,nProType,nTreeId);

		m_treedata.subNode.push_back(TreeItem);
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

BOOL Pro1553B::BuilSubProTree(TREEDATA& subTreeData, int nProSheetNo,int nProType,int& nTreeId)
{
	if(nProSheetNo == -1)
		return FALSE;

	subTreeData.subNode.clear();
	vector<vector<string>>vec;
	m_config_1553.ReadInfo_Sub_Item(nProSheetNo+1,vec,1,-1);

	bool bFindStartFlag = false;
	for(int i = 0,end = vec.size(); i < end; ++ i)
	{
		if(m_bExitLoadIcd)
		{
			return FALSE;
		}

		vector<string>& RowDataItem = vec[i];
		TREEDATA TreeDataItem;

		if(!bFindStartFlag)
		{
			if(RowDataItem[1] == "信号名称")
				bFindStartFlag = true;
		}
		else if(RowDataItem[1] == "结束符")
			break;
		else
		{
			// 非ITEM项不构建子节点
			if(RowDataItem[4] != "ITEM")
				continue;

			TreeDataItem.lID = nTreeId ++;
			TreeDataItem.nProType = nProType;
			TreeDataItem.sName = RowDataItem[1].c_str();
			TreeDataItem.sDataType = "DATA";
			TreeDataItem.sProNo = RowDataItem[0].c_str();
			TreeDataItem.pParent = &subTreeData;

			vector<vector<string>> vtItemData;
			int nSubSheetNo = -1;
			vector<CString>::iterator it_pro = find(m_protocoNameVec.begin(),m_protocoNameVec.end(),TreeDataItem.sProNo);
			if(it_pro != m_protocoNameVec.end())
			{
				//CString sInfor = "";
				//sInfor.Format("未找到%s协议页！",sProSheet);
				//AfxMessageBox(sInfor);
				//return;
				nSubSheetNo =(int)(it_pro-m_protocoNameVec.begin());
				m_config_1553.ReadInfo_Sub_Item(nSubSheetNo+1,vtItemData,1,-1);
			}

			TreeDataItem.vtNodeData = vtItemData;
			TreeDataItem.nData = nSubSheetNo;
			TreeDataItem.nDlgId = subTreeData.nDlgId;
			TreeDataItem.nSysDriveId = subTreeData.nSysDriveId;
			TreeDataItem.nRecSend = subTreeData.nRecSend;
			TreeDataItem.subNode.clear();
			// 可级联构建树数据（现只取两级树）
			//BuilSubProTree(TreeDataItem,nSubSheetNo,nProType);
			subTreeData.subNode.push_back(TreeDataItem);
		}
	}

	return TRUE;
}

CString Pro1553B::GetExePath(void)  
{  
	CString strExePath;  
	GetModuleFileName(NULL, strExePath.GetBuffer(MAX_PATH), MAX_PATH);  
	strExePath.ReleaseBuffer();  
	strExePath = strExePath.Left(strExePath.ReverseFind(_T('\\')));  
	return strExePath;  
}

// 获得模板的头部和尾部数据
void Pro1553B::GetTempHeadTail(vector<vector<string>>& vtTemp,vector<vector<string>>& vtTHead,
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
		else if(vtTHead[0].size() < 4)
			bIsValid = false;
	}
	else if(nType == 2)
	{
		if(nTHeadRows < 14)
			bIsValid = false;
		else if(vtTHead[0].size() < 11)
			bIsValid = false;
		else if(nTTailRows < 2)
			bIsValid = false;
	}
	else if(nType == 3)
	{
		if(nTHeadRows < 9)
			bIsValid = false;
		else if(vtTHead[0].size() < 11)
			bIsValid = false;
		else if(nTTailRows < 2)
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
			nColNum = 4;
		}
		else if(nType == 2)
		{
			nTHeadRows = 14;
			nTTailRows = 2;
			nColNum = 11;
		}
		else if(nType == 3)
		{
			nTHeadRows = 9;
			nTTailRows = 2;
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

void Pro1553B::InitialRowData(vector<vector<string>>& vtTemp,vector<string>& vtRowData,int nType)
{
	int nTempRow = vtTemp.size();
	bool bIsValid = true;
	if(nTempRow > 0)
	{
		int nTempCol = vtTemp[0].size();
		for(int i = 0; i < nTempCol; ++ i)
			vtRowData.push_back("");
	}

	//// 检验模板的合法性
	//int nRowDataLen = vtRowData.size();
	//if(nType == 1)
	//{
	//	if(nRowDataLen < 4)
	//		bIsValid = false;
	//}
	//else if(nType == 2)
	//{
	//	if(nRowDataLen < 7)
	//		bIsValid = false;
	//}
	//else if(nType == 3)
	//{
	//	if(nRowDataLen < 10)
	//		bIsValid = false;
	//}

	//if(!bIsValid)
	//{
	//	vtRowData.clear();
	//	int nColNum = 0;
	//	if(nType == 1)
	//		nColNum = 4;
	//	else if(nType == 2)
	//		nColNum = 7;
	//	else if(nType == 3)
	//		nColNum = 10;
	//	for(int i = 0; i < nColNum; ++ i)
	//		vtRowData.push_back("");
	//}
}

BOOL Pro1553B::BuildDBTree(TREEDATA& DBTree, __SYS_Drive& DInforItem, int nProType, int& nTreeId,int& nStatus)
{
	vector<__Comm_Protocol> vtProtocol;
	CManageDb_Sqllite::GetInstance().Query_Comm_Protocol(vtProtocol,DBTree.nDlgId,DInforItem.nId);

	int nProNum = vtProtocol.size();
	if(nProNum <= 0)
		return TRUE;

	m_nDlgId = DBTree.nDlgId;
	m_nSysDriveId = DInforItem.nId;

	//// 获取协议模板
	//CString strPath = GetExePath();
	//CString strTempFile;
	//strTempFile.Format("%s\\Template\\1553B.xlsx",strPath);

	//vector<CString> vtSheetName;
	//m_config_1553.OpenFile(strTempFile);
	//m_config_1553.LoadSheetName(vtSheetName);

	//vector<vector<string>> vtTemp1;
	//vector<vector<string>> vtTemp2;
	//vector<vector<string>> vtTemp3;

	//int nTempNum = vtSheetName.size();
	//for(int i = 0; i < nTempNum; ++ i)
	//{
	//	if(vtSheetName[i] == "Temp1")
	//		m_config_1553.ReadInfo_Sub_Item(i+1,vtTemp1,1,-1);
	//	else if(vtSheetName[i] == "Temp2")
	//		m_config_1553.ReadInfo_Sub_Item(i+1,vtTemp2,1,-1);
	//	else if(vtSheetName[i] == "Temp3")
	//		m_config_1553.ReadInfo_Sub_Item(i+1,vtTemp3,1,-1);
	//}
	//m_config_1553.close_excel();

	vector<vector<string>> vtTemp1Head;
	vector<vector<string>> vtTemp1Tail;
	vector<vector<string>> vtTemp2Head;
	vector<vector<string>> vtTemp2Tail;
	vector<vector<string>> vtTemp3Head;
	vector<vector<string>> vtTemp3Tail;

	vector<string> vtRowTData1;
	vector<string> vtRowTData2;
	vector<string> vtRowTData3;

	GetTempHeadTail(m_vtTemp1,vtTemp1Head,vtTemp1Tail,1);
	GetTempHeadTail(m_vtTemp2,vtTemp2Head,vtTemp2Tail,2);
	GetTempHeadTail(m_vtTemp3,vtTemp3Head,vtTemp3Tail,3);

	InitialRowData(vtTemp1Head,vtRowTData1,1);
	InitialRowData(vtTemp2Head,vtRowTData2,2);
	InitialRowData(vtTemp3Head,vtRowTData3,3);

	TREEDATA TreeItem;
	TreeItem.lID = nTreeId ++;
	TreeItem.nProType = nProType;
	TreeItem.nData = 0;
	TreeItem.nDlgId = m_nDlgId;
	TreeItem.nSysDriveId = m_nSysDriveId;
	TreeItem.nRecSend = -1;
	TreeItem.sName = DInforItem.sName.c_str();
	TreeItem.sProNo = "目录";
	TreeItem.sDataType = "PROOT";
	TreeItem.pParent = &DBTree;
	TreeItem.vtNodeData.clear();
	TreeItem.subNode.clear();

	vector<vector<string>> vtProData1;	// 目录页数据
	vector<string> vtRowData1;

	vtProData1 = vtTemp1Head;
	vtRowData1 = vtRowTData1;
	// 从DB中获取1553B协议内容
	for(int i = 0; i < nProNum; ++ i)
	{
		if(m_bExitLoadDBIcd)
		{
			nStatus = 1;
			//m_bExitLoadDBIcd = false;
			return false;
		}

		__Comm_Protocol& ProItem = vtProtocol[i];

		TREEDATA TreeSubItem;
		TreeSubItem.lID = 0;
		TreeSubItem.nProType = nProType;
		TreeSubItem.nData = 0;
		TreeSubItem.nDlgId = DBTree.nDlgId;
		TreeSubItem.nSysDriveId = DInforItem.nId;
		TreeSubItem.nRecSend = ProItem.nRecOrSend;
		TreeSubItem.sName = "";
		TreeSubItem.sProNo = "";
		TreeSubItem.sDataType = "ITEM";
		TreeSubItem.pParent = &TreeItem;
		TreeSubItem.vtNodeData.clear();
		TreeSubItem.subNode.clear();

		vector<vector<string>> vtProData2;	// BLOCK数据
		vector<string> vtRowData2;

		// 协议号
		vtRowData1[0] = "";
		// 协议名称
		vtRowData1[1] = ProItem.sName;
		// 协议类型
		vtRowData1[2] = "ITEM";
		// 接收或发送
		vtRowData1[3] = ProItem.nRecOrSend==1?"接收":"发送";

		CMarkup xml(ProItem.sProtocol.c_str());		// 块协议的XML文件

		if(xml.FindElem(XML_Attr))
		{
			vtProData2 = vtTemp2Head;
			vtRowData2 = vtRowTData2;

			//<Attr Name="1102-大气机1攻角" 
			//	Describe="A-ADC1-LCP1-00-00" 
			//	RecSend="接收" 
			//	SendMode="周期" 
			//	RefreshTime="50" 
			//	CheckMode="" 
			//	DataUnit="BYTE" 
			//	DataLen="4" 
			//	RtAddr="1" 
			//	RtSubAddr="1" 
			//	RtAddr1="" 
			//	RtSubAddr1="" 
			//	Interrupt="N" 
			//	SrcEquipment="ADC1" 
			//	DestEquipment="LCP1" 
			//	Shared="67239936">
			TreeSubItem.lID = nTreeId ++;
			TreeSubItem.sName = ProItem.sName.c_str();
			TreeSubItem.sName += "-";
			TreeSubItem.sName += vtRowData1[3].c_str();

			CString sTxt = "";
			CString sInforName = "";
			// 协议名称
			sTxt = xml.GetAttrib(XML_Name);
			vtProData2[0][3] = sTxt.GetBuffer();
			// 描述
			CString sBProNo = xml.GetAttrib(XML_Describe);
			vtProData2[1][3] = sBProNo.GetBuffer();
			vtRowData1[0] = sBProNo.GetBuffer();
			TreeSubItem.sProNo = sBProNo;
			// 接收或发送
			sTxt = xml.GetAttrib(XML_RecSend);
			vtProData2[10][5] = sTxt.GetBuffer();
			// 发送模式
			sTxt = xml.GetAttrib(XML_SendMode);
			vtProData2[5][2] = sTxt.GetBuffer();
			// 数据块ID
			sTxt = xml.GetAttrib(XML_BlockId);
			vtProData2[8][2] = sTxt.GetBuffer();
			// 刷新时间
			sTxt = xml.GetAttrib(XML_RefreshTime);
			vtProData2[6][5] = sTxt.GetBuffer();
			// 校验方式
			sTxt = xml.GetAttrib(XML_CheckMode);
			vtProData2[11][5] = sTxt.GetBuffer();
			// 数据类型
			vtProData2[2][2] = "ITEM";
			// 数据单位
			CString sDataUnit = xml.GetAttrib(XML_DataUnit);
			vtProData2[2][5] = sDataUnit.GetBuffer();
			// 数据长度
			sTxt = xml.GetAttrib(XML_DataLen);
			vtProData2[3][5] = sTxt.GetBuffer();
			// RT地址
			sTxt = xml.GetAttrib(XML_RtAddr);
			vtProData2[9][2] = sTxt.GetBuffer();
			// RT子地址
			sTxt = xml.GetAttrib(XML_RtSubAddr);
			vtProData2[10][2] = sTxt.GetBuffer();
			// 备用RT地址
			sTxt = xml.GetAttrib(XML_RtAddr1);
			vtProData2[11][2] = sTxt.GetBuffer();
			// 备用RT子地址
			sTxt = xml.GetAttrib(XML_RtSubAddr1);
			vtProData2[12][2] = sTxt.GetBuffer();
			// 中断
			sTxt = xml.GetAttrib(XML_Interrupt);
			vtProData2[4][5] = sTxt.GetBuffer();
			// 源
			sTxt = xml.GetAttrib(XML_SourceEquipment);
			vtProData2[3][2] = sTxt.GetBuffer();
			// 目的
			sTxt = xml.GetAttrib(XML_DestEquipment);
			vtProData2[4][2] = sTxt.GetBuffer();
			// 共享内存
			sTxt = xml.GetAttrib(XML_Shared);
			vtProData2[12][5] = sTxt.GetBuffer();

			if(xml.FindChildElem("SubItems"))
			{
				string sSubInfor =xml.GetChildSubDoc();	// 块协议的所有ITEM项

				CMarkup xml2(sSubInfor.c_str());
				if(xml2.FindElem("SubItems"))
				{
					CString str = xml2.GetElemContent();
					CMarkup xml3(str);

					while(xml3.FindElem("Item"))
					{
						if(m_bExitLoadDBIcd)
						{
							nStatus = 1;
							//m_bExitLoadDBIcd = false;
							return false;
						}

						//// 数据类型
						//sTxt = xml3.GetAttrib(XML_Item_Item_type);
						//vtProData2[2][2] = sTxt.GetBuffer();
						
						// ITEM属性
						// 编号
						sTxt = xml3.GetAttrib(XML_Item_NO);
						//vtRowData2[0] = sTxt.GetBuffer();
						// 信号规范号
						sTxt = xml3.GetAttrib(XML_Item_Code);
						vtRowData2[0] = sTxt.GetBuffer();
						// 信号名称
						sTxt = xml3.GetAttrib(XML_Name);
						vtRowData2[1] = sTxt.GetBuffer();
						//vtRowData2[2] = sInforName.GetBuffer();
						// 起始地址
						sTxt = xml3.GetAttrib(XML_Item_Addr);
						vtRowData2[2] = sTxt.GetBuffer();
						// 长度
						sTxt = xml3.GetAttrib(XML_Item_Length);
						vtRowData2[3] = sTxt.GetBuffer();
						// 协议项类型
						sTxt = xml3.GetAttrib(XML_Item_Item_type);
						vtRowData2[4] = sTxt.GetBuffer();
						// 数据类型
						sTxt = xml3.GetAttrib(XML_Item_Data_type);
						vtRowData2[5] = sTxt.GetBuffer();
						// 表达式
						sTxt = xml3.GetAttrib(XML_Item_Express);
						vtRowData2[6] = sTxt.GetBuffer();
						// 转义值
						sTxt = xml3.GetAttrib(XML_Item_Transfer);
						vtRowData2[7] = sTxt.GetBuffer();
						// 默认值
						sTxt = xml3.GetAttrib(XML_Item_Default);
						vtRowData2[8] = sTxt.GetBuffer();
						// 备注
						sTxt = xml3.GetAttrib(XML_Item_Mark);
						vtRowData2[9] = sTxt.GetBuffer();
						// 共享内存
						sTxt = xml3.GetAttrib(XML_Shared);
						vtRowData2[10] = sTxt.GetBuffer();

						GetSubAttr(xml3,TreeSubItem,vtTemp3Head,vtTemp3Tail,vtRowTData3,sDataUnit,nTreeId,nProType);

						vtProData2.push_back(vtRowData2);
					}
					// 添加尾部模板
					AttachTailTemp(vtProData2,vtTemp2Tail);

					TreeSubItem.vtNodeData = vtProData2;
					TreeItem.subNode.push_back(TreeSubItem);
				}
			}
		}
		vtProData1.push_back(vtRowData1);
	}

	// 清除树中对应节点以前的数据
	vector<vector<string>>::iterator it_tb = DBTree.vtNodeData.begin();
	for(; it_tb != DBTree.vtNodeData.end(); ++ it_tb)
	{
		vector<string>& vtTbData = *it_tb;
		if(vtTbData[1] == DInforItem.sName)
		{
			DBTree.vtNodeData.erase(it_tb);
			break;
		}
	}

	// 删除同名节点
	vector<TREEDATA>::iterator it_pt = DBTree.subNode.begin();
	for(; it_pt != DBTree.subNode.end(); ++ it_pt)
	{
		// 删除同名节点
		if(it_pt->sName == TreeItem.sName)
		{
			DBTree.subNode.erase(it_pt);
			break;
		}
	}

	vector<string> vtParentNode;
	vtParentNode.push_back(DBTree.sName.GetBuffer());
	vtParentNode.push_back(DInforItem.sName);
	DBTree.vtNodeData.push_back(vtParentNode);

	TreeItem.vtNodeData = vtProData1;
	DBTree.subNode.push_back(TreeItem);

	return TRUE;
}

void Pro1553B::GetSubAttr(CMarkup& xml,TREEDATA& TreeSubItem,
	vector<vector<string>>& vtTempHead, vector<vector<string>>& vtTempTail,
	vector<string>& vtRowTData,CString sTDataUnit, int& nTreeId, int nProType)
{
	vector<vector<string>> vtProData3;	// ITEM数据
	vector<string> vtRowData3;
	CString sTxt = "";
	CString sProName = "";
	CString sProNo = "";
	CString sDataUnit = "";
	CString sDataLen = "";

	vtProData3 = vtTempHead;
	// 协议类型（非ITEM则不构建子节点）
	CString sItemType = xml.GetAttrib(XML_Item_Item_type);
		if(sItemType != "ITEM")
			return;

	// 协议名称
	sProName = xml.GetAttrib(XML_Name);
	vtProData3[0][3] = sProName.GetBuffer();

	if(xml.FindChildElem("SubItems"))
	{
		// 规范号
		sProNo = xml.GetAttrib(XML_Item_Code);
		vtProData3[1][3] = sProNo.GetBuffer();
		// 数据单位
		sDataUnit = xml.GetAttrib(XML_DataUnit);
		vtProData3[6][2] = sDataUnit.GetBuffer();
		//// 数据类型
		//vtProData3[2][2] = sItemType;
		// 数据长度
		sDataLen = xml.GetAttrib(XML_DataLen);
		vtProData3[7][2] = sDataLen.GetBuffer();

		string sSubInfor =xml.GetChildSubDoc();	// 块协议的所有ITEM项

		CMarkup xml2(sSubInfor.c_str());
		if(xml2.FindElem("SubItems"))
		{
			CString str = xml2.GetElemContent();
			CMarkup xml3(str);

			while(xml3.FindElem("Item"))
			{
				if(m_bExitLoadDBIcd)
				{
					return;
				}

				vtRowData3 = vtRowTData;

				// 数据类型
				sTxt = xml3.GetAttrib(XML_Item_Item_type);
				vtProData3[2][2] = sTxt.GetBuffer();

				// DATA属性
				// 起始地址
				sTxt = xml3.GetAttrib(XML_Item_Addr);
				vtRowData3[1] = sTxt.GetBuffer();
				// 长度
				sTxt = xml3.GetAttrib(XML_Item_Length);
				vtRowData3[2] = sTxt.GetBuffer();
				// 信号名称
				sTxt = xml3.GetAttrib(XML_Name);
				vtRowData3[3] = sTxt.GetBuffer();
				// 协议项类型
				sTxt = xml3.GetAttrib(XML_Item_Item_type);
				vtRowData3[4] = sTxt.GetBuffer();
				// 存储类型
				sTxt = xml3.GetAttrib(XML_Item_Data_type);
				vtRowData3[5] = sTxt.GetBuffer();
				// 表达式
				sTxt = xml3.GetAttrib(XML_Item_Express);
				vtRowData3[6] = sTxt.GetBuffer();
				// 转义值
				sTxt = xml3.GetAttrib(XML_Item_Transfer);
				vtRowData3[7] = sTxt.GetBuffer();
				// 默认值
				sTxt = xml3.GetAttrib(XML_Item_Default);
				vtRowData3[8] = sTxt.GetBuffer();
				// 备注
				sTxt = xml3.GetAttrib(XML_Item_Mark);
				vtRowData3[9] = sTxt.GetBuffer();
				// 共享内存ID
				sTxt = xml3.GetAttrib(XML_Shared);
				vtRowData3[10] = sTxt.GetBuffer();

				vtProData3.push_back(vtRowData3);
			}
		}
	}
	// 兼容老协议，将DATA项展开成ITEM项，该ITEM中只包含DATA项
	else
	{
		vtRowData3 = vtRowTData;
		// 规范号
		vtProData3[1][3] = "";
		// 数据单位
		vtProData3[6][2] = sTDataUnit.GetBuffer();
		// 数据长度
		sDataLen = xml.GetAttrib(XML_Item_Length);
		vtProData3[7][2] = sDataLen.GetBuffer();
		// 数据类型
		sTxt = xml.GetAttrib(XML_Item_Item_type);
		vtProData3[2][2] = sTxt.GetBuffer();

		// DATA属性
		// 起始地址
		sTxt = xml.GetAttrib(XML_Item_Addr);
		vtRowData3[1] = sTxt.GetBuffer();
		// 长度
		vtRowData3[2] = sDataLen.GetBuffer();
		// 信号名称
		vtRowData3[3] = sProName.GetBuffer();
		// 协议项类型
		sTxt = xml.GetAttrib(XML_Item_Item_type);
		vtRowData3[4] = sTxt.GetBuffer();
		// 存储类型
		sTxt = xml.GetAttrib(XML_Item_Data_type);
		vtRowData3[5] = sTxt.GetBuffer();
		// 表达式
		vtRowData3[6] = "";
		// 转义值
		vtRowData3[7] = "";
		// 默认值
		sTxt = xml.GetAttrib(XML_Item_Default);
		vtRowData3[8] = sTxt.GetBuffer();
		// 备注
		sTxt = xml.GetAttrib(XML_Item_Mark);
		vtRowData3[9] = sTxt.GetBuffer();
		// 共享内存ID
		sTxt = xml.GetAttrib(XML_Shared);
		vtRowData3[10] = sTxt.GetBuffer();

		vtProData3.push_back(vtRowData3);
	}

	AttachTailTemp(vtProData3,vtTempTail);

	TREEDATA TreeItem;
	TreeItem.lID = nTreeId ++;
	TreeItem.nProType = nProType;
	TreeItem.nData = 0;
	TreeItem.nDlgId = m_nDlgId;
	TreeItem.nSysDriveId = m_nSysDriveId;
	TreeItem.nRecSend = TreeSubItem.nRecSend;
	TreeItem.sName = sProName;
	TreeItem.sProNo = sProNo;
	TreeItem.sDataType = "DATA";
	TreeItem.pParent = &TreeSubItem;
	TreeItem.vtNodeData = vtProData3;
	TreeItem.subNode.clear();

	TreeSubItem.subNode.push_back(TreeItem);
}

void Pro1553B::AttachTailTemp(vector<vector<string>>& vtProData, vector<vector<string>>& vtTempTail)
{
	int nTailRow = vtTempTail.size();

	for(int i = 0; i < nTailRow; ++ i)
	{
		vtProData.push_back(vtTempTail[i]);
	}
}

void Pro1553B::DeleteBLOCKFromDB(TREEDATA& TreeNode)
{
	if(TreeNode.sDataType != "ITEM")
		return;

	CString sCondition = "";
	CString sName = TreeNode.vtNodeData[0][3].c_str();

	// 删除该协议下的所有数据结点
	int nSubNodeNum = TreeNode.subNode.size();
	CString sTxt = "";
	CString sGroupId = "";
	CString sSubName = "";

	// 删除本节点下的所有数据
	CString sItemType = "";
	vector<vector<string>>& vtNodeData = TreeNode.vtNodeData;
	int nNDRowNum = vtNodeData.size();
	bool bFindNodeStart = false;
	int j = 0;
	int k = 0;
	for(int i = 0; i < nNDRowNum; ++ i)
	{
		if(m_bExitDeleteDB)
			return;

		vector<string>& vtNodeRow = vtNodeData[i];
		if(bFindNodeStart)
		{
			sTxt = vtNodeRow[1].c_str();
			if(sTxt == "结束符")
				break;

			sGroupId.Format("%d",j ++);
			sItemType = vtNodeRow[4].c_str();
			if(sItemType == "ITEM")
			{
				if(k < nSubNodeNum)
				{
					TREEDATA& subTreeNode = TreeNode.subNode[k ++];
					vector<vector<string>>& vtItemData = subTreeNode.vtNodeData;
					int nRowNum = vtItemData.size();
					BOOL bFindStart = FALSE;
					for(int m = 0; m < nRowNum; ++ m)
					{
						if(m_bExitDeleteDB)
							return;

						vector<string>& vtSubItem = vtItemData[m];
						if(bFindStart)
						{
							sTxt = vtSubItem[3].c_str();
							if(sTxt == "结束符")
								break;

							if(!sGroupId.IsEmpty())
								sSubName = sName+"."+sGroupId+"."+sTxt;
							else
								sSubName = sName+"."+sTxt;

							sCondition.Format("nDlgId=%d and nSysDriveId=%d and sName='%s' and nInOut=%d",
								TreeNode.nDlgId,TreeNode.nSysDriveId,sSubName,TreeNode.nRecSend);
							CManageDb_Sqllite::GetInstance().Delete_DataConfig(sCondition);
						}
						else
						{
							sTxt = vtSubItem[3].c_str();
							if(!sTxt.Compare("信号名称"))
								bFindStart = true;
						}
					}
				}
			}
			else if(sItemType == "DATA" || sItemType == "SBID")
			{
				if(!sGroupId.IsEmpty())
					sSubName = sName+"."+sGroupId+"."+sTxt;
				else
					sSubName = sName+"."+sTxt;

				sCondition.Format("nDlgId=%d and nSysDriveId=%d and sName='%s' and nInOut=%d",
					TreeNode.nDlgId,TreeNode.nSysDriveId,sSubName,TreeNode.nRecSend);
				CManageDb_Sqllite::GetInstance().Delete_DataConfig(sCondition);
			}
		}
		else
		{
			sTxt = vtNodeRow[1].c_str();
			if(!sTxt.Compare("信号名称"))
				bFindNodeStart = true;
		}
	}

	// 删除t_comm_protocol中的协议
	sCondition.Format("nDlgId=%d and nSysDriveId=%d and sName='%s' and nType=%d",
		TreeNode.nDlgId,TreeNode.nSysDriveId,sName,TreeNode.nRecSend);
	CManageDb_Sqllite::GetInstance().Delete_Comm_Protocel(sCondition);

	// 删除协议块的共享内存记录
	sCondition.Format("nDlgId=%d and nSysDriveId=%d and sName='%s' and nInOut=%d",
		TreeNode.nDlgId,TreeNode.nSysDriveId,sName,TreeNode.nRecSend);
	CManageDb_Sqllite::GetInstance().Delete_DataConfig(sCondition);
}

void Pro1553B::SaveDataToDB(TREEDATA& TreeData, int nCurDrive)
{
	if(m_nDriveIndex < 0) return;
	if(m_DriveVec.size() <= m_nDriveIndex)
	{
		//AfxMessageBox("没有获得驱动板卡的信息！");
		return;
	}

	m_nDlgId = TreeData.nDlgId;
	m_nDriveId = m_DriveVec[m_nDriveIndex].nDriveId;
	m_nSysDriveId =m_DriveVec[m_nDriveIndex].nId;

	//对应t_data_configID、nSharedId
	int nProtocol_ConfigId = 0;
	int nProtocol_SharedId = 0;

	int nItem_ConfigId;
	int nItem_SharedId;

	// 获取目录（协议）页数据
	vector<vector<string>>& vec = TreeData.vtNodeData;
	//m_config_1553.ReadInfo_Sub_Item(TreeData.nData+1,vec,1,-1);

	int nRow =1;
	for(int i = nRow,end = vec.size(); i < end /*&& false*/; ++ i)
	{
		if(m_bExitSaveToDB)
		{
			//m_bExitSaveToDB = FALSE;
			return;
		}

		vector<string>& vtRowData = vec[i];
		//// 判断树节点是否存在该节点数据
		//bool bFindNode = false;
		//for(int j = 0,nodenum = TreeData.subNode.size(); j < nodenum; ++ j)
		//{
		//	TREEDATA& TNodeItem = TreeData.subNode[j];
		//	if(TNodeItem.sProNo == vtRowData[0].c_str())
		//	{
		//		bFindNode = true;
		//		break;
		//	}
		//}
		//if(!bFindNode)
		//	continue;

		// 协议数据块存入数据库
		//规范号
		CString sProtocolNo = vtRowData[0].c_str();
		//协议名称
		CString sProtocolName = vtRowData[1].c_str();
		//数据块类型
		CString sBlockType = vtRowData[2].c_str();
		//接收/发送
		CString  sProtocolRecSend= vtRowData[3].c_str();

		CString strXml = "";
		BOOL bFindBlockProtocol = FALSE;

		if(sBlockType == "ITEM" /*&& false*/)
		{
			//vector<CString>::iterator it = find(m_protocoNameVec.begin(),m_protocoNameVec.end(),sProtocolNo);
			//if(it == m_protocoNameVec.end())
			//	continue;
			//int nTabIndex =(int)(it-m_protocoNameVec.begin());

			vector<vector<string>>& vtItem = TreeData.subNode[i-1].vtNodeData;
			//m_config_1553.ReadInfo_Sub_Item(nTabIndex+1,vtItem,1,-1);

			if(vtItem.size()==0)
				continue;

			//解析协议属性
			CMarkup xml;

			//协议名称;名称
			xml.AddElem(XML_Attr);	

			//名称
			xml.AddAttrib(XML_Name,sProtocolName);
			//描述（协议号）
			CString sTxt = vtItem[1][3].c_str();
			xml.AddAttrib(XML_Describe,sTxt);
			//接收/发送--取主目录的属性
			xml.AddAttrib(XML_RecSend,sProtocolRecSend);
			//发送模式
			sTxt = vtItem[5][2].c_str();
			xml.AddAttrib(XML_SendMode,sTxt);
			//数据块ID
			sTxt = vtItem[8][2].c_str();
			xml.AddAttrib(XML_BlockId,sTxt);
			//刷新时间
			sTxt = vtItem[6][5].c_str();
			sTxt.Trim("ms");// 去掉ms
			xml.AddAttrib(XML_RefreshTime,sTxt);
			//校验方式
			sTxt = vtItem[11][5].c_str();
			xml.AddAttrib(XML_CheckMode,sTxt);
			//数据单位
			CString sDataUnit = vtItem[2][5].c_str();
			xml.AddAttrib(XML_DataUnit,sDataUnit);
			//数据长度
			sTxt = vtItem[3][5].c_str();
			xml.AddAttrib(XML_DataLen,sTxt);
			int nDataLen = _tstoi(sTxt);
			//RT地址
			sTxt = vtItem[9][2].c_str();
			xml.AddAttrib(XML_RtAddr,sTxt);
			//RT子地址
			sTxt = vtItem[10][2].c_str();
			xml.AddAttrib(XML_RtSubAddr,sTxt);
			//RT地址（备用）
			sTxt = vtItem[11][2].c_str();
			xml.AddAttrib(XML_RtAddr1,sTxt);
			//RT子地址（备用）
			sTxt = vtItem[12][2].c_str();
			xml.AddAttrib(XML_RtSubAddr1,sTxt);
			//中断
			sTxt = vtItem[4][5].c_str();
			if(sTxt == "否")
				sTxt = "N";
			xml.AddAttrib(XML_Interrupt,sTxt);
			//源
			sTxt = vtItem[3][2].c_str();
			xml.AddAttrib(XML_SourceEquipment,sTxt);
			//目标
			sTxt = vtItem[4][2].c_str();
			xml.AddAttrib(XML_DestEquipment,sTxt);
			//共享内存值
			bFindBlockProtocol = XProtocol_Funtion::GetProtocolId(m_nDlgId,m_nSysDriveId,sProtocolName,sProtocolRecSend,m_nProtocolId);
			//创建对应协议的共享内存
			XProtocol_Funtion::Save_Protocl_ShareDataInfor(m_nDlgId,sProtocolName,m_nProtocolId,m_nDriveId,m_nSysDriveId,sProtocolRecSend,nDataLen,nProtocol_ConfigId,nProtocol_SharedId,9);

			//nConfigId = GetNewConfigDatId();
			xml.AddAttrib(XML_Shared,nProtocol_SharedId);

			// 协议数据项都从1开始编号
			m_nProtoclItem_Index = 1;
			//协议子项
			CMarkup xmlItem;
			xmlItem.AddElem(XML_SubItems);

			int nIdx = 0;
			int nItemNo = 0;
			int nBRow = vtItem.size();
			bool bFindStart = false;
			for(int j = 0; j < nBRow; ++ j)
			{
				if(m_bExitSaveToDB)
				{
					//m_bExitSaveToDB = FALSE;
					return;
				}

				vector<string>& vtSubItem = vtItem[j];
				int nEndSubItemNum = vtSubItem.size();
				if(nEndSubItemNum <= 0) continue;

				if(bFindStart)
				{
					sTxt = vtSubItem[1].c_str();
					if(sTxt == "结束符")
						break;

					CMarkup xmlChild;
					xmlChild.AddElem(XML_Item_Item);

					//编号
					CString sItemNo = ""/* = vtSubItem[0].c_str()*/;
					sItemNo.Format("%d",nItemNo ++);
					xmlChild.AddAttrib(XML_Item_NO,sItemNo);
					//块代码（规范号）
					CString sProNo = vtSubItem[0].c_str();
					xmlChild.AddAttrib(XML_Item_Code,sProNo);
					//信号名称
					CString sItemName = vtSubItem[1].c_str();
					xmlChild.AddAttrib(XML_Name,sItemName);
					//位起始地址
					sTxt = vtSubItem[2].c_str();
					xmlChild.AddAttrib(XML_Item_Addr,sTxt);
					//长度
					CString sItemLength = vtSubItem[3].c_str();
					xmlChild.AddAttrib(XML_Item_Length,sItemLength);
					//块类型
					//sTxt = vtItem[2][2].c_str();
					CString sItemType = vtSubItem[4].c_str();
					xmlChild.AddAttrib(XML_Item_Item_type,sItemType);
					//数据类型
					//sTxt = "整型";
					CString sDataType = vtSubItem[5].c_str();
					xmlChild.AddAttrib(XML_Item_Data_type,sDataType);
					// ITEM 项不加入表达式及转义值属性
					if(sItemType != "ITEM")
					{
						//表达式
						sTxt = vtSubItem[6].c_str();
						sTxt.MakeLower();
						sTxt.Trim();
						xmlChild.AddAttrib(XML_Item_Express,sTxt);
						//转义值
						sTxt = vtSubItem[7].c_str();
						xmlChild.AddAttrib(XML_Item_Transfer,sTxt);
					}
					//默认值
					//sTxt = "";
					CString sDefault = vtSubItem[8].c_str();
					xmlChild.AddAttrib(XML_Item_Default,sDefault);
					//备注
					CString sMark = vtSubItem[9].c_str();
					xmlChild.AddAttrib(XML_Item_Mark,sMark);

					if(sItemType == "ITEM")
					{
						// 获得数据项属性
						GetDataAttr(xmlChild,TreeData.subNode[i-1],nIdx,sProtocolRecSend,sItemNo,sItemName,sProtocolName,m_nProtocolId);
						++ nIdx;
					}
					else if((sItemType == "DATA" || sItemType == "SBID") && sItemName != "")
					{
						//构建共享内存ID
						int nItemLen = _tstoi(sItemLength);
						if(sDataUnit == "BIT")
							nItemLen = nItemLen/8+1;
						else if(sDataUnit == "WORD")
							nItemLen *= 2;
						else if(sDataUnit == "DWORD")
							nItemLen *= 4;
						XProtocol_Funtion::Save_ProtoclData_toConfigData(m_nDlgId,sProtocolRecSend,sProtocolName+"."+sItemNo+"."+sItemName,
							sDataType,nItemLen,m_nProtocolId,m_nProtoclItem_Index,
							m_nDriveId,m_nSysDriveId,sDefault,1.0,0.0,sMark,
							nItem_ConfigId,nItem_SharedId,9);

						++ m_nProtoclItem_Index;

						sTxt.Format("%d",nItem_SharedId);
						xmlChild.AddAttrib(XML_Shared,sTxt);
					}

					xmlItem.AddChildSubDoc(xmlChild.GetSubDoc());
				}
				else
				{
					sTxt = vtSubItem[1].c_str();
					if(!sTxt.Compare("信号名称"))
						bFindStart = true;
				}
			}

			xml.AddChildSubDoc(xmlItem.GetSubDoc());

			strXml = xml.GetSubDoc();

			XProtocol_Funtion::SaveProtocol(
				m_nDlgId,
				m_nProtocolId,
				sProtocolRecSend,
				sProtocolName,
				m_nSysDriveId,
				strXml,
				bFindBlockProtocol);

			// 进度条回调
			m_func(m_lpParam);
		}
	}
}

void Pro1553B::GetDataAttr(CMarkup& hxml, TREEDATA& TreeData, int nIdx, CString sRecSend,
	CString sItemNo, CString sItemName, CString sProtocoName, int nProtocolId)
{
	//vector<CString>::iterator it = find(m_protocoNameVec.begin(),m_protocoNameVec.end(),sItemCode);
	//if(it == m_protocoNameVec.end())
	//	return;
	//int nTabIndex =(int)(it-m_protocoNameVec.begin());

	vector<vector<string>>& vec = TreeData.subNode[nIdx].vtNodeData;
	//m_config_1553.ReadInfo_Sub_Item(nTabIndex+1,vec,1,-1,10);

	if(vec.size()==0)
		return;

	//解析协议属性
	//数据单位
	CString sTxt = vec[6][2].c_str();
	hxml.AddAttrib(XML_DataUnit,sTxt);
	//数据长度
	sTxt = vec[7][2].c_str();
	hxml.AddAttrib(XML_DataLen,sTxt);

	int nItem_ConfigId;
	int nItem_SharedId;

	//// debug
	//bool bTstFind = false;
	//if(sItemCode == "ADC_013")
	//{
	//	bTstFind = true;
	//}

	CMarkup xmlChild;
	xmlChild.AddElem(XML_SubItems);

	CString sSubItemNo = "";
	int nSubItemNo = 0;
	int nBRow = vec.size();
	bool bFindStart = false;
	for(int j = 0; j < nBRow; ++ j)
	{
		if(m_bExitSaveToDB)
		{
			return;
		}

		vector<string>& vtSubItem = vec[j];
		int nEndSubItemNum = vtSubItem.size();
		if(nEndSubItemNum <= 0) continue;

		if(bFindStart)
		{
			sTxt = vtSubItem[3].c_str();
			if(sTxt == "结束符")
				break;

			CMarkup xmlSubChild;
			xmlSubChild.AddElem(XML_Item_Item);

			//编号
			sSubItemNo.Format("%d",nSubItemNo ++);
			xmlSubChild.AddAttrib(XML_Item_NO,sSubItemNo);
			//起始地址
			sTxt = vtSubItem[1].c_str();
			xmlSubChild.AddAttrib(XML_Item_Addr,sTxt);
			//位长
			sTxt = vtSubItem[2].c_str();
			xmlSubChild.AddAttrib(XML_Item_Length,sTxt);
			int nSubInforLength = int(_tstoi(sTxt)/8.0+1);
			//块类型
			sTxt = vec[2][2].c_str();
			xmlSubChild.AddAttrib(XML_Item_Item_type,sTxt);
			//协议项类型
			sTxt = vtSubItem[4].c_str();
			xmlSubChild.AddAttrib(XML_Item_Item_type,sTxt);
			//数据类型
			CString sDataType = vtSubItem[5].c_str();
			xmlSubChild.AddAttrib(XML_Item_Data_type,sDataType);
			//信号名称
			CString sSubItemName = vtSubItem[3].c_str();
			xmlSubChild.AddAttrib(XML_Name,sSubItemName);
			//表达式
			sTxt = vtSubItem[6].c_str();
			sTxt.MakeLower();
			sTxt.Trim();
			xmlSubChild.AddAttrib(XML_Item_Express,sTxt);
			//转义值
			sTxt = vtSubItem[7].c_str();
			xmlSubChild.AddAttrib(XML_Item_Transfer,sTxt);
			//默认值
			CString sDefaultValue = vtSubItem[8].c_str();
			xmlSubChild.AddAttrib(XML_Item_Default,sDefaultValue);
			//备注
			CString sMark = vtSubItem[9].c_str();
			xmlSubChild.AddAttrib(XML_Item_Mark,sMark);
			//构建共享内存ID
			//只对信号名称不为空的记录构建共享内存ID
			if(sItemName != "")
			{
				XProtocol_Funtion::Save_ProtoclData_toConfigData(m_nDlgId,sRecSend,sProtocoName+"."+sItemNo+"."+sItemName+"."+sSubItemNo+"."+sSubItemName,
					sDataType,nSubInforLength,nProtocolId,m_nProtoclItem_Index,
					m_nDriveId,m_nSysDriveId,sDefaultValue,1.0,0.0,sMark,
					nItem_ConfigId,nItem_SharedId,9);

				++ m_nProtoclItem_Index;

				sTxt.Format("%d",nItem_SharedId);
				xmlSubChild.AddAttrib(XML_Shared,sTxt);
			}

			xmlChild.AddChildSubDoc(xmlSubChild.GetSubDoc());
		}
		else
		{
			sTxt = vtSubItem[3].c_str();
			if(!sTxt.Compare("信号名称"))
				bFindStart = true;
		}
	}

	hxml.AddChildSubDoc(xmlChild.GetSubDoc());
}

// 1553B协议子项存入共享内存数据库
void Pro1553B::SaveProtocolItemConfig(void)
{
	int nItem_ConfigId;
	int nItem_SharedId;

	vector<__Comm_Protocol> vec;
	CManageDb_Sqllite::GetInstance().Query_1553B_All_Protocol(vec);

	int nProNum = vec.size();
	for(int i = 0; i < nProNum; ++ i)
	{
		__Comm_Protocol& ProItem = vec[i];

		CString sProtocoName = ProItem.sName.c_str();
		// 块协议处理
		if(ProItem.sBlockType == "ITEM")
		{
			m_nProtoclItem_Index = 1;		// 协议字项都从1开始编号

			CMarkup xml(ProItem.sProtocol.c_str());

			if(xml.FindChildElem("SubItems"))
			{
				string sSubInfor =xml.GetChildSubDoc();

				CMarkup xml2(sSubInfor.c_str());
				if(!xml2.FindElem("SubItems"))
					continue;
				CString str = xml2.GetElemContent();
				CMarkup xml3(str);

				CString sDispName;
				CString sDispValue;
				CString sDispStyle;
				while(xml3.FindElem("Item"))
				{	
					//显示的信号名称
					sDispName = xml3.GetAttrib(XML_DispName);
					sDispValue = xml3.GetAttrib(XML_DispValue);
					sDispStyle = xml3.GetAttrib(XML_DispStyle);

					CString sRecSend = ProItem.nRecOrSend == 1 ? "接收":"发送";

					if(sDispStyle == "横向")
					{
						vector<string> vtContent;
						string strContent = CT2A(sDispValue.GetBuffer());
						SplitString(strContent,vtContent,";");
						int nDispColNum = vtContent.size();
						for(int j = 0; j < nDispColNum; ++ j)
						{
							for(int k = 0; k < nProNum; ++ k)
							{
								__Comm_Protocol& SubProItem = vec[k];
								string strProNo = vtContent[j];
								if(strProNo == SubProItem.sProtocolNo)
								{
									CMarkup xml4(SubProItem.sProtocol.c_str());
									if(!xml4.FindElem("SubItems"))
										continue;
									CString strSubItem = xml4.GetElemContent();
									CMarkup xml5(strSubItem);

									CString sItemDispLastValue = "";
									CString sItemDispName = "";
									int nSubInforLength = 0;
									CString sDefaultValue = "";
									float fCoefficient = 1.0;
									float fOffset = 0.0;
									CString sMark = "";
									while(xml5.FindElem("Item"))
									{
										bool bWriteDB = false;

										CString sItemDispStyle = xml5.GetAttrib(XML_Item_DispStyle);
										if(sItemDispStyle == "7")
										{
											CString sItemDispValue = xml5.GetAttrib(XML_Item_DispValue);
											if(sItemDispValue != sItemDispLastValue)
											{
												sItemDispLastValue = sItemDispValue;
												//sItemDispName = xml5.GetAttrib(XML_Item_DispName);
												sItemDispName = sItemDispValue;
												nSubInforLength = _tstoi(xml5.GetAttrib(XML_Item_Length));
												//sDefaultValue = "";
												fCoefficient = _tstof(xml5.GetAttrib(XML_Item_Coefficient));
												fOffset = _tstof(xml5.GetAttrib(XML_Item_Offset));
												sMark = xml5.GetAttrib(XML_Item_Mark);
												bWriteDB = true;
											}
										}
										else
										{
											sItemDispName = xml5.GetAttrib(XML_Item_DispName);
											nSubInforLength = _tstoi(xml5.GetAttrib(XML_Item_Length));
											//sDefaultValue = "";
											fCoefficient = _tstof(xml5.GetAttrib(XML_Item_Coefficient));
											fOffset = _tstof(xml5.GetAttrib(XML_Item_Offset));
											sMark = xml5.GetAttrib(XML_Item_Mark);
											bWriteDB = true;
										}

										if(bWriteDB)
										{
											XProtocol_Funtion::Save_ProtoclItem_toConfigData(m_nPulicProId/*m_nDlgId*/,sRecSend,sProtocoName+"."+sItemDispName+"."+sRecSend,DATA_TYPE_INT_NAME,nSubInforLength/8,ProItem.nId,m_nProtoclItem_Index,
												m_nDriveId,m_nSysDriveId,sDefaultValue,fCoefficient,fOffset,sMark,
												nItem_ConfigId,nItem_SharedId);
											m_nProtoclItem_Index++;
										}
									}

									break;
								}
							}
						}
					}
					else
					{
						string strProNo = CT2A(sDispValue.GetBuffer());
						for(int k = 0; k < nProNum; ++ k)
						{
							__Comm_Protocol& SubProItem = vec[k];
							if(strProNo == SubProItem.sProtocolNo)
							{
								CMarkup xml4(SubProItem.sProtocol.c_str());
								if(!xml4.FindElem("SubItems"))
									continue;
								CString strSubItem = xml4.GetElemContent();
								CMarkup xml5(strSubItem);
								while(xml5.FindElem("Item"))
								{
									CString sItemDispName = xml5.GetAttrib(XML_Item_DispName);
									int nSubInforLength = _tstoi(xml5.GetAttrib(XML_Item_Length));
									CString sDefaultValue = "";
									float fCoefficient = _tstof(xml5.GetAttrib(XML_Item_Coefficient));
									float fOffset = _tstof(xml5.GetAttrib(XML_Item_Offset));
									CString sMark = xml5.GetAttrib(XML_Item_Mark);

									XProtocol_Funtion::Save_ProtoclItem_toConfigData(m_nPulicProId/*m_nDlgId*/,sRecSend,sProtocoName+"."+sItemDispName+"."+sRecSend,DATA_TYPE_INT_NAME,nSubInforLength/8,ProItem.nId,m_nProtoclItem_Index,
										m_nDriveId,m_nSysDriveId,sDefaultValue,fCoefficient,fOffset,sMark,
										nItem_ConfigId,nItem_SharedId);
									m_nProtoclItem_Index++;
								}

								break;
							}
						}
					}
				}
			}  
		}
	}
}

void Pro1553B::SetProgressFunc(CallFunc func,LPVOID lpParam)
{
	m_lpParam = lpParam;
	m_func = func;
}

