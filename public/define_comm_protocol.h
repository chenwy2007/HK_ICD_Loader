#pragma  once

#define ZERO_VALUE   0.001
 
 
//Э��������
//  
 
//-- Э�������� ---
#define  PROTOCOL_Item_Header		"HEADER"
#define  PROTOCOL_Item_Label		"LABEL"
#define  PROTOCOL_Item_Data			"DATA"
#define  PROTOCOL_Item_Check_mode	"CHECK_MODE"
#define  PROTOCOL_Item_Data_len		"DATALEN"
#define  PROTOCOL_Item_Cmd			"CMD"
#define  PROTOCOL_Item_Item			"ITEM"
  
//----У������---
#define  PROTOCOL_CHECK_MODE_No		"��У��"
#define  PROTOCOL_CHECK_MODE_Odd	"��У��"
#define  PROTOCOL_CHECK_MODE_Even 	"żУλ"
#define  PROTOCOL_CHECK_MODE_XOR 	"���"
#define  PROTOCOL_CHECK_MODE_CRC 	"CRC"

//���ݳ��ȵ�λ
#define PROTOCOL_Data_Len_Unit_Bit  "BIT"
#define PROTOCOL_Data_Len_Unit_Byte "BYTE"

//--���ͻ���  ---
#define  PROTOCOL_SendMode_Value_1	"��ʱ"
#define  PROTOCOL_SendMode_Value_2	"ָ��" 
#define  PROTOCOL_SendMode_Value_3	"ָ�ʱ"
#define  PROTOCOL_SendMode_Value_4	"Ӧ��"


//--���ͻ���  ---
#define  PROTOCOL_RecSend_Rec	"����"
#define  PROTOCOL_RecSend_Send	"����" 


//----У�� e---

//xml�нڵ���������
#define  XML_Attr				"Attr"
#define  XML_Name				"Name"
#define  XML_Describe			"Describe"
#define  XML_RecSend			"RecSend" 
#define  XML_SendMode			"SendMode"
#define  XML_BlockId			"BlockId"
#define  XML_CheckMode			"CheckMode"
#define  XML_RefreshTime		"RefreshTime"
#define  XML_MaxDelay			"MaxDelayTime"
#define  XML_SendStyle			"SendStyle"		//1553B�еĹ㲥���ǵ�Ե㣨�°�δ�ø����ԣ�
#define  XML_DispName			"DispName"		//�ź���ʾ������
#define  XML_DispValue			"DispValue"		//�ź���ʾ��ֵ
#define  XML_DispStyle			"DispStyle"		//�ź���ʾ�����з�ʽ
#define  XML_Shared				"Shared"
#define  XML_DataUnit			"DataUnit"
#define  XML_DataLen			"DataLen"
#define  XML_Reply_Condition	"Reply_Condition"
#define  XML_Reply_Condition_Shared	"Reply_Condition_Shared"
#define  XML_Reply_Value		"Reply_Value"   ////Ӧ������ = Ӧ��ֵ ʱ���ظ�XML_Reply_protocol
#define  XML_Reply_protocol		"Reply_protocol"
#define  XML_SubItems			"SubItems"
#define  XML_Item_Item			"Item"
#define	 XML_Frame_Type			"FrameType"		//CAN֡����
#define  XML_Frame_Format		"FrameFormat"	//CAN֡��ʽ
#define  XML_MailBoxID			"MailBoxId"		//CAN�����ʶ
#define  XML_CANID				"CANID"			//CANID����CAN�����ʶ
#define  XML_MaskLen			"MaskLen"		//��ʶ������
#define  XML_BaudRate			"BaudRate"		//������
#define  XML_BUSTYPE			"BusType"		//��������A��B
#define  XML_BLOGICID			"BLogicId"		//�߼����
// Э��ؼ�����
#define  XML_CTRL_BTYPE			"CTRLBTYPE"		//������
#define  XML_CTRL_DType			"CTRLDTYPE"		//�ؼ���ʾ����
#define  XML_CTRL_ROWNUM		"CTRLROWNUM"	//�ؼ�����
#define  XML_CTRL_COLNUM		"CTRLCOLNUM"	//�ؼ�����
#define  XML_PRO_NAME			"PRONAME"		//Э������

//��ʼλ�����ȡ�Э�������͡��������͡�ϵ��������ֵ��Ĭ��ֵ ����ע
#define XML_Item_Addr			"Addr" 
#define XML_Item_Length			"Length" 
#define XML_Item_Resolution		"Resolution" 
#define XML_Item_Unit			"Unit" 
#define XML_Item_Range			"Range" 
#define XML_Item_DispName		"ItemDispName"
#define XML_Item_DispValue		"ItemDispValue"
#define XML_Item_DispStyle		"ItemDispStyle"
#define XML_Item_Sign			"ItemSign"
#define XML_Item_Item_type		"Item_type" 
#define XML_Item_Data_type		"Data_type" 
#define XML_Item_Coefficient	"Coefficient" 
#define XML_Item_Offset			"Offset" 
#define XML_Item_NO				"No"
#define XML_Item_Code			"Code"
#define XML_Item_Group			"GroupNo"
#define XML_Item_Default		"Default" 
#define XML_Item_Mark			"Mark" 
#define XML_Item_Express		"Formula"
#define XML_Item_Transfer		"Transfer"

