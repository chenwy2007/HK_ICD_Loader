// Dlg429.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "Dlg429.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlg429 �Ի���

IMPLEMENT_DYNAMIC(CDlg429, CDialogEx)

CDlg429::CDlg429(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg429::IDD, pParent)
{

}

CDlg429::~CDlg429()
{
}

void CDlg429::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_protocol, m_combo_protocol);
	DDX_Control(pDX, IDC_CUSTOM1, m_grid);
	DDX_Control(pDX, IDC_COMBO_DRIVE, m_comboDrive);
	 
}


BEGIN_MESSAGE_MAP(CDlg429, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SelectFile, &CDlg429::OnBnClickedButtonSelectfile)
 
	ON_BN_CLICKED(IDC_BUTTON_Read, &CDlg429::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlg429::OnBnClickedButtonSave)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlg429 ��Ϣ�������


void CDlg429::OnBnClickedButtonSelectfile()
{
	UpdateData();
	CString strFilePath;
	CFileDialog dlg(TRUE);

	if(dlg.DoModal()!=IDOK)
		return;
	strFilePath = dlg.GetPathName(); 
	GetDlgItem(IDC_EDIT_file_path)->SetWindowText(strFilePath);
	 
	m_combo_protocol.ResetContent();

	vector<CString>vec;
	m_config_429.OpenFile(strFilePath,vec);

	for(int i =0,end =vec.size();i<end;i++)
		m_combo_protocol.AddString(vec[i]);

	m_combo_protocol.SetCurSel(0);
}


BOOL CDlg429::OnInitDialog()
{
	CDialogEx::OnInitDialog();
		 
	CString strWhere;
	strWhere.Format("and nDriveId in(  select nid from t_drive where smark ='429')");
	 
	CManageDb_Sqllite::GetInstance().Query_SysDrive(m_DriveVec,m_nCurSysId,strWhere.GetBuffer());

	int nIndex;
	for(int i =0,end =m_DriveVec.size();i<end;i++)
	{
		nIndex =m_comboDrive.AddString(m_DriveVec[i].sName.c_str());		
		
	}
	m_comboDrive.SetCurSel(0);
	
	return TRUE;  
}

void CDlg429::OnBnClickedButtonRead()
{

	int nSel = m_combo_protocol.GetCurSel();
	if(nSel ==-1)
		return;
	m_grid.DeleteAllItems();
	m_grid.Invalidate();
	vector<vector<string>>vec;	
	m_config_429.ReadInfo_Sub_Item(nSel+1,vec,1,-1);
	if(vec.size()<0 ||vec[1].size()<0 || vec[0][1]!="429")
	{
		AfxMessageBox("���ݱ�־λ���󣬲�Ϊ429");
		 return ;
	}
	 
	int nColCount =9;
	CRect rt;
	m_grid.GetClientRect(rt);
	int nW =rt.Width()/nColCount;

	m_grid.SetEditable(TRUE);
	m_grid.SetDefCellHeight(20);
	m_grid.SetColumnCount(nColCount);
	m_grid.SetRowCount(vec.size()+1);

	for(int i =0;i<nColCount;i++)
	{
		m_grid.SetColumnWidth(i,nW-2);
	}
	for(int k =0,end =vec.size();k<end;k++)
	{
		vector<string>&info = vec[k];
		for(int i =0,end1 = info.size();i<nColCount &&i<end1 ;i++)
		{
			SetGirdItemText(m_grid,k,i,info[i].c_str());	
		}
	} 
}
 

