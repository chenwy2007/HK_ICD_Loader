#pragma once
/*
��Ҫ����ϵͳ����ʱ���洢������Ϣ
*/
class CAppData_StdSys
{
public:
	CAppData_StdSys(void);
	~CAppData_StdSys(void);	
	static CAppData_StdSys &GetInstance();
public:
	//��ǰ���Բ�Ʒ��ID
	int m_nCurTestSysId; 

	//��ʼ����
	bool m_nStartTest;
	bool m_bStartWrite;

};
#define  g_App_StdSys CAppData_StdSys::GetInstance()

#define  CHECK_START_READ  if(g_App_StdSys.m_nStartTest==0) return ;

