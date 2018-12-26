#include "stdafx.h"
#include "ManageDb.h"
#include "Markup.h"
#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/locale.hpp"
#include "boost/format.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  CHECK_NO_RETURN  if(!m_bConnected) return ;
#define  CHECK_RETURN  if(!m_bConnected) return  false;

CManageDb_Sqllite_Sys::CManageDb_Sqllite_Sys()
{
	ConnectDb();
}
CManageDb_Sqllite_Sys::~CManageDb_Sqllite_Sys()
{
	m_SQLiteDB.close();
}

int CManageDb_Sqllite_Sys::GetMaxId(const char *lpSql)
{	  
	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());
	int nMaxId=0;
	if(!Query.eof())
	{   
		nMaxId = Query.getIntField(0,0);
	}

	return nMaxId; 
}

BOOL CManageDb_Sqllite_Sys::ConnectDb()
{
	CString strPathdb = GetExecFilePath()+"HK_Sys.db";
	try
	{
		//gbkתutf8
		std::string strPath = boost::locale::conv::between(strPathdb, "UTF-8", "GBK");
		m_SQLiteDB.open(strPath.c_str());
		m_bConnected = true;
	}
	catch(CppSQLite3Exception& ex)
	{
		::OutputDebugString(ex.errorMessage());
		m_bConnected = false;
	}

	return m_bConnected;	

}
 BOOL CManageDb_Sqllite_Sys::Update_Sys(string sSysName,string sKey,int &nId)
 {
	 char lpSql[1024];
	 sprintf(lpSql,"select max(nId) from t_sys");
	 nId = GetMaxId(lpSql)+1;
	 sprintf(lpSql,"insert into t_sys values(%d,'%s','%s')",nId,sSysName.c_str(),sKey.c_str());

	 CString strSrcPath = GetExecFilePath()+"HK_standard.db";
	 CString strDestPath = GetExecFilePath()+sKey.c_str()+".db";

	 CopyFile(strSrcPath,strDestPath,true);
	 std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	
	 return m_SQLiteDB.execDML(sql.c_str()); 	 
 
 }
  BOOL CManageDb_Sqllite_Sys::Query_Sys(vector<__SysInfo>&vec)
  {
	  char lpSql[256];
	  sprintf(lpSql,"select * from t_sys ");
	  std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	  CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	  int n=0;
	  while(!Query.eof())
	  {   
		  __SysInfo info;
		  n=0;
		  info.nId = Query.getIntField(n++,0);
		  string str =Query.getStringField(n++,"");
		  info.sName =boost::locale::conv::between(str.c_str(),  "GB2312","UTF-8");
		  str =Query.getStringField(n++,"");
		  info.sKey =boost::locale::conv::between(str.c_str(),  "GB2312","UTF-8");
		  vec.push_back(info);
		  Query.nextRow();

	  }
	  return true;	  
  }


  BOOL CManageDb_Sqllite_Sys::TableIsValid(string sTableName, string* pFieldName, int nFieldNum, int& nErrId)
  {
	  nErrId = 0;

	  char lpTbName[256];
	  sprintf(lpTbName,"%s",sTableName.c_str());

	  std::string sql_tb = boost::locale::conv::between(lpTbName, "UTF-8", "GBK");

	  BOOL bTableExist = m_SQLiteDB.tableExists(sql_tb.c_str());
	  if(!bTableExist)
	  {
		  nErrId = 1;
		  return FALSE;
	  }

	  for(int i = 0; i < nFieldNum; ++ i)
	  {
		  char lpFieldName[1024];
		  sprintf(lpFieldName,"select count(*) from sqlite_master where name='%s' and sql like '%%%s%%';",sTableName.c_str(),pFieldName[i].c_str());

		  std::string sql_fd = boost::locale::conv::between(lpFieldName, "UTF-8", "GBK");

		  CppSQLite3Query Query = m_SQLiteDB.execQuery(sql_fd.c_str());

		  if(Query.eof())
		  {
			  nErrId = 2;
			  return FALSE;
		  }
	  }

	  return TRUE;
  }


CManageDb_Sqllite::CManageDb_Sqllite(void)
{
	//ConnectDb();
	//CreateTable();
}


CManageDb_Sqllite::~CManageDb_Sqllite(void)
{
	Close();
}

CManageDb_Sqllite &CManageDb_Sqllite::GetInstance()
{
	static CManageDb_Sqllite inst;
	return inst;
}

BOOL CManageDb_Sqllite::ConnectDb()
{
	CString strPathdb = GetExecFilePath()+m_strDbFileName.c_str();
	try
	{
		//gbkתutf8
		std::string strPath = boost::locale::conv::between(strPathdb, "UTF-8", "GBK");
		m_SQLiteDB.open(strPath.c_str());
		m_bConnected = true;
	}
	catch(CppSQLite3Exception& ex)
	{
		::OutputDebugString(ex.errorMessage());
		m_bConnected = false;
	}

	return m_bConnected;	

} 

//ip
void CManageDb_Sqllite::CreateTable()
{
	 
	 
}

int CManageDb_Sqllite::GetMaxId(const char *lpSql)
{	 
	CHECK_RETURN;
	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());
	int nMaxId=0;
	if(!Query.eof())
	{   
		nMaxId = Query.getIntField(0,0);
	}

	return nMaxId; 
}

BOOL CManageDb_Sqllite::QueryDataId(const char *lpSql,int& nId)
{
	CHECK_RETURN;
	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());
	if(!Query.eof())
	{
		nId = Query.getIntField(0,0);
		return TRUE;
	}

	return FALSE; 
}

BOOL CManageDb_Sqllite::ExecDML(char * lpSql)
{
	CHECK_RETURN;
	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	m_SQLiteDB.execDML(sql.c_str()); 
	return TRUE;
}

