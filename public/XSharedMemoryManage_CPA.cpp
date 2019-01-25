#include "StdAfx.h"
#include "XSharedMemoryManage_CPA.h"

 XSharedMemoryManage * g_pShared;
XSharedMemoryManage_CPA::XSharedMemoryManage_CPA(void)
{
	m_nSysId=1;
	memset(&m_sysInfo,0,sizeof(__SysInfoMemory));
}


XSharedMemoryManage_CPA::~XSharedMemoryManage_CPA(void)
{
}
XSharedMemoryManage_CPA &XSharedMemoryManage_CPA::GetInstance()
{
	static XSharedMemoryManage_CPA inst;
	return inst;
}
bool XSharedMemoryManage_CPA::SetDutId(int nDutId[5])
{
	  
	
	return true;
}
bool XSharedMemoryManage_CPA::SetDut_DlgId(int nDlgId[5])
{
	memcpy(m_sysInfo.nDutId,nDlgId,sizeof(int)*5);
	WriteDataToMemory(0,(void *)&m_sysInfo,sizeof(__SysInfoMemory));
	return true;
}
int XSharedMemoryManage_CPA::SetDutId(int nIndex,int nDutId,bool bWriteMemory )
{
	if(nIndex>4 || nIndex<0)
		return  0;
	//从数据库中读取，nDutId，第对应的DLGID
	if(nDutId>0)
		m_sysInfo.nDutId[nIndex] = CManageDb_Sqllite::GetInstance().Query_DlgId(nDutId, m_nSysId);
	else
		m_sysInfo.nDutId[nIndex]  =0;

	 
	if(!bWriteMemory)
		return m_sysInfo.nDutId[nIndex];
 
	WriteDataToMemory(0,(void *)&m_sysInfo,sizeof(__SysInfoMemory));
	return m_sysInfo.nDutId[nIndex];
}
void XSharedMemoryManage_CPA::WriteSysInfo()
{
	WriteDataToMemory(0,(void *)&m_sysInfo,sizeof(__SysInfoMemory));
}
 

bool XSharedMemoryManage_CPA::ReadSysInfo()
{	 
	ReadDataFromMemory(0,(void *)&m_sysInfo,sizeof(__SysInfoMemory));
	
	int nAddr =sizeof(__SysInfoMemory);
	int nSize = sizeof(__SharedM);
	map<int,__SharedM>SharedMap;
	if(m_sysInfo.nReCreate ==0 )
	{
		for(int i =0;i<m_sysInfo.nShareDCount;i++)
		{
			__SharedM share;
			ReadDataFromMemory(nAddr,(void *)&share,nSize);
			SharedMap[share.nId] = share;	
			nAddr+=nSize;
			
		}
		m_mapShare.swap(SharedMap);
	}

	//m_sysInfo.nReCreate =1;
	WriteDataToMemory(0,(void *)&m_sysInfo,sizeof(__SysInfoMemory));
	return true;
	
}
void XSharedMemoryManage_CPA::InitStart()
{
	m_sysInfo.nOk =0;
	WriteDataToMemory(0,(void *)&m_sysInfo,sizeof(__SysInfoMemory));

}
void XSharedMemoryManage_CPA::InitEnd()
{
	m_sysInfo.nOk =1;
	m_sysInfo.nReCreate=0;	
	WriteDataToMemory(0,(void *)&m_sysInfo,sizeof(__SysInfoMemory));
}