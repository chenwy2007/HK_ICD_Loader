#pragma  once

#define ZERO_VALUE   0.001
 
 
//协议项名称
//  
 
//-- 协议项类型 ---
#define  PROTOCOL_Item_Header		"HEADER"
#define  PROTOCOL_Item_Label		"LABEL"
#define  PROTOCOL_Item_Data			"DATA"
#define  PROTOCOL_Item_Check_mode	"CHECK_MODE"
#define  PROTOCOL_Item_Data_len		"DATALEN"
#define  PROTOCOL_Item_Cmd			"CMD"
#define  PROTOCOL_Item_Item			"ITEM"
  
//----校验类型---
#define  PROTOCOL_CHECK_MODE_No		"无校验"
#define  PROTOCOL_CHECK_MODE_Odd	"奇校验"
#define  PROTOCOL_CHECK_MODE_Even 	"偶校位"
#define  PROTOCOL_CHECK_MODE_XOR 	"异或"
#define  PROTOCOL_CHECK_MODE_CRC 	"CRC"

//数据长度单位
#define PROTOCOL_Data_Len_Unit_Bit  "BIT"
#define PROTOCOL_Data_Len_Unit_Byte "BYTE"

//--发送机制  ---
#define  PROTOCOL_SendMode_Value_1	"定时"
#define  PROTOCOL_SendMode_Value_2	"指令" 
#define  PROTOCOL_SendMode_Value_3	"指令定时"
#define  PROTOCOL_SendMode_Value_4	"应答"


//--发送机制  ---
#define  PROTOCOL_RecSend_Rec	"接收"
#define  PROTOCOL_RecSend_Send	"发送" 


//----校验 e---

//xml中节点属性名称
#define  XML_Attr				"Attr"
#define  XML_Name				"Name"
#define  XML_Describe			"Describe"
#define  XML_RecSend			"RecSend" 
#define  XML_SendMode			"SendMode"
#define  XML_BlockId			"BlockId"
#define  XML_CheckMode			"CheckMode"
#define  XML_RefreshTime		"RefreshTime"
#define  XML_MaxDelay			"MaxDelayTime"
#define  XML_SendStyle			"SendStyle"		//1553B中的广播还是点对点（新版未用该属性）
#define  XML_DispName			"DispName"		//信号显示的名称
#define  XML_DispValue			"DispValue"		//信号显示的值
#define  XML_DispStyle			"DispStyle"		//信号显示的排列方式
#define  XML_Shared				"Shared"
#define  XML_DataUnit			"DataUnit"
#define  XML_DataLen			"DataLen"
#define  XML_Reply_Condition	"Reply_Condition"
#define  XML_Reply_Condition_Shared	"Reply_Condition_Shared"
#define  XML_Reply_Value		"Reply_Value"   ////应答条件 = 应答值 时，回复XML_Reply_protocol
#define  XML_Reply_protocol		"Reply_protocol"
#define  XML_SubItems			"SubItems"
#define  XML_Item_Item			"Item"
#define	 XML_Frame_Type			"FrameType"		//CAN帧类型
#define  XML_Frame_Format		"FrameFormat"	//CAN帧格式
#define  XML_MailBoxID			"MailBoxId"		//CAN邮箱标识
#define  XML_CANID				"CANID"			//CANID就是CAN邮箱标识
#define  XML_MaskLen			"MaskLen"		//标识符长度
#define  XML_BaudRate			"BaudRate"		//波特率
#define  XML_BUSTYPE			"BusType"		//总线类型A或B
#define  XML_BLOGICID			"BLogicId"		//逻辑块号
// 协议控件属性
#define  XML_CTRL_BTYPE			"CTRLBTYPE"		//块类型
#define  XML_CTRL_DType			"CTRLDTYPE"		//控件显示类型
#define  XML_CTRL_ROWNUM		"CTRLROWNUM"	//控件行数
#define  XML_CTRL_COLNUM		"CTRLCOLNUM"	//控件列数
#define  XML_PRO_NAME			"PRONAME"		//协议名称

//起始位、长度、协议项类型、数据类型、系数、补偿值、默认值 、备注
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
//RT地址、子地址、中断、源、目标
#define XML_RtAddr				"RtAddr"
#define XML_RtSubAddr			"RtSubAddr"
#define XML_RtAddr1				"RtAddr1"			// 备用地址用于RT->RT
#define XML_RtSubAddr1			"RtSubAddr1"		// 备用地址用于RT->RT
#define XML_Interrupt			"Interrupt"
#define XML_SourceEquipment		"SrcEquipment"
#define XML_DestEquipment		"DestEquipment"

//xml节点属性名称（中文版）
#define  XML_ProType_CN			"协议类型"
#define  XML_Name_CN			"协议名称"
#define  XML_Describe_CN		"描述"
#define  XML_Ver_CN				"版本"

#define  XML_DataUnit_CN		"数据单位"
#define  XML_SendMode_CN		"传输类型"	// 如周期或事件等
#define  XML_RecSend_CN			"方向"		// 接收或发送 
#define  XML_CheckMode_CN		"校验位"	// 奇偶校或异或校
												// Lable已定义
