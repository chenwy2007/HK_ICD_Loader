#include "stdafx.h"
#include "ManageDb.h"
#include "Markup.h"
#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/locale.hpp"
#include "boost/format.hpp"

#define  CHECK_NO_RETURN  if(!m_bConnected) return ;
#define  CHECK_RETURN  if(!m_bConnected) return  false;

CManageDb_Sqllite_base::CManageDb_Sqllite_base()
{
	ConnectDb();
}
CManageDb_Sqllite_base::~CManageDb_Sqllite_base()
{

}
int CManageDb_Sqllite_base::GetMaxId(const char *lpSql)
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

BOOL CManageDb_Sqllite_base::ConnectDb()
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
CManageDb_Sqllite_base &CManageDb_Sqllite_base::GetInstance()
{
	static CManageDb_Sqllite_base inst;
	return inst;
}