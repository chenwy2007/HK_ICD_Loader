#pragma once
#include "afxwin.h"
#include "ProDataType.h"


// ProDiscrete ��ɢ��Э�鴦����

class ProDiscrete
{
public:
	ProDiscrete();   // ��׼���캯��
	virtual ~ProDiscrete();

	void SaveDataToDB(TREEDATA& TreeData, int nCurDrive = 0);

	void Initailize(void);
	void LoadIcdFile(CString strFilePath);
	TREEDATA& BuildProtocolTree(TREEDATA& ParentTree,int nProType,int nDlgId,int& nTreeId);
	BOOL BuildDBTree(TREEDATA& DBTree, __SYS_Drive& DInforItem, int nProType, int& nTreeId, int& nStatus);

	void SetProgressFunc(CallFunc func,LPVOID lpParam);

	void StopSaveToDB(BOOL bStop) {m_bExitSaveToDB = bStop;}
	void StopLoadIcd(bool bStop) {m_bExitLoadIcd = bStop;}
	void StopLoadDBIcd(bool bStop) {m_bExitLoadDBIcd = bStop;}
	void StopDeleteDB(bool bStop) {m_bExitDeleteDB = bStop;}
	void GetTempData(vector<vector<string>>& vtTemp, CString csSheetNo);

	void DeleteBLOCKFromDB(TREEDATA& TreeNode);

	void SetDriveInfor(vector<__SYS_Drive>& vtDriveInfor) {m_DriveVec = vtDriveInfor;}
	
protected:
	CString GetExePath(void);
	//void SaveTblData(CString& sPageName);
	// ���ģ���ͷ����β������
	void GetTempHeadTail(vector<vector<string>>& vtTemp,vector<vector<string>>& vtTHead,
		vector<vector<string>>& vtTTail,int nType);
	CString GetDataType(int nDataType);

public:
	Execel_Base  m_excelFile;

protected:
	TREEDATA m_treedata;	// ��������

	vector<CString> m_protocoNameVec;
	int m_nDlgId;

	BOOL m_bExitSaveToDB;
	bool m_bExitLoadIcd;
	bool m_bExitLoadDBIcd;
	bool m_bExitDeleteDB;

	vector<__SYS_Drive> m_DriveVec;

	CallFunc m_func;
	LPVOID m_lpParam;

	// Excel����ģ��
	vector<vector<string>> m_vtTemp1;
	vector<vector<string>> m_vtTemp2;
	vector<vector<string>> m_vtTemp3;

private:
	map<long, CString> m_mSystemName;



};
