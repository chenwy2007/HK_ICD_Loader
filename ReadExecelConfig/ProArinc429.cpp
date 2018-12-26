// ProArinc429.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "ProArinc429.h"
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
	int nStartPosi;				//��ʼλ
	int nItemLen;				//λ��
	string sItemName;			//��ʾ����
	int nInforType;				//�ź�����
	float fOffset;				//ƫ�ò���
	float fCoef;				//ϵ������
	string sDispValue;			//��ʾֵ��ת���ã�
	string sRemark;				//ע��
}SUBITEMATTR,*pSUBITEMATTR;
// ProArinc429


ProArinc429::ProArinc429()
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

ProArinc429::~ProArinc429()
{

}

// ProArinc429
void ProArinc429::LoadIcdFile(CString strFilePath)
{
	m_protocoNameVec.clear();

	//m_excelFile.OpenFile(strFilePath,m_protocoNameVec);

	m_excelFile.OpenFile(strFilePath);

	m_excelFile.LoadSheetName(m_protocoNameVec);
}

void ProArinc429::ReadSheetData(int nCurSheetNo,vector<vector<string>>& vtSheetData)
{
	if(nCurSheetNo ==-1)
		return;

	vtSheetData.clear() ;
	CString str;

	m_nGirdColCount =8;

	m_excelFile.ReadInfo_Sub_Item(nCurSheetNo+1,vtSheetData,1,-1,m_nGirdColCount);	 
}

void ProArinc429::GetTempData(vector<vector<string>>& vtTemp, CString csSheetNo)
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

void ProArinc429::Initailize(void)
{
	// ��ȡЭ��ģ��
	CString strPath = GetExePath();
	CString strTempFile;
	strTempFile.Format("%s\\Template\\429_DISP.xlsx",strPath);

	vector<CString> vtSheetName;
	m_excelFile.OpenFile(strTempFile);
	m_excelFile.LoadSheetName(vtSheetName);

	vector<vector<string>> vtTemp1;
	vector<vector<string>> vtTemp2;
	vector<vector<string>> vtTemp3;

	int nTempNum = vtSheetName.size();
	for(int i = 0; i < nTempNum; ++ i)
	{
		if(vtSheetName[i] == "Ŀ¼")
			m_excelFile.ReadInfo_Sub_Item(i+1,m_vtTemp1,1,-1);
		else if(vtSheetName[i] == "ITEM")
			m_excelFile.ReadInfo_Sub_Item(i+1,m_vtTemp2,1,-1,11);
		else if(vtSheetName[i] == "DATA")
			m_excelFile.ReadInfo_Sub_Item(i+1,m_vtTemp3,1,-1);
	}
	m_excelFile.close_excel();
}

void ProArinc429::GetTemplate(vector<vector<string>>& vtTemp, CString csSheetNo)
{
	vector<CString>::iterator it = find(m_protocoNameVec.begin(),m_protocoNameVec.end(),csSheetNo);
	if(it == m_protocoNameVec.end())
	{
		return;
	}
	int nTabIndex =(int)(it-m_protocoNameVec.begin());

	m_excelFile.ReadInfo_Sub_Item(nTabIndex+1,vtTemp,1,-1);
}

