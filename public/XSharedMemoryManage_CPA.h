#pragma once
#include "..\\public\\xsharedmemorymanage.h"
/*
1.ǰ64�ֽڣ���ϵͳ��Ϣ�� __SysInfoMemory
 2.��ַ��������Ϣ
 3.������ڴ�ֵ
*/

class XSharedMemoryManage_CPA :
	public XSharedMemoryManage
{
public:
	XSharedMemoryManage_CPA(void);
	~XSharedMemoryManage_CPA(void);
	static XSharedMemoryManage_CPA &GetInstance();

	void InitStart();
	void InitEnd();
	 //nIndex:0~4
	bool SetDutId(int nDutId[5]);
	int  SetDutId(int nIndex,int nDutId,bool bWriteMemory =false);

	bool SetDut_DlgId(int nDlgId[5]);
	void WriteSysInfo();
	
	
	bool ReadSysInfo();
		
	int m_nSysId;
public:
	
	
	
};

