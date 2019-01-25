#include "StdAfx.h"
#include "AppData_StdSys.h"


CAppData_StdSys::CAppData_StdSys(void)
{
	//m_nStartTest =true;
	m_nStartTest = false;
	m_bStartWrite = false;
}


CAppData_StdSys::~CAppData_StdSys(void)
{
}

CAppData_StdSys & CAppData_StdSys::GetInstance()
{
	static  CAppData_StdSys inst;
	return inst;	
}