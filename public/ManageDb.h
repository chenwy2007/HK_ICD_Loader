#pragma once

#define g_DB  (CManageDb_Sqllite::GetInstance())

#include "AppData.h"
 class CManageDb_Sqllite_Sys
 {
 public:
	 CManageDb_Sqllite_Sys();
	 ~CManageDb_Sqllite_Sys();

	 BOOL ConnectDb();

	 int GetMaxId(const char *lpSql);

	 BOOL Update_Sys(string sSysName,string sKey,int &nId);

	 BOOL Query_Sys(vector<__SysInfo>&vec);

	 BOOL TableIsValid(string sTableName, string* pFieldName, int nFieldNum, int& nErrId);
 public:
	  CppSQLite3DB	m_SQLiteDB; 
	  bool m_bConnected;
 };
 
class CManageDb_Sqllite
{
public:
	CManageDb_Sqllite(void);
	~CManageDb_Sqllite(void);
    static CManageDb_Sqllite &GetInstance();

	void Close(void) {m_SQLiteDB.close();}

public:

	void SetDbFileName(string sfileName){m_strDbFileName = sfileName;};
	string GetDbFileName(void) {return m_strDbFileName;}

	BOOL ConnectDb();

	void CreateTable();

	BOOL IsConnect(){return m_bConnected;};

	int GetMaxId(const char *lpSql);

	BOOL QueryDataId(const char *lpSql,int& nId);

	BOOL ExecDML(char * lpSql);

	BOOL TableIsValid(string sTableName, string* pFieldName, int nFieldNum, int& nErrId);
public:
//	BOOL CManageDb_Sqllite::IsLoginSuccess(string sLoginName,string sPsw,int& nUserId,int &nPower);
	 	  
	BOOL Query_CtrlInfo(__CtrlInfo & info,int nId);

	BOOL Query_SubCtrlInfo(vector<__CtrlInfo> & vec,int nInterfaceId,int nCtrlType=-1 ,CString strWhere ="" ,CString strOrder ="order by nIndex");
	
	BOOL Query_DlgInfo(int nDlgId,string &sInfo);

	BOOL Query_DlgInfo(vector<__DLGInfo>&vec,int nSysId);

	BOOL Query_Sys(vector<__SysInfo>&vec);

	BOOL Query_ShareId(vector<__DATA_CONFIG>&vec,int nDlgId);

	BOOL Query_ShareId(__DATA_CONFIG & info,int nDlgId,int nSharedId);

	BOOL Query_Drive(vector<__DRIVE>&vec);
	
	BOOL Query_SysDrive(vector<__SYS_Drive>&vec,int nSysId, string sWhere ="");

	BOOL Query_DataConfig(vector<__DATA_CONFIG>&vec,int nDlgId,string strWhere ="" );

	BOOL Delete_DataConfig(CString strWhere);

	BOOL Query_CtrlEvent(vector<__Ctrl_Event>&vec,int nCtrlId,int nEventType =-1,int nGroup =-1);
	
	int Query_DlgId(int nDutId,int nSysID);

	int Query_DutMark(int nDlgId);

	BOOL Query_ImageCtrl_Image(vector<string>&vec,int nCtrlId);
	
	BOOL Query_ImageCtrl_Image(map<int,string>&sMap,int nCtrlId);

	BOOL Query_TestTip(vector<__Ctrl_TestTip>&vec,int nCtrlId);

	void Query_NextTip(vector<__Ctrl_TestTip>&vec,int nCtrlId);

	BOOL Query_UnitData(vector<__UnitData>&vec,int nSysId);

	//查询产品识别号
	BOOL Query_Goods_Number(vector<__Goods_Number>&vec,string strWhere );
	BOOL Query_Goods_Number(vector<__Goods_Number>&vec,int nDlgId,int nIndex );

	bool Query_Ctrl_FontColor(map<int,COLORREF>&_map,int nCtrlId,int nType);

	bool Query_Ctrl_FontColor(vector<COLORREF>&vec,int nCtrlId,int nType);

	bool Query_GridInfo(vector<TBLDATA>&vec, int nDlgId);

public:
	BOOL Update_Sys(int &nId,string sName,string sKey);

	BOOL UpdateCtrlInfo(int &nCtrlId,int &nCltrIndex,int nParentId);

	BOOL UpdateDlgAttr(__DLGAttr & attr,int& nDlgId,int nParentId,int nSysId);

	BOOL UpdateButtonAttr(__ButtonAttr & attr,int& nId,int nParentId);

	BOOL UpdateStaticAttr(__StaticAttr &attr,int& nId,int nParentId,int nCtrlType=2);

	BOOL UpdateDigitalStaticAttr(__DigitalStaticAttr &attr,int& nId,int nParentId,int nCtrlType);

	BOOL UpdateSwitchStatic(__SwitchStaticAttr & attr ,int& nId,int nParentId);