TREEDATA& ProArinc429::BuildProtocolTree(TREEDATA& ParentTree,int nProType,int nDlgId,int& nTreeId)
{
	m_nSysDriveId = -1;
	if(-1 < m_nDriveIndex && m_nDriveIndex < m_DriveVec.size())
		m_nSysDriveId = m_DriveVec[m_nDriveIndex].nId;
	m_nDlgId = nDlgId;

	// ���Ŀ¼ҳ���ݣ���ҳ��¼���е�Э��飩
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

	vector<CString>::iterator it = find(m_protocoNameVec.begin(),m_protocoNameVec.end(),"Ŀ¼");
	if(it == m_protocoNameVec.end())
	{
		//AfxMessageBox("δ�ҵ�Э��Ŀ¼��");
		return m_treedata;
	}
	int nTabIndex =(int)(it-m_protocoNameVec.begin());

	vector<vector<string>>vec;
	m_excelFile.ReadInfo_Sub_Item(nTabIndex+1,vec,1,-1);

	if(vec.size()==0)
	{
		//AfxMessageBox("��ҳΪ�գ�");
		return m_treedata;
	}

	// �޳�Ŀ¼ҳ������ΪDATA�ļ�¼
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

	//// ���ģ��ҳ����
	//GetTemplate(m_vtTemp1,"Temp1");
	//GetTemplate(m_vtTemp2,"Temp2");
	//GetTemplate(m_vtTemp3,"Temp3");

	m_treedata.lID = nTreeId ++;
	m_treedata.sProNo = "Ŀ¼";
	m_treedata.nData = nTabIndex;
	m_treedata.sName = "Arinc429_ICD";
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
		// ֻ�п�Э��������ڵ�
		if(sDataType != "ITEM")
			continue;

		vector<vector<string>> vtBlockData;
		CString sProSheet = RowDataItem[0].c_str();
		vector<CString>::iterator it_pro = find(m_protocoNameVec.begin(),m_protocoNameVec.end(),sProSheet);
		if(it_pro != m_protocoNameVec.end())
		{
			//CString sInfor = "";
			//sInfor.Format("δ�ҵ�%sЭ��ҳ��",sProSheet);
			//AfxMessageBox(sInfor);
			//return;
			nProSheetNo =(int)(it_pro-m_protocoNameVec.begin());

			m_excelFile.ReadInfo_Sub_Item(nProSheetNo+1,vtBlockData,1,-1);
		}
		string sRecSend = RowDataItem[3];
		int nRecSend = sRecSend == "����" ? 1:2;

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
		// ɾ��ͬ���ڵ�
		if(it_pt->sName == m_treedata.sName)
		{
			ParentTree.subNode.erase(it_pt);
			break;
		}
	}
	ParentTree.subNode.push_back(m_treedata);

	return ParentTree.subNode[ParentTree.subNode.size()-1];
}

BOOL ProArinc429::BuilSubProTree(TREEDATA& subTreeData, int nProSheetNo,int nProType,int& nTreeId)
{
	if(nProSheetNo == -1)
		return FALSE;

	subTreeData.subNode.clear();
	vector<vector<string>>vec;
	m_excelFile.ReadInfo_Sub_Item(nProSheetNo+1,vec,1,-1);

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
			if(RowDataItem[1] == "�ź�����")
				bFindStartFlag = true;
		}
		else if(RowDataItem[1] == "������")
			break;
		else
		{
			// ��ITEM������ӽڵ�
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
				//sInfor.Format("δ�ҵ�%sЭ��ҳ��",sProSheet);
				//AfxMessageBox(sInfor);
				//return;
				nSubSheetNo =(int)(it_pro-m_protocoNameVec.begin());
				m_excelFile.ReadInfo_Sub_Item(nSubSheetNo+1,vtItemData,1,-1);
			}

			TreeDataItem.vtNodeData = vtItemData;
			TreeDataItem.nData = nSubSheetNo;
			TreeDataItem.nDlgId = subTreeData.nDlgId;
			TreeDataItem.nSysDriveId = subTreeData.nSysDriveId;
			TreeDataItem.nRecSend = subTreeData.nRecSend;
			TreeDataItem.subNode.clear();
			// �ɼ������������ݣ���ֻȡ��������
			//BuilSubProTree(TreeDataItem,nSubSheetNo,nProType);
			subTreeData.subNode.push_back(TreeDataItem);
		}
	}

	return TRUE;
}

CString ProArinc429::GetExePath(void)  
{  
	CString strExePath;  
	GetModuleFileName(NULL, strExePath.GetBuffer(MAX_PATH), MAX_PATH);  
	strExePath.ReleaseBuffer();  
	strExePath = strExePath.Left(strExePath.ReverseFind(_T('\\')));  
	return strExePath;  
}

