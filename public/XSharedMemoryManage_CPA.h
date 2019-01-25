#pragma once
#include "..\\public\\xsharedmemorymanage.h"
/*
1.前64字节，是系统信息， __SysInfoMemory
 2.地址的索引信息
 3.具体的内存值
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

