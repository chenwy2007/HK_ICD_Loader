// Dlg1553.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "Dlg1553.h"
#include "afxdialogex.h"


// CDlg1553 �Ի���

IMPLEMENT_DYNAMIC(CDlg1553, CDialogEx)

CDlg1553::CDlg1553(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg1553::IDD, pParent)
{

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


// CDlg1553 ��Ϣ�������


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
	//CString strItem[]={"��ʼλ","����","Э��������","��������","ϵ��","����ֵ","����","Ĭ��ֵ","��ע"};
	//CString strItem[]={"���ݿ�����","�淶��","�ź�����","����","AT��ַ","AT�ӵ�ַ","����/����","���ߺ�"};
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
	sTxt.Format("�Ƿ�Э�鱣�浽\"%s\"",m_sCurDlgName);
	
	if(AfxMessageBox(sTxt,MB_YESNO) == IDNO)
		return;;

	CWaitCursor wait;
	int nSel = m_comboDrive.GetCurSel();
	if(nSel==-1)
		return;

	m_nProtoclItem_Index =1;
	m_nDriveId = m_DriveVec[nSel].nDriveId;
	m_nSysDriveId =m_DriveVec[nSel].nId;

	//��Ӧt_data_configID��nSharedId
	int nProtocol_ConfigId;
	int nProtocol_SharedId;
	//Э������

	int nRow =1;
	for(long i = nRow,end = m_grid.GetRowCount(); i < end; ++ i)
	{

	//���ݿ�����
	CString sBlockName = m_grid.GetItemText(nRow,0);
	//�淶��
	CString sProtocolNo = m_grid.GetItemText(nRow,1);
	//Э������
	CString sProtocolName = m_grid.GetItemText(nRow,2);
	//���ݿ�����
	CString sBlockType = m_grid.GetItemText(nRow,3);
	//AT��ַ
	int nATAddr = _tstoi(m_grid.GetItemText(nRow,4));
	//AT�ӵ�ַ
	int nATsuAddr = _tstoi(m_grid.GetItemText(nRow,5));
	//����/����
	CString  sProtocolRecSend= m_grid.GetItemText(nRow,6);
	//��������
	CString sBusType= m_grid.GetItemText(nRow,7);

	if(sBlockType == "ITEM")
	{
		int nDataLen = 0;

		//ɾ����ʷ��¼�������Э����е�nId
		XProtocol_Funtion::DeleteProtoclRecod(m_nCurDlgId,sProtocolName,m_nDriveId,m_nSysDriveId,1,m_nProtocolId);

		vector<vector<string>>vec;
		m_config_1553.ReadInfo_Sub_Item(1+1,vec,1,-1);

		if(vec.size()==0)
			continue;


		//������ӦЭ��Ĺ����ڴ�
		XProtocol_Funtion::Save_Protocl_toConfigData(m_nCurDlgId,sProtocolName,m_nProtocolId,m_nDriveId,m_nSysDriveId,sProtocolRecSend,nDataLen,nProtocol_ConfigId,nProtocol_SharedId);

	}
	else if(sBlockType == "DATA")
	{

	}
	}

	CString sProtocolName;
	CString  sProtocolRecSend;
	CMarkup xml;

	//Э������;����
	xml.AddElem(XML_Attr);	
	xml.AddAttrib(XML_Name,sProtocolName);

	//����
	sTxt = m_grid.GetItemText(nRow,2);	 
	xml.AddAttrib(XML_Describe,sTxt);

	xml.AddAttrib(XML_RecSend,sProtocolRecSend);

	//���ͻ���
	sTxt= m_grid.GetItemText(nRow,4);	 
	xml.AddAttrib(XML_SendMode,sTxt);

	//ˢ��ʱ��(ms)
	sTxt= m_grid.GetItemText(nRow,5);	 
	xml.AddAttrib(XML_RefreshTime,sTxt);

	//У��λ
	sTxt= m_grid.GetItemText(nRow,6);
	xml.AddAttrib(XML_CheckMode,sTxt);

	//���ݵ�λ
	sTxt= m_grid.GetItemText(nRow,7);	 
	xml.AddAttrib(XML_DataUnit,sTxt);

	//���ݳ���
	sTxt= m_grid.GetItemText(nRow,8);	 
	xml.AddAttrib(XML_DataLen,sTxt);

	nRow =3;

	//��ǰֻ��һ������
	if(sProtocolRecSend==PROTOCOL_RecSend_Rec)
	{
		//Ӧ������ ,����ֵ,Ӧ��Э��
		sTxt= m_grid.GetItemText(nRow,0);	 
		xml.AddAttrib(XML_Reply_Condition,sTxt);
		sTxt= m_grid.GetItemText(nRow,1);	 
		xml.AddAttrib(XML_Reply_Value,sTxt);
		sTxt= m_grid.GetItemText(nRow,2);	 
		xml.AddAttrib(XML_Reply_protocl,sTxt);
	}	 

	xml.AddAttrib(XML_RtAddr,m_grid.GetItemText(nRow,3));
	xml.AddAttrib(XML_RtSubAddr,m_grid.GetItemText(nRow,4));
	xml.AddAttrib(XML_Interrupt,m_grid.GetItemText(nRow,5));
	xml.AddAttrib(XML_SourceEquipment,m_grid.GetItemText(nRow,6));
	xml.AddAttrib(XML_DestEquipment,m_grid.GetItemText(nRow,7));
	 

	//�����ڴ�ֵ
	xml.AddAttrib(XML_Shared,nProtocol_SharedId);

	int nSubRow =7;

	CString sChildUnit;

	//Э������
	CMarkup xmlItem;
	xmlItem.AddElem(XML_SubItems);

	int n=0;
	CString sItemType;
	CString sDataType;
	CString sProtocoItemName;
	CString sDataLength;
	int nItem_ConfigId;
	int nItem_SharedId;

	CString sDefaultValue;
	CString sMark;
	float fCoefficient;
	float fOffset;
	for(int k=10,end = m_grid.GetRowCount();k<end;k++)
	{	
		n=0;
		sTxt =m_grid.GetItemText(k,n++);
		if(sTxt=="")
			continue;

		CMarkup xmlChild;
		xmlChild.AddElem(XML_Item_Item);

		xmlChild.AddAttrib(XML_Item_Addr,sTxt);
		sDataLength =m_grid.GetItemText(k,n++);
		xmlChild.AddAttrib(XML_Item_Length,sDataLength);

		sItemType = m_grid.GetItemText(k,n++);
		xmlChild.AddAttrib(XML_Item_Item_type,sItemType);

		sDataType=m_grid.GetItemText(k,n++);
		xmlChild.AddAttrib(XML_Item_Data_type,sDataType);

		sTxt = m_grid.GetItemText(k,n++);
		xmlChild.AddAttrib(XML_Item_Coefficient,sTxt);
		fCoefficient = atof(sTxt.GetBuffer());

		sTxt = m_grid.GetItemText(k,n++);
		xmlChild.AddAttrib(XML_Item_Offset,sTxt);
		fOffset = atof(sTxt.GetBuffer());

		sProtocoItemName = m_grid.GetItemText(k,n++);
		xmlChild.AddAttrib(XML_Name,sProtocoItemName);

		sDefaultValue =m_grid.GetItemText(k,n++);
		xmlChild.AddAttrib(XML_Item_Default,sDefaultValue);

		sMark =m_grid.GetItemText(k,n++);
		xmlChild.AddAttrib(XML_Item_Mark,sMark);

		if(sItemType.MakeUpper()==PROTOCOL_Item_Item)
		{
		 	WriteSubInfoToXml(xmlChild,sProtocolName,sProtocoItemName,sProtocolRecSend,nProtocol_ConfigId,nProtocol_SharedId);
		}
		else if(sItemType.MakeUpper()==PROTOCOL_Item_Data)
		{
				if(!sProtocoItemName.IsEmpty() && XProtocol_Funtion::Save_ProtoclItem_toConfigData(m_nCurDlgId,sProtocolRecSend,sProtocolName+"."+sProtocoItemName,sDataType,atoi(sDataLength),m_nProtocolId,m_nProtoclItem_Index,
					m_nDriveId,m_nSysDriveId,sDefaultValue,fCoefficient,fOffset,sMark,
					nItem_ConfigId,nItem_SharedId))
				{
					XProtocol_Funtion::Save_Data_Sets(m_nProtocolId,nProtocol_ConfigId,nProtocol_SharedId,nItem_ConfigId,nItem_SharedId);
					xmlChild.AddAttrib("Shared",nItem_SharedId);
					m_nProtoclItem_Index++;				
				}
			 
		}

		xmlItem.AddChildSubDoc(xmlChild.GetSubDoc());

	}

	xml.AddChildSubDoc(xmlItem.GetSubDoc());

	CString strXml = xml.GetSubDoc();

	XProtocol_Funtion::SaveProtocol(m_nCurDlgId,m_nProtocolId,sProtocolRecSend,sProtocolName,m_nSysDriveId,strXml);
 
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

		//����Ļ�����Ϣ�����ݵ�λ�����ݳ���
		vector<string>SubVec;
		m_config_1553.Read_One_RowInfo(nIndex+1,SubVec,2,4,2);
		if(SubVec.size()!=2)
			return ;

		//��������ݵ�λ�����ݳ���
		xml.AddAttrib(XML_DataUnit,SubVec[0].c_str());
		xml.AddAttrib(XML_DataLen,SubVec[1].c_str());

		vector<vector<string>>vec;
		m_config_1553.ReadInfo_Sub_Item(nIndex+1,vec);

		if(vec.size()==0)
			return;

		//Э������
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

			//��ʼλ������
			xmlChild.AddAttrib(XML_Item_Addr,info[0].c_str());
			xmlChild.AddAttrib(XML_Item_Length,info[1].c_str());

			//Э��������,��������		
			xmlChild.AddAttrib(XML_Item_Item_type,info[2].c_str());
			xmlChild.AddAttrib(XML_Item_Data_type,info[3].c_str());

			//ϵ��,����ֵ
			xmlChild.AddAttrib(XML_Item_Coefficient,info[4].c_str());
			xmlChild.AddAttrib(XML_Item_Offset,info[5].c_str());

			//���ơ�Ĭ��ֵ����ע
			xmlChild.AddAttrib(XML_Name,info[6].c_str());
			xmlChild.AddAttrib(XML_Item_Default,info[7].c_str());
			xmlChild.AddAttrib(XML_Item_Mark,info[8].c_str());

			//Э�������� ��DATA	,���ͣ� 
			if(CString(info[2].c_str()).MakeUpper() ==PROTOCOL_Item_Data)
			{
				if(!info[6].empty()) //����
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
//	//����Ļ�����Ϣ�����ݵ�λ�����ݳ���
//	vector<string>SubVec;
//	m_config_1553.ReadInfo_Item_SubInfo(nIndex+1,SubVec);
//	if(SubVec.size()<3)
//		return;
//
//	//��������ݵ�λ�����ݳ���
//	xml.AddAttrib("DataUnit",SubVec[0].c_str());
//	xml.AddAttrib("DataLen",SubVec[1].c_str());
//	 
//	vector<vector<string>>vec;
//	m_config_1553.ReadInfo_Sub_Item(nIndex+1,vec);
//	
//	if(vec.size()==0)
//		return;
//
//	//Э������
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
//		//���浽�����ڴ��
//		xmlItem.AddChildSubDoc(xmlChild.GetSubDoc());
//	}	 
//	 xml.AddChildSubDoc(xmlItem.GetSubDoc());
//}
// 
bool CDlg1553::CheckData()
{
	bool bRe =true;
	CString sTxt;

	//����/����
	int nRow,nCol;
	nRow =0;
	nCol=3;
	sTxt = m_grid.GetItemText(nRow,nCol);
	SetGridItemColor(m_grid,nRow,nCol,sTxt!="����" && sTxt!="����");
	  
	//RT��ַ
	nRow =2;
	nCol=1;
	sTxt = m_grid.GetItemText(nRow,nCol);
	int nRtAddr = atoi(sTxt);
	SetGridItemColor(m_grid,nRow,nCol,nRtAddr<0 ||nRtAddr>=32);
 
	//RT��ַ
	nCol=3;
	sTxt = m_grid.GetItemText(nRow,nCol);
	int nSubRtAddr = atoi(sTxt);
	SetGridItemColor(m_grid,nRow,nCol,nSubRtAddr<0 ||nSubRtAddr>=32);

	//���ݵ�λ
	nRow =2;
	nCol=1;
	sTxt = m_grid.GetItemText(nRow,nCol);
	sTxt.MakeUpper();
	SetGridItemColor(m_grid,nRow,nCol,sTxt!="BYTE" && sTxt!="BIT");

	//���ݳ���
	nRow =2;
	nCol=1;
	sTxt = m_grid.GetItemText(nRow,nCol);
	int nDataLen = atoi(sTxt);
	SetGridItemColor(m_grid,nRow,nCol,nDataLen<0 );

	//�����Ļ���������ʱ�����

	//Э����
	 
	m_grid.Invalidate();

	return true;
}
 
 
 