BOOL CDlg429::CheckData()
{
	bool bRe =true;
	CString sTxt = m_grid.GetItemText(1,0);
	int nChannel =atoi(sTxt);
	if(sTxt=="" ||nChannel<-1 ||  nChannel>15)
		m_grid.SetItemBkColour(0,5,Color_ERR);
	else
		m_grid.SetItemBkColour(0,5,RGB(255,255,255));

	//����/����
	sTxt = m_grid.GetItemText(1,3);
	if(sTxt!="����" && sTxt!="����")
		m_grid.SetItemBkColour(0,5,Color_ERR);
	else
		m_grid.SetItemBkColour(0,5,RGB(255,255,255));

	if(sTxt =="����")
	{
		sTxt = m_grid.GetItemText(1,5);
		if(sTxt!="��ʱ" && sTxt!="ָ��" && sTxt!="ָ�ʱ")
		{
			m_grid.SetItemBkColour(0,5,Color_ERR);
			bRe =false;
		}
		else
			m_grid.SetItemBkColour(0,5,RGB(255,255,255));
	}

	sTxt = m_grid.GetItemText(2,5);

	if(sTxt!="��У��" && sTxt!="��У��" && sTxt!="żУλ")
	{
		m_grid.SetItemBkColour(0,5,Color_ERR);
		bRe =false;
	}
	else
		m_grid.SetItemBkColour(0,5,RGB(255,255,255));

	int nRow =10;
	int nPos=0,nLastPast =0,nlen=0;
	bool bItemType;
	CString strDataType[]={"label","data","parity"};

	//��ַ�Ϸ��Լ��
	for(int i =nRow,end = m_grid.GetRowCount();i<end;i++)
	{

		sTxt = m_grid.GetItemText(i,0);

		if(sTxt=="")
			break;
		nPos =atoi(sTxt);
		if(nPos != nLastPast+nlen)
		{
			m_grid.SetItemBkColour(i,0,Color_ERR);
			bRe =false;
			break;
		}
		else 
			m_grid.SetItemBkColour(i,0,RGB(255,255,255));
		sTxt = m_grid.GetItemText(i,1);
		nlen = atoi(sTxt);

		nLastPast = nPos;
		bItemType =false;

		sTxt = m_grid.GetItemText(i,2);
		sTxt.MakeLower();
		sTxt.Trim();
		for(int k =0;k<3;k++)
		{
			if(sTxt==strDataType[k])
			{
				bItemType =true;
				break;
			}
		}
		if(!bItemType)
		{
			m_grid.SetItemBkColour(i,2,Color_ERR);
				bRe =false;

		}
		else 
			m_grid.SetItemBkColour(i,2,RGB(255,255,255));

	}	
	if((nPos +nlen)!=32)
	{
		AfxMessageBox("���ݳ�������������32");
		bRe=false;
	}
	m_grid.Invalidate();
	int nSel = m_comboDrive.GetCurSel();
	if(nSel ==-1)
	{
		bRe=false;
		return bRe;
	}

	m_nDriveId = m_DriveVec[nSel].nDriveId;
	m_nSysDriveId =m_DriveVec[nSel].nId;
 
	
	return bRe;
}

CString CDlg429::GetSimData()
{
	CMarkup xmlSimData;
	xmlSimData.AddElem("SimData");
	
	bool bFind =false;
	CString sTxt;
	for(int i =6;i<9;i++)
	{
		for(int k=1;k<9;k++)
		{
			sTxt =m_grid.GetItemText(i,k);
			if(sTxt=="")
				continue;
			CMarkup xmlItem;
			xmlItem.AddElem("item",sTxt);
		 
			xmlSimData.AddChildSubDoc(xmlItem.GetSubDoc());
			bFind =true;
		}
	}

	return bFind ? xmlSimData.GetSubDoc():"";
}