#define XML_Item_RowNo			"ItemRowNo"
#define XML_Item_ColNo			"ItemColNo"
#define XML_Item_SignalNum		"SignalNum"
 
//5276
#define  XML_ChannelNo			"ChannelNo"

//5273-1553
//RT��ַ���ӵ�ַ���жϡ�Դ��Ŀ��
#define XML_RtAddr				"RtAddr"
#define XML_RtSubAddr			"RtSubAddr"
#define XML_RtAddr1				"RtAddr1"			// ���õ�ַ����RT->RT
#define XML_RtSubAddr1			"RtSubAddr1"		// ���õ�ַ����RT->RT
#define XML_Interrupt			"Interrupt"
#define XML_SourceEquipment		"SrcEquipment"
#define XML_DestEquipment		"DestEquipment"

//xml�ڵ��������ƣ����İ棩
#define  XML_ProType_CN			"Э������"
#define  XML_Name_CN			"Э������"
#define  XML_Describe_CN		"����"
#define  XML_Ver_CN				"�汾"

#define  XML_DataUnit_CN		"���ݵ�λ"
#define  XML_SendMode_CN		"��������"	// �����ڻ��¼���
#define  XML_RecSend_CN			"����"		// ���ջ��� 
#define  XML_CheckMode_CN		"У��λ"	// ��żУ�����У
												// Lable�Ѷ���
#define  XML_DataLen_CN			"���ݳ���"
#define  XML_RefreshTime_CN		"��������"
#define  XML_ChannelNo_CN		"ͨ����"
#define  XML_BlockId_CN			"���ݿ�ID"

#define  XML_BlockName_CN		"���ݿ�����"


//ͨ��Э�����ݽṹ
struct __Comm_Protocol
{
	int nId;
	int nDlgId;				//��Ӧ��������
	int nSysDriveId;		//��Ӧ�豸ID
	string sName;			
	string sBlockType;		//���ݿ����ͣ�ITEM��DATA
	int nATAddr;			//RT��ַ
	int nATsubAddr;			//RT�ӵ�ַ
	int nRecOrSend;			//Э�����ͣ�1:���ܣ�2�����ͣ�
	char cBusType;			//A��B����
	string sProtocolNo;		//Э�����
	string sProtocol ;		//Э�����ϸ��Ϣ	
	string sMark;
}; 

//ͨ��Э�����ݽṹ
struct __CAN_Protocol
{
	int nId;
	int nDlgId;				//��Ӧ��������
	int nSysDriveId;		//��Ӧ�豸ID
	string sName;			
	string sBlockType;		//���ݿ����ͣ�ITEM��DATA
	int nMailBoxId;			//�����ַ
	int nMaskLen;			//��ʶ������
	string sFrameType;		//֡����
	string sFrameFormat;	//֡��ʽ
	int nFrameLen;			//֡����
	string sCanStandard;	//CAN�淶
	string sRate;			//����
	int nRecOrSend;			//Э�����ͣ�1:���ܣ�2�����ͣ�
	string sProtocolNo;		//Э�����
	string sProtocol ;		//Э�����ϸ��Ϣ	
	string sMark;
}; 

struct __Comm_Protocol_Item
{
	__Comm_Protocol_Item()
	{
		fCoefficient =0;
		fOffset=0;
	};
	//��ʼλ������
	string sName;
	
	//��ʼλ������
	int nAddr;			//��ַ
	int nDataLen;		//���ݳ���

	//Э�������͡���������
	string sItemType;
	string sDataType;
	
	//�����ڴ�,Ĭ��ֵ
	int nShareId;		
	string sDefaultValue;

	//ϵ��,����ֵ,��ע
	float fCoefficient;  
	float fOffset;	
	string sMark;
	
	//nItemType:λITEM,��Ч
	//�������ݵ�λ��BYTE,BIT���������ݳ���
	string sSubDataUnit;	
	int nSubDataLen;		

	vector<__Comm_Protocol_Item>items;
};

struct __Protocol_429
{
	int nId;		
	int nChannel;	 //ͨ����	//channel
	u8  nLabel;

