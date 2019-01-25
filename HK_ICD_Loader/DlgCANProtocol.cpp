// DlgCANProtocol.cpp : 实现文件
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "DlgCANProtocol.h"
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

// CDlgCANProtocol 对话框

IMPLEMENT_DYNAMIC(CDlgCANProtocol, CDialogEx)

CDlgCANProtocol::CDlgCANProtocol(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCANProtocol::IDD, pParent)
{
	m_nProtocolId = 0;
	m_nProtoclItem_Index = 0;
	m_nPulicProId = 0xffff;
}

CDlgCANProtocol::~CDlgCANProtocol()
{
}

void CDlgCANProtocol::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_protocol, m_combo_protocol);
	DDX_Control(pDX, IDC_CUSTOM1, m_grid);
	DDX_Control(pDX, IDC_COMBO_DRIVE, m_comboDrive);
}


BEGIN_MESSAGE_MAP(CDlgCANProtocol, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SelectFile, &CDlgCANProtocol::OnBnClickedButtonSelectfile)
	ON_BN_CLICKED(IDC_BUTTON_Read, &CDlgCANProtocol::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgCANProtocol::OnBnClickedButtonSave)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgCANProtocol 消息处理程序


void CDlgCANProtocol::OnBnClickedButtonSelectfile()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
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


void CDlgCANProtocol::OnBnClickedButtonRead()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_combo_protocol.GetCurSel();
	if(nSel ==-1)
		return;
	vector<vector<string>>Basevec ;
	CString str;

	m_nGirdColCount =12;//9;

	m_excelFile.ReadInfo_Sub_Item(nSel+1,Basevec,1,-1,12);

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


void CDlgCANProtocol::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
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

	strSql.Format("delete from t_CAN_protocol;");
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
		//邮箱地址
		CString sMailBoxId = m_grid.GetItemText(i,4);
		char   szValue[10] = {0};   
		strcpy_s(szValue,sMailBoxId.GetBuffer());
		int nMailBoxId = 0;
		sscanf(szValue,"%x",&nMailBoxId);   

		//标识符长度
		int nMaskLen = _tstoi(m_grid.GetItemText(i,5));
		//帧类型
		CString sFrameType = m_grid.GetItemText(i,6);
		//帧格式
		CString sFrameFormat = m_grid.GetItemText(i,7);
		//帧长度
		int nFrameLen = _tstoi(m_grid.GetItemText(i,8));
		//CAN规范
		CString sCanStandard = m_grid.GetItemText(i,9);
		//速率
		CString sRate = m_grid.GetItemText(i,10);
		//接收/发送
		CString  sProtocolRecSend= m_grid.GetItemText(i,11);

		CString strXml = "";

		if(sBlockType == "ITEM" /*&& false*/)
		{
			int nDataLen = 0;

			//删除历史记录，并获得协议表中的nId
			int nRecSend = sProtocolRecSend==PROTOCOL_RecSend_Rec?1:2;	// 1=接收；2=发送
			XProtocol_Funtion::DeleteProtoclRecod(m_nPulicProId/*m_nCurDlgId*/,sProtocolNo,m_nDriveId,m_nSysDriveId,nRecSend,1,m_nProtocolId,"t_CAN_protocol");

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
			m_excelFile.ReadInfo_Sub_Item(nTabIndex+1,vec,1,-1);

			if(vec.size()==0)
				continue;

			//解析协议属性
			CMarkup xml;

			//协议名称;名称
			xml.AddElem(XML_Attr);	

			nProtocol_SharedId =  (m_nDriveId<<24) +(m_nSysDriveId<<16) +(m_nProtocolId<<8);
			//nConfigId = GetNewConfigDatId();

			//共享内存值
			xml.AddAttrib(XML_Shared,nProtocol_SharedId);

			//协议子项
			CMarkup xmlItem;
			xmlItem.AddElem(XML_SubItems);

			//显示项作为子项
			vector<SUBITEMATTR> vtSubItem;
			int nSubNum = vec.size();

			//解析起始位，位长及符号位
			bool bFindStart = false;
			bool bFindEnd = false;
			SUBITEMATTR SubItemData;
			int nStartBit = 0;
			int nStartRow = -1;
			for(int j = 0; j < nSubNum; ++ j)
			{
				if(vec[j][2] == "位数")
				{
					nStartRow = j+1;
					break;
				}
			}
			// 如果没找到“位数”字符，则处理下一条协议
			if(nStartRow == -1) continue;
			for(int j = nStartRow; nStartRow < nSubNum; ++ j)
			{
				sTxt = vec[j][2].c_str();
				if(sTxt == "偏置补偿")
					break;
				int nBitNum = _tstoi(sTxt);

				nStartBit %= 16;
				if(nStartBit == 0)
					nDataLen += 2;
				SubItemData.nStartPosi = nStartBit;
				SubItemData.nItemLen = nBitNum;
				SubItemData.bIsSign = 0;
				vtSubItem.push_back(SubItemData);
				nStartBit += nBitNum;
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
					// 代码不用加入
					vtSubItem[nIdx].sRemark = vec[j][6];
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

				xmlItem.AddChildSubDoc(xmlSub.GetSubDoc());
			}

			xml.AddChildSubDoc(xmlItem.GetSubDoc());

			strXml = xml.GetSubDoc();

			//创建对应协议的共享内存
			XProtocol_Funtion::Save_Protocl_toConfigData(m_nPulicProId/*m_nCurDlgId*/,sProtocolName,m_nProtocolId,m_nDriveId,m_nSysDriveId,
				sProtocolRecSend,nDataLen,nProtocol_ConfigId,nProtocol_SharedId);
		}

		//  int nDlgId, 
		//	int nProtocolId,			//主协议ID
		//	CString sRecSend,			//接收/发送
		//	CString sName,				//协议名称
		//	int nSysDriveId,			//系统驱动ID
		//	CString sInfo,				//协议属性（xml格式文本）
		//	CString sBlockName,			//数据块名称
		//	CString sProtocolNo,		//规范号
		//	CString sBlockType,			//数据块类型
		//	int nMailAddr,				//邮箱地址
		//	int nMaskLen,				//标识符长度
		//	CString sFrameType,			//帧类型
		//	CString sFrameFormat,		//帧格式
		//	int nFrameLen,				//帧长度
		//	CString sCanStandard,		//CAN规范
		//	CString sRate,);			//速率


		XProtocol_Funtion::SaveCANProtocol(
			m_nPulicProId/*m_nCurDlgId*/,
			m_nProtocolId,
			sProtocolRecSend,
			sProtocolName,
			m_nSysDriveId,
			strXml,
			sBlockName,			//数据块名称
			sProtocolNo,		//规范号
			sBlockType,			//数据块类型
			nMailBoxId,			//邮箱地址
			nMaskLen,			//标识符长度
			sFrameType,			//帧类型
			sFrameFormat,		//帧格式
			nFrameLen,			//帧长度
			sCanStandard,		//CAN规范 
			sRate);				//速率
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