void CDlg429::OnBnClickedButtonSave()
{
	//CString sTxt;
	//sTxt.Format("�Ƿ�Э�鱣�浽\"%s\"",m_sCurDlgName);

	//if(AfxMessageBox(sTxt,MB_YESNO) == IDNO)
	//	return;;

	//int nSel = m_comboDrive.GetCurSel();
	//if(nSel==-1)
	//	return;

	//m_nProtoclItem_Index =1;
	//m_nDriveId = m_DriveVec[nSel].nDriveId;
	//m_nSysDriveId =m_DriveVec[nSel].nId;
	//
	//CString strSql;
	// 
 //	CString sProtocolRecSend;
	//CString sItemType;
	//
	//CString sProtocolItemName;
	//CString sProtocolItemDataType;
 //	 
	////��Ӧt_data_configID��nSharedId
	//int nProtocol_ConfigId;
	//int nProtocol_SharedId;
	//
	////Э������
	//CString sProtocolName;

	////Э������
	//sProtocolName = m_grid.GetItemText(0,3);

	////ɾ����ʷ��¼�������Э����е�nId
	//XProtocol_Funtion::DeleteProtoclRecod(m_nCurDlgId,sProtocolName,m_nDriveId,m_nSysDriveId,m_nProtocolId);

	////������ӦЭ��Ĺ����ڴ�
	//XProtocol_Funtion::Save_Protocl_toConfigData(m_nCurDlgId,sProtocolName,m_nProtocolId,m_nDriveId,m_nSysDriveId,nProtocol_ConfigId,nProtocol_SharedId);
 // 
	////��������
	//CString sSimData = GetSimData();


	//CMarkup xml;
	//
	//xml.AddElem("Attr");
	//xml.AddAttrib("Name",sProtocolName);

	////�汾
	//sTxt = m_grid.GetItemText(0,5);
	//xml.AddAttrib("Ver",sTxt);
	//
	////ͨ����
	//sTxt =m_grid.GetItemText(1,1);
	//xml.AddAttrib("channel",sTxt); 

	////����/����
	//sProtocolRecSend =m_grid.GetItemText(1,3);
	//xml.AddAttrib("RecSend",sProtocolRecSend); 

	////���ͻ���
	//sTxt =m_grid.GetItemText(1,5);
	//xml.AddAttrib("SendMode",sTxt); 

	////��������(Kbps)
	//sTxt =m_grid.GetItemText(2,1);
	//xml.AddAttrib("SendSpeed",sTxt); 

	////ʱ����(ms)
	//sTxt =m_grid.GetItemText(2,3);
	//xml.AddAttrib("times",sTxt); 

	////У��λ
	//sTxt =m_grid.GetItemText(2,5);
	//xml.AddAttrib("parity",sTxt); 

	//xml.AddAttrib("DataUnit","BIT"); 
	//xml.AddAttrib("DataLen",32); 
	//xml.AddAttrib("nShared",nProtocol_SharedId); 


	//int nRow =9;
	//int nCol; 
	//int nlen;
	//
	// 	
	//CMarkup xmlItem;
	//xmlItem.AddElem("SubItems");

	//nRow=10;	
	//int nAddr;
	//CString sItemName;
	//CString sDefaultValue;
	//CString sMark;
	//int nItem_ConfigId,nItem_SharedId;
	// float fCoefficient,fOffset;
	//for(int i =nRow,end = m_grid.GetRowCount();i<end;i++)
	//{ 
	//	nCol =0;
	//	//��ʼλ
	//	sTxt = m_grid.GetItemText(i,nCol++);
	//	if(sTxt=="")
	//		break;
	//
	//	CMarkup xmlChild;
	//	xmlChild.AddElem("item");

	//	//��ʼλ	 
	//	xmlChild.AddAttrib("addr",sTxt);
	//	nAddr = atoi(sTxt.GetBuffer());

	//	//����
	//	sTxt = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("length",sTxt);
	//	nlen = atoi(sTxt);

	//	//Э��������
	//	sItemType = m_grid.GetItemText(i,nCol++);
	//	sItemType.MakeUpper();
	//	xmlChild.AddAttrib("type",sItemType);

	//	//��������
	//	sProtocolItemDataType = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("data_type",sProtocolItemDataType);
	//	
	//	//ϵ��
	//	sTxt = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("coefficient",sTxt);
	//	fCoefficient  =atof(sTxt.GetBuffer());

	//	//����ֵ
	//	sTxt = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("offset",sTxt);
	//	fOffset =atof(sTxt.GetBuffer());

	//	//����
	//	sProtocolItemName = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("name",sProtocolItemName);
	//
	//	sDefaultValue  = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("default",sDefaultValue  );
	//	
	//	//Ĭ��ֵ
	//	sMark = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("mark",sMark);

	//	sItemName =sProtocolName+"."+sProtocolItemName;
	// 	
	//	 if(XProtocol_Funtion::Save_ProtoclItem_toConfigData(m_nCurDlgId,sItemType,sItemName,sProtocolItemDataType,nlen,m_nProtocolId,m_nProtoclItem_Index,m_nDriveId,m_nSysDriveId,sDefaultValue,
	//		 fCoefficient,fOffset,sMark,
	//	nItem_ConfigId,nItem_SharedId))
	//	{
	//	XProtocol_Funtion::Save_Data_Sets(m_nProtocolId,nProtocol_ConfigId,nProtocol_SharedId,nItem_ConfigId,nItem_SharedId);
	//	xmlChild.AddAttrib("Shared",nItem_SharedId);
	//	m_nProtoclItem_Index++;
	//	}		 	 

	//	xmlItem.AddChildSubDoc(xmlChild.GetSubDoc());	

	//}	
	//xml.AddChildSubDoc(xmlItem.GetSubDoc());

	//CString strXml = xml.GetSubDoc();
	//
	//XProtocol_Funtion::SaveProtocol(m_nCurDlgId,m_nProtocolId,sProtocolRecSend,sProtocolName,m_nSysDriveId,strXml); 
} 

void CDlg429::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(m_grid.GetSafeHwnd())
	{
		m_grid.MoveWindow(30,60,cx-60,cy-80);
	}
}
