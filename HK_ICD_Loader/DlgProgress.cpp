// DlgProgress.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReadExecelConfig.h"
#include "DlgProgress.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlgProgress �Ի���

IMPLEMENT_DYNAMIC(CDlgProgress, CDialogEx)

CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgProgress::IDD, pParent)
{
	m_strTitle = "����";
	m_nMaxCount = 0;
	m_nMinCount = 0;
	m_nCurPos = 0;
	m_nProgressType = 0;
	m_nExitCode = 0;
	m_strInfor = "����������...";
}

CDlgProgress::~CDlgProgress()
{

}

void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRGS_EXCUTE, m_progressctrl);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_static_progress);
}

BEGIN_MESSAGE_MAP(CDlgProgress, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(PROGRESS_POS, OnSetPos)
	ON_MESSAGE(EXIT_PROGRESS, OnExitProgress)
END_MESSAGE_MAP()


// CDlgProgress ��Ϣ�������

void CDlgProgress::SetProgressTitle(CString csTitle)
{
	m_strTitle = csTitle;

	//SetWindowText(csTitle);
}

void CDlgProgress::SetInfor(CString csInfor)
{
	m_strInfor = csInfor;
}

void CDlgProgress::InitialProgress(int nMin, int nCurPos, int nMax)
{
	//m_progressctrl.SetRange(nMin,nMax);
	m_nMaxCount = nMax;
	m_nMinCount = nMin;
	m_nCurPos = nCurPos;
}

void CDlgProgress::SetProgressPos(int nPos)
{
	m_nCurPos = nPos;

	if(m_progressctrl.GetSafeHwnd())
		m_progressctrl.SetPos(nPos);

	CString sTxt = "";
	sTxt.Format("���ȣ�%d/%d",nPos,m_nMaxCount);
	if(m_static_progress.GetSafeHwnd())
		m_static_progress.SetWindowText(sTxt);

	if(nPos >= m_nMaxCount)
	{
		m_nExitCode = 1;
		HWND hProgress = GetSafeHwnd();
		::PostMessage(hProgress,WM_CLOSE,0,0);
	}
}

LRESULT CDlgProgress::OnSetPos(WPARAM wParam, LPARAM lParam)
{
	m_nCurPos = (int)wParam;
	m_progressctrl.SetPos(m_nCurPos);

	CString sTxt = "";
	sTxt.Format("���ȣ�%d/%d",m_nCurPos,m_nMaxCount);
	m_static_progress.SetWindowText(sTxt);

	if(m_nCurPos >= m_nMaxCount)
	{
		//OnOK();
		m_nExitCode = 1;
		HWND hProgress = GetSafeHwnd();
		::PostMessage(hProgress,WM_CLOSE,0,0);
	}
	return 0;
}

LRESULT CDlgProgress::OnExitProgress(WPARAM wParam, LPARAM lParam)
{
	//OnOK();
	m_nExitCode = 1;
	HWND hProgress = GetSafeHwnd();
	::PostMessage(hProgress,WM_CLOSE,0,0);

	return 0;
}

void CDlgProgress::ExitWindow(void)
{
	//HWND hProgress = GetSafeHwnd();
	//::PostMessage(hProgress,WM_COMMAND,MAKEWPARAM(IDOK,BN_CLICKED),0);
	m_nExitCode = 1;
}

void CDlgProgress::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_nExitCode == 0)
	{
		CString sTxt = "";
		sTxt.Format("�Ƿ���ֹ%s",m_strTitle);
		if(AfxMessageBox(sTxt,MB_YESNO) == IDNO)
			return;
		m_nExitCode = 2;
	}

	//// �˳���ǰ����ִ�еĹ���
	//if(m_nExitCode != 2)
	//	return;

	CDialogEx::OnClose();
}

BOOL CDlgProgress::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SetWindowText(m_strTitle);

	if(m_nProgressType == 0)
	{
		m_progressctrl.SetRange(m_nMinCount,m_nMaxCount);
		m_progressctrl.SetPos(m_nCurPos);

		CString sTxt = "";
		sTxt.Format("�������ȣ�%d/%d",m_nCurPos,m_nMaxCount);
		m_static_progress.SetWindowText(sTxt);
	}
	else if(m_nProgressType == 1)
	{
		m_nCurPos = 0;
		m_progressctrl.SetRange(0,100);
		m_progressctrl.SetPos(m_nCurPos);

		//CString sTxt = "����������...";
		m_static_progress.SetWindowText(m_strInfor);

		SetTimer(1,100,0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgProgress::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent == 1)
	{
		m_progressctrl.SetPos(m_nCurPos+1);
		m_nCurPos = (m_nCurPos+1)%100;

		if(m_nExitCode == 1)
		{
			//OnOK();
			HWND hProgress = GetSafeHwnd();
			::PostMessage(hProgress,WM_CLOSE,0,0);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgProgress::OnDestroy()
{
	KillTimer(1);

	CDialogEx::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
}
