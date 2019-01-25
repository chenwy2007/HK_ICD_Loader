#pragma once
#include "afxwin.h"
#include "Execel_1553.h"
#include "ProDataType.h"

//typedef void (* CallFunc)(LPVOID lpParam);

// Pro1553B
class Pro1553B
{

public:
	Pro1553B();   // 标准构造函数
	virtual ~Pro1553B();

	 
	vector<__DLGInfo> m_dlgVec;

	Execel_1553 m_config_1553;

	__protocol_1553 m_data_1553;

	vector<CString> m_protocoNameVec;

	void LoadIcdFile(CString strFilePath);
	void ReadSheetData(int nCurSheetNo, vector<vector<string>>& vtSheetData);
	void SaveDataToDB(TREEDATA& TreeData, int nCurDrive = 0);
	TREEDATA& BuildProtocolTree(TREEDATA& ParentTree,int nProType,int nDlgId,int& nTreeId);
	BOOL BuilSubProTree(TREEDATA& subTreeData, int nProSheetNo,int nProType,int& nTreeId);
	BOOL BuildDBTree(TREEDATA& DBTree, __SYS_Drive& DInforItem, int nProType, int& nTreeId, int& nStatus);

	void SetProgressFunc(CallFunc func,LPVOID lpParam);

	void StopSaveToDB(BOOL bStop) {m_bExitSaveToDB = bStop;}
	void StopLoadIcd(bool bStop) {m_bExitLoadIcd = bStop;}
	void StopLoadDBIcd(bool bStop) {m_bExitLoadDBIcd = bStop;}
	void StopDeleteDB(bool bStop) {m_bExitDeleteDB = bStop;}
	void GetTempData(vector<vector<string>>& vtTemp, CString csSheetNo);

	void Initailize(void);

	void DeleteBLOCKFromDB(TREEDATA& TreeNode);

	void SetDriveInfor(vector<__SYS_Drive>& vtDriveInfor,int nCurDIdx) {m_DriveVec = vtDriveInfor,m_nDriveIndex = nCurDIdx;}

protected:
	void SaveProtocolItemConfig(void);
	void GetDataAttr(CMarkup& hxml, TREEDATA& TreeData, int nIdx, CString sRecSend,
		CString sItemNo, CString sItemName, CString sProtocoName, int nProtocolId);

	void GetTemplate(vector<vector<string>>& vtTemp, CString csSheetNo);
	CString GetExePath(void);
	// 获得模板的头部和尾部数据
	void GetTempHeadTail(vector<vector<string>>& vtTemp,vector<vector<string>>& vtTHead,
		vector<vector<string>>& vtTTail,int nType);
	void InitialRowData(vector<vector<string>>& vtTemp,vector<string>& vtRowData,int nType);
	void GetSubAttr(CMarkup& xml,TREEDATA& TreeSubItem,
		vector<vector<string>>& vtTempHead, vector<vector<string>>& vtTempTail,
		vector<string>& vtRowTData, CString sTDataUnit, int& nTreeId, int nProType);
	void AttachTailTemp(vector<vector<string>>& vtProData, vector<vector<string>>& vtTempTail);

protected:
	TREEDATA m_treedata;	// 树形数据

	int m_nPulicProId;
	vector<vector<string>> m_Basevec ;

	//设备ID
	int m_nSysDriveId;
	int m_nDriveId;
	int m_nDriveIndex;

	int m_nProtocolId;
	int m_nProtoclItem_Index;

	int m_nGirdColCount;
	int m_nDlgId;

	vector<__SYS_Drive> m_DriveVec;

	CallFunc m_func;
	LPVOID m_lpParam;

	BOOL m_bExitSaveToDB;
	bool m_bExitLoadIcd;
	bool m_bExitLoadDBIcd;
	bool m_bExitDeleteDB;

	// Excel数据模板
	vector<vector<string>> m_vtTemp1;
	vector<vector<string>> m_vtTemp2;
	vector<vector<string>> m_vtTemp3;
};
