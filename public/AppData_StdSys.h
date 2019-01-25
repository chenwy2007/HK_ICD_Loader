#pragma once
/*
主要用于系统运行时，存储数据信息
*/
class CAppData_StdSys
{
public:
	CAppData_StdSys(void);
	~CAppData_StdSys(void);	
	static CAppData_StdSys &GetInstance();
public:
	//当前测试产品的ID
	int m_nCurTestSysId; 

	//开始测试
	bool m_nStartTest;
	bool m_bStartWrite;

};
#define  g_App_StdSys CAppData_StdSys::GetInstance()

#define  CHECK_START_READ  if(g_App_StdSys.m_nStartTest==0) return ;