BOOL CManageDb_Sqllite::TableIsValid(string sTableName, string* pFieldName, int nFieldNum, int& nErrId)
{
	nErrId = 0;

	char lpTbName[256];
	sprintf(lpTbName,"%s",sTableName.c_str());

	std::string sql_tb = boost::locale::conv::between(lpTbName, "UTF-8", "GBK");

	BOOL bTableExist = m_SQLiteDB.tableExists(sql_tb.c_str());
	if(!bTableExist)
	{
		nErrId = 1;
		return FALSE;
	}

	for(int i = 0; i < nFieldNum; ++ i)
	{
		char lpFieldName[1024];
		sprintf(lpFieldName,"select count(*) from sqlite_master where name='%s' and sql like '%%%s%%';",sTableName.c_str(),pFieldName[i].c_str());

		std::string sql_fd = boost::locale::conv::between(lpFieldName, "UTF-8", "GBK");

		CppSQLite3Query Query = m_SQLiteDB.execQuery(sql_fd.c_str());

		if(Query.eof())
		{
			nErrId = 2;
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CManageDb_Sqllite::Query_CtrlInfo(__CtrlInfo & info,int nId)
{
	char lpSql[128];
	sprintf(lpSql," select  a.* from t_ctrl a where  a.nId = %d ",nId);

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());
	int n=0;
	if(!Query.eof())
	{   
		n=0;
		info.nId = Query.getIntField(n++,0);
		info.nIndex = Query.getIntField(n++,0);

		info.nInterfaceId  = Query.getIntField(n++,0);
		info.nType  = Query.getIntField(n++,0);
		info.nX  = Query.getIntField(n++,0);
		info.nY  = Query.getIntField(n++,0);
		info.nW  = Query.getIntField(n++,0);
		info.nH  = Query.getIntField(n++,0);		 
		
		info.sTitle =boost::locale::conv::between(Query.getStringField(n++,""),"GB2312","UTF-8");
		info.sAttr=  boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		
		info.nSharedId = Query.getIntField(n++,0);
		info.sName =  boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		
		info.sReserve1 =  boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		info.sReserve2 =  boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");

		return TRUE;		
	}
	return false;
}

BOOL CManageDb_Sqllite::Query_SubCtrlInfo(vector<__CtrlInfo> & vec,int nInterfaceId,int nCtrlType ,CString strWhere,CString strOrder)
{
	
	char lpSql[128];
	sprintf(lpSql,"select * from	t_ctrl where nInterfaceId = %d",nInterfaceId);
	if(nCtrlType!=-1)
		sprintf(lpSql +strlen(lpSql)," and nType = %d ",nCtrlType);
	sprintf(lpSql+strlen(lpSql),"  %s",strWhere);
	
	sprintf(lpSql+strlen(lpSql)," %s ",strOrder);

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());
	int n=0;
	while(!Query.eof())
	{   
		__CtrlInfo info;
		 n=0;
		info.nId = Query.getIntField(n++,0);
		info.nIndex = Query.getIntField(n++,0);
		info.nInterfaceId  = Query.getIntField(n++,0);
		info.nType  = Query.getIntField(n++,0);
		info.nX  = Query.getIntField(n++,0);
		info.nY  = Query.getIntField(n++,0);
		info.nW  = Query.getIntField(n++,0);
		info.nH  = Query.getIntField(n++,0);			 
		
		info.sTitle =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");		 
		info.sAttr=  boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");

		info.nSharedId = Query.getIntField(n++,0);
		info.sName =  boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");

		info.sReserve1 =  boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		info.sReserve2 =  boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");

		vec.push_back(info);
		Query.nextRow();
		 
	}
	return TRUE;
	 
}
BOOL CManageDb_Sqllite::Query_Sys(vector<__SysInfo>&vec)
{
	char lpSql[256];
	sprintf(lpSql,"select * from t_sys ");
	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	while(!Query.eof())
	{   
		__SysInfo info;
		n=0;
		info.nId = Query.getIntField(n++,0);
		string str =Query.getStringField(n++,"");
		info.sName =boost::locale::conv::between(str.c_str(),  "GB2312","UTF-8");
		str =Query.getStringField(n++,"");
		info.sKey =boost::locale::conv::between(str.c_str(),  "GB2312","UTF-8");
		vec.push_back(info);
		Query.nextRow();

	}
	return true;
}
BOOL CManageDb_Sqllite::Query_DlgInfo(int nDlgId,string &sInfo)
{
	char lpSql[256];
	sprintf(lpSql,"select Info from t_sys_ctrl where nDlgId  = %d ",nDlgId);
	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	if(!Query.eof())
	{   
		sInfo=boost::locale::conv::between(Query.getStringField(0,""),  "GB2312","UTF-8"); 
		return true;
	}
	return false;
}
BOOL CManageDb_Sqllite::Query_DlgInfo(vector<__DLGInfo>&vec,int nSysId)
{
	char lpSql[256];
	if(nSysId !=-1)
		sprintf(lpSql,"select * from t_sys_ctrl where nSysId  = %d order by nDlgId",nSysId);
	else
		sprintf(lpSql,"select * from t_sys_ctrl  order by nDlgId");

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	 int n=0;
	while(!Query.eof())
	{   
		__DLGInfo info;
		n=0;
		info.nSysId = Query.getIntField(n++,0);
		info.nId  = Query.getIntField(n++,0);		
		string str =Query.getStringField(n++,"");
		info.sInfo =boost::locale::conv::between(str.c_str(),  "GB2312","UTF-8");
		
		vec.push_back(info);
		Query.nextRow();

	}
	return true;
}

BOOL CManageDb_Sqllite::Query_ShareId(__DATA_CONFIG & info,int nDlgId,int nSharedId)
{
	char lpSql[1024];
	sprintf(lpSql,"select * from t_data_config where nDlgId = %d and nSharedId = %d ",nDlgId,nSharedId);

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	if(!Query.eof())
	{    
		n=0;
		info.nId = Query.getIntField(n++,0);
		n++;

		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.nDutIndex = Query.getIntField(n++,0);

		info.nSysDriveId = Query.getIntField(n++,0);
		info.nDriveId = Query.getIntField(n++,0);

		info.nChannelIndex = Query.getIntField(n++,0);
		info.nSharedID = Query.getIntField(n++,0);	

		info.nDataType = Query.getIntField(n++,0);	
		info.nDataSize = Query.getIntField(n++,0);				

		info.fCoefficient = Query.getFloatField(n++,1);	
		info.fOffset = Query.getFloatField(n++,1);	
		info.nInOut = Query.getIntField(n++,0);	

		info.sMark =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.nDealMode = Query.getIntField(n++,0);	

		return true;
	}	
	return false;
}

bool CManageDb_Sqllite::Query_GridInfo(vector<TBLDATA>&vec, int nDlgId)
{
	char lpSql[1024];
	sprintf(lpSql,"select * from t_grid where nDlgId = %d;",nDlgId);

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	TBLDATA info;

	int n=0;
	while(!Query.eof())
	{    
		n=0;
		info.nId = Query.getIntField(n++,0);

		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.sUnit =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.fValue = Query.getFloatField(n++,0);

		info.nPrecision = Query.getIntField(n++,0);
		info.sRef =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.sStatus =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.fMinV = Query.getFloatField(n++,0);
		info.fMaxV = Query.getFloatField(n++,0);
		info.nSharedId = Query.getIntField(n++,0);	

		info.nDlgId = Query.getIntField(n++,0);	
		vec.push_back(info);
	}	
	return true;
}


BOOL CManageDb_Sqllite::Query_ShareId(vector<__DATA_CONFIG>&vec,int nDlgId)
{ 
	return false;
	char lpSql[4][1024];
	/*sprintf(lpSql[0],"select * from t_data_config where nSharedId in (  select  nShareId from t_ctrl a where a.nInterfaceId  =  %d)",nDlgId);
	sprintf(lpSql[1],"select * from t_data_config where nSharedId in (select nShareId from t_ctrl where nInterfaceId in (select  nId from t_ctrl a where a.nInterfaceId  =  %d and   nType =0 ))",nDlgId);
	sprintf(lpSql[2],"select * from t_data_config where nSharedId in  (select nShareId from t_ctrl where nType = 0  and nInterfaceId in  (select nId from t_ctrl where nInterfaceId = %d   and  nType = 7))",nDlgId);

	sprintf(lpSql[3],"select * from t_data_config where nSharedId in ( select nSharedId from t_ctrl_event where nctrlid in ( select nId from t_ctrl where nInterfaceId in(select   nId from t_ctrl a where a.nInterfaceId  = %d ))  and nEventType == 3)",nDlgId);*/

	for(int i=0;i<4;i++)
	{
		std::string sql = boost::locale::conv::between(lpSql[i], "UTF-8", "GBK");

		CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

		int n=0;
		while(!Query.eof())
		{   
			__DATA_CONFIG info;
			n=0;
			info.nId = Query.getIntField(n++,0);
			n++;

			info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
			info.nDutIndex = Query.getIntField(n++,0);
			
			info.nSysDriveId = Query.getIntField(n++,0);
			info.nDriveId = Query.getIntField(n++,0);

			info.nChannelIndex = Query.getIntField(n++,0);
			info.nSharedID = Query.getIntField(n++,0);	

			info.nDataType = Query.getIntField(n++,0);	
			info.nDataSize = Query.getIntField(n++,0);				

			info.fCoefficient =Query.getFloatField(n++,1);
			info.fOffset =Query.getFloatField(n++,1);

			info.nInOut = Query.getIntField(n++,0);	
			
			info.sMark =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	

			vec.push_back(info);
			Query.nextRow(); 
		}	
	}
	return true;
}

BOOL CManageDb_Sqllite::Query_Drive(vector<__DRIVE>&vec)
{ 
	char lpSql[1024];
	sprintf(lpSql,"select * from t_drive ");

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	while(!Query.eof())
	{   
		__DRIVE info;
		n=0;
		info.nId = Query.getIntField(n++,0);		
		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");		
		vec.push_back(info);
		Query.nextRow();
	}	
	return true;
}
BOOL CManageDb_Sqllite::Query_SysDrive(vector<__SYS_Drive>&vec,int nSysId,string sWhere)
{
	char lpSql[1024];
	if(nSysId!=-1)
 		sprintf(lpSql,"select a.*,b.ndriveType,b.smark from t_sys_drive a,t_drive b  where   a.nDriveid  =b.nid  and a.nSysId = %d  %s order by a.nId;",nSysId,sWhere.c_str());
  	else if(sWhere.size()>0)		
 		sprintf(lpSql,"select a.*,b.ndriveType,b.smark from t_sys_drive a,t_drive b  where   a.nDriveid  =b.nid   %s order by a.nId;",sWhere.c_str());
	else
		sprintf(lpSql,"select a.*,b.ndriveType,b.smark from t_sys_drive a,t_drive b  where   a.nDriveid  =b.nid   order by a.nId;" );
	 
	 
	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	while(!Query.eof())
	{   
		__SYS_Drive info;
		n=0;
		info.nId = Query.getIntField(n++,0);
		n++;

		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.sDriveName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.nDriveId =Query.getIntField(n++,0);
		info.nBusNumber =Query.getIntField(n++,0);
		info.nDeviceNumber=Query.getIntField(n++,0);
		info.sAttr =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.nAutoConnect=Query.getIntField(n++,0);
		info.nDriveType=Query.getIntField(n++,0);
		info.sMark =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		vec.push_back(info);
		Query.nextRow();
	}	
	return true;
}

BOOL CManageDb_Sqllite::Update_Sys(int &nId,string sName,string sKey)
{
	char lpSql[1014];
	sprintf(lpSql,"select max(nId) from t_sys");
	nId = GetMaxId(lpSql)+1;
	
	sprintf(lpSql,"insert into t_sys values(%d,'%s','%s')",nId,sName.c_str(),sKey.c_str());

	return ExecDML(lpSql);
}

BOOL CManageDb_Sqllite::UpdateCtrlInfo(int &nCtrlId,int &nCtrlIndex,int nParentId)
{
	char lpSql[2048];	 
	if(nCtrlId!=-1)
	{
		sprintf(lpSql,"select  nIndex from t_ctrl where nId  =%d",nCtrlId);
		nCtrlIndex = GetMaxId(lpSql);
		if(nCtrlIndex==0)
		{
			sprintf(lpSql,"select  max(nIndex) from t_ctrl where nInterfaceId   =%d",nParentId);
			nCtrlIndex = GetMaxId(lpSql)+1;		 
		}

		sprintf(lpSql,"delete from t_ctrl where nId = %d",nCtrlId);
		if(!ExecDML(lpSql))
			return false;
	}
	else 
	{
		
		nCtrlId = GetMaxId("select max(nId) from t_ctrl ")+1;	
		if(nCtrlId>=1)
			return true;

		sprintf(lpSql,"select  max(nIndex) from t_ctrl where nInterfaceId   =%d",nParentId);
		nCtrlIndex = GetMaxId(lpSql)+1;		 
	}
 return true;

}
BOOL CManageDb_Sqllite::UpdateDlgAttr(__DLGAttr & attr,int& nDlgId,int nParentId,int nSysId)
{
	char lpSql[2048];

	if(nDlgId!=-1)
	{
		sprintf(lpSql,"delete from t_ctrl where nId = %d",nDlgId);
		if(!ExecDML(lpSql))
			return false;
		//nSysId int,nDlgId 
		if(attr.nStyle ==DLG_STYLE_POPOP)
		{
			sprintf(lpSql,"delete from t_sys_ctrl where nSysId = %d and nDlgId = %d ",nSysId,nDlgId);
			if(!ExecDML(lpSql))
				return false;
		}
	}
	else 
	{
		nDlgId = GetMaxId("select max(nId) from t_ctrl ")+1;			
	}

	if(attr.nStyle ==DLG_STYLE_POPOP)
	{
		sprintf(lpSql,"insert into t_sys_ctrl values(%d,%d,'%s')",nSysId,nDlgId,attr.sName.c_str());
		if(!ExecDML(lpSql))
			return false;
	}

	CMarkup xml;
	xml.AddElem("Attr");
	xml.AddAttrib("BackColor",attr.backColor);
	if(attr.nTimer >0)
		xml.AddAttrib("Timer",attr.nTimer);
	if(attr.nDutIndex>=0)
		xml.AddAttrib("DutIndex",attr.nDutIndex);

	CMarkup xmlChild;
	xmlChild.AddElem("Title");
	xmlChild.AddAttrib("Height",attr.nTitleH);
	xmlChild.AddAttrib("Color",attr.colorTitle);
	xmlChild.AddAttrib("FontColor",attr.colorFont);
	xmlChild.AddAttrib("FontSize",attr.nFontSize);
	
		

	xml.AddChildSubDoc(xmlChild.GetSubDoc());
	CString strXml = xml.GetSubDoc();

	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',0,'%s','','')",nDlgId,attr.nCtrlIndex,nParentId,WND_type_dlg,attr.nX,attr.nY,attr.nW,attr.nH,attr.sTitle.c_str(),strXml,
		attr.sName.c_str());

	return ExecDML(lpSql);
}


