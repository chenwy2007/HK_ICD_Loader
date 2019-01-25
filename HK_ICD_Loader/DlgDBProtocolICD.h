#pragma once
#include "afxcmn.h"
//#include "g:\non-standardteq-hk\trunk\commonlib\gridctrl\gridctrl.h"
#include "Pro1553B.h"
#include "ProDiscrete.h"
#include "ProCAN.h"
#include "ProSerial.h"
#include "ProArinc429.h"
#include "DlgProgress.h"


// CDlgDBProtocolICD 对话框

class CDlgDBProtocolICD : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDBProtocolICD)

public:
	CDlgDBProtocolICD(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDBProtocolICD();

public:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	void BuilProTree(TREEDATA& PTData, BOOL bSelectRoot = TRUE);
	void SetPro1553B(Pro1553B* pPro) {m_pPro1553B = pPro;}
	void SetProDiscrete(ProDiscrete* pPro) {m_pProDiscrete = pPro;}
	void SetProCAN(ProCAN* pPro) {m_pProCan = pPro;}
	void SetProSerial(ProSerial* pPro) {m_pProSerial = pPro;}
	void SetProArinc429(ProArinc429* pPro) {m_pProArinc429 = pPro;}

	void SetTreeData(TREEDATA* pTreeData) {m_pTreeData = pTreeData;}

	BOOL DeleteDBICD(void);

// 对话框数据
	enum { IDD = IDD_DLG_DBVIEW };

protected:
	afx_msg void OnTvnSelchangedTreeProtocol(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg  void  OnRBClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnDeleteDbicd();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void InsertTreeNode(TREEDATA& ProTree,HTREEITEM hParentItem,int nNodeNo);
	void OnGridDBClick(NMHDR* pNMHDR, LRESULT* pResult);

	void DisplayGridData(TREEDATA& TreeNode);
	void DispNodeInfor(vector<vector<string>>& vtInfor, vector<vector<string>>& vtTemp,TREEDATA& TreeItem);
	void ClearGridData(void);

	HTREEITEM FindTreeNode(HTREEITEM hItem,int nTreeId,int nNodeNo);

	BOOL DeleteTreeData(TREEDATA& TreeNode, HTREEITEM hTreeItem);
	BOOL DeleteDBICD(TREEDATA& TreeNodeItem);
	HTREEITEM TravelAll(HTREEITEM ht);

	DECLARE_MESSAGE_MAP()

private:
	CDlgProgress* m_pdlgProgress;
	CImageList m_ImageList;

	CTreeCtrl m_tree_protocol;
	CGridCtrl m_grid_protocol;

	Pro1553B* m_pPro1553B;
	ProDiscrete* m_pProDiscrete;
	ProCAN* m_pProCan;
	ProSerial* m_pProSerial;
	ProArinc429* m_pProArinc429;

	TREEDATA* m_pTreeData;
	TREEDATA m_ParentTreeNode;

	int m_nExitCode;
};

unsigned WINAPI DeleteDBData(LPVOID lpParam);

