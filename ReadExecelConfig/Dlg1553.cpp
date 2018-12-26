// Dlg1553.cpp : 实现文件
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "Dlg1553.h"
#include "afxdialogex.h"

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
// CDlg1553 对话框

IMPLEMENT_DYNAMIC(CDlg1553, CDialogEx)

CDlg1553::CDlg1553(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg1553::IDD, pParent)
{
	m_nProtocolId = 0;
	m_nPulicProId=0xffff;
}

CDlg1553::~CDlg1553()
{
}

void CDlg1553::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DRIVE, m_comboDrive);
	//DDX_Control(pDX, IDC_COMBO_SYS, m_comboSys);
	DDX_Control(pDX, IDC_COMBO_protocol, m_combo_protocol);
	DDX_Control(pDX, IDC_CUSTOM1, m_grid);
	DDX_Control(pDX, IDC_CUSTOM2, m_GridChild);
}


BEGIN_MESSAGE_MAP(CDlg1553, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SelectFile, &CDlg1553::OnBnClickedButtonSelectfile)
	ON_BN_CLICKED(IDC_BUTTON_Read, &CDlg1553::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlg1553::OnBnClickedButtonSave)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlg1553 消息处理程序


BOOL CDlg1553::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strWhere;
	strWhere.Format("and nDriveId in(  select nid from t_drive where smark ='1553B')");

	CManageDb_Sqllite::GetInstance().Query_SysDrive(m_DriveVec,m_nCurSysId,strWhere.GetBuffer());

	int nIndex;
	for(int i =0,end =m_DriveVec.size();i<end;i++)
	{
		nIndex =m_comboDrive.AddString(m_DriveVec[i].sName.c_str());		

	}
	m_comboDrive.SetCurSel(0);
 


	return TRUE;
}


void CDlg1553::OnBnClickedButtonSelectfile()
{
  
	UpdateData();
	CString strFilePath;
	CFileDialog dlg(TRUE);

	if(dlg.DoModal()!=IDOK)
		return;
	strFilePath = dlg.GetPathName(); 
	GetDlgItem(IDC_EDIT_file_path)->SetWindowText(strFilePath);

	m_combo_protocol.ResetContent();

	m_protocoNameVec.clear();

	m_config_1553.OpenFile(strFilePath,m_protocoNameVec);

	for(int i =0,end =m_protocoNameVec.size();i<end;i++)
		m_combo_protocol.AddString(m_protocoNameVec[i]);

	m_combo_protocol.SetCurSel(0);

}


void CDlg1553::OnBnClickedButtonRead()
{

	int nSel = m_combo_protocol.GetCurSel();
	if(nSel ==-1)
		return;
	vector<vector<string>>Basevec ;
	CString str;

	m_nGirdColCount =8;//9;

	m_config_1553.ReadInfo_Sub_Item(nSel+1,Basevec,1,-1,m_nGirdColCount);

	CRect rt; 
	m_grid.GetClientRect(rt);
	int nW =rt.Width()/m_nGirdColCount;
	m_grid.DeleteAllItems();
	m_grid.SetEditable(TRUE);
	m_grid.SetDefCellHeight(20);
	m_grid.SetColumnCount(m_nGirdColCount);
	m_grid.SetRowCount(Basevec.size());
	for(int i =0;i<m_nGirdColCount;i++)
	{
		m_grid.SetColumnWidth(i,nW-6);
	}
	//CString strItem[]={"起始位","长度","协议项类型","数据类型","系数","补充值","名称","默认值","备注"};
	//CString strItem[]={"数据块名称","规范号","信号名称","类型","AT地址","AT子地址","接收/发送","总线号"};
	//m_GridChild.DeleteAllItems();
	//m_GridChild.SetEditable(TRUE);
	//m_GridChild.SetDefCellHeight(20);
	//m_GridChild.SetColumnCount(m_nGirdColCount);
	//m_GridChild.SetRowCount(1);
	//for(int i =0;i<m_nGirdColCount;i++)
	//{
	//	SetGirdItemText_string(m_GridChild,0,i,strItem[i]);
	//	m_GridChild.SetColumnWidth(i,nW-2);
	//}
	bool bItem =false;
	for(int i=0,end = Basevec.size();i<end;i++)
	{
		vector<string>&vec =Basevec[i];
		int n=0;
		bItem =false;
		for(int k =0,end1=vec.size();k<end1 && k<m_nGirdColCount;k++)
		{ 
			SetGirdItemText_string(m_grid,i,n++,vec[k].c_str());

			//if(i>=10 && k ==2 && CString(vec[k].c_str()).MakeUpper() ==PROTOCOL_Item_Item)
			//{
			//	bItem =true;
			//}
			//if(k==6 && bItem)
			//{
			//	ReadSubInfo(vec[k].c_str());				 
			//}
		}
	}
	m_grid.Invalidate();
	//m_GridChild.Invalidate();

	return;
	 
}

