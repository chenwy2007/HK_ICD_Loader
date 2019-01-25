// XWnd_Edit.cpp : 实现文件
//

#include "stdafx.h"
 
#include "XWnd_Edit.h"



// XWnd_Edit


void Analysis_And(int nSrcValue,string sExp,int& nReValue)
{
	int nV;
	int n = sscanf(sExp.c_str(),"@And:%x",&nV);
	if(n==1)
	{
		nReValue = nSrcValue & nV;
	} 
}
void Analysis_OR(int nSrcValue,string sExp,int& nReValue)
{
	int nV;
	int n = sscanf(sExp.c_str(),"@OR:%x",&nV);
	if(n==1)
	{
		nReValue = nSrcValue | nV;
	} 
}
void Analysis_MoveR(int nSrcValue,string sExp,int& nReValue)
{
	int nV;
	int n = sscanf(sExp.c_str(),"@MoveR:%x",&nV);
	if(n==1)
	{
		nReValue = nSrcValue << nV;
	} 
}
void Analysis_MoveL(int nSrcValue,string sExp,int& nReValue)
{
	int nV;
	int n = sscanf(sExp.c_str(),"@MoveL:%x",&nV);
	if(n==1)
	{
		nReValue = nSrcValue >> nV;
	} 
}
void Analysis_Symbol(int nSrcValue,string sExp,int& nReValue)
{
	int nV;
	int n = sscanf(sExp.c_str(),"@Symbol:%x",&nV);
	if(n==1)
	{
		if(nSrcValue&nV)
		{
			nReValue = nSrcValue &(~nV);
			nReValue*=-1;
		}

	} 
}
void Analysis(int nSrcValue,string sExp,int& nReValue) 
{
	int nPos = sExp.find("@And:");
	if(nPos!=-1)
	{
		Analysis_And(nSrcValue,sExp,nReValue);
		return;
	}
	nPos = sExp.find("@Or:");
	if(nPos!=-1)
	{
		Analysis_OR(nSrcValue,sExp,nReValue);
		return;
	}
	nPos =sExp.find("@MoveR:");
	if(nPos!=-1)
	{
		Analysis_MoveR(nSrcValue,sExp,nReValue);
		return;
	}
	nPos =sExp.find("@MoveL:");
	if(nPos!=-1)
	{
		Analysis_MoveL(nSrcValue,sExp,nReValue);
		return;
	}
	nPos = sExp.find("@Symbol:");
	if(nPos!=-1)
	{
		Analysis_Symbol(nSrcValue,sExp,nReValue);
		return;
	}

}


IMPLEMENT_DYNAMIC(XWnd_Edit, CEdit)

XWnd_Edit::XWnd_Edit(__CtrlInfo &info,float fScale)
{
	m_ctrlInfo = info;
	m_fScale =fScale;
	m_nDataType =DATA_TYPE_INT;
	//m_hAlerthIcon = AfxGetApp()->LoadIcon(IDI_ICON_ALERT);
	//m_bIsAlert = 0;
	m_sRange = "";
	m_pTopBase = NULL;
	m_pAlertIcon = NULL;
	m_sDefaultValue = "";
}

XWnd_Edit::XWnd_Edit()
{
	m_nDataType =DATA_TYPE_INT;
	//m_hAlerthIcon = AfxGetApp()->LoadIcon(IDI_ICON_ALERT);
	//m_bIsAlert = 0;
	m_sRange = "";
	m_pTopBase = NULL;
	m_pAlertIcon = NULL;
	m_sDefaultValue = "";
}

XWnd_Edit::~XWnd_Edit()
{
	//DestroyIcon(m_hAlerthIcon);

	if(m_pAlertIcon)
		m_pAlertIcon->DestroyWindow();
	delete_safe(m_pAlertIcon);
}


