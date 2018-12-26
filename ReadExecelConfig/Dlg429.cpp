// Dlg429.cpp : 实现文件
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

// CDlg429 对话框

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


// CDlg429 消息处理程序


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
		AfxMessageBox("数据标志位错误，不为429");
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

	//接收/发送
	sTxt = m_grid.GetItemText(1,3);
	if(sTxt!="接收" && sTxt!="发送")
		m_grid.SetItemBkColour(0,5,Color_ERR);
	else
		m_grid.SetItemBkColour(0,5,RGB(255,255,255));

	if(sTxt =="发送")
	{
		sTxt = m_grid.GetItemText(1,5);
		if(sTxt!="定时" && sTxt!="指令" && sTxt!="指令定时")
		{
			m_grid.SetItemBkColour(0,5,Color_ERR);
			bRe =false;
		}
		else
			m_grid.SetItemBkColour(0,5,RGB(255,255,255));
	}

	sTxt = m_grid.GetItemText(2,5);

	if(sTxt!="奇校验" && sTxt!="无校验" && sTxt!="偶校位")
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

	//地址合法性检查
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
		AfxMessageBox("数据长度总数不等于32");
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
	//sTxt.Format("是否将协议保存到\"%s\"",m_sCurDlgName);

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
	////对应t_data_configID、nSharedId
	//int nProtocol_ConfigId;
	//int nProtocol_SharedId;
	//
	////协议名称
	//CString sProtocolName;

	////协议名称
	//sProtocolName = m_grid.GetItemText(0,3);

	////删除历史记录，并获得协议表中的nId
	//XProtocol_Funtion::DeleteProtoclRecod(m_nCurDlgId,sProtocolName,m_nDriveId,m_nSysDriveId,m_nProtocolId);

	////创建对应协议的共享内存
	//XProtocol_Funtion::Save_Protocl_toConfigData(m_nCurDlgId,sProtocolName,m_nProtocolId,m_nDriveId,m_nSysDriveId,nProtocol_ConfigId,nProtocol_SharedId);
 // 
	////仿真数据
	//CString sSimData = GetSimData();


	//CMarkup xml;
	//
	//xml.AddElem("Attr");
	//xml.AddAttrib("Name",sProtocolName);

	////版本
	//sTxt = m_grid.GetItemText(0,5);
	//xml.AddAttrib("Ver",sTxt);
	//
	////通道号
	//sTxt =m_grid.GetItemText(1,1);
	//xml.AddAttrib("channel",sTxt); 

	////接收/发送
	//sProtocolRecSend =m_grid.GetItemText(1,3);
	//xml.AddAttrib("RecSend",sProtocolRecSend); 

	////发送机制
	//sTxt =m_grid.GetItemText(1,5);
	//xml.AddAttrib("SendMode",sTxt); 

	////发送速率(Kbps)
	//sTxt =m_grid.GetItemText(2,1);
	//xml.AddAttrib("SendSpeed",sTxt); 

	////时间间隔(ms)
	//sTxt =m_grid.GetItemText(2,3);
	//xml.AddAttrib("times",sTxt); 

	////校验位
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
	//	//起始位
	//	sTxt = m_grid.GetItemText(i,nCol++);
	//	if(sTxt=="")
	//		break;
	//
	//	CMarkup xmlChild;
	//	xmlChild.AddElem("item");

	//	//起始位	 
	//	xmlChild.AddAttrib("addr",sTxt);
	//	nAddr = atoi(sTxt.GetBuffer());

	//	//长度
	//	sTxt = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("length",sTxt);
	//	nlen = atoi(sTxt);

	//	//协议项类型
	//	sItemType = m_grid.GetItemText(i,nCol++);
	//	sItemType.MakeUpper();
	//	xmlChild.AddAttrib("type",sItemType);

	//	//数据类型
	//	sProtocolItemDataType = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("data_type",sProtocolItemDataType);
	//	
	//	//系数
	//	sTxt = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("coefficient",sTxt);
	//	fCoefficient  =atof(sTxt.GetBuffer());

	//	//补偿值
	//	sTxt = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("offset",sTxt);
	//	fOffset =atof(sTxt.GetBuffer());

	//	//名称
	//	sProtocolItemName = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("name",sProtocolItemName);
	//
	//	sDefaultValue  = m_grid.GetItemText(i,nCol++);
	//	xmlChild.AddAttrib("default",sDefaultValue  );
	//	
	//	//默认值
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
