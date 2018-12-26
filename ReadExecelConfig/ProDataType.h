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

	int lID;						// ���ڵ��Ψһ��ʶ��
	int nProType;					// Э�����0��Э����ڵ㣬
	//			-1������ϵͳ�ڵ㣬
	//			-2��ϵͳ���ڵ�
	int nData;						// ��ǰ�ڵ�������ţ����ڴ�Excel����ʱsheet�ı��)
	int nDlgId;						// ��ǰϵͳ���
	int nSysDriveId;				// ϵͳ����ID
	int nRecSend;					// ���ջ���
	CString sProNo;					// Э�����
	CString sDataType;				// �������ͣ�BLOCK��ITEM��
	CString sName;					// ��ǰ�ڵ�����
	vector<vector<string>> vtNodeData;	// �ڵ�����
	TREEDATA* pParent;				// ���ڵ�
	vector<TREEDATA> subNode;
};

typedef void (* CallFunc)(LPVOID lpParam);