BEGIN_MESSAGE_MAP(XWnd_Edit, CEdit)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR_REFLECT()
	//ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void XWnd_Edit::CreateCtrl(DWORD  nCtrlId,CWnd * pParentWnd,int nShow )
{
	m_nCtrlId = nCtrlId;
	CRect rt;
	rt.left = m_ctrlInfo.nX*m_fScale;
	rt.right = rt.left+m_ctrlInfo.nW*m_fScale;
	rt.top = m_ctrlInfo.nY*m_fScale;
	rt.bottom = rt.top +m_ctrlInfo.nH*m_fScale;
	
	Create(ES_MULTILINE|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER,rt,pParentWnd,m_nCtrlId);
	// 创建一个标签控件，用于显示图标
	__CtrlInfo AlertIconInfo;

	AlertIconInfo.nH = rt.Height();
	AlertIconInfo.nId = GetCtrlId();
	AlertIconInfo.nIndex = 0;
	AlertIconInfo.nInterfaceId = 0;
	AlertIconInfo.nSharedId = 0;
	AlertIconInfo.nType = 2;
	AlertIconInfo.nW = rt.Height()-10;
	AlertIconInfo.nX = rt.right;
	AlertIconInfo.nY = rt.top;
	AlertIconInfo.sAttr = "";
	AlertIconInfo.sName = "";
	AlertIconInfo.sReserve1 = "";
	AlertIconInfo.sReserve2 = "";
	AlertIconInfo.sTitle = "";

	m_pAlertIcon = new XWnd_Static(AlertIconInfo,1.0);
	m_pAlertIcon->SetParentBase(m_pTopBase);
	m_pAlertIcon->m_strTitle = "";
	m_pAlertIcon->m_bBack = 0;
	//m_pAlertIcon->m_backColor = RGB(156,24,78);
	//m_pAlertIcon->m_FontColor = RGB(10,10,10);
	m_pAlertIcon->CreateCtrl(AlertIconInfo.nId,pParentWnd);
	m_pAlertIcon->ShowWindow(SW_HIDE);
}

void XWnd_Edit::MoveWnd()
{
	MoveWindow(m_ctrlInfo.nX *m_fScale,m_ctrlInfo.nY * m_fScale,m_ctrlInfo.nW * m_fScale,m_ctrlInfo.nH*m_fScale);	
}

//void XWnd_Edit::OnMouseMove(UINT /*nFlags*/, CPoint point)
//{
//	CRect rect;
//	GetClientRect(rect);
//
//	GetWindowRect(&rect);
//
//	ScreenToClient(&point);
//
//	ScreenToClient(&rect);
//
//	// If outside client area, return (unless we are drag n dropping)
//	if (!rect.PtInRect(point))
//		return;
//}

void XWnd_Edit::DecodeXml()
{
	//if(m_ctrlInfo.sAttr.empty())
	//	return;
	//string sAttr = m_ctrlInfo.sAttr;
	//string str;
	//int pos = sAttr.find(";");
	//while(pos!=-1)
	//{
	//	str = sAttr.substr(0,pos);
	//	m_sExpressionVec.push_back(str);
	//	sAttr.erase(0,pos+1);
	//	pos = sAttr.find(";");
	//}

	CMarkup xml(m_ctrlInfo.sAttr.c_str());
	CString str;
	if(xml.FindElem("Attr"))
	{

		// 获取控件属性值
		m_sRange = xml.GetAttrib("ValueRange");
	} 
}
void XWnd_Edit::SetSelect(bool bSel)
{
	m_bSel = bSel;
	Invalidate(FALSE);
}
void XWnd_Edit::OnPaint()
{
	CEdit::OnPaint();

	CPaintDC dc(this); 
	CDC *pDC = &dc;
	CRect rt;
	GetClientRect(rt);
	

	//if(!CAppData::GetInstance().m_bBuilder || !m_bSel )	 
	//	return;
	//
	//CPen pen(PS_DASHDOTDOT,0,RGB(255,0,0));
	//
	//CPen *oldPen =pDC->SelectObject(&pen);

	//pDC->MoveTo(0,0);
	//pDC->LineTo(0,rt.bottom-1);
	//pDC->LineTo(rt.right-1,rt.bottom-1);
	//pDC->LineTo(rt.right-1,0);
	//pDC->LineTo(0,0);
	//pDC->SelectObject(oldPen);

}

HBRUSH XWnd_Edit::CtlColor(CDC* pDc,UINT nCtlColor)
{
	CRect rt;
	GetClientRect(rt);

	int x = rt.right;
	int y = rt.top;

	rt.InflateRect(1,1,1,1);
	
	// 当数据出错时，置出错标志
	//if(m_bIsAlert)
	//	DrawIcon(pDc->m_hDC,x,y-10,m_hAlerthIcon);

	// 当选中时绘制选择框
	if(!CAppData::GetInstance().m_bBuilder || !m_bSel )	 
		return NULL;

	CPen pen(PS_DASHDOTDOT,0,RGB(255,0,0));

	CPen *oldPen =pDc->SelectObject(&pen);

	pDc->MoveTo(0,0);
	pDc->LineTo(0,rt.bottom-1);
	pDc->LineTo(rt.right-1,rt.bottom-1);
	pDc->LineTo(rt.right-1,0);
	pDc->LineTo(0,0);
	pDc->SelectObject(oldPen);

	return NULL;
}