BOOL CManageDb_Sqllite::UpdateButtonAttr(__ButtonAttr & attr,int& nId,int nParentId)
{
	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nId,nIndex,nParentId);
	//if(nId!=-1)
	//{
	//	sprintf(lpSql,"delete from t_ctrl where nId = %d",nId);
	//	if(!ExecDML(lpSql))
	//		return false;
	//}
	//else
	//	nId = GetMaxId("select max(nId) from t_ctrl ")+1;	
	
	CMarkup xml;
	xml.AddElem("Attr");
	xml.AddAttrib("BackColor",attr.backColor);
	xml.AddAttrib("LineColor",attr.lineColor);
	xml.AddAttrib("FontSize",attr.nFontSize);	 
	xml.AddAttrib("FontColor",attr.fontColor);
	

	for(int i=0,end = attr.actVec.size();i<end;i++)
	{
		CMarkup xmlChild;
		xmlChild.AddElem("Act");
		xmlChild.AddAttrib("Name",attr.actVec[i].sName.c_str());
		xmlChild.AddAttrib("Param",attr.actVec[i].sParam.c_str());
		xml.AddChildSubDoc(xmlChild.GetSubDoc());
	}
	CString strXml = xml.GetSubDoc();

	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',0,'%s','','')",nId,attr.nCtrlIndex,nParentId,WND_type_button,attr.nX,attr.nY,attr.nW,attr.nH,attr.sTitle.c_str(),strXml,
		attr.sName.c_str());

	return ExecDML(lpSql);
}

BOOL CManageDb_Sqllite::UpdateStaticAttr(__StaticAttr &attr,int& nId,int nParentId,int nCtrlType)
{
	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nId,nIndex,nParentId);
	/*if(nId!=-1)
	{
	sprintf(lpSql,"delete from t_ctrl where nId = %d",nId);
	if(!ExecDML(lpSql))
	return false;
	}
	else
	{
	nId = GetMaxId("select max(nId) from t_ctrl ")+1;	
	}*/

	CMarkup xml;
	xml.AddElem("Attr");
	xml.AddAttrib("Back",attr.bBackColor);
	xml.AddAttrib("BackColor",attr.backColor);
	xml.AddAttrib("Align",attr.nAlign);
	xml.AddAttrib("FontSize",attr.nFontSize);	
	xml.AddAttrib("FontColor",attr.fontColor);
	xml.AddAttrib("FontName",attr.sFontName.c_str());

	CString strXml = xml.GetSubDoc();


	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',0,'%s','','')",nId,attr.nCtrlIndex,nParentId,nCtrlType,attr.nX,attr.nY,attr.nW,attr.nH,attr.sTitle.c_str(),strXml,
		attr.sName.c_str());

	return ExecDML(lpSql);
}

BOOL CManageDb_Sqllite::UpdateDigitalStaticAttr(__DigitalStaticAttr &attr,int& nId,int nParentId,int nCtrlType)
{
	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nId,nIndex,nParentId);
	/*if(nId!=-1)
	{
	sprintf(lpSql,"delete from t_ctrl where nId = %d",nId);
	if(!ExecDML(lpSql))
	return false;
	}
	else
	{
	nId = GetMaxId("select max(nId) from t_ctrl ")+1;	
	}*/

	CMarkup xml;
	xml.AddElem("Attr");
	xml.AddAttrib("Back",attr.bBackColor);
	xml.AddAttrib("BackColor",attr.backColor);
	xml.AddAttrib("Align",attr.nAlign);
	xml.AddAttrib("FontSize",attr.nFontSize);	
	xml.AddAttrib("FontColor",attr.fontColor);
	xml.AddAttrib("FontName",attr.sFontName.c_str());

	xml.AddAttrib("CtrlType",attr.nCtrlType);
	xml.AddAttrib("DispContend",attr.sDispContend.c_str());
	xml.AddAttrib("ValueRange",attr.sRange.c_str());

	CString strXml = xml.GetSubDoc();


	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','','')",nId,attr.nCtrlIndex,nParentId,nCtrlType,attr.nX,attr.nY,attr.nW,attr.nH,attr.sTitle.c_str(),strXml,
		attr.nSharedId,attr.sName.c_str());

	return ExecDML(lpSql);
}

BOOL CManageDb_Sqllite::UpdateSwitchStatic(__SwitchStaticAttr & attr ,int& nId,int nParentId)
{

	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nId,nIndex,nParentId);

	//if(nId!=-1)
	//{
	//	sprintf(lpSql,"delete from t_ctrl where nId = %d",nId);
	//	if(!ExecDML(lpSql))
	//		return false;
	//}
	//else
	//{
	//	nId = GetMaxId("select max(nId) from t_ctrl ")+1;	
	//}

	CMarkup xml;
	xml.AddElem("Attr");
	xml.AddAttrib("Back",attr.bBackColor);
	xml.AddAttrib("BackColor",attr.backColor);
	xml.AddAttrib("LineColor",attr.lineColor);
	xml.AddAttrib("Default",attr.nDefaultValue);	 	
	for( map<int,COLORREF>::iterator it = attr.valuesMap.begin();it!=attr.valuesMap.end();it++)	
	{
		CMarkup xmlChild;
		xmlChild.AddElem("Color");
		xmlChild.AddAttrib("Index",it->first);
		xmlChild.AddAttrib("Value",it->second);
		xml.AddChildSubDoc(xmlChild.GetSubDoc());
	}
	CString strXml = xml.GetSubDoc();


	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','','')",nId,attr.nCtrlIndex,nParentId,WND_type_switch_static,attr.nX,attr.nY,attr.nW,attr.nH,attr.sTitle.c_str(),strXml,
		attr.nSharedId,attr.sName.c_str());

	return ExecDML(lpSql);
}


BOOL CManageDb_Sqllite::UpdateGroup(__GroupAttr & attr ,int& nId,int nParentId)
{
	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nId,nIndex,nParentId);
	//if(nId!=-1)
	//{
	//	sprintf(lpSql,"delete from t_ctrl where nId = %d",nId);
	//	if(!ExecDML(lpSql))
	//		return false;
	//}
	//else
	//{
	//	nId = GetMaxId("select max(nId) from t_ctrl ")+1;	
	//}

	CMarkup xml;
	xml.AddElem("Attr");	
	xml.AddAttrib("FontColor",attr.fontColor);
	xml.AddAttrib("FontSize",attr.nFontSize);
	

	CString strXml = xml.GetSubDoc();


	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',0,'%s','','')",nId,attr.nCtrlIndex,nParentId,WND_type_group,attr.nX,attr.nY,attr.nW,attr.nH,attr.sTitle.c_str(),strXml,
		attr.sName.c_str());

	return ExecDML(lpSql);
}


