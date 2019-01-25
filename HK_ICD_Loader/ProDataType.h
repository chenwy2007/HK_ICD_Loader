#pragma once

struct TREEDATA {
	TREEDATA()
	{
		lID = 0;
		nProType = -2;
		nData = -1;
		nDlgId = -1;
		nSysDriveId = -1;
		nRecSend = -1;
		sName = "";
		sProNo = "";
		sDataType = "";
		pParent = NULL;
		vtNodeData.clear();
		subNode.clear();
	}

	int lID;						// 树节点的唯一标识号
	int nProType;					// 协议类别，0：协议根节点，
	//			-1：测试系统节点，
	//			-2：系统根节点
	int nData;						// 当前节点的索引号（用于从Excel导入时sheet的编号)
	int nDlgId;						// 当前系统编号
	int nSysDriveId;				// 系统驱动ID
	int nRecSend;					// 接收或发送
	CString sProNo;					// 协议代码
	CString sDataType;				// 数据类型（BLOCK、ITEM）
	CString sName;					// 当前节点名称
	vector<vector<string>> vtNodeData;	// 节点数据
	TREEDATA* pParent;				// 父节点
	vector<TREEDATA> subNode;
};

typedef void (* CallFunc)(LPVOID lpParam);
