// DlgCANProtocol.cpp : ʵ���ļ�
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
	int nStartPosi;				//��ʼλ
	int nItemLen;				//λ��
	string sItemName;			//��ʾ����
	int nInforType;				//�ź�����
	float fOffset;				//ƫ�ò���
	float fCoef;				//ϵ������
	string sDispValue;			//��ʾֵ��ת���ã�
	string sRemark;				//ע��
}SUBITEMATTR,*pSUBITEMATTR;

// CDlgCANProtocol �Ի���

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


// CDlgCANProtocol ��Ϣ�������


void CDlgCANProtocol::OnBnClickedButtonSelectfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sTxt;
	sTxt.Format("�Ƿ�Э�鱣�浽\"%s\"",m_sCurDlgName);

	if(AfxMessageBox(sTxt,MB_YESNO) == IDNO)
		return;

	CWaitCursor wait;
	int nSel = m_comboDrive.GetCurSel();
	if(nSel==-1)
		return;

	//m_nProtoclItem_Index =1;
	m_nDriveId = m_DriveVec[nSel].nDriveId;
	m_nSysDriveId =m_DriveVec[nSel].nId;

	//��Ӧt_data_configID��nSharedId
	int nProtocol_ConfigId;
	int nProtocol_SharedId;
	//Э������

	// ɾ�������ڴ���ʱ������
	CString strSql;

	strSql.Format("delete from t_data_config_1;");
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	strSql.Format("delete from t_CAN_protocol;");
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	int nRow =1;
	for(long i = nRow,end = m_grid.GetRowCount(); i < end /*&& false*/; ++ i)
	{
		//m_nProtoclItem_Index = 1;

		//���ݿ�����
		CString sBlockName = m_grid.GetItemText(i,0);
		//�淶��
		CString sProtocolNo = m_grid.GetItemText(i,1);
		//Э������
		CString sProtocolName = m_grid.GetItemText(i,2);
		//���ݿ�����
		CString sBlockType = m_grid.GetItemText(i,3);
		//�����ַ
		CString sMailBoxId = m_grid.GetItemText(i,4);
		char   szValue[10] = {0};   
		strcpy_s(szValue,sMailBoxId.GetBuffer());
		int nMailBoxId = 0;
		sscanf(szValue,"%x",&nMailBoxId);   

		//��ʶ������
		int nMaskLen = _tstoi(m_grid.GetItemText(i,5));
		//֡����
		CString sFrameType = m_grid.GetItemText(i,6);
		//֡��ʽ
		CString sFrameFormat = m_grid.GetItemText(i,7);
		//֡����
		int nFrameLen = _tstoi(m_grid.GetItemText(i,8));
		//CAN�淶
		CString sCanStandard = m_grid.GetItemText(i,9);
		//����
		CString sRate = m_grid.GetItemText(i,10);
		//����/����
		CString  sProtocolRecSend= m_grid.GetItemText(i,11);

		CString strXml = "";

		if(sBlockType == "ITEM" /*&& false*/)
		{
			int nDataLen = 0;

			//ɾ����ʷ��¼�������Э����е�nId
			int nRecSend = sProtocolRecSend==PROTOCOL_RecSend_Rec?1:2;	// 1=���գ�2=����
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

			//����Э������
			CMarkup xml;

			//Э������;����
			xml.AddElem(XML_Attr);	

			nProtocol_SharedId =  (m_nDriveId<<24) +(m_nSysDriveId<<16) +(m_nProtocolId<<8);
			//nConfigId = GetNewConfigDatId();

			//�����ڴ�ֵ
			xml.AddAttrib(XML_Shared,nProtocol_SharedId);

			//Э������
			CMarkup xmlItem;
			xmlItem.AddElem(XML_SubItems);

			//��ʾ����Ϊ����
			vector<SUBITEMATTR> vtSubItem;
			int nSubNum = vec.size();

			//������ʼλ��λ��������λ
			bool bFindStart = false;
			bool bFindEnd = false;
			SUBITEMATTR SubItemData;
			int nStartBit = 0;
			int nStartRow = -1;
			for(int j = 0; j < nSubNum; ++ j)
			{
				if(vec[j][2] == "λ��")
				{
					nStartRow = j+1;
					break;
				}
			}
			// ���û�ҵ���λ�����ַ���������һ��Э��
			if(nStartRow == -1) continue;
			for(int j = nStartRow; nStartRow < nSubNum; ++ j)
			{
				sTxt = vec[j][2].c_str();
				if(sTxt == "ƫ�ò���")
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
			//���ֶε���ʾ��Ϣ
			bool bFindDisp = false;
			int nSubItemNum = vtSubItem.size();
			int nIdx = 0;
			for(int j = 0; j < nSubNum && nIdx < nSubItemNum; ++ j)
			{
				sTxt = vec[j][0].c_str();
				if(!bFindDisp)
				{
					if(!sTxt.Compare("�ź���ʾ����"))
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
					// ���벻�ü���
					vtSubItem[nIdx].sRemark = vec[j][6];
					++ nIdx;
				}
			}

			// SubItem��Ϣ����XML�ļ�
			for(int j = 0; j < nSubItemNum; ++ j)
			{
				CMarkup xmlSub;
				xmlSub.AddElem(XML_Item_Item);

				SUBITEMATTR& ItemData = vtSubItem[j];
				//��ʼ��ַ
				sTxt.Format("%d",ItemData.nStartPosi);
				xmlSub.AddAttrib(XML_Item_Addr,sTxt);
				//λ��
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

				//������ʾ������
				sTxt = ItemData.sItemName.c_str();
				xmlSub.AddAttrib(XML_Item_DispName,sTxt);

				//��ʾ�����ͣ����ı�����Ͽ򣬶�ѡ��ȣ�
				sTxt.Format("%d",ItemData.nInforType);
				xmlSub.AddAttrib(XML_Item_DispStyle,sTxt);

				//������ʾ��ֵ
				sTxt = ItemData.sDispValue.c_str();
				xmlSub.AddAttrib(XML_Item_DispValue,sTxt);

				//����λ
				sTxt.Format("%d",int(ItemData.bIsSign));
				xmlSub.AddAttrib(XML_Item_Sign,sTxt);

				sTxt =ItemData.sRemark.c_str();
				xmlSub.AddAttrib(XML_Item_Mark,sTxt);

				xmlItem.AddChildSubDoc(xmlSub.GetSubDoc());
			}

			xml.AddChildSubDoc(xmlItem.GetSubDoc());

			strXml = xml.GetSubDoc();

			//������ӦЭ��Ĺ����ڴ�
			XProtocol_Funtion::Save_Protocl_toConfigData(m_nPulicProId/*m_nCurDlgId*/,sProtocolName,m_nProtocolId,m_nDriveId,m_nSysDriveId,
				sProtocolRecSend,nDataLen,nProtocol_ConfigId,nProtocol_SharedId);
		}

		//  int nDlgId, 
		//	int nProtocolId,			//��Э��ID
		//	CString sRecSend,			//����/����
		//	CString sName,				//Э������
		//	int nSysDriveId,			//ϵͳ����ID
		//	CString sInfo,				//Э�����ԣ�xml��ʽ�ı���
		//	CString sBlockName,			//���ݿ�����
		//	CString sProtocolNo,		//�淶��
		//	CString sBlockType,			//���ݿ�����
		//	int nMailAddr,				//�����ַ
		//	int nMaskLen,				//��ʶ������
		//	CString sFrameType,			//֡����
		//	CString sFrameFormat,		//֡��ʽ
		//	int nFrameLen,				//֡����
		//	CString sCanStandard,		//CAN�淶
		//	CString sRate,);			//����


		XProtocol_Funtion::SaveCANProtocol(
			m_nPulicProId/*m_nCurDlgId*/,
			m_nProtocolId,
			sProtocolRecSend,
			sProtocolName,
			m_nSysDriveId,
			strXml,
			sBlockName,			//���ݿ�����
			sProtocolNo,		//�淶��
			sBlockType,			//���ݿ�����
			nMailBoxId,			//�����ַ
			nMaskLen,			//��ʶ������
			sFrameType,			//֡����
			sFrameFormat,		//֡��ʽ
			nFrameLen,			//֡����
			sCanStandard,		//CAN�淶 
			sRate);				//����
	}

	SaveProtocolItemConfig();

	//CString strSql = "";
	strSql.Format("insert into t_data_config select * from t_data_config_1");
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	// �����źŵĹ����ڴ浥Ԫ
	//XProtocol_Funtion::Save_ProtoclItem_toConfigData(m_nCurDlgId,sProtocolRecSend,sProtocolName+"."+sProtocoItemName,sDataType,atoi(sDataLength),m_nProtocolId,m_nProtoclItem_Index,
	//	m_nDriveId,m_nSysDriveId,sDefaultValue,fCoefficient,fOffset,sMark,
	//	nItem_ConfigId,nItem_SharedId);
}

// CANЭ��������빲���ڴ����ݿ�
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
		// ��Э�鴦��
		if(ProItem.sBlockType == "ITEM")
		{
			m_nProtoclItem_Index = 1;		// Э�������1��ʼ���

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
					//��ʾ���ź�����
					//sDispName = xml3.GetAttrib(XML_DispName);
					//sDispValue = xml3.GetAttrib(XML_DispValue);
					//sDispStyle = xml3.GetAttrib(XML_DispStyle);

					CString sRecSend = ProItem.nRecOrSend == 1 ? "����":"����";

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

	// TODO: �ڴ˴������Ϣ����������
}


BOOL CDlgCANProtocol::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}