	BOOL UpdateGroup(__GroupAttr & attr ,int& nId,int nParentId);

	BOOL UpdateSwitchButton(__SwitchButtonAttr & attr ,int& nId,int nParentId);

	BOOL UpdateEdit(__EditAttr & attr ,int& nId,int nParentId);

	BOOL Update_ImageCtrl(__ImageCtrlAttr& attr,int& nId,int nParentId );

	BOOL Update_Static_Rectangle(__RectangleAttr& attr,int& nId,int nParentId  );

	BOOL UpdateDataConfig(vector<__DATA_CONFIG>&vec,int nDlgId,string sWhere="");

	// 根据Id更新数据项内容
	BOOL UpdateDataConfig(__DATA_CONFIG& uData,int nId);
	 
	BOOL Update_SysDrive(vector<__SYS_Drive>&vec,int nSysId);

	BOOL Update_CtrlEvent(vector<__Ctrl_Event>&vec,int nCtrlId);
	
	BOOL Update_DutMark(int nDlgId,int nMark);

	BOOL Update_ImageCtrl_Image(vector<string>&vec,int nCtrlId);

	BOOL Update_TabAttr(__TabAttr &attr,int &nId,int nParentId);

	BOOL Update_LineAttr(__LineAttr & attr,int nId,int nParentId);

	BOOL Delete_Ctrl(__CtrlInfo &info);

	BOOL Delete_Ctrl(int nCtrlId);
	 
	void Delete_Dlg(int nDlgId);	
 	 
	BOOL Copy_Ctrl(int nSrcCtrlId,int nSrcCtrlType,int nToParentId);
	
	BOOL Update_TestTip(vector<__Ctrl_TestTip>&vec,int nCtrlId,int nIndex);
	
	BOOL Update_UnitData(vector<__UnitData>&vec,int nSysId);
	
	void Delete_UnitData(int nId);	 
	 
	BOOL Update_Goods_Number(int nDlgId,int nValues[5],int nStartIndex);

	BOOL Update_Image_Button_V02(__ButtonAttr_V02 &attr,int& nCtrlId,int nParentId);

	BOOL Update_Image_Led_V02(__ButtonAttr_V02 &attr,int& nCtrlId,int nParentId);

	BOOL Update_Ctrl_FontColor(vector<COLORREF>&vec,int nCtrlId,int nType);

	 
	 
public:

	void Delete_CtrlEvent_Text(int nTextId,int nCtrlId);

	bool Query_CtrlEvent_Text(vector<__Ctrl_Event_Text>&vec,int nTextId,int nCtrlId,int nEventType =-1);

	void Update_CtrlEvent_Text(vector<__Ctrl_Event_Text>&vec,int nTextId,int nCtrlId);

	void Delete_CtrlText(int nId,int nCtrlId);

	int  Query_CtrlText_Id(int nCtrlId,int nType =0);

	bool Query_CtrlText(vector<__TextAttr_V02>&vec,int nCtrlId,int nType =-1);

	bool Update_CtrlText(__TextAttr_V02 &info,int nCtrlId,int nType);

	bool Update_CtrlText(vector<__TextAttr_V02>&vec,int nCtrlId,int nType);

	//通信协议更新、查询
	bool Update_Comm_Protocol(__Comm_Protocol & info,int nDlgId,int nSysDriveId);

	bool Update_Comm_Protocel_Detail(string &sInfo,int nId);//nType :0 ,send,1:rec

	bool Delete_Comm_Protocel(int nId);

	bool Delete_Comm_Protocel(CString strWhere);

	bool Query_Comm_Protocol(vector<__Comm_Protocol>&vec,int nDlgId,int nSysDriveId);

	bool Query_Comm_Protocol_Detail(string &sInfo,int nId);	
	
	bool Query_1553B_All_Protocol(vector<__Comm_Protocol>&vec);

	bool Query_CAN_All_Protocol(vector<__CAN_Protocol>&vec);

	//配置信息
	bool Query_Config(string skey,string &sInfo,int &nId);
	bool Update_Config(string sKey,string sInfo);

	BOOL Update_Image_Rotate_V02(__RotateAttr_V02 &attr,int& nCtrlId,int nParentId);
	
	//测试系统信息
	bool Query_TestSysInfo(vector<__TestSysInfo>&vec);
	bool Update_TestSysInfo(string sName,string sMark,int &nId);

	//测试系统信息,窗口信息
	bool Query_TestSysInfo_UI(vector<__TestSysInfo_UI>&vec,int nSysId ,CString strWhere ="");
	bool Update_TestSysInfo_UI(int nSysId,string sName,string sMark,int nUIType,int &nUIDlgId);

	bool Query_1553B_Protocol_Detail(string &sInfo,int nId);
private:
	BOOL m_bConnected;

	 CppSQLite3DB	m_SQLiteDB; 
	 string m_strDbFileName;
};