BOOL CManageDb_Sqllite::UpdateSwitchButton(__SwitchButtonAttr & attr ,int& nId,int nParentId)
{
	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nId,nIndex,nParentId);
	/*if(nId!=-1)
	{
	sprintf(lpSql,"delete from t_ctrl where nId = %d",nId);
	if(!ExecDML(lpSql))
	return false;
	}
	else
	{
	nId = GetMaxId("select max(nId) from t_ctrl ")+1;	
	}*/

	CMarkup xml;
	xml.AddElem("Attr");	
	xml.AddAttrib("Enable",attr.bEnable);
	xml.AddAttrib("DefualtValue",attr.nDefualtValue);
	for(int i =0;i<2;i++)
	{
		CMarkup xmlChild;
		xmlChild.AddElem("State");
		xmlChild.AddAttrib("Index",i);
		xmlChild.AddAttrib("Value",attr.nValues[i]);
		xml.AddChildSubDoc(xmlChild.GetSubDoc());
	}

	CString strXml = xml.GetSubDoc();


	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','','')",nId,attr.nCtrlIndex,nParentId,WND_type_swtich_button,attr.nX,attr.nY,attr.nW,attr.nH,attr.sName.c_str(),strXml,
		attr.nSharedId,attr.sName.c_str());

	return ExecDML(lpSql);
}
BOOL CManageDb_Sqllite::UpdateEdit(__EditAttr & attr ,int& nId,int nParentId)
{
	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nId,nIndex,nParentId);
	/*if(nId!=-1)
	{
	sprintf(lpSql,"delete from t_ctrl where nId = %d",nId);
	if(!ExecDML(lpSql))
	return false;
	}
	else
	{
	nId = GetMaxId("select max(nId) from t_ctrl ")+1;	
	}*/

	CMarkup xml;
	xml.AddElem("Attr");
	xml.AddAttrib("ValueRange",attr.sRange.c_str());

	CString strXml = xml.GetSubDoc();
	 
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','','')",nId,attr.nCtrlIndex,nParentId,WND_type_edit,attr.nX,attr.nY,attr.nW,attr.nH,attr.sName.c_str(),strXml,
		attr.nSharedId,attr.sName.c_str());

	return ExecDML(lpSql);
}


BOOL CManageDb_Sqllite::Update_ImageCtrl(__ImageCtrlAttr& attr,int& nId,int nParentId )
{
	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nId,nIndex,nParentId);
	/*if(nId!=-1)
	{
	sprintf(lpSql,"delete from t_ctrl where nId = %d",nId);
	if(!ExecDML(lpSql))
	return false;
	}
	else
	{
	nId = GetMaxId("select max(nId) from t_ctrl ")+1;	
	}*/

	CMarkup xml;
	xml.AddElem("Attr");	
	xml.AddAttrib("FontColor",attr.fontColor);
	xml.AddAttrib("FontSize",attr.nFontSize);
 

	CString strXml = xml.GetSubDoc();
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','','')",nId,attr.nCtrlIndex,nParentId,attr.nCtrlType,attr.nX,attr.nY,attr.nW,attr.nH,attr.sTitle.c_str(),strXml,
		0,attr.sName.c_str());

	return ExecDML(lpSql);
}


BOOL CManageDb_Sqllite::Update_Static_Rectangle(__RectangleAttr& attr,int& nId,int nParentId )
{

	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nId,nIndex,nParentId);
	//if(nId!=-1)
	//{
	//	sprintf(lpSql,"delete from t_ctrl where nId = %d",nId);
	//	if(!ExecDML(lpSql))
	//		return false;
	//}
	//else
	//{
	//	nId = GetMaxId("select max(nId) from t_ctrl ")+1;	
	//}

	CMarkup xml;
	xml.AddElem("Attr");	
	xml.AddAttrib("FontColor",attr.colorFont);
	xml.AddAttrib("FontSize",attr.nFontSize);
	for(int i =0;i<2;i++)
	{
		CMarkup xmlChild;
		xmlChild.AddElem("Shared");
		xmlChild.AddAttrib("Value",attr.vs[i].nValue);
		xmlChild.AddAttrib("Color",attr.vs[i].colorRef);
		xml.AddChildSubDoc(xmlChild.GetSubDoc());
	}

	CString strXml = xml.GetSubDoc();
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','','')",nId,attr.nCtrlIndex,nParentId,attr.nCtrlType,attr.nX,attr.nY,attr.nW,attr.nH,attr.sTitle.c_str(),strXml,
		0,attr.sName.c_str());

	return ExecDML(lpSql);
}

//更新与设备有关系的共享内存
BOOL CManageDb_Sqllite::UpdateDataConfig(vector<__DATA_CONFIG>&vec,int nDlgId,string sWhere)
{
	//char lpSql[1024];
	//sprintf(lpSql,"delete from t_data_config where nDlgId = %d %s ",nDlgId,sWhere.c_str());
	//ExecDML(lpSql);
	//for(int  i=0,end = vec.size();i<end;i++)
	//{
	//	sprintf(lpSql,"insert into t_data_config values(%d,%d,'%s',%d,%d,  %d ,  %d,%d,%d,%d,%f,%d,'%s',%d,'')",
	//		i+1,nDlgId,vec[i].sName.c_str(),vec[i].nDutIndex,vec[i].nSysDriveId, vec[i].nDriveId,vec[i].nChannelIndex,vec[i].nSharedID,vec[i].nDataType,vec[i].nDataSize,vec[i].fTimes,
	//		vec[i].nInOut,vec[i].sMark.c_str(),vec[i].nDealMode);
	//	ExecDML(lpSql);

	//}
	return true;
}

// 根据Id更新数据项内容
BOOL CManageDb_Sqllite::UpdateDataConfig(__DATA_CONFIG& uData,int nId)
{

	return TRUE;
}

BOOL CManageDb_Sqllite::Update_SysDrive(vector<__SYS_Drive>&vec,int nSysId)
{
	char lpSql[1024];

	for(int  i=0,end = vec.size();i<end;i++)
	{
		if(vec[i].nId ==0)
		{
			sprintf(lpSql,"select max(nId) from t_sys_drive where nSysId = %d",nSysId);
			vec[i].nId  =GetMaxId(lpSql)+1;
		}
		else
		{
			sprintf(lpSql,"delete from t_sys_drive where nId = %d ",vec[i].nId);
			ExecDML(lpSql);
		}
		
		sprintf(lpSql,"insert into t_sys_drive values(%d,%d,'%s','%s',%d,  %d ,  %d,'%s',%d)",
			vec[i].nId,nSysId,vec[i].sName.c_str(),vec[i].sDriveName.c_str(),vec[i].nDriveId,vec[i].nBusNumber,vec[i].nDeviceNumber,vec[i].sAttr.c_str(),vec[i].nAutoConnect);
		ExecDML(lpSql);

	}
	return true;
}

BOOL CManageDb_Sqllite::Delete_DataConfig(CString strWhere)
{
	if(strWhere.IsEmpty())
		return FALSE;

	char lpSql[1024];
	sprintf(lpSql,"delete from t_data_config where %s;", strWhere);
	//return TRUE;
	return ExecDML(lpSql);
}

BOOL CManageDb_Sqllite::Query_DataConfig(vector<__DATA_CONFIG>& vec,int nDlgId,string strWhere)
{
	char lpSql[1024];
 	sprintf(lpSql,"select a.*,b.sName from t_data_config a ,t_sys_drive b where (a.nDlgId = %d and b.nId =a.nSysDriveId) %s;",nDlgId,strWhere.c_str());

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;	
	while(!Query.eof())
	{   
		__DATA_CONFIG info;
		n=0;
		info.nId = Query.getIntField(n++,0);
		n++;

		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.nDutIndex = Query.getIntField(n++,0);
		
		info.nSysDriveId = Query.getIntField(n++,0);
		info.nDriveId = Query.getIntField(n++,0);

		info.nChannelIndex = Query.getIntField(n++,0);
		info.nSharedID = Query.getIntField(n++,0);	
		
		info.nDataType = Query.getIntField(n++,0);	
		info.nDataSize = Query.getIntField(n++,0);	
		
		info.fCoefficient = Query.getFloatField(n++,1);	
		info.fOffset = Query.getFloatField(n++,1);	

		info.nInOut = Query.getIntField(n++,0);	

		info.sMark =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.nDealMode = Query.getIntField(n++,0);	
		info.sDefaultValue =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.sExtArr =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");

		info.sSysDriveName= boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		vec.push_back(info);
		
		Query.nextRow();
	}	
	return true;
}

