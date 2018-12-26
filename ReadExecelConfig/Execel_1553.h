#pragma once
#include "Execel_base.h"

struct ____protocol_1553_Item
{
	int nBeginAddr;
	int nDataLen;
	string sName;
	string sAliasName;	 
};
struct __protocol_1553
{
	__protocol_1553()
	{
		nRtAddr =0;
		nSubRtAddr=0;
		nDataLen=0;
		nRefreshTimes=0;

	};
	string sName;
	string sAliasName;
	string sRecSend;	// 接收，发送
	string sVer;		//版本

	int nRtAddr;
	int nSubRtAddr;
	string sUnitType;	//数据单位：BYTE、BIT
	int nDataLen;		//数据长度
	int nRefreshTimes;	//刷新间隔 
	string sInterrupt;	//中断
	string sSource;		//源
	string sDest;		//目标

	vector<____protocol_1553_Item>DataItemsVec;

};

class Execel_1553 :public Execel_Base
{
public:
	Execel_1553(void);
	~Execel_1553(void);
	   
	//bool ReadInfo(int nSheetIndex, __protocol_1553 &Data);
	void ReadInfo_Item(int nSheetIndex, vector<vector<string>>&BaseInfovec,vector<vector<string>>&ItemInfovec );
	void ReadInfo_Item_SubInfo(int nSheetIndex,vector<string>&vec);
};