// ���ģ���ͷ����β������
void ProArinc429::GetTempHeadTail(vector<vector<string>>& vtTemp,vector<vector<string>>& vtTHead,
	vector<vector<string>>& vtTTail,int nType)
{
	// ���ģ��2��ͷ����β��
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

	// ����ģ��ĺϷ���
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
		if(nTHeadRows < 10)
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
		else if(vtTHead[0].size() < 10)
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
			nTHeadRows = 10;
			nTTailRows = 2;
			nColNum = 11;
		}
		else if(nType == 3)
		{
			nTHeadRows = 9;
			nTTailRows = 2;
			nColNum = 10;
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

void ProArinc429::InitialRowData(vector<vector<string>>& vtTemp,vector<string>& vtRowData,int nType)
{
	int nTempRow = vtTemp.size();
	bool bIsValid = true;
	if(nTempRow > 0)
	{
		int nTempCol = vtTemp[0].size();
		for(int i = 0; i < nTempCol; ++ i)
			vtRowData.push_back("");
	}

	//// ����ģ��ĺϷ���
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

BOOL ProArinc429::BuildDBTree(TREEDATA& DBTree, __SYS_Drive& DInforItem, int nProType, int& nTreeId,int& nStatus)
{
	vector<__Comm_Protocol> vtProtocol;
	CManageDb_Sqllite::GetInstance().Query_Comm_Protocol(vtProtocol,DBTree.nDlgId,DInforItem.nId);

	int nProNum = vtProtocol.size();
	if(nProNum <= 0)
		return TRUE;

	m_nDlgId = DBTree.nDlgId;
	m_nSysDriveId = DInforItem.nId;

	//// ��ȡЭ��ģ��
	//CString strPath = GetExePath();
	//CString strTempFile;
	//strTempFile.Format("%s\\Template\\1553B.xlsx",strPath);

	//vector<CString> vtSheetName;
	//m_excelFile.OpenFile(strTempFile);
	//m_excelFile.LoadSheetName(vtSheetName);

	//vector<vector<string>> vtTemp1;
	//vector<vector<string>> vtTemp2;
	//vector<vector<string>> vtTemp3;

	//int nTempNum = vtSheetName.size();
	//for(int i = 0; i < nTempNum; ++ i)
	//{
	//	if(vtSheetName[i] == "Temp1")
	//		m_excelFile.ReadInfo_Sub_Item(i+1,vtTemp1,1,-1);
	//	else if(vtSheetName[i] == "Temp2")
	//		m_excelFile.ReadInfo_Sub_Item(i+1,vtTemp2,1,-1);
	//	else if(vtSheetName[i] == "Temp3")
	//		m_excelFile.ReadInfo_Sub_Item(i+1,vtTemp3,1,-1);
	//}
	//m_excelFile.close_excel();

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
	TreeItem.sProNo = "Ŀ¼";
	TreeItem.sDataType = "PROOT";
	TreeItem.pParent = &DBTree;
	TreeItem.vtNodeData.clear();
	TreeItem.subNode.clear();

	vector<vector<string>> vtProData1;	// Ŀ¼ҳ����
	vector<string> vtRowData1;

	vtProData1 = vtTemp1Head;
	vtRowData1 = vtRowTData1;
	// ��DB�л�ȡ1553BЭ������
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

		vector<vector<string>> vtProData2;	// BLOCK����
		vector<string> vtRowData2;

		// Э���
		vtRowData1[0] = "";
		// Э������
		vtRowData1[1] = ProItem.sName;
		// Э������
		vtRowData1[2] = "ITEM";
		// ���ջ���
		vtRowData1[3] = ProItem.nRecOrSend==1?"����":"����";

		CMarkup xml(ProItem.sProtocol.c_str());		// ��Э���XML�ļ�

		if(xml.FindElem(XML_Attr))
		{
			vtProData2 = vtTemp2Head;
			vtRowData2 = vtRowTData2;

			//<Attr Name="1102-������1����" 
			//	Describe="A-ADC1-LCP1-00-00" 
			//	RecSend="����" 
			//	SendMode="����" 
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
			// Э������
			sTxt = xml.GetAttrib(XML_Name);
			vtProData2[0][3] = sTxt.GetBuffer();
			// ����
			CString sBProNo = xml.GetAttrib(XML_Describe);
			vtProData2[1][3] = sBProNo.GetBuffer();
			vtRowData1[0] = sBProNo.GetBuffer();
			TreeSubItem.sProNo = sBProNo;
			// ���ջ���
			sTxt = xml.GetAttrib(XML_RecSend);
			vtProData2[6][5] = sTxt.GetBuffer();
			// ����ģʽ
			sTxt = xml.GetAttrib(XML_SendMode);
			vtProData2[5][2] = sTxt.GetBuffer();
			// ˢ��ʱ��
			sTxt = xml.GetAttrib(XML_RefreshTime);
			vtProData2[4][5] = sTxt.GetBuffer();
			// У�鷽ʽ
			sTxt = xml.GetAttrib(XML_CheckMode);
			vtProData2[7][5] = sTxt.GetBuffer();
			// ��������
			vtProData2[2][2] = "ITEM";
			// ���ݵ�λ
			CString sDataUnit = xml.GetAttrib(XML_DataUnit);
			vtProData2[2][5] = sDataUnit.GetBuffer();
			// ���ݳ���
			sTxt = xml.GetAttrib(XML_DataLen);
			vtProData2[3][5] = sTxt.GetBuffer();
			// ����
			sTxt = xml.GetAttrib(XML_BaudRate);
			vtProData2[5][5] = sTxt.GetBuffer();
			// Դ
			sTxt = xml.GetAttrib(XML_SourceEquipment);
			vtProData2[3][2] = sTxt.GetBuffer();
			// Ŀ��
			sTxt = xml.GetAttrib(XML_DestEquipment);
			vtProData2[4][2] = sTxt.GetBuffer();
			// ͨ�����
			sTxt = xml.GetAttrib(XML_ChannelNo);
			vtProData2[7][2] = sTxt.GetBuffer();
			// �����ڴ�
			sTxt = xml.GetAttrib(XML_Shared);
			vtProData2[8][5] = sTxt.GetBuffer();

			if(xml.FindChildElem("SubItems"))
			{
				string sSubInfor =xml.GetChildSubDoc();	// ��Э�������ITEM��

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

						//// ��������
						//sTxt = xml3.GetAttrib(XML_Item_Item_type);
						//vtProData2[2][2] = sTxt.GetBuffer();
						
						// ITEM����
						// ���
						sTxt = xml3.GetAttrib(XML_Item_NO);
						//vtRowData2[0] = sTxt.GetBuffer();
						// �źŹ淶��
						sTxt = xml3.GetAttrib(XML_Item_Code);
						vtRowData2[0] = sTxt.GetBuffer();
						// �ź�����
						sTxt = xml3.GetAttrib(XML_Name);
						vtRowData2[1] = sTxt.GetBuffer();
						//vtRowData2[2] = sInforName.GetBuffer();
						// ��ʼ��ַ
						sTxt = xml3.GetAttrib(XML_Item_Addr);
						vtRowData2[2] = sTxt.GetBuffer();
						// ����
						sTxt = xml3.GetAttrib(XML_Item_Length);
						vtRowData2[3] = sTxt.GetBuffer();
						// Э��������
						sTxt = xml3.GetAttrib(XML_Item_Item_type);
						vtRowData2[4] = sTxt.GetBuffer();
						// ��������
						sTxt = xml3.GetAttrib(XML_Item_Data_type);
						vtRowData2[5] = sTxt.GetBuffer();
						// ���ʽ
						sTxt = xml3.GetAttrib(XML_Item_Express);
						vtRowData2[6] = sTxt.GetBuffer();
						// ת��ֵ
						sTxt = xml3.GetAttrib(XML_Item_Transfer);
						vtRowData2[7] = sTxt.GetBuffer();
						// Ĭ��ֵ
						sTxt = xml3.GetAttrib(XML_Item_Default);
						vtRowData2[8] = sTxt.GetBuffer();
						// ��ע
						sTxt = xml3.GetAttrib(XML_Item_Mark);
						vtRowData2[9] = sTxt.GetBuffer();
						// �����ڴ�
						sTxt = xml3.GetAttrib(XML_Shared);
						vtRowData2[10] = sTxt.GetBuffer();

						GetSubAttr(xml3,TreeSubItem,vtTemp3Head,vtTemp3Tail,vtRowTData3,sDataUnit,nTreeId,nProType);

						vtProData2.push_back(vtRowData2);
					}
					// ���β��ģ��
					AttachTailTemp(vtProData2,vtTemp2Tail);

					TreeSubItem.vtNodeData = vtProData2;
					TreeItem.subNode.push_back(TreeSubItem);
				}
			}
		}
		vtProData1.push_back(vtRowData1);
	}

	// ������ж�Ӧ�ڵ���ǰ������
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

	// ɾ��ͬ���ڵ�
	vector<TREEDATA>::iterator it_pt = DBTree.subNode.begin();
	for(; it_pt != DBTree.subNode.end(); ++ it_pt)
	{
		// ɾ��ͬ���ڵ�
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

void ProArinc429::GetSubAttr(CMarkup& xml,TREEDATA& TreeSubItem,
	vector<vector<string>>& vtTempHead, vector<vector<string>>& vtTempTail,
	vector<string>& vtRowTData,CString sTDataUnit, int& nTreeId, int nProType)
{
	vector<vector<string>> vtProData3;	// ITEM����
	vector<string> vtRowData3;
	CString sTxt = "";
	CString sProName = "";
	CString sProNo = "";
	CString sDataUnit = "";
	CString sDataLen = "";

	vtProData3 = vtTempHead;
	// Э�����ͣ���ITEM�򲻹����ӽڵ㣩
	CString sItemType = xml.GetAttrib(XML_Item_Item_type);
		if(sItemType != "ITEM")
			return;

	// �淶��
	sProNo = xml.GetAttrib(XML_Item_Code);
	// ���ݵ�λ
	sDataUnit = xml.GetAttrib(XML_DataUnit);
	// ���ݳ���
	sDataLen = xml.GetAttrib(XML_DataLen);

	if(xml.FindChildElem("SubItems"))
	{
		// Э������
		sProName = xml.GetAttrib(XML_Name);
		vtProData3[0][3] = sProName.GetBuffer();
		// �淶��
		vtProData3[1][3] = sProNo.GetBuffer();
		//// ��������
		//vtProData3[2][2] = sItemType;
		// ���ݵ�λ
		vtProData3[6][2] = sDataUnit.GetBuffer();
		// ���ݳ���
		vtProData3[7][2] = sDataLen.GetBuffer();

		string sSubInfor =xml.GetChildSubDoc();	// ��Э�������ITEM��

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

				// ��������
				sTxt = xml3.GetAttrib(XML_Item_Item_type);
				vtProData3[2][2] = sTxt.GetBuffer();

				// DATA����
				// ��ʼ��ַ
				sTxt = xml3.GetAttrib(XML_Item_Addr);
				vtRowData3[1] = sTxt.GetBuffer();
				// ����
				sTxt = xml3.GetAttrib(XML_Item_Length);
				vtRowData3[2] = sTxt.GetBuffer();
				// �ź�����
				sTxt = xml3.GetAttrib(XML_Name);
				vtRowData3[3] = sTxt.GetBuffer();
				// �洢����
				sTxt = xml3.GetAttrib(XML_Item_Data_type);
				vtRowData3[4] = sTxt.GetBuffer();
				// ���ʽ
				sTxt = xml3.GetAttrib(XML_Item_Express);
				vtRowData3[5] = sTxt.GetBuffer();
				// ת��ֵ
				sTxt = xml3.GetAttrib(XML_Item_Transfer);
				vtRowData3[6] = sTxt.GetBuffer();
				// Ĭ��ֵ
				sTxt = xml3.GetAttrib(XML_Item_Default);
				vtRowData3[7] = sTxt.GetBuffer();
				// ��ע
				sTxt = xml3.GetAttrib(XML_Item_Mark);
				vtRowData3[8] = sTxt.GetBuffer();
				// �����ڴ�ID
				sTxt = xml3.GetAttrib(XML_Shared);
				vtRowData3[9] = sTxt.GetBuffer();

				vtProData3.push_back(vtRowData3);
			}
		}
	}
	// ������Э�飬��DATA��չ����ITEM���ITEM��ֻ����DATA��
	else
	{
		vtRowData3 = vtRowTData;
		// �淶��
		vtProData3[1][3] = "";
		// ���ݵ�λ
		vtProData3[6][2] = sTDataUnit.GetBuffer();
		// ���ݳ���
		sDataLen = xml.GetAttrib(XML_Item_Length);
		vtProData3[7][2] = sDataLen.GetBuffer();
		// ��������
		sTxt = xml.GetAttrib(XML_Item_Item_type);
		vtProData3[2][2] = sTxt.GetBuffer();

		// DATA����
		// ��ʼ��ַ
		sTxt = xml.GetAttrib(XML_Item_Addr);
		vtRowData3[1] = sTxt.GetBuffer();
		// ����
		vtRowData3[2] = sDataLen.GetBuffer();
		// �ź�����
		vtRowData3[3] = sProName.GetBuffer();
		// �洢����
		sTxt = xml.GetAttrib(XML_Item_Data_type);
		vtRowData3[4] = sTxt.GetBuffer();
		// ���ʽ
		vtRowData3[5] = "";
		// ת��ֵ
		vtRowData3[6] = "";
		// Ĭ��ֵ
		sTxt = xml.GetAttrib(XML_Item_Default);
		vtRowData3[7] = sTxt.GetBuffer();
		// ��ע
		sTxt = xml.GetAttrib(XML_Item_Mark);
		vtRowData3[8] = sTxt.GetBuffer();
		// �����ڴ�ID
		sTxt = xml.GetAttrib(XML_Shared);
		vtRowData3[9] = sTxt.GetBuffer();

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

void ProArinc429::AttachTailTemp(vector<vector<string>>& vtProData, vector<vector<string>>& vtTempTail)
{
	int nTailRow = vtTempTail.size();

	for(int i = 0; i < nTailRow; ++ i)
	{
		vtProData.push_back(vtTempTail[i]);
	}
}

void ProArinc429::DeleteBLOCKFromDB(TREEDATA& TreeNode)
{
	if(TreeNode.sDataType != "ITEM")
		return;

	CString sCondition = "";
	CString sName = TreeNode.vtNodeData[0][3].c_str();

	// ɾ����Э���µ��������ݽ��
	int nSubNodeNum = TreeNode.subNode.size();
	CString sTxt = "";
	CString sGroupId = "";
	CString sSubName = "";

	// ɾ�����ڵ��µ���������
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
			if(sTxt == "������")
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
							if(sTxt == "������")
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
							if(!sTxt.Compare("�ź�����"))
								bFindStart = true;
						}
					}
				}
			}
			else if(sItemType == "DATA")
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
			if(!sTxt.Compare("�ź�����"))
				bFindNodeStart = true;
		}
	}

	// ɾ��t_comm_protocol�е�Э��
	sCondition.Format("nDlgId=%d and nSysDriveId=%d and sName='%s' and nType=%d",
		TreeNode.nDlgId,TreeNode.nSysDriveId,sName,TreeNode.nRecSend);
	CManageDb_Sqllite::GetInstance().Delete_Comm_Protocel(sCondition);

	// ɾ��Э���Ĺ����ڴ��¼
	sCondition.Format("nDlgId=%d and nSysDriveId=%d and sName='%s' and nInOut=%d",
		TreeNode.nDlgId,TreeNode.nSysDriveId,sName,TreeNode.nRecSend);
	CManageDb_Sqllite::GetInstance().Delete_DataConfig(sCondition);
}