#define  XML_DataLen_CN			"数据长度"
#define  XML_RefreshTime_CN		"传输周期"
#define  XML_ChannelNo_CN		"通道号"
#define  XML_BlockId_CN			"数据块ID"

#define  XML_BlockName_CN		"数据块名称"


//通信协议数据结构
struct __Comm_Protocol
{
	int nId;
	int nDlgId;				//对应窗口名称
	int nSysDriveId;		//对应设备ID
	string sName;			
	string sBlockType;		//数据块类型：ITEM和DATA
	int nATAddr;			//RT地址
	int nATsubAddr;			//RT子地址
	int nRecOrSend;			//协议类型，1:接受，2：发送，
	char cBusType;			//A或B总线
	string sProtocolNo;		//协议代码
	string sProtocol ;		//协议的详细信息	
	string sMark;
}; 

//通信协议数据结构
struct __CAN_Protocol
{
	int nId;
	int nDlgId;				//对应窗口名称
	int nSysDriveId;		//对应设备ID
	string sName;			
	string sBlockType;		//数据块类型：ITEM和DATA
	int nMailBoxId;			//邮箱地址
	int nMaskLen;			//标识符长度
	string sFrameType;		//帧类型
	string sFrameFormat;	//帧格式
	int nFrameLen;			//帧长度
	string sCanStandard;	//CAN规范
	string sRate;			//速率
	int nRecOrSend;			//协议类型，1:接受，2：发送，
	string sProtocolNo;		//协议代码
	string sProtocol ;		//协议的详细信息	
	string sMark;
}; 

struct __Comm_Protocol_Item
{
	__Comm_Protocol_Item()
	{
		fCoefficient =0;
		fOffset=0;
	};
	//起始位、长度
	string sName;
	
	//起始位、长度
	int nAddr;			//地址
	int nDataLen;		//数据长度

	//协议项类型、数据类型
	string sItemType;
	string sDataType;
	
	//共享内存,默认值
	int nShareId;		
	string sDefaultValue;

	//系数,补偿值,备注
	float fCoefficient;  
	float fOffset;	
	string sMark;
	
	//nItemType:位ITEM,有效
	//子项数据单位，BYTE,BIT，子项数据长度
	string sSubDataUnit;	
	int nSubDataLen;		

	vector<__Comm_Protocol_Item>items;
};

struct __Protocol_429
{
	int nId;		
	int nChannel;	 //通道号	//channel
	u8  nLabel;

	string sName;		//Name
	string sVer;		//Ver
	int   nSendMode;	//SendMode,发送机制,0:定时、1，指令、2，指令定时,	-1，表示不用
	int	  SendSpeed;    //SendSpeed 发送速率(Kbps)
	int   nTimes;		//times 发送时间间隔
	int nParity;		//校验位  0:无校验，1:偶校验，2：奇校验 N=None无校验 E=Even偶校验 O=Odd奇校验
	int nShared;		//共享内存值
	vector<__Comm_Protocol_Item>items;
	 	
};
//
//struct __Protocol_1553_Sub_Item
//{
//	string sName;
//	string sDescribe;	//描述
//	int nAddr;			//开始地址
//	int nDataLen;		//数据长度
//
//	string sSubDataUnit;	//子项数据单位，BYTE,BIT
//	int nSubDataLen;		//子项数据长度
//	string sSubInfoType;	//内容类型，数值、字符
//	vector<__Comm_Protocol_Item>items;	
//};
//
//struct __Protocol_1553
//{
//	int nId;			
//	string sVer;		//Ver
//	string sName;		//Name
//	string sDescribe;	//描述
//	
//	int nRtAddr;		//rt地址
//	int nSubRtAddr;     //子地址
//	
//	string sDataUnit;	//数据单位，BYTE,BIT
//	int nDataLen;		//数据长度
//
//	string sRecSend;	//接收，发送
//	int nSendMode;		//SendMode,发送有效，发送机制,PROTOCOL_SendMode_0_Value,PROTOCOL_SendMode_1_Value,PROTOCOL_SendMode_2_Value,PROTOCOL_SendMode_3_Value
//	int nRefreshTime;	// 数据刷新/发送时间间隔
//
//	string sInterrupt;
//	string sSource;
//	string sDest;
//
//	vector<__Protocol_1553_Sub_Item>items;
//};

 //协议，基本结构
 struct __Protocol_Base
 {
	 __Protocol_Base()
	{
		 nReplay_Condition_ShareId =0;
		 nReply_protocol_ShareId =0;
	} ;
	int nId;
	 
	 string sName;			//Name
	 string sDescribe;		//描述
	 
	 string sRecSend;		//接收，发送
	 string sSendMode;		//发送机制
	 
	 int	nRefreshTime;   // 数据刷新/发送时间间隔
	 string sCheeckMode;	//校验位
	
	 string sSubDataUnit;	//子项数据单位，BYTE,BIT
	 int nSubDataLen;		//子项数据长度

	 int nSharedId;			//变量值：共享内存
	 //sRecSend：接收，有效
	 string sReply_Condition;			//应答条件
	 
	 int nReplay_Condition_ShareId;		//共享内存值
	 string sReplay_Value;				//应答值
	 
	 string sReply_protocol_Name;		//应答协议名称
	 int nReply_protocol_ShareId;		//应答协议的共享内存	 

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