BOOL CManageDb_Sqllite::Query_CtrlEvent(vector<__Ctrl_Event>&vec,int nCtrlId,int nEventType,int nGroup)
{
	vector<__Ctrl_Event>().swap(vec);

	char lpSql[1024];
	sprintf(lpSql,"Select * from t_Ctrl_Event where nCtrlId =%d ",nCtrlId);
	
	if(nEventType !=-1)
		sprintf(lpSql+strlen(lpSql),"  and nEventType = %d",nEventType);
	
	
	sprintf(lpSql+strlen(lpSql),"  and nGroup = %d",nGroup);


 
	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	while(!Query.eof())
	{   
		__Ctrl_Event info;
		n=0;
		info.nId = Query.getIntField(n++,0);
		info.nCtrlId = Query.getIntField(n++,0);
		info.nEventType = Query.getIntField(n++,0);
		info.nGroup = Query.getIntField(n++,0);

		info.nSharedId = Query.getIntField(n++,0);
		info.nValue1 = Query.getIntField(n++,0);
		info.nValue2 = Query.getIntField(n++,0);
		info.color = Query.getIntField(n++,0);
		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.sPath =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	

		info.sRemarks =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	

		vec.push_back(info);
		Query.nextRow();
	}	

	return true;
}
BOOL CManageDb_Sqllite::Update_CtrlEvent(vector<__Ctrl_Event>&vec,int nCtrlId)
{

	char lpSql[1024];
	sprintf(lpSql,"delete from t_Ctrl_Event where nCtrlId = %d",nCtrlId);
	ExecDML(lpSql);

	for(int i =0,end = vec.size();i<end;i++)
	{
		sprintf(lpSql,"insert into t_Ctrl_Event values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s')",i+1,nCtrlId,vec[i].nEventType,vec[i].nGroup, vec[i].nSharedId,
			vec[i].nValue1,vec[i].nValue2,vec[i].color, vec[i].sName.c_str(),vec[i].sPath.c_str(),vec[i].sRemarks.c_str());
		ExecDML(lpSql);
	}

	return TRUE;
}
int CManageDb_Sqllite::Query_DlgId(int nDutId,int nSysID)
{
	char lpSql[128];
	sprintf(lpSql,"select nDlgId from t_dut where nModBusValue = %d and nDlgId in (select nDlgId from t_sys_ctrl where nSysId = %d)",nDutId,nSysID);

	return  GetMaxId(lpSql);
}
int CManageDb_Sqllite::Query_DutMark(int nDlgId)
{
	char lpSql[128];
	sprintf(lpSql,"select nModBusValue from t_dut where nDlgId = %d",nDlgId);
	
	return  GetMaxId(lpSql);
	
}
BOOL CManageDb_Sqllite::Update_DutMark(int nDlgId,int nMark)
{
	char lpSql[128];
	sprintf(lpSql,"delete from t_dut where nDlgId = %d",nDlgId);
	ExecDML(lpSql);
	if(nMark>0)
	{	
		sprintf(lpSql,"insert into t_dut values(%d,%d,'')",nDlgId,nMark);
		ExecDML(lpSql);
	}

	return true;
}
BOOL CManageDb_Sqllite::Query_ImageCtrl_Image(map<int,string>&sMap,int nCtrlId)
{
	map<int,string>().swap(sMap);
	char lpSql[1024];

	sprintf(lpSql,"Select sPicPath from t_ctrl_image where nCtrlId =%d  order by nId",nCtrlId);


	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	string str;
	int nIndex=0;
	while(!Query.eof())
	{   
		str=boost::locale::conv::between(Query.getStringField(0,""),  "GB2312","UTF-8");	
		sMap[nIndex++] =str;
		 
		Query.nextRow();
	}	
	 
	return TRUE;
}
BOOL CManageDb_Sqllite::Query_ImageCtrl_Image(vector<string>&vec,int nCtrlId)
{
	 vector<string>().swap(vec);

	char lpSql[1024];
	
	sprintf(lpSql,"Select sPicPath from t_ctrl_image where nCtrlId =%d  order by nId",nCtrlId);
	

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	string str;
	while(!Query.eof())
	{   
		str=boost::locale::conv::between(Query.getStringField(0,""),  "GB2312","UTF-8");	

		vec.push_back(str);
		Query.nextRow();
	}	

	return true;
}
BOOL CManageDb_Sqllite::Update_ImageCtrl_Image(vector<string>&vec,int nCtrlId)
{
	char lpSql[1024];
	sprintf(lpSql,"delete from t_ctrl_image where nCtrlId = %d",nCtrlId);
	ExecDML(lpSql);

	for(int i =0,end = vec.size();i<end;i++)
	{
		sprintf(lpSql,"insert into t_ctrl_image values(%d,%d,'%s')",i+1,nCtrlId,vec[i].c_str());
		ExecDML(lpSql);
	}
	return true;
}
BOOL CManageDb_Sqllite::Delete_Ctrl(__CtrlInfo &info)
{
	if(info.nType == WND_type_dlg)
	{
		Delete_Ctrl(info.nId);
		if(info.nInterfaceId!=-1)
			return true;

		//删除t_sys_ctrl表中的记录
		char lpSql[1024];
		sprintf(lpSql,"delete from t_sys_ctrl where  nDlgId = %d",info.nId);
		ExecDML(lpSql);		
		sprintf(lpSql,"delete from t_goods_Number where  nDlgId = %d ",info.nId);
		ExecDML(lpSql);

	}
	else
	{
		Delete_Ctrl(info.nId);
	}
	return FALSE;
}
BOOL CManageDb_Sqllite::Delete_Ctrl(int nCtrlId)
{ 
	//查询是否有子控件
	vector<__CtrlInfo>  vec;
	Query_SubCtrlInfo(vec,nCtrlId);
	for(int i=0,end =vec.size();i<end;i++)
	{
		Delete_Ctrl(vec[i]);
	}

	char lpsSql[1024];
	sprintf(lpsSql,"delete from t_ctrl_event where nCtrlId =%d",nCtrlId);
	bool b=ExecDML(lpsSql);
	sprintf(lpsSql,"delete from t_ctrl_image where nCtrlId =%d",nCtrlId);
	b=ExecDML(lpsSql);
	sprintf(lpsSql,"delete from t_test_tip where nCtrlId =%d",nCtrlId);
	b=ExecDML(lpsSql);
	sprintf(lpsSql,"delete from t_ctrl where nId  =%d ",nCtrlId);
	b=ExecDML(lpsSql);

	sprintf(lpsSql,"delete from t_ctrl_text where nCtrlId  =%d ",nCtrlId);
	b=ExecDML(lpsSql);
	sprintf(lpsSql,"delete from t_ctrl_Font_Color where nCtrlId  =%d ",nCtrlId);
	b=ExecDML(lpsSql);

	sprintf(lpsSql,"delete from t_Ctrl_Event_Text where nCtrlId  =%d ",nCtrlId);
	b=ExecDML(lpsSql);

	sprintf(lpsSql,"delete from t_goods_Number where nDlgId  =%d ",nCtrlId);
	b=ExecDML(lpsSql);

	return true;
}
  

BOOL CManageDb_Sqllite::Update_TabAttr(__TabAttr &attr,int& nId,int nParentId)
{
	char lpSql[2048];
	int nIndex =attr.nCtrlIndex;
	UpdateCtrlInfo(nId,nIndex,nParentId);	 
	 
	CMarkup xml;
	xml.AddElem("Attr");	
	xml.AddAttrib("colorBack",attr.colorBack);
	xml.AddAttrib("colorLine",attr.colorLine);

	xml.AddAttrib("TabWidth",attr.nTabWidth);
	xml.AddAttrib("TabHeight",attr.nTabHeight);

	xml.AddAttrib("FontName",attr.sFontName.c_str());

	xml.AddAttrib("FontColor",attr.colorFont);
	xml.AddAttrib("FontSize",attr.nFontSize);

	xml.AddAttrib("colorSelTab",attr.colorSelTab);
	xml.AddAttrib("colorUnSelTab",attr.colorUnSelTab);	

	CString strXml = xml.GetSubDoc();
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','','')",nId,attr.nCtrlIndex,nParentId,WND_type_tab,attr.nX,attr.nY,attr.nW,attr.nH,attr.sTitle.c_str(),strXml,
		0,attr.sName.c_str());

	return ExecDML(lpSql);
}

BOOL CManageDb_Sqllite::Update_LineAttr(__LineAttr & attr,int nId,int nParentId)
{
	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nId,nIndex,nParentId);	 

	CMarkup xml;
	xml.AddElem("Attr");	
	xml.AddAttrib("LineWidth",attr.nLineWidth);
	xml.AddAttrib("colorLine",attr.colorLine);

	xml.AddAttrib("RealLineLength",attr.nRealLineLength);
	xml.AddAttrib("DottedLineLength",attr.nDottedLineLength);

	
	CString strXml = xml.GetSubDoc();
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','','')",nId,attr.baseAttr.nCtrlIndex,nParentId,WND_type_Line,
		attr.baseAttr.nX,attr.baseAttr.nY,attr.baseAttr.nW,attr.baseAttr.nH,"",strXml,
		0,attr.baseAttr.sName.c_str());

	return ExecDML(lpSql);
}

BOOL CManageDb_Sqllite::Copy_Ctrl(int nSrcCtrlId,int nSrcCtrlType,int nToParentId)
{
	//t_ctrl,t_ctrl_event,t_ctrl_image,t_ctrl_tip
	char lpSql[1024];
	int nToId =GetMaxId("select max(nId) from  t_ctrl")+1;
	
	sprintf(lpSql,"insert into t_ctrl select %d,nIndex,%d,nType ,nx,nY,nW,nH,title,Attr,0,name,reserve1,reserve2 from t_ctrl where nid = %d",nToId,nToParentId,nSrcCtrlId);
	ExecDML(lpSql);
	sprintf(lpSql,"insert into t_ctrl_event  select nId,%d,nEventType,nGroup,0,nValue_1,nValue_2,color,sname,sPath,sMark from t_ctrl_event   where nCtrlId = %d",nToId,nSrcCtrlId);
	ExecDML(lpSql);
	sprintf(lpSql,"insert into t_ctrl_image select nId,%d,sPicPath from t_ctrl_image where nCtrlId = %d",nToId,nSrcCtrlId);
	ExecDML(lpSql);

	sprintf(lpSql," insert into t_ctrl_text select nId,%d, nType,sTilte,sFontName,colorFont,nFontSize,nAlign, nX,nY from t_ctrl_text where nCtrlId = %d",nToId,nSrcCtrlId);
	ExecDML(lpSql);
	 
	if(nSrcCtrlType == WND_type_dlg)
	{
		vector<__CtrlInfo> vec;
		Query_SubCtrlInfo(vec, nSrcCtrlId);
		for(int i =0,end = vec.size();i<end;i++)
		{
			Copy_Ctrl(vec[i].nId,vec[i].nType,nToId);
		}
		return true;
	}
	
	return true;
}
 