	string sName;		//Name
	string sVer;		//Ver
	int   nSendMode;	//SendMode,���ͻ���,0:��ʱ��1��ָ�2��ָ�ʱ,	-1����ʾ����
	int	  SendSpeed;    //SendSpeed ��������(Kbps)
	int   nTimes;		//times ����ʱ����
	int nParity;		//У��λ  0:��У�飬1:żУ�飬2����У�� N=None��У�� E=EvenżУ�� O=Odd��У��
	int nShared;		//�����ڴ�ֵ
	vector<__Comm_Protocol_Item>items;
	 	
};
//
//struct __Protocol_1553_Sub_Item
//{
//	string sName;
//	string sDescribe;	//����
//	int nAddr;			//��ʼ��ַ
//	int nDataLen;		//���ݳ���
//
//	string sSubDataUnit;	//�������ݵ�λ��BYTE,BIT
//	int nSubDataLen;		//�������ݳ���
//	string sSubInfoType;	//�������ͣ���ֵ���ַ�
//	vector<__Comm_Protocol_Item>items;	
//};
//
//struct __Protocol_1553
//{
//	int nId;			
//	string sVer;		//Ver
//	string sName;		//Name
//	string sDescribe;	//����
//	
//	int nRtAddr;		//rt��ַ
//	int nSubRtAddr;     //�ӵ�ַ
//	
//	string sDataUnit;	//���ݵ�λ��BYTE,BIT
//	int nDataLen;		//���ݳ���
//
//	string sRecSend;	//���գ�����
//	int nSendMode;		//SendMode,������Ч�����ͻ���,PROTOCOL_SendMode_0_Value,PROTOCOL_SendMode_1_Value,PROTOCOL_SendMode_2_Value,PROTOCOL_SendMode_3_Value
//	int nRefreshTime;	// ����ˢ��/����ʱ����
//
//	string sInterrupt;
//	string sSource;
//	string sDest;
//
//	vector<__Protocol_1553_Sub_Item>items;
//};

 //Э�飬�����ṹ
 struct __Protocol_Base
 {
	 __Protocol_Base()
	{
		 nReplay_Condition_ShareId =0;
		 nReply_protocol_ShareId =0;
	} ;
	int nId;
	 
	 string sName;			//Name
	 string sDescribe;		//����
	 
	 string sRecSend;		//���գ�����
	 string sSendMode;		//���ͻ���
	 
	 int	nRefreshTime;   // ����ˢ��/����ʱ����
	 string sCheeckMode;	//У��λ
	
	 string sSubDataUnit;	//�������ݵ�λ��BYTE,BIT
	 int nSubDataLen;		//�������ݳ���

	 int nSharedId;			//����ֵ�������ڴ�
	 //sRecSend�����գ���Ч
	 string sReply_Condition;			//Ӧ������
	 
	 int nReplay_Condition_ShareId;		//�����ڴ�ֵ
	 string sReplay_Value;				//Ӧ��ֵ
	 
	 string sReply_protocol_Name;		//Ӧ��Э������
	 int nReply_protocol_ShareId;		//Ӧ��Э��Ĺ����ڴ�	 

	 vector<__Comm_Protocol_Item>items;
 };


 struct  __Protocol_5276
 {
	 __Protocol_Base  base;
	 int nChannelNo;
	 string sHeaderFlag;//
 };
  
 struct  __Protocol_1553
 {
	 __Protocol_1553()
	 {
		nRtAddr = 0;
		nRtSubAddr = 0;
		nMessno = 0;
		sInterrupt = "";
		sSrcEquipment = "";
		sDestEquipment = "";
		sRecData = "";
	 }
	 __Protocol_Base  base;
	 int nRtAddr;
	 int nRtSubAddr;
	 int nMessno;
	 string sInterrupt;
	 string sSrcEquipment;
	 string sDestEquipment;
	 string sRecData;		 
 }; 

 struct __Protocol_Can
 {
	 __Protocol_Can()
	 {
		 sSubItemName = "";
		 nStartAddr = 0;
		 nLength = 0;
		 nShareId = 0;
		 nCtrlType = 0;
	 }

	 string sSubItemName;
	 int nStartAddr;
	 int nLength;
	 int nShareId;
	 int nCtrlType;
 };

 struct __Protocol_Can_Item
 {
	 vector<__Protocol_Can> vtSubItem;
 };

 struct __Protocol_Can_Package
 {
	 __Protocol_Can_Package()
	 {
		 nMailBoxId = 0;
		 nMaskLength = 0;
		 sFrameType = "";
		 sFrameFormat = "";
		 nFrameLen = 0;
		 sCANStandard = "";
		 sRate = "";
		 nPShareId = 0;
	 }
	 int nMailBoxId;
	 int nMaskLength;
	 string sFrameType;
	 string sFrameFormat;
	 int nFrameLen;
	 string sCANStandard;
	 string sRate;
	 int nPShareId;
	 vector<__Protocol_Can_Item> vtPackageInfor;
 };
