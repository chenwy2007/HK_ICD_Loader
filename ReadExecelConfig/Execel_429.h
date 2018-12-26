#pragma once
struct __protocol_Item
{
	string sName;
	int nAddr;
	int nLen;
	string sItemType;//协议项类型
	string sDataType;//数据类型
	string sDefaultValue;
	string sMark;
	float fRatio;  //系数
	float fSupply;//补充值
};

struct  __protocol_429
{
	string  sName;
	string sVer;
	string sUnitType;	//数据单位：BYTE、BIT
	int nChannel;		//通道号，0~15
	string sRecSend;	// 接收，发送
	int nSendSpeed;		//发送速度 
	int nTimes;			//发送间隔 
	string sSendMode;	//发送机制
	string sParity;		//校验位：0：无校验，1：奇校验  2：偶校位
	vector<__protocol_Item>DataItems;

	vector<string>sSimulationDataVec; //仿真数据			
};


struct __Data_Config
{
	string sName;
	int nDataType;
	int nDataSize;
};




//#define DATA_TYPE_INT	0
//#define DATA_TYPE_INT_LEN	4
//#define DATA_TYPE_INT_NAME	"整型"
//
//#define DATA_TYPE_FLOAT 1
//#define DATA_TYPE_FLOAT_LEN 4
//#define DATA_TYPE_FLOAT_NAME	"浮点型"
//
//#define DATA_TYPE_STRING 2
//#define DATA_TYPE_STRING_NAME	"字符串"
//
//#define DATA_TYPE_DOUBLE 3
//#define DATA_TYPE_DOUBLE_LEN 8
//#define DATA_TYPE_DOUBLE_NAME	"双精度型"
//
//#define DATA_TYPE_CHAR 4
//#define DATA_TYPE_CHAR_LEN 1
//#define DATA_TYPE_CHAR_NAME	"字符"






class Execel_429 :public Execel_Base
{
public:
	Execel_429(void);
	~Execel_429(void);
 
public:
	void ReadInfo(int nSheetIndex, __protocol_429 &Data);

	void ReadInfo(int nSheetIndex, vector<vector<string>>&vec);

public:
	_Application objApp; 
	Workbooks objBooks;
	_Workbook objBook;
	Worksheets objSheets;
};