void CDlg1553::ReadSubInfo(CString sItemName)
{
	int nIndex =-1;
	for(int i =0,end =m_protocoNameVec.size();i<end;i++)
		if(m_protocoNameVec[i] == sItemName)
		{
			nIndex = i;
			break;
		} 
		if(nIndex==-1)
			return ;
		vector<string>vec;
		vector<vector<string>>dataVec;
		m_config_1553.ReadRowInfo(nIndex+1,vec,2,5);	
		m_config_1553.ReadInfo_Sub_Item(nIndex+1,dataVec,11);

		int nRow = m_GridChild.GetRowCount();
		m_GridChild.SetRowCount(nRow+dataVec.size()+1);

		vec.resize(5);
		CString str;
		str.Format("%s/%s/%s/%s/%s",vec[0].c_str(),vec[1].c_str(),vec[2].c_str(),vec[3].c_str(),vec[4].c_str());


		CCellRange cell(nRow,0,nRow,m_nGirdColCount-1);
		m_GridChild.MergeCells(cell);

		SetGirdItemText_string(m_GridChild,nRow++,0,str);

		for(int i =0,end = dataVec.size();i<end;i++)
		{	
			vector<string>&data =dataVec[i];
			if(data[0]=="")
				continue;
			for(int k=0,end1 =data.size();k<end1;k++)
			{
				SetGirdItemText_string(m_GridChild,nRow,k,data[k].c_str());
			}
			nRow++;
		}

		m_GridChild.SetRowCount(nRow);

}