BOOL CManageDb_Sqllite::Query_TestTip(vector<__Ctrl_TestTip>&vec,int nCtrlId)
{

	char lpSql[1024];
	sprintf(lpSql,"select a.*,b.name from t_test_tip a ,t_ctrl b where b.nId = a.nCtrlId and a.nCtrlId =%d order by a.nId",nCtrlId);
	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	while(!Query.eof())
	{   
		__Ctrl_TestTip info;
		n=0;
		info.nCtrlId = Query.getIntField(n++,0);
		info.nId = Query.getIntField(n++,0); 
		info.nIndex= Query.getIntField(n++,0); 
		info.sTip =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		 
		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		vec.push_back(info);
		Query.nextRow();

	}
	return true;
}
BOOL CManageDb_Sqllite::Update_TestTip(vector<__Ctrl_TestTip>&vec,int nCtrlId,int nIndex)
{
	 char lpSql[1024];
	sprintf(lpSql,"delete from t_test_tip where nCtrlId = %d",nCtrlId);
	ExecDML(lpSql);

	for(int i=0,end = vec.size();i<end;i++)
	{
		sprintf(lpSql,"insert into t_test_tip values(%d,%d,%d,'%s') ",nCtrlId,i,nIndex,vec[i].sTip.c_str());
		ExecDML(lpSql);
	}
	return true;
}

void CManageDb_Sqllite::Query_NextTip(vector<__Ctrl_TestTip>&vec,int nCtrlId)
{

}

BOOL CManageDb_Sqllite::Query_UnitData(vector<__UnitData>&vec,int nSysId)
{
	char lpSql[1024];
	sprintf(lpSql,"select * from t_unit_data where nSysId = %d ",nSysId);

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	//nId  ,nSysId int,sName text,nDataType int,nDataLen int,sDefault text,times float ,sMark text)
	while(!Query.eof())
	{   
		__UnitData info;
		n=0;
		info.nID = Query.getIntField(n++,0);
		n++;
		
		info.sName=boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		info.nDataType = Query.getIntField(n++,0);
		info.nDataLen =  Query.getIntField(n++,0);
		info.nDecimalNum =  Query.getIntField(n++,0);
		
		info.sDefault=boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		info.fTimes = Query.getFloatField(n++,1);
		info.sMark=boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");

		vec.push_back(info);
		Query.nextRow();

	}

	return true;
	 
}
BOOL CManageDb_Sqllite::Update_UnitData(vector<__UnitData>&vec,int nSysId)
{
	
	char lpbuf[1024];
	int nId;
	for(int i =0,end = vec.size();i<end;i++)
	{
		if(vec[i].nID==0)
			nId =GetMaxId("select max(nId) from t_unit_data")+1;
		else
		{
			nId = vec[i].nID;
			Delete_UnitData(nId);			
		}
		//nId  ,nSysId int,sName text,nDataType int,nDataLen int,sDefault text,times float ,sMark text)
		sprintf(lpbuf,"insert into t_unit_data values(%d,%d,'%s',%d,%d,%d,'%s',%f,'%s')",nId,nSysId,vec[i].sName.c_str(),
			vec[i].nDataType,vec[i].nDataLen,vec[i].nDecimalNum,
			vec[i].sDefault.c_str(),vec[i].fTimes,vec[i].sMark.c_str());
		ExecDML(lpbuf);

	}

	return true;
}
void CManageDb_Sqllite::Delete_UnitData(int nId)
{
	char lpbuf[1024];
	sprintf(lpbuf,"delete from t_unit_data where nid = %d ",nId);
	ExecDML(lpbuf);
}
BOOL CManageDb_Sqllite::Query_Goods_Number(vector<__Goods_Number>&vec,string strWhere )
{
	char lpSql[1024];
	sprintf(lpSql,"select * from t_goods_Number  %s ",strWhere.c_str());
 
	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");

	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n =0;
	while(!Query.eof())
	{   
		__Goods_Number info;
		n =0;
		info.nDlgId = Query.getIntField(n++,0);
		for(int i =0;i<5;i++)
		{
			info.nModBusValue[i]=Query.getIntField(n++,0);
		}
		info.nStartIndex  =Query.getIntField(n++,0);

		vec.push_back(info);
		Query.nextRow();
	}
	return true;	
}
BOOL CManageDb_Sqllite::Query_Goods_Number(vector<__Goods_Number>&vec,int nDlgId,int nIndex)
{
	char lpSql[1024]={0};
	 
	if(nDlgId !=-1 && nIndex!=-1 )
		sprintf(lpSql+strlen(lpSql),"where nDlgId = %d  and nStartIndex = %d ",nDlgId,nIndex);
	else if(nDlgId !=-1)
		sprintf(lpSql+strlen(lpSql),"where nDlgId = %d  ",nDlgId);
	 
	return Query_Goods_Number(vec,lpSql);
}
BOOL CManageDb_Sqllite::Update_Goods_Number(int nDlgId,int nValues[5],int nStartIndex)
{

	char lpSql[1024];
	sprintf(lpSql,"delete from  t_goods_Number where nDlgId = %d and nStartIndex = %d ",nDlgId,nStartIndex);
	ExecDML(lpSql);

	sprintf(lpSql,"insert into t_goods_Number values(%d,%d,%d,%d,%d,%d,%d)",nDlgId,nValues[0],nValues[1],nValues[2],nValues[3],nValues[4],nStartIndex);
	return ExecDML(lpSql);
}

bool CManageDb_Sqllite::Query_Ctrl_FontColor(map<int,COLORREF>&_map,int nCtrlId,int nType)
{
	map<int,COLORREF>().swap(_map);
	char lpSql[1024];
	sprintf(lpSql,"Select color from t_ctrl_Font_Color where nCtrlId =%d  and nType =%d order by nId",nCtrlId,nType);

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	 
	int nIndex=0;
	while(!Query.eof())
	{   
		_map[nIndex++]= Query.getIntField(0,0);		 

		Query.nextRow();
	}	

	return TRUE;
}
bool CManageDb_Sqllite::Query_Ctrl_FontColor(vector<COLORREF>&vec,int nCtrlId,int nType)
{
	vector<COLORREF>().swap(vec);

	char lpSql[1024];
	sprintf(lpSql,"Select color from t_ctrl_Font_Color where nCtrlId =%d  and nType =%d order by nId",nCtrlId,nType);

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;

	int nIndex=0;
	while(!Query.eof())
	{   
		vec.push_back(Query.getIntField(0,0));		 

		Query.nextRow();
	}	
	return true;
}
BOOL CManageDb_Sqllite::Update_Ctrl_FontColor(vector<COLORREF>&vec,int nCtrlId,int nType)
{
	char lpSql[1024];
	sprintf(lpSql,"delete from t_ctrl_Font_Color where nCtrlId = %d and nType =%d ",nCtrlId,nType);
	ExecDML(lpSql);
	for(int i =0,end = vec.size();i<end;i++)
	{
		sprintf(lpSql," Insert into t_ctrl_Font_Color values(%d,%d,%d,%d)",i+1,nCtrlId,vec[i],nType);
		ExecDML(lpSql);
	}
	return true;
}

BOOL CManageDb_Sqllite::Update_Image_Button_V02(__ButtonAttr_V02 &attr,int &nCtrlId,int nParentId)
{ 
	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nCtrlId,nIndex,nParentId);	

	if(attr.baseAttr.nCtrlIndex==0)
		attr.baseAttr.nCtrlIndex =nIndex;

	CMarkup xml;
	xml.AddElem("Attr");	

	xml.AddAttrib("bBackColor",attr.bBackColor);
	xml.AddAttrib("ColorBack",attr.colorBack);
	xml.AddAttrib("ColorLine",attr.colorLine);

	xml.AddAttrib("ColorFont",attr.colorFont);
	xml.AddAttrib("FontSize",attr.nFontSize);
	xml.AddAttrib("FontName",attr.sFontName.c_str());

	xml.AddAttrib("bBackBmp",attr.bBackBmp);
	xml.AddAttrib("BackBmpX",attr.nBmpX);
	xml.AddAttrib("BackBmpY",attr.nBmpY);
	xml.AddAttrib("EventType",attr.nEventType);

	CString strXml = xml.GetSubDoc();
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','','')",nCtrlId,attr.baseAttr.nCtrlIndex,nParentId,WND_type_image_button_V02,
		attr.baseAttr.nX,attr.baseAttr.nY,attr.baseAttr.nW,attr.baseAttr.nH,attr.sTitle.c_str(),strXml,
		0,attr.baseAttr.sName.c_str());

	return ExecDML(lpSql);

}