// CAN协议子项存入共享内存数据库
void CDlgCANProtocol::SaveProtocolItemConfig(void)
{
	int nItem_ConfigId;
	int nItem_SharedId;

	vector<__CAN_Protocol> vec;
	CManageDb_Sqllite::GetInstance().Query_CAN_All_Protocol(vec);

	int nProNum = vec.size();
	for(int i = 0; i < nProNum; ++ i)
	{
		__CAN_Protocol& ProItem = vec[i];

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
					//sDispName = xml3.GetAttrib(XML_DispName);
					//sDispValue = xml3.GetAttrib(XML_DispValue);
					//sDispStyle = xml3.GetAttrib(XML_DispStyle);

					CString sRecSend = ProItem.nRecOrSend == 1 ? "接收":"发送";

					CString sItemDispName = xml3.GetAttrib(XML_Item_DispName);
					int nSubInforLength = _tstoi(xml3.GetAttrib(XML_Item_Length));
					CString sDefaultValue = "";
					float fCoefficient = _tstof(xml3.GetAttrib(XML_Item_Coefficient));
					float fOffset = _tstof(xml3.GetAttrib(XML_Item_Offset));
					CString sMark = xml3.GetAttrib(XML_Item_Mark);

					XProtocol_Funtion::Save_ProtoclItem_toConfigData(m_nPulicProId/*m_nCurDlgId*/,sRecSend,
						sProtocoName+"."+sItemDispName+"."+sRecSend,DATA_TYPE_INT_NAME,nSubInforLength/8,
						ProItem.nId,m_nProtoclItem_Index,
						m_nDriveId,m_nSysDriveId,sDefaultValue,fCoefficient,fOffset,sMark,
						nItem_ConfigId,nItem_SharedId);
					m_nProtoclItem_Index++;
				}
			}  
		}
	}
}


void CDlgCANProtocol::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


BOOL CDlgCANProtocol::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strWhere;
	strWhere.Format("and nDriveId in(  select nid from t_drive where smark ='CAN')");

	CManageDb_Sqllite::GetInstance().Query_SysDrive(m_DriveVec,m_nCurSysId,strWhere.GetBuffer());

	int nIndex;
	for(int i =0,end =m_DriveVec.size();i<end;i++)
	{
		nIndex =m_comboDrive.AddString(m_DriveVec[i].sName.c_str());		

	}
	m_comboDrive.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