void CDlg1553::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(m_grid.GetSafeHwnd())
	{
		m_grid.MoveWindow(30,60,cx-60,(cy-80)/2);
		m_GridChild.MoveWindow(30,60+(cy-80)/2,cx-60,(cy-80)/2);

	}
}
void CDlg1553::OnBnClickedButtonSave()
{
	CString sTxt;
	sTxt.Format("是否将协议保存到\"%s\"",m_sCurDlgName);
	
	if(AfxMessageBox(sTxt,MB_YESNO) == IDNO)
		return;

	CWaitCursor wait;
	int nSel = m_comboDrive.GetCurSel();
	if(nSel==-1)
		return;

	//m_nProtoclItem_Index =1;
	m_nDriveId = m_DriveVec[nSel].nDriveId;
	m_nSysDriveId =m_DriveVec[nSel].nId;

	//对应t_data_configID、nSharedId
	int nProtocol_ConfigId;
	int nProtocol_SharedId;
	//协议名称

	// 删除共享内存临时数据区
	CString strSql;

	strSql.Format("delete from t_data_config_1;");
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	strSql.Format("delete from t_1553B_protocol;");
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	int nRow =1;
	for(long i = nRow,end = m_grid.GetRowCount(); i < end /*&& false*/; ++ i)
	{
		//m_nProtoclItem_Index = 1;

		//数据块名称
		CString sBlockName = m_grid.GetItemText(i,0);
		//规范号
		CString sProtocolNo = m_grid.GetItemText(i,1);
		//协议名称
		CString sProtocolName = m_grid.GetItemText(i,2);
		//数据块类型
		CString sBlockType = m_grid.GetItemText(i,3);
		//AT地址
		int nATAddr = _tstoi(m_grid.GetItemText(i,4));
		//AT子地址
		int nATsuAddr = _tstoi(m_grid.GetItemText(i,5));
		//接收/发送
		CString  sProtocolRecSend= m_grid.GetItemText(i,6);
		//总线类型
		CString sBusType= m_grid.GetItemText(i,7);

		CString strXml = "";

		if(sBlockType == "ITEM" /*&& false*/)
		{
			int nDataLen = 0;

			//删除历史记录，并获得协议表中的nId
			int nRecSend = sProtocolRecSend==PROTOCOL_RecSend_Rec?1:2;	// 1=接收；2=发送
			XProtocol_Funtion::DeleteProtoclRecod(m_nPulicProId/*m_nCurDlgId*/,sProtocolNo,m_nDriveId,m_nSysDriveId,nRecSend,1,m_nProtocolId);

			// debug
			//bool bTstFind = false;
			//if(sProtocolNo == "A-LCP1-CWS1-20-00")
			//{
			//	bTstFind = true;
			//}

			int nTabIndex = m_combo_protocol.FindString(-1,sProtocolNo);
			if(nTabIndex == -1)
			{
				continue;
			}
			vector<vector<string>>vec;
			m_config_1553.ReadInfo_Sub_Item(nTabIndex+1,vec,1,-1);

			if(vec.size()==0)
				continue;

			//解析协议属性
			CMarkup xml;

			//协议名称;名称
			xml.AddElem(XML_Attr);	

			//源
			sTxt = vec[3][2].c_str();
			xml.AddAttrib(XML_SourceEquipment,sTxt);
			//目标
			sTxt = vec[4][2].c_str();
			xml.AddAttrib(XML_DestEquipment,sTxt);

			//发送机制（传输类型）
			sTxt= vec[5][2].c_str();	 
			xml.AddAttrib(XML_SendMode,sTxt);

			//数据块ID
			sTxt= vec[8][2].c_str();	 
			xml.AddAttrib(XML_BlockId,sTxt);

			//数据单位
			sTxt= vec[2][4].c_str();	 
			xml.AddAttrib(XML_DataUnit,sTxt);

			//数据长度
			sTxt= vec[2][5].c_str();	 
			xml.AddAttrib(XML_DataLen,sTxt);
			nDataLen = _tstoi(sTxt)*2;

			//中断
			sTxt= vec[3][5].c_str();
			xml.AddAttrib(XML_Interrupt,sTxt);

			//刷新时间(ms)
			sTxt= vec[5][5].c_str();	 
			xml.AddAttrib(XML_RefreshTime,sTxt);

			//最大延迟时间(ms)
			sTxt= vec[6][5].c_str();	 
			xml.AddAttrib(XML_MaxDelay,sTxt);
			
			//发送方式
			sTxt= vec[7][5].c_str();	 
			xml.AddAttrib(XML_SendStyle,sTxt);

			nProtocol_SharedId =  (m_nDriveId<<24) +(m_nSysDriveId<<16) +(m_nProtocolId<<8);
			//nConfigId = GetNewConfigDatId();

			//共享内存值
			xml.AddAttrib(XML_Shared,nProtocol_SharedId);

			//协议子项
			CMarkup xmlItem;
			xmlItem.AddElem(XML_SubItems);

			int nBRow = vec.size();
			bool bFindStart = false;
			for(int j = 0; j < nBRow; ++ j)
			{
				vector<string>& vtSubItem = vec[j];
				int nEndSubItemNum = vtSubItem.size();
				if(nEndSubItemNum <= 0) continue;

				if(bFindStart)
				{
					CMarkup xmlChild;
					xmlChild.AddElem(XML_Item_Item);

					for(int k = 0; k < nEndSubItemNum && k < 3; ++ k)
					{
						if(k == 0)//信号显示的名称
						{
							sTxt = vtSubItem[k].c_str();
							// 增加组号
							if(nEndSubItemNum > 2)
							{
								CString sGroupNo = vtSubItem[3].c_str();
								if(!sGroupNo.IsEmpty())
									sTxt += ("."+sGroupNo);
							}
							xmlChild.AddAttrib(XML_DispName,sTxt);
						}
						else if(k == 1)//信号显示的值
						{
							sTxt = vtSubItem[k].c_str();
							xmlChild.AddAttrib(XML_DispValue,sTxt);
						}
						else if(k == 2)//信号显示的排列方式
						{
							sTxt = vtSubItem[k].c_str();
							xmlChild.AddAttrib(XML_DispStyle,sTxt);
						}
					}
					xmlItem.AddChildSubDoc(xmlChild.GetSubDoc());

				}
				else
				{
					sTxt = vtSubItem[0].c_str();
					if(!sTxt.Compare("信号显示名称"))
						bFindStart = true;

				}
			}

			xml.AddChildSubDoc(xmlItem.GetSubDoc());

			strXml = xml.GetSubDoc();

			//创建对应协议的共享内存
			XProtocol_Funtion::Save_Protocl_toConfigData(m_nPulicProId/*m_nCurDlgId*/,sProtocolName,m_nProtocolId,m_nDriveId,m_nSysDriveId,sProtocolRecSend,nDataLen,nProtocol_ConfigId,nProtocol_SharedId);

		}
		else if(sBlockType == "DATA")
		{
			//删除协议
			//CString	strSql = "";
			//strSql.Format("delete  from t_1553B_protocol where nDlgId = %d and nSysDriveId =%d and sName = '%s' ",m_nCurDlgId,m_nSysDriveId,sProtocolName);

			//CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
			//1--ITEM;2--DATA
			XProtocol_Funtion::DeleteProtoclRecod(m_nPulicProId/*m_nCurDlgId*/,sProtocolNo,m_nDriveId,m_nSysDriveId,-1,2,m_nProtocolId);

			int nTabIndex = m_combo_protocol.FindString(-1,sProtocolNo);
			if(nTabIndex == -1)
			{
				continue;
			}
			vector<vector<string>>vec;
			m_config_1553.ReadInfo_Sub_Item(nTabIndex+1,vec,1,-1,6,4);

			if(vec.size()==0)
				continue;

			//解析协议属性

			//CString sDataType;

			int nItem_ConfigId;
			int nItem_SharedId;

			// debug
			bool bTstFind = false;
			if(sProtocolNo == "ADC_013")
			{
				bTstFind = true;
			}
			//float fCoefficient;
			//float fOffset;

			CMarkup xmlChild;
			xmlChild.AddElem(XML_SubItems);

			// 信号长度
			sTxt = vec[14][2].c_str();
			xmlChild.AddAttrib(XML_Item_Length,sTxt);
			int nItemLength = 16;

			// 分辨率
			sTxt = vec[15][2].c_str();
			xmlChild.AddAttrib(XML_Item_Resolution,sTxt);

			// 单位
			sTxt = vec[14][4].c_str();
			xmlChild.AddAttrib(XML_Item_Unit,sTxt);

			//信号范围
			sTxt = vec[15][4].c_str();
			xmlChild.AddAttrib(XML_Item_Range,sTxt);

			//显示项作为子项
			vector<SUBITEMATTR> vtSubItem;
			int nSubNum = vec.size();

			//解析起始位，位长及符号位
			bool bFindStart = false;
			bool bFindEnd = false;
			SUBITEMATTR SubItemData;
			int nStartBit = 0;
			int nStartRow = -1;
			for(int j = 0; j < vec.size(); ++ j)
			{
				if(vec[j][2] == "说明")
				{
					nStartRow = j+1;
					break;
				}
			}
			for(int j = 0; j < nItemLength && nStartRow != -1; ++ j)
			{
				if(j < 8)
					sTxt = vec[nStartRow+j][2].c_str();
				else
					sTxt = vec[nStartRow+j%8][5].c_str();
				if(!sTxt.IsEmpty())
				{
					if(!bFindStart)
					{
						if(sTxt.Find("-/") >= 0)
						{
							bFindStart = true;
							nStartBit = j;
						}
					}
					else if(!bFindEnd)
					{
						//CString csTst = "-\\";
						//int nTst = sTxt.Find(csTst);
						if(sTxt.Find("-\\") >= 0)
							bFindEnd = true;
					}
					// 未找到位的前导符，
					if(!bFindStart)
					{
						SubItemData.nStartPosi = j;
						SubItemData.nItemLen = 1;
						SubItemData.bIsSign = 0;
						vtSubItem.push_back(SubItemData);
					}
					// 找到结束符，计算起始位，位长及符号位
					else if(bFindEnd)
					{
						SubItemData.nStartPosi = nStartBit;
						SubItemData.nItemLen = j-nStartBit+1;
						if(sTxt.Find("SIGN") >= 0)
							SubItemData.bIsSign = true;
						vtSubItem.push_back(SubItemData);
						bFindStart = false;
						bFindEnd = false;
					}
				}
			}
			//读字段的显示信息
			bool bFindDisp = false;
			int nSubItemNum = vtSubItem.size();
			int nIdx = 0;
			for(int j = 0; j < nSubNum && nIdx < nSubItemNum; ++ j)
			{
				sTxt = vec[j][0].c_str();
				if(!bFindDisp)
				{
					if(!sTxt.Compare("信号显示名称"))
						bFindDisp = true;
				}
				else
				{
					vtSubItem[nIdx].sItemName = vec[j][0];
					vtSubItem[nIdx].nInforType = atoi(vec[j][1].c_str());
					sTxt = vec[j][2].c_str();
					if(!sTxt.IsEmpty())
						vtSubItem[nIdx].fOffset = atof(vec[j][2].c_str());
					sTxt = vec[j][3].c_str();
					if(!sTxt.IsEmpty())
						vtSubItem[nIdx].fCoef = atof(vec[j][3].c_str());
					vtSubItem[nIdx].sDispValue = vec[j][4];
					vtSubItem[nIdx].sRemark = vec[j][5];
					++ nIdx;
				}
			}
			
			// SubItem信息存入XML文件
			for(int j = 0; j < nSubItemNum; ++ j)
			{

				CMarkup xmlSub;
				xmlSub.AddElem(XML_Item_Item);

				SUBITEMATTR& ItemData = vtSubItem[j];
				//起始地址
				sTxt.Format("%d",ItemData.nStartPosi);
				xmlSub.AddAttrib(XML_Item_Addr,sTxt);
				//位长
				sTxt.Format("%d",ItemData.nItemLen);
				xmlSub.AddAttrib(XML_Item_Length,sTxt);

				//sItemType = m_grid.GetItemText(k,n++);
				//xmlChild.AddAttrib(XML_Item_Item_type,sItemType);

				//sDataType=m_grid.GetItemText(k,n++);
				//xmlChild.AddAttrib(XML_Item_Data_type,sDataType);

				sTxt.Format("%0.2f",ItemData.fCoef);
				xmlSub.AddAttrib(XML_Item_Coefficient,sTxt);
				//fCoefficient = atof(sTxt.GetBuffer());

				sTxt.Format("%0.2f",ItemData.fOffset);
				xmlSub.AddAttrib(XML_Item_Offset,sTxt);
				//fOffset = atof(sTxt.GetBuffer());

				//用于显示的名称
				sTxt = ItemData.sItemName.c_str();
				xmlSub.AddAttrib(XML_Item_DispName,sTxt);

				//显示的类型（如文本框，组合框，多选框等）
				sTxt.Format("%d",ItemData.nInforType);
				xmlSub.AddAttrib(XML_Item_DispStyle,sTxt);

				//用于显示的值
				sTxt = ItemData.sDispValue.c_str();
				xmlSub.AddAttrib(XML_Item_DispValue,sTxt);

				//符号位
				sTxt.Format("%d",int(ItemData.bIsSign));
				xmlSub.AddAttrib(XML_Item_Sign,sTxt);

				sTxt =ItemData.sRemark.c_str();
				xmlSub.AddAttrib(XML_Item_Mark,sTxt);
				
				xmlChild.AddChildSubDoc(xmlSub.GetSubDoc());
			}

			strXml = xmlChild.GetSubDoc();
		}

		XProtocol_Funtion::SaveProtocol(
			m_nPulicProId/*m_nCurDlgId*/,
			m_nProtocolId,
			sProtocolRecSend,
			sProtocolName,
			m_nSysDriveId,
			strXml,
			sBlockName,			//数据块名称
			sProtocolNo,		//规范号
			sBlockType,			//数据块类型
			nATAddr,			//AT地址
			nATsuAddr,			//AT子地址
			sBusType);			//sBusType
	}

	SaveProtocolItemConfig();

	//CString strSql = "";
	strSql.Format("insert into t_data_config select * from t_data_config_1");
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	// 构建信号的共享内存单元
	//XProtocol_Funtion::Save_ProtoclItem_toConfigData(m_nCurDlgId,sProtocolRecSend,sProtocolName+"."+sProtocoItemName,sDataType,atoi(sDataLength),m_nProtocolId,m_nProtoclItem_Index,
	//	m_nDriveId,m_nSysDriveId,sDefaultValue,fCoefficient,fOffset,sMark,
	//	nItem_ConfigId,nItem_SharedId);

}

