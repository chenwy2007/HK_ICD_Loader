#pragma once

#define g_DB  (CManageDb_Sqllite::GetInstance())

#include "AppData.h"
 class CManageDb_Sqllite_base
 {
 public:
	 CManageDb_Sqllite_base();
	 ~CManageDb_Sqllite_base();
	 CManageDb_Sqllite_base &GetInstance();
	 BOOL ConnectDb();
	 int GetMaxId(const char *lpSql);
	 BOOL Query_DataConfig(vector<__DATA_CONFIG>&vec,int nDlgId,string strWhere ="" );
 public:
	  CppSQLite3DB	m_SQLiteDB; 
	  bool m_bConnected;
 };
  