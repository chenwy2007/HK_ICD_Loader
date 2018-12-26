#pragma once
struct __protocol_Item
{
	string sName;
	int nAddr;
	int nLen;
	string sItemType;//Э��������
	string sDataType;//��������
	string sDefaultValue;
	string sMark;
	float fRatio;  //ϵ��
	float fSupply;//����ֵ
};

struct  __protocol_429
{
	string  sName;
	string sVer;
	string sUnitType;	//���ݵ�λ��BYTE��BIT
	int nChannel;		//ͨ���ţ�0~15
	string sRecSend;	// ���գ�����
	int nSendSpeed;		//�����ٶ� 
	int nTimes;			//���ͼ�� 
	string sSendMode;	//���ͻ���
	string sParity;		//У��λ��0����У�飬1����У��  2��żУλ
	vector<__protocol_Item>DataItems;

	vector<string>sSimulationDataVec; //��������			
};


struct __Data_Config
{
	string sName;
	int nDataType;
	int nDataSize;
};




//#define DATA_TYPE_INT	0
//#define DATA_TYPE_INT_LEN	4
//#define DATA_TYPE_INT_NAME	"����"
//
//#define DATA_TYPE_FLOAT 1
//#define DATA_TYPE_FLOAT_LEN 4
//#define DATA_TYPE_FLOAT_NAME	"������"
//
//#define DATA_TYPE_STRING 2
//#define DATA_TYPE_STRING_NAME	"�ַ���"
//
//#define DATA_TYPE_DOUBLE 3
//#define DATA_TYPE_DOUBLE_LEN 8
//#define DATA_TYPE_DOUBLE_NAME	"˫������"
//
//#define DATA_TYPE_CHAR 4
//#define DATA_TYPE_CHAR_LEN 1
//#define DATA_TYPE_CHAR_NAME	"�ַ�"






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

