#pragma once
#include "../public/Wnd/BaseDialog.h"
#include "../public/Wnd/OwnerDrawTabCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"
//#include "g:\non-standardteq-hk\trunk\commonlib\gridctrl\gridctrl.h"
#include "Pro1553B.h"
#include "ProDiscrete.h"
#include "ProCAN.h"
#include "ProSerial.h"
#include "ProArinc429.h"
#include "ProCommCtrl.h"
#include "CTreeCtrlX.h"
#include "DlgProgress.h"
#include "DlgDBProtocolICD.h"
#include "MyToolBar.h"


// CDlgCommProMain 对话框

class CDlgCommProMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommProMain)

public:
	CDlgCommProMain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCommProMain();

	void SaveProtocolToDB();
	void SetProgressPos(void);
	void LoadIcdFile();
	void LoadIcdFromDB();
	void LoadCtrlFile();

	// 对话框数据
	enum { IDD = IDD_DLG_PRO_MAIN };

protected:

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTvnSelchangedTreeProtocol(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLoadicdfile();
	afx_msg void OnSavetodb();
	afx_msg void OnSelectsys();
	afx_msg void OnHelp();
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	afx_msg void OnProtocolDb();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();

	void OnGridDBClick(NMHDR* pNMHDR, LRESULT* pResult);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void InitTabCtrl();
	void ShowTab(int nIndex);
	void InitialCombox();
	void InitTree();
	//void OnNMCustomdrawTreeProtocol(NMHDR *pNMHDR, LRESULT *pResult);
	void InsertTreeNode(TREEDATA& ProTree,HTREEITEM hChildItem,int nNodeNo);
	void DisplayGridData(TREEDATA& TreeNode);
	void DispNodeInfor(vector<vector<string>>& vtInfor, vector<vector<string>>& vtTemp,TREEDATA& TreeItem);
	void ClearGridData(void);

	void ReleaseThread(HANDLE& hThread);
	HTREEITEM FindTreeNode(HTREEITEM hItem,int nTreeId,int nNodeNo);

	DECLARE_MESSAGE_MAP()

private:
	//COwnerDrawTabCtrl m_tabCtrl;
	int m_nCurTabIndex;
	int m_nTotalProNum;
	int m_nCurPos;
	int m_nTreeId;
	int m_nTreeExcelId;
	int m_nTreeDBId;

	int m_nCurProType;
	CString m_sCurIcdFilePath;

protected:
	CProgressCtrl m_myProCtrl;
	CDlgDBProtocolICD* m_pdlgDBProData;
	HICON m_hIcon;
	CBrush m_BKBrush;

public:
	CDlgProgress* m_pdlgProgress;
	//afx_msg void OnTcnSelchangeTabProtype(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CImageList m_ImageList;
	CImageList m_ToolBarImage;
	//CStatic m_static_cursys;
	//CComboBox m_combox_cursys;
	CGridCtrl m_grid_protocol;
	CTreeCtrlX m_tree_protocol;
	//CTreeCtrl m_tree_protocol;
	CMenu m_menu_main;
	CToolBar m_wndToolBar;
	CReBar m_wndReBar;
	CStatusBar m_Statusbar;
	vector<CString> m_vtSubSystem;
	vector<__SYS_Drive> m_vtDriveInfor;

	int m_nCurComChannelNo;
	Pro1553B m_Pro1553B;
	ProDiscrete m_ProDiscrete;
	ProCAN m_ProCan;
	ProSerial m_ProSerial;
	ProArinc429 m_ProArinc429;

	ProCommCtrl m_ProCommCtrl;

	TREEDATA m_ParentTree;
	TREEDATA m_DBIcdTree;
	TREEDATA m_BackupTree;

	vector<CString> m_vtSaveNode;

	HANDLE m_hSaveToDB_Thread;
	HANDLE m_hLoadIcd_Thread;

	BOOL m_bLoadDBICD;

	long m_lCurProType;					// 当前协议类型
public:
	afx_msg void OnLoadctrlfile();
	afx_msg void OnAbout();
};

unsigned WINAPI SaveDataToDB(LPVOID lpParam);
unsigned WINAPI LoadDataFile(LPVOID lpParam);
unsigned WINAPI LoadDBData(LPVOID lpParam);
unsigned WINAPI tLoadCtrlFile(LPVOID lpParam);


void ProgressPos(LPVOID lpParam);