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
	string sRecSend;	// ���գ�����
	string sVer;		//�汾

	int nRtAddr;
	int nSubRtAddr;
	string sUnitType;	//���ݵ�λ��BYTE��BIT
	int nDataLen;		//���ݳ���
	int nRefreshTimes;	//ˢ�¼�� 
	string sInterrupt;	//�ж�
	string sSource;		//Դ
	string sDest;		//Ŀ��

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