void XWnd_Edit::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(CAppData::GetInstance().m_bBuilder )	 
	{
		CAppData::GetInstance().SetCurSelectWnd(this);
	}

	//m_bSel = true;
	__super::OnLButtonUp(nFlags, point);
	CAppData::GetInstance().SetCurTipWnd(this);
}

void XWnd_Edit::RefreshData()
{
	if(m_ctrlInfo.nSharedId<=0)
		return;
  
	string str; 
	int nDataType;
	int bUpdateFlag;
	DWORD dwUpdateTime;

	int nInOut = -1;
	int nDataSize = -1;
	g_XShared.ReadData_Attr(m_ctrlInfo.nSharedId,nDataType,nDataSize,nInOut);
	if(nInOut == 1)
	{
		if(g_XShared.ReadData_ToString(m_ctrlInfo.nSharedId,str,nDataType,bUpdateFlag,dwUpdateTime))
		{
			if(m_sExpressionVec.size()>0)
			{
				Analysis_Expression(str,nDataType);
				return;
			}

			if(!bUpdateFlag)
				return;

			if(!m_sRange.IsEmpty())
			{
				int nPos = m_sRange.Find("~");
				if(nPos > 0)
				{
					float fMin = _tstof(m_sRange.Left(nPos));
					float fMax = _tstof(m_sRange.Right(m_sRange.GetLength()-nPos-1));
					float fValue = _tstof(str.c_str());
					// 显示一个警示图标
					if(fValue < fMin || fValue > fMax)
					{
						m_pAlertIcon->m_sToolTipInfor = "测量值超出范围（"+m_sRange+"）！";
						m_pAlertIcon->m_bIsAlert = true;
						m_pAlertIcon->ShowWindow(SW_SHOW);
						//m_pAlertIcon->Invalidate();
					}
					else if(m_pAlertIcon->IsWindowVisible())
					{
						m_pAlertIcon->m_sToolTipInfor = "";
						m_pAlertIcon->m_bIsAlert = false;
						m_pAlertIcon->ShowWindow(SW_HIDE);
						//m_pAlertIcon->Invalidate();
					}
				}
			}

			SetWindowText(str.c_str());		
			//Invalidate();
		}
		////debug
		//if(m_ctrlInfo.sName == "LKH69.PC_VI2")
		//	m_bIsAlert = true;
	}
	// 当信号类型为输出时，将文本框的内容转成浮点数，并写入共享内存
	else if(nInOut == 2)
	{
		if(g_App_StdSys.m_bStartWrite==false)
			return;

		CString sValue = "";
		float fValue = 0.0;
		GetWindowText(sValue);
		if(sValue.IsEmpty())
		{
			sValue.Format("%.2f",fValue);
			SetWindowText(sValue);
		}
		else
		{
			fValue = _tstof(sValue);
		}
		g_XShared.WriteData_Float(m_ctrlInfo.nSharedId,fValue);
	}
}
void XWnd_Edit::UpdateAttr(__EditAttr & attr)
{
	m_ctrlInfo.sName = attr.sName;
	m_ctrlInfo.sTitle = attr.sName;
	m_ctrlInfo.nH = attr.nH;
	m_ctrlInfo.nW = attr.nW;
	m_ctrlInfo.nY = attr.nY;
	m_ctrlInfo.nX = attr.nX;
	m_ctrlInfo.nSharedId = attr.nSharedId;
}
void  XWnd_Edit::Analysis_Expression(string sData,int nDataType)
{
	if(nDataType ==DATA_TYPE_INT)
	{
		int nValue = atoi(sData.c_str());		
		for(int i=0,end = m_sExpressionVec.size();i<end;i++)
		{
			Analysis(nValue,m_sExpressionVec[i],nValue);
		}

		CString str;
		str.Format("%d",nValue);
		SetWindowText(str);		
		Invalidate();
	}
	
}