BOOL CManageDb_Sqllite::Update_Image_Led_V02(__ButtonAttr_V02 &attr,int& nCtrlId,int nParentId)
{
	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nCtrlId,nIndex,nParentId);	

	if(attr.baseAttr.nCtrlIndex==0)
		attr.baseAttr.nCtrlIndex =nIndex;

	CMarkup xml;
	xml.AddElem("Attr");	

	xml.AddAttrib("bBackColor",attr.bBackColor);
	xml.AddAttrib("ColorBack",attr.colorBack);
	xml.AddAttrib("ColorLine",attr.colorLine);

	xml.AddAttrib("ColorFont",attr.colorFont);
	xml.AddAttrib("FontSize",attr.nFontSize);
	xml.AddAttrib("FontName",attr.sFontName.c_str());

	xml.AddAttrib("bBackBmp",attr.bBackBmp);
	xml.AddAttrib("BackBmpX",attr.nBmpX);
	xml.AddAttrib("BackBmpY",attr.nBmpY);


	CString strXml = xml.GetSubDoc();
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','','')",nCtrlId,attr.baseAttr.nCtrlIndex,nParentId,WND_type_image_Led_V02,
		attr.baseAttr.nX,attr.baseAttr.nY,attr.baseAttr.nW,attr.baseAttr.nH,attr.sTitle.c_str(),strXml,
		0,attr.baseAttr.sName.c_str());

	return ExecDML(lpSql);
}

void CManageDb_Sqllite:: Delete_CtrlEvent_Text(int nTextId,int nCtrlId)
{
	char lpSql[1024];
	sprintf(lpSql,"delete from   t_Ctrl_Event_Text where nTextId  =%d and nCtrlId = %d ",nTextId,nCtrlId);
	ExecDML(lpSql);
}
bool CManageDb_Sqllite::Query_CtrlEvent_Text(vector<__Ctrl_Event_Text>&vec,int nTextId,int nCtrlId,int nEventType )
{
	vector<__Ctrl_Event_Text>().swap(vec);

	char lpSql[1024];
	sprintf(lpSql,"Select * from t_Ctrl_Event_Text where nTextId = %d and nCtrlId =%d ",nTextId,nCtrlId);

	if(nEventType !=-1)
		sprintf(lpSql+strlen(lpSql),"  and nEventType = %d",nEventType);

	sprintf(lpSql+strlen(lpSql)," order by nId ");
	
	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	while(!Query.eof())
	{   
		__Ctrl_Event_Text info;
		n=0;
		info.nId = Query.getIntField(n++,0);
		info.nCtrlId = Query.getIntField(n++,0);
		info.nTextId = Query.getIntField(n++,0);		
		info.nEventType = Query.getIntField(n++,0);
		info.nSharedId = Query.getIntField(n++,0);
		info.nValue1 = Query.getIntField(n++,0);		
		info.nValue2 = Query.getIntField(n++,0);
		info.color = Query.getIntField(n++,0);
		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		
		info.sRemarks =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	

		vec.push_back(info);
		Query.nextRow();
	}	

	return true;
}
void CManageDb_Sqllite::Update_CtrlEvent_Text(vector<__Ctrl_Event_Text>&vec,int nTextId,int nCtrlId)
{
	char lpSql[1024];
	sprintf(lpSql,"delete from t_Ctrl_Event_Text where nCtrlId = %d ",nCtrlId);
	ExecDML(lpSql);
	for(int i =0,end =vec.size();i<end;i++)
	{
		sprintf(lpSql,"insert into t_Ctrl_Event_Text values(%d,%d,%d,3,%d,%d,%d,%d,'%s','')",i+1,vec[i].nTextId,nCtrlId,vec[i].nSharedId,vec[i].nValue1,vec[i].nValue2,vec[i].color,vec[i].sName.c_str());
		ExecDML(lpSql);
	}
}
void CManageDb_Sqllite::Delete_CtrlText(int nId,int nCtrlId)
{
	char lpSql[1024];
	sprintf(lpSql,"delete from t_ctrl_text where nId = %d  and nCtrlId =%d ",nId,nCtrlId);
	ExecDML(lpSql);
}
int CManageDb_Sqllite::Query_CtrlText_Id(int nCtrlId,int nType )
{
	char lpSql[1024];
	sprintf(lpSql,"select nId from t_Ctrl_Text where nCtrlId = %d and nType =%d",nCtrlId,nType);
	return GetMaxId(lpSql);
}
bool CManageDb_Sqllite::Query_CtrlText(vector<__TextAttr_V02>&vec,int nCtrlId,int nType)
{
	vector<__TextAttr_V02>().swap(vec);

	char lpSql[1024];
	sprintf(lpSql,"Select * from t_Ctrl_Text where nCtrlId =%d  ",nCtrlId);
	
	if(nType !=-1)
		sprintf(lpSql+strlen(lpSql),"  and nType = %d",nType);
	
	sprintf(lpSql+strlen(lpSql),"  order by nId ");

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	while(!Query.eof())
	{   
		__TextAttr_V02 info;
		n=0;
		info.nId = Query.getIntField(n++,0);
		info.nCtrlId = Query.getIntField(n++,0);
		info.nType = Query.getIntField(n++,0);
		info.sTitle =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.sFontName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		
		info.colorFont = Query.getIntField(n++,0);
		info.nFontSize = Query.getIntField(n++,0);
		
		info.nAlign = Query.getIntField(n++,0);
		info.nX = Query.getIntField(n++,0);
		info.nY = Query.getIntField(n++,0);

		vec.push_back(info);
		Query.nextRow();
	}	

	return true;
}

bool CManageDb_Sqllite::Update_CtrlText(__TextAttr_V02 &info,int nCtrlId,int nType)
{
	char lpSql[1024];
	if(info.nId!=0)
	{
		sprintf(lpSql,"delete from t_Ctrl_Text where nId =%d  and nCtrlId = %d ",info.nId,nCtrlId);
		ExecDML(lpSql);
	}

	if(info.nId ==0)
	{
		sprintf(lpSql,"select max(nId) from t_Ctrl_Text where nCtrlId = %d",nCtrlId);
		info.nId =GetMaxId(lpSql)+1;
	}

	sprintf(lpSql,"Insert into t_ctrl_Text values(%d,%d,%d,'%s','%s',%d,%d,%d,%d,%d)",info.nId,nCtrlId,nType,info.sTitle.c_str(),info.sFontName.c_str(),
		info.colorFont,info.nFontSize,info.nAlign,info.nX,info.nY);
	ExecDML(lpSql);		

	return true;
}
bool CManageDb_Sqllite::Update_CtrlText(vector<__TextAttr_V02>&vec,int nCtrlId,int nType)
{ 
	char lpSql[1024];
	sprintf(lpSql,"delete from t_Ctrl_Text where nCtrlId = %d   ",nCtrlId);
	ExecDML(lpSql);

	for(int i=0,end = vec.size();i<end;i++)
	{
	/*	if(vec[i].nId ==0)
		{
			sprintf(lpSql,"select max(nId) from t_Ctrl_Text where nCtrlId = %d",nCtrlId);
			info.nId =GetMaxId(lpSql)+1;
		}*/
		vec[i].nId=i+1;
		sprintf(lpSql,"Insert into t_ctrl_Text values(%d,%d,%d,'%s','%s',%d,%d,%d,%d,%d)",vec[i].nId,nCtrlId,vec[i].nType,vec[i].sTitle.c_str(),vec[i].sFontName.c_str(),
			vec[i].colorFont,vec[i].nFontSize,vec[i].nAlign,vec[i].nX,vec[i].nY);
		ExecDML(lpSql);		 
	}

	return  true;
}

bool CManageDb_Sqllite::Update_Comm_Protocol(__Comm_Protocol & info,int nDlgId,int nSysDriveId)
{
	char lpSql[1024];
	if(info.nId!=0)
	{
		Query_Comm_Protocol_Detail(info.sProtocol,info.nId);
		Delete_Comm_Protocel(info.nId);		
	}
	else 
		info.nId = GetMaxId("select max(nId) from t_comm_protocol")+1;

	sprintf(lpSql,"insert into t_comm_protocol values(%d,%d,%d,'%s', %d,'%s','%s')",info.nId,nDlgId,nSysDriveId,info.sName.c_str(),
		info.nRecOrSend,info.sProtocol.c_str(),info.sMark.c_str());
	return ExecDML(lpSql);
}

bool CManageDb_Sqllite::Update_Comm_Protocel_Detail(string &sInfo,int nId)
{
	char lpSql[11204];
 
	sprintf(lpSql,"update t_comm_protocol set sProtocol = '%s'  where nId = %d ", sInfo.c_str(),nId);
	 
	return ExecDML(lpSql);
}

bool CManageDb_Sqllite::Delete_Comm_Protocel(int nId)
{
	char lpSql[1024];
	sprintf(lpSql,"delete from t_comm_protocol where nId = %d ",nId);
	return ExecDML(lpSql);
}

bool CManageDb_Sqllite::Delete_Comm_Protocel(CString strWhere)
{
	if(strWhere == "")
		return false;

	char lpSql[1024];
	sprintf(lpSql,"delete from t_comm_protocol where %s;", strWhere);

	//return true;
	return ExecDML(lpSql);
}

bool CManageDb_Sqllite::Query_Comm_Protocol(vector<__Comm_Protocol>&vec,int nDlgId,int nSysDriveId)
{
 	
	char lpSql[1024];
	sprintf(lpSql,"Select * from t_comm_protocol where nDlgId =%d  and nSysDriveId = %d  ",nDlgId,nSysDriveId);

	sprintf(lpSql+strlen(lpSql),"  order by sName ");

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	while(!Query.eof())
	{   
		__Comm_Protocol info;
		n=0;

		info.nId = Query.getIntField(n++,0);
		info.nDlgId = Query.getIntField(n++,0);
		info.nSysDriveId = Query.getIntField(n++,0);

		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");		
		info.nRecOrSend = Query.getIntField(n++,0);

		info.sProtocol =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		
		info.sMark =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		vec.push_back(info);
		Query.nextRow();
	}	

	return true;
}