// 1553B协议子项存入共享内存数据库
void CDlg1553::SaveProtocolItemConfig(void)
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
											XProtocol_Funtion::Save_ProtoclItem_toConfigData(m_nPulicProId/*m_nCurDlgId*/,sRecSend,sProtocoName+"."+sItemDispName+"."+sRecSend,DATA_TYPE_INT_NAME,nSubInforLength/8,ProItem.nId,m_nProtoclItem_Index,
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

									XProtocol_Funtion::Save_ProtoclItem_toConfigData(m_nPulicProId/*m_nCurDlgId*/,sRecSend,sProtocoName+"."+sItemDispName+"."+sRecSend,DATA_TYPE_INT_NAME,nSubInforLength/8,ProItem.nId,m_nProtoclItem_Index,
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

void CDlg1553::WriteSubInfoToXml(CMarkup &xml,CString sProtocoName,CString sProtocoItemName,CString sRecSend,int nParent_ConfigId,int nParent_SharedId)
{ 
	int nIndex =-1;	
	for(int i =0,end =m_protocoNameVec.size();i<end;i++)
		if(m_protocoNameVec[i] == sProtocoItemName)
		{
			nIndex = i;
			break;
		}
		if(nIndex==-1)
			return ;

		//子项的基本信息：数据单位，数据长度
		vector<string>SubVec;
		m_config_1553.Read_One_RowInfo(nIndex+1,SubVec,2,4,2);
		if(SubVec.size()!=2)
			return ;

		//子项的数据单位，数据长度
		xml.AddAttrib(XML_DataUnit,SubVec[0].c_str());
		xml.AddAttrib(XML_DataLen,SubVec[1].c_str());

		vector<vector<string>>vec;
		m_config_1553.ReadInfo_Sub_Item(nIndex+1,vec);

		if(vec.size()==0)
			return;

		//协议子项
		CMarkup xmlItem;
		xmlItem.AddElem(XML_SubItems);
		int n=0;
		int nItem_ConfigId;
		int nItem_SharedId;
		CString sItemName;
		CString sTxt;

		for(int i=0,end =vec.size();i<end;i++)
		{
			n=0;
			vector<string>&info= vec[i];
			if(info[0]=="")
				continue;
			if(vec[i].size()<9)
				continue;

			CMarkup xmlChild;
			xmlChild.AddElem(XML_Item_Item);

			//起始位、长度
			xmlChild.AddAttrib(XML_Item_Addr,info[0].c_str());
			xmlChild.AddAttrib(XML_Item_Length,info[1].c_str());

			//协议项类型,数据类型		
			xmlChild.AddAttrib(XML_Item_Item_type,info[2].c_str());
			xmlChild.AddAttrib(XML_Item_Data_type,info[3].c_str());

			//系数,补偿值
			xmlChild.AddAttrib(XML_Item_Coefficient,info[4].c_str());
			xmlChild.AddAttrib(XML_Item_Offset,info[5].c_str());

			//名称、默认值、备注
			xmlChild.AddAttrib(XML_Name,info[6].c_str());
			xmlChild.AddAttrib(XML_Item_Default,info[7].c_str());
			xmlChild.AddAttrib(XML_Item_Mark,info[8].c_str());

			//协议项类型 ：DATA	,类型， 
			if(CString(info[2].c_str()).MakeUpper() ==PROTOCOL_Item_Data)
			{
				if(!info[6].empty()) //名称
				{ 
					if(XProtocol_Funtion::Save_ProtoclItem_toConfigData(m_nCurDlgId,sRecSend,sProtocoName+"."+sProtocoItemName+"."+info[6].c_str(),info[3].c_str(),atoi(info[1].c_str())/8,m_nProtocolId,m_nProtoclItem_Index,
						m_nDriveId,m_nSysDriveId,info[7].c_str(),atof(info[4].c_str()),atof(info[5].c_str()),info[8].c_str(),
						nItem_ConfigId,nItem_SharedId))
					{
						XProtocol_Funtion::Save_Data_Sets(m_nProtocolId,nParent_ConfigId,nParent_SharedId,nItem_ConfigId,nItem_SharedId);
						xmlChild.AddAttrib("Shared",nItem_SharedId);
						m_nProtoclItem_Index++;
					}
				}
			}
			xmlItem.AddChildSubDoc(xmlChild.GetSubDoc());
		}	 
		xml.AddChildSubDoc(xmlItem.GetSubDoc());
}
//void CDlg1553::WriteSubInfoToXml(CMarkup &xml,CString sProtocoName,CString sProtocoItemName,int nParent_ConfigId,int nParent_SharedId)
//{
//	int nIndex =-1;	
//	for(int i =0,end =m_protocoNameVec.size();i<end;i++)
//		if(m_protocoNameVec[i] == sProtocoItemName)
//		{
//			nIndex = i;
//			break;
//		}
//
//	if(nIndex==-1)
//		return ;
//
//	//子项的基本信息：数据单位，数据长度
//	vector<string>SubVec;
//	m_config_1553.ReadInfo_Item_SubInfo(nIndex+1,SubVec);
//	if(SubVec.size()<3)
//		return;
//
//	//子项的数据单位，数据长度
//	xml.AddAttrib("DataUnit",SubVec[0].c_str());
//	xml.AddAttrib("DataLen",SubVec[1].c_str());
//	 
//	vector<vector<string>>vec;
//	m_config_1553.ReadInfo_Sub_Item(nIndex+1,vec);
//	
//	if(vec.size()==0)
//		return;
//
//	//协议子项
//	CMarkup xmlItem;
//	xmlItem.AddElem("SubItems");
//	int n=0;
//	int nItem_ConfigId;
//	int nItem_SharedId;
//	CString sItemName;
//	for(int i=0,end =vec.size();i<end;i++)
//	{
//		vector<string>&info= vec[i];
//		if(info[0]=="")
//			continue;
//		if(vec[i].size()<9)
//			continue;;
//		n=0;
//		CMarkup xmlChild;
//		xmlChild.AddElem("Item");
//		xmlChild.AddAttrib("addr",info[0].c_str());
//		xmlChild.AddAttrib("length",info[1].c_str());
//		xmlChild.AddAttrib("type",info[2].c_str());
//		xmlChild.AddAttrib("data_type",info[3].c_str());
//		xmlChild.AddAttrib("coefficient",info[4].c_str());
//		xmlChild.AddAttrib("offset",info[5].c_str());
//		xmlChild.AddAttrib("name",info[6].c_str());
//		xmlChild.AddAttrib("default",info[7].c_str());
//		xmlChild.AddAttrib("mark",info[8].c_str());
//		
//		sItemName =sProtocoName+"."+sProtocoItemName+"."+info[6].c_str();
//		//bool  Save_ProtoclItem_toConfigData(int nDlgId,CString sProtocolItemType,CString sDataName,CString sDataType,int nDataLen,int nProtocolId,int nIndex,int nDriveId,int nSysDriveId,int &nItemId,int &nItem_SharedId);
//		/*if(XProtocol_Funtion::Save_ProtoclItem_toConfigData(m_nCurDlgId,info[2].c_str(),sItemName,info[3].c_str(),atoi(info[1].c_str())/8,m_nProtocolId,m_nProtoclItem_Index,m_nDriveId,m_nSysDriveId,"",
//			nItem_ConfigId,nItem_SharedId))
//		{
//			XProtocol_Funtion::Save_Data_Sets(m_nProtocolId,nParent_ConfigId,nParent_SharedId,nItem_ConfigId,nItem_SharedId);
//			xmlChild.AddAttrib("Shared",nItem_SharedId);
//			m_nProtoclItem_Index++;
//		}*/
//		//保存到共享内存表
//		xmlItem.AddChildSubDoc(xmlChild.GetSubDoc());
//	}	 
//	 xml.AddChildSubDoc(xmlItem.GetSubDoc());
//}
// 
bool CDlg1553::CheckData()
{
	bool bRe =true;
	CString sTxt;

	//接收/发送
	int nRow,nCol;
	nRow =0;
	nCol=3;
	sTxt = m_grid.GetItemText(nRow,nCol);
	SetGridItemColor(m_grid,nRow,nCol,sTxt!="接收" && sTxt!="发送");
	  
	//RT地址
	nRow =2;
	nCol=1;
	sTxt = m_grid.GetItemText(nRow,nCol);
	int nRtAddr = atoi(sTxt);
	SetGridItemColor(m_grid,nRow,nCol,nRtAddr<0 ||nRtAddr>=32);
 
	//RT地址
	nCol=3;
	sTxt = m_grid.GetItemText(nRow,nCol);
	int nSubRtAddr = atoi(sTxt);
	SetGridItemColor(m_grid,nRow,nCol,nSubRtAddr<0 ||nSubRtAddr>=32);

	//数据单位
	nRow =2;
	nCol=1;
	sTxt = m_grid.GetItemText(nRow,nCol);
	sTxt.MakeUpper();
	SetGridItemColor(m_grid,nRow,nCol,sTxt!="BYTE" && sTxt!="BIT");

	//数据长度
	nRow =2;
	nCol=1;
	sTxt = m_grid.GetItemText(nRow,nCol);
	int nDataLen = atoi(sTxt);
	SetGridItemColor(m_grid,nRow,nCol,nDataLen<0 );

	//其他的基础数据暂时不检查

	//协议项
	 
	m_grid.Invalidate();

	return true;
}
 
 
 