void ProArinc429::SaveDataToDB(TREEDATA& TreeData, int nCurDrive)
{
	if(m_nDriveIndex < 0) return;
	if(m_DriveVec.size() <= m_nDriveIndex)
	{
		//AfxMessageBox("û�л�������忨����Ϣ��");
		return;
	}

	m_nDlgId = TreeData.nDlgId;
	m_nDriveId = m_DriveVec[m_nDriveIndex].nDriveId;
	m_nSysDriveId =m_DriveVec[m_nDriveIndex].nId;

	//// debug
	//m_nDriveId = 8;
	//m_nSysDriveId = 9;

	//��Ӧt_data_configID��nSharedId
	int nProtocol_ConfigId = 0;
	int nProtocol_SharedId = 0;

	int nItem_ConfigId;
	int nItem_SharedId;

	// ��ȡĿ¼��Э�飩ҳ����
	vector<vector<string>>& vec = TreeData.vtNodeData;
	//m_excelFile.ReadInfo_Sub_Item(TreeData.nData+1,vec,1,-1);

	int nRow =1;
	for(int i = nRow,end = vec.size(); i < end /*&& false*/; ++ i)
	{
		if(m_bExitSaveToDB)
		{
			//m_bExitSaveToDB = FALSE;
			return;
		}

		vector<string>& vtRowData = vec[i];
		//// �ж����ڵ��Ƿ���ڸýڵ�����
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

		// Э�����ݿ�������ݿ�
		//�淶��
		CString sProtocolNo = vtRowData[0].c_str();
		//Э������
		CString sProtocolName = vtRowData[1].c_str();
		//���ݿ�����
		CString sBlockType = vtRowData[2].c_str();
		//����/����
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
			//m_excelFile.ReadInfo_Sub_Item(nTabIndex+1,vtItem,1,-1);

			if(vtItem.size()==0)
				continue;

			//����Э������
			CMarkup xml;

			//Э������;����
			xml.AddElem(XML_Attr);	

			//����
			xml.AddAttrib(XML_Name,sProtocolName);
			//������Э��ţ�
			CString sTxt = vtItem[1][3].c_str();
			xml.AddAttrib(XML_Describe,sTxt);
			//����/����--ȡ��Ŀ¼������
			xml.AddAttrib(XML_RecSend,sProtocolRecSend);
			//����ģʽ
			sTxt = vtItem[5][2].c_str();
			xml.AddAttrib(XML_SendMode,sTxt);
			//ˢ��ʱ��
			sTxt = vtItem[4][5].c_str();
			sTxt.Trim("ms");// ȥ��ms
			xml.AddAttrib(XML_RefreshTime,sTxt);
			//У�鷽ʽ
			sTxt = vtItem[7][5].c_str();
			xml.AddAttrib(XML_CheckMode,sTxt);
			//���ݵ�λ
			CString sDataUnit = vtItem[2][5].c_str();
			xml.AddAttrib(XML_DataUnit,sDataUnit);
			//���ݳ���
			sTxt = vtItem[3][5].c_str();
			xml.AddAttrib(XML_DataLen,sTxt);
			int nDataLen = _tstoi(sTxt);
			//����
			sTxt = vtItem[5][5].c_str();
			xml.AddAttrib(XML_BaudRate,sTxt);
			//Դ
			sTxt = vtItem[3][2].c_str();
			xml.AddAttrib(XML_SourceEquipment,sTxt);
			//Ŀ��
			sTxt = vtItem[4][2].c_str();
			xml.AddAttrib(XML_DestEquipment,sTxt);
			//ͨ�����
			sTxt = vtItem[7][2].c_str();
			xml.AddAttrib(XML_ChannelNo,sTxt);
			//�����ڴ�ֵ
			bFindBlockProtocol = XProtocol_Funtion::GetProtocolId(m_nDlgId,m_nSysDriveId,sProtocolName,sProtocolRecSend,m_nProtocolId);
			//������ӦЭ��Ĺ����ڴ�
			XProtocol_Funtion::Save_Protocl_ShareDataInfor(m_nDlgId,sProtocolName,m_nProtocolId,m_nDriveId,m_nSysDriveId,sProtocolRecSend,nDataLen,nProtocol_ConfigId,nProtocol_SharedId);

			//nConfigId = GetNewConfigDatId();
			xml.AddAttrib(XML_Shared,nProtocol_SharedId);

			// Э���������1��ʼ���
			m_nProtoclItem_Index = 1;
			//Э������
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
					if(sTxt == "������")
						break;

					CMarkup xmlChild;
					xmlChild.AddElem(XML_Item_Item);

					//���
					CString sItemNo = ""/* = vtSubItem[0].c_str()*/;
					sItemNo.Format("%d",nItemNo ++);
					xmlChild.AddAttrib(XML_Item_NO,sItemNo);
					//����루�淶�ţ�
					CString sProNo = vtSubItem[0].c_str();
					xmlChild.AddAttrib(XML_Item_Code,sProNo);
					//�ź�����
					CString sItemName = vtSubItem[1].c_str();
					xmlChild.AddAttrib(XML_Name,sItemName);
					//λ��ʼ��ַ
					sTxt = vtSubItem[2].c_str();
					xmlChild.AddAttrib(XML_Item_Addr,sTxt);
					//����
					CString sItemLength = vtSubItem[3].c_str();
					xmlChild.AddAttrib(XML_Item_Length,sItemLength);
					//������
					//sTxt = vtItem[2][2].c_str();
					CString sItemType = vtSubItem[4].c_str();
					xmlChild.AddAttrib(XML_Item_Item_type,sItemType);
					//��������
					//sTxt = "����";
					CString sDataType = vtSubItem[5].c_str();
					xmlChild.AddAttrib(XML_Item_Data_type,sDataType);
					// ITEM �������ʽ��ת��ֵ����
					if(sItemType != "ITEM")
					{
						//���ʽ
						sTxt = vtSubItem[6].c_str();
						sTxt.MakeLower();
						sTxt.Trim();
						xmlChild.AddAttrib(XML_Item_Express,sTxt);
						//ת��ֵ
						sTxt = vtSubItem[7].c_str();
						xmlChild.AddAttrib(XML_Item_Transfer,sTxt);
					}
					//Ĭ��ֵ
					//sTxt = "";
					CString sDefault = vtSubItem[8].c_str();
					xmlChild.AddAttrib(XML_Item_Default,sDefault);
					//��ע
					CString sMark = vtSubItem[9].c_str();
					xmlChild.AddAttrib(XML_Item_Mark,sMark);

					if(sItemType == "ITEM")
					{
						// �������������
						GetDataAttr(xmlChild,TreeData.subNode[i-1],nIdx,sProtocolRecSend,sItemNo,sProtocolName,m_nProtocolId);
						++ nIdx;
					}
					else if(sItemType == "DATA" && sItemName != "")
					{
						//���������ڴ�ID
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
							nItem_ConfigId,nItem_SharedId);

						++ m_nProtoclItem_Index;

						sTxt.Format("%d",nItem_SharedId);
						xmlChild.AddAttrib(XML_Shared,sTxt);
					}

					xmlItem.AddChildSubDoc(xmlChild.GetSubDoc());
				}
				else
				{
					sTxt = vtSubItem[1].c_str();
					if(!sTxt.Compare("�ź�����"))
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

			// �������ص�
			m_func(m_lpParam);
		}
	}
}

void ProArinc429::GetDataAttr(CMarkup& hxml, TREEDATA& TreeData, int nIdx, CString sRecSend,
	CString sItemNo, CString sProtocoName, int nProtocolId)
{
	//vector<CString>::iterator it = find(m_protocoNameVec.begin(),m_protocoNameVec.end(),sItemCode);
	//if(it == m_protocoNameVec.end())
	//	return;
	//int nTabIndex =(int)(it-m_protocoNameVec.begin());

	vector<vector<string>>& vec = TreeData.subNode[nIdx].vtNodeData;
	//m_excelFile.ReadInfo_Sub_Item(nTabIndex+1,vec,1,-1,10);

	if(vec.size()==0)
		return;

	//����Э������
	//���ݵ�λ
	CString sTxt = vec[6][2].c_str();
	hxml.AddAttrib(XML_DataUnit,sTxt);
	//���ݳ���
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
			if(sTxt == "������")
				break;

			CMarkup xmlSubChild;
			xmlSubChild.AddElem(XML_Item_Item);

			//��ʼ��ַ
			sTxt = vtSubItem[1].c_str();
			xmlSubChild.AddAttrib(XML_Item_Addr,sTxt);
			//λ��
			sTxt = vtSubItem[2].c_str();
			xmlSubChild.AddAttrib(XML_Item_Length,sTxt);
			int nSubInforLength = int(_tstoi(sTxt)/8.0+1);
			//������
			sTxt = vec[2][2].c_str();
			xmlSubChild.AddAttrib(XML_Item_Item_type,sTxt);
			//��������
			CString sDataType = vtSubItem[4].c_str();
			xmlSubChild.AddAttrib(XML_Item_Data_type,sDataType);
			//�ź�����
			CString sItemName = vtSubItem[3].c_str();
			xmlSubChild.AddAttrib(XML_Name,sItemName);
			//���ʽ
			sTxt = vtSubItem[5].c_str();
			sTxt.MakeLower();
			sTxt.Trim();
			xmlSubChild.AddAttrib(XML_Item_Express,sTxt);
			//ת��ֵ
			sTxt = vtSubItem[6].c_str();
			xmlSubChild.AddAttrib(XML_Item_Transfer,sTxt);
			//Ĭ��ֵ
			CString sDefaultValue = vtSubItem[7].c_str();
			xmlSubChild.AddAttrib(XML_Item_Default,sDefaultValue);
			//��ע
			CString sMark = vtSubItem[8].c_str();
			xmlSubChild.AddAttrib(XML_Item_Mark,sMark);
			//���������ڴ�ID
			//ֻ���ź����Ʋ�Ϊ�յļ�¼���������ڴ�ID
			if(sItemName != "")
			{
				XProtocol_Funtion::Save_ProtoclData_toConfigData(m_nDlgId,sRecSend,sProtocoName+"."+sItemNo+"."+sItemName,
					sDataType,nSubInforLength,nProtocolId,m_nProtoclItem_Index,
					m_nDriveId,m_nSysDriveId,sDefaultValue,1.0,0.0,sMark,
					nItem_ConfigId,nItem_SharedId);

				++ m_nProtoclItem_Index;

				sTxt.Format("%d",nItem_SharedId);
				xmlSubChild.AddAttrib(XML_Shared,sTxt);
			}

			xmlChild.AddChildSubDoc(xmlSubChild.GetSubDoc());
		}
		else
		{
			sTxt = vtSubItem[3].c_str();
			if(!sTxt.Compare("�ź�����"))
				bFindStart = true;
		}
	}

	hxml.AddChildSubDoc(xmlChild.GetSubDoc());
}


void ProArinc429::SetProgressFunc(CallFunc func,LPVOID lpParam)
{
	m_lpParam = lpParam;
	m_func = func;
}