bool CManageDb_Sqllite::Query_1553B_All_Protocol(vector<__Comm_Protocol>&vec)
{
	char lpSql[1024];
	sprintf(lpSql,"Select * from t_1553B_protocol");
	sprintf(lpSql+strlen(lpSql),"  order by sName ");

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	while(!Query.eof())
	{   
		__Comm_Protocol info;
		n=0;

		info.nId = Query.getIntField(n++,0);
		info.nDlgId = Query.getIntField(n++,0);
		info.nSysDriveId = Query.getIntField(n++,0);
		n ++;
		info.sProtocolNo =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");		
		info.sBlockType = boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		info.nATAddr = Query.getIntField(n++,0);
		info.nATsubAddr = Query.getIntField(n++,0);
		info.nRecOrSend = Query.getIntField(n++,0);
		const char* pCharValue = Query.getStringField(n++,0);
		info.cBusType = pCharValue[0];

		info.sProtocol =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	

		info.sMark =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		vec.push_back(info);
		Query.nextRow();
	}	

	return true;
}

bool CManageDb_Sqllite::Query_CAN_All_Protocol(vector<__CAN_Protocol>&vec)
{
	char lpSql[1024];
	sprintf(lpSql,"Select * from t_CAN_protocol");
	sprintf(lpSql+strlen(lpSql),"  order by sName ");

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	while(!Query.eof())
	{   
		__CAN_Protocol info;
		n=0;

		info.nId = Query.getIntField(n++,0);
		info.nDlgId = Query.getIntField(n++,0);
		info.nSysDriveId = Query.getIntField(n++,0);
		n ++;
		info.sProtocolNo =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");		
		info.sBlockType = boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		info.nMailBoxId = Query.getIntField(n++,0);		// 邮箱地址
		info.nMaskLen = Query.getIntField(n++,0);		// 标识符长度
		info.sFrameType = boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		info.sFrameFormat = boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		info.nFrameLen = Query.getIntField(n++,0);
		info.sCanStandard = boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		info.sRate = boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");
		info.nRecOrSend = Query.getIntField(n++,0);
		info.sProtocol =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		info.sMark =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		vec.push_back(info);
		Query.nextRow();
	}	

	return true;
}

bool CManageDb_Sqllite::Query_Comm_Protocol_Detail(string &sInfo,int nId)
{
	 char lpSql[1024];
	 sprintf(lpSql,"select sProtocol from t_comm_protocol where nId = %d ",nId);
	 std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	 CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	 if(Query.eof())
		 return false;

	 sInfo =boost::locale::conv::between(Query.getStringField(0,""),  "GB2312","UTF-8");	
	
	
	return true;
}

bool CManageDb_Sqllite::Query_1553B_Protocol_Detail(string &sInfo,int nId)
{
	char lpSql[1024];
	sprintf(lpSql,"select sProtocol from t_1553B_protocol where nId = %d ",nId);
	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	if(Query.eof())
		return false;

	sInfo =boost::locale::conv::between(Query.getStringField(0,""),  "GB2312","UTF-8");	


	return true;
}

//配置信息
bool CManageDb_Sqllite::Query_Config(string skey,string &sInfo,int &nId)
{
	char lpSql[1024];
	sprintf(lpSql,"select * from t_sys_Config where sKey ='%s' ",skey.c_str());

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	if(Query.eof())
		return false;

	int n=0;
	nId = Query.getIntField(n++,0);
	n++;
	sInfo =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	

	return true;
}

bool CManageDb_Sqllite::Update_Config(string sKey,string  sInfo)
{
	char lpSql[1024];
	sprintf(lpSql,"delete from t_sys_Config where sKey ='%s' ",sKey.c_str());
	ExecDML(lpSql);

	
	int nId = GetMaxId("select max(nId) from t_sys_config")+1;

	sprintf(lpSql," insert into t_sys_config values(%d,'%s','%s')",nId,sKey.c_str(),sInfo.c_str());
	return ExecDML(lpSql);
}

BOOL CManageDb_Sqllite::Update_Image_Rotate_V02(__RotateAttr_V02 &attr,int& nCtrlId,int nParentId)
{
	char lpSql[2048];
	int nIndex =0;
	UpdateCtrlInfo(nCtrlId,nIndex,nParentId);	

	if(attr.baseAttr.nCtrlIndex==0)
		attr.baseAttr.nCtrlIndex =nIndex;

	CMarkup xml;
	xml.AddElem("Attr");	

	xml.AddAttrib("bBackColor",attr.bShowBackColor);
	xml.AddAttrib("ColorBack",attr.colorBack);
	xml.AddAttrib("ColorLine",attr.colorLine);
	
	xml.AddAttrib("bShowText",attr.bShowText);

	xml.AddAttrib("ColorFont",attr.colorFont);
	xml.AddAttrib("FontSize",attr.nFontSize);
	xml.AddAttrib("FontName",attr.sFontName.c_str());
	xml.AddAttrib("nFontX",attr.nFontX);
	xml.AddAttrib("nFontY",attr.nFontY);
	xml.AddAttrib("nFontW",attr.nFontW);
	xml.AddAttrib("nFontH",attr.nFontH);

	xml.AddAttrib("sPicName",attr.sPicName.c_str());
	xml.AddAttrib("nAngleStart",attr.nAngleStart);
	xml.AddAttrib("nAngleEnd",attr.nAngleEnd);
	xml.AddAttrib("nPicX",attr.nPicX);
	xml.AddAttrib("nPicY",attr.nPicY);

	xml.AddAttrib("fValueMin",attr.fValueMin);
	xml.AddAttrib("fValueMax",attr.fValueMax);


	CString strXml = xml.GetSubDoc();
	sprintf(lpSql,"insert into  t_ctrl values(%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s',%d,'%s','','')",nCtrlId,attr.baseAttr.nCtrlIndex,nParentId,WND_type_image_Rotate_V02,
		attr.baseAttr.nX,attr.baseAttr.nY,attr.baseAttr.nW,attr.baseAttr.nH,attr.baseAttr.sName.c_str(),strXml,
		attr.nShareId,attr.baseAttr.sName.c_str());

	return ExecDML(lpSql);
}

bool CManageDb_Sqllite::Query_TestSysInfo(vector<__TestSysInfo>&vec)
{
	char lpSql[1024];
	sprintf(lpSql,"select * from t_Test_Sys  order by nId");

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());
	  
	int n=0;
	while(!Query.eof())
	{   
		__TestSysInfo info;
		n=0;
		info.nId = Query.getIntField(n++,0);
		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");			
		info.sMark =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		vec.push_back(info);
		Query.nextRow();
	}	

	return true;
}
bool CManageDb_Sqllite::Update_TestSysInfo(string sName,string sMark,int &nId)
{
	char lpSql[1024];
	if(nId ==0)
		nId =GetMaxId("select max(nid) from t_Test_Sys") +1;
	else
	{
		sprintf(lpSql,"delete from t_test_sys where nId = %d ",nId);
		ExecDML(lpSql);
	}
	sprintf(lpSql,"insert into t_Test_Sys values(%d,'%s','%s')",nId,sName.c_str(),sMark.c_str());
	return ExecDML(lpSql);
}

bool CManageDb_Sqllite::Query_TestSysInfo_UI(vector<__TestSysInfo_UI>&vec,int nSysId ,CString strWhere )
{
	char lpSql[1024];
	sprintf(lpSql,"select * from t_test_sys_UI  where nSysId = %d %s ",nSysId,strWhere);

	std::string sql = boost::locale::conv::between(lpSql, "UTF-8", "GBK");
	CppSQLite3Query Query = m_SQLiteDB.execQuery(sql.c_str());

	int n=0;
	while(!Query.eof())
	{   
		__TestSysInfo_UI info;
		n=0;		
		info.nSysId = Query.getIntField(n++,0);
		info.nDlgId = Query.getIntField(n++,0);
		info.sName =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");			
		info.nType = Query.getIntField(n++,0);
		info.sMark =boost::locale::conv::between(Query.getStringField(n++,""),  "GB2312","UTF-8");	
		vec.push_back(info);
		Query.nextRow();
	}	
	return true;
}

bool CManageDb_Sqllite::Update_TestSysInfo_UI(int nSysId,string sName,string sMark,int nUIType,int &nUIDlgId)
{
	CString strSql;
	if(nUIDlgId==0)
		nUIDlgId = GetMaxId("select max(nId) from t_ctrl")+1;
	else
	{
		strSql.Format(" delete from t_test_sys_UI where nDlgId = %d ",nUIDlgId);
		ExecDML(strSql.GetBuffer());	 
	}

	strSql.Format(" insert into t_test_sys_UI values(%d,%d,'%s',%d,'%s') ",nSysId,nUIDlgId,sName.c_str(),nUIType,sMark.c_str());
	ExecDML(strSql.GetBuffer());

	strSql.Format(" insert into t_ctrl values(%d,0,%d,0,0,0,800,600,'','',0,'%s',	'','')",nUIDlgId,nUIType==0?-1:-2,sName.c_str());
	return ExecDML(strSql.GetBuffer());
}