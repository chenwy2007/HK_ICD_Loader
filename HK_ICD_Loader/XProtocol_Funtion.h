#pragma once
class XProtocol_Funtion
{
public:
	XProtocol_Funtion(void);
	~XProtocol_Funtion(void);

public:
	static int GetNewConfigDatId();
	static int GetNewDataId();
	static int GetNewProtocolId();
	static int GetNewProCommCtrlId();

	static BOOL GetConfigDataId(int nDlgId,CString sProtocolName,
		int nDriveId,int nSysDriveId,CString sRecSend,int& nConfigId);
	static BOOL GetProtocolId(int nDlgId,int nSysDriveId,CString sProtocolName,
		CString sRecSend,int& nProtocolId);
	static BOOL GetProCommCtrlId(int nDlgId,int nProType,CString sProtocolName,
		CString sRecSend,int& nProtocolId);
	
	//共享内存集合，主要是对应协议中，协议对应的共享ID，对应协议中子项的共享ID
	static void  Save_Data_Sets(int nProtoclId,  int nProtocol_ConfigId,int nProtocol_SharedId,int nChild_ConfigId,int nChild_SharedId);

	//删除某一个协议的所有的与共享内存有关系的信息，并获得nProtocolId
	static void DeleteProtoclRecod(int nDlgId,CString sProtocolName,int nDriveId,int nSysDriveId,int nRecSend,int nDataType,
		int& nProtocolId,CString sTableName="t_1553B_protocol");
	
	//保存协议
	static void SaveProtocol(int nDlgId, 
		int nProtocolId,
		CString sRecSend,
		CString sName,
		int nSysDriveId,
		CString sInfo,
		CString sBlockName,			//数据块名称
		CString sProtocolNo,		//规范号
		CString sBlockType,			//数据块类型
		int nATAddr,			//AT地址
		int nATsuAddr,			//AT子地址
		CString sBusType);			//sBusType		);

	static void SaveProtocol(int nDlgId, 
		int nProtocolId,
		CString sRecSend,
		CString sName,
		int nSysDriveId,
		CString sInfo,
		BOOL bUpdate);

	// 保存协议控件信息
	static void SaveProCommCtrl(int nDlgId, 
		int nProtocolId,
		CString sRecSend,
		CString sName,
		int nProType,
		int nGroupId,
		CString sInfo,
		BOOL bUpdate);

	//保存CAN协议
	static void SaveCANProtocol(
		int nDlgId, 
		int nProtocolId,			//主协议ID
		CString sRecSend,			//接收/发送
		CString sName,				//协议名称
		int nSysDriveId,			//系统驱动ID
		CString sInfo,				//协议属性（xml格式文本）
		CString sBlockName,			//数据块名称
		CString sProtocolNo,		//规范号
		CString sBlockType,			//数据块类型
		int nMailAddr,				//邮箱地址
		int nMaskLen,				//标识符长度
		CString sFrameType,			//帧类型
		CString sFrameFormat,		//帧格式
		int nFrameLen,				//帧长度
		CString sCanStandard,		//CAN规范
		CString sRate);				//速率

	//保存协议到共享内存表，数据类型为DATA_TYPE_PROTOCL(99)
	static bool Save_Protocl_toConfigData(int nDlgId,CString sProtocolName,int nProtocolId,int nDriveId,int nSysDriveId,CString sRecSend,int nDataLen,int &nConfigId,int &nConfigSharedId);

	static bool Save_Protocl_ShareDataInfor(int nDlgId,CString sProtocolName,int nProtocolId,
		int nDriveId,int nSysDriveId,CString sRecSend,int nDataLen,int &nConfigId,int &nConfigSharedId,int nShiftBitNum=8);

	static bool  Save_ProtoclItem_toConfigData(int nDlgId,CString  sRecSend,CString sDataName,
		CString sDataType,int nDataLen,int nProtocolId,int nIndex,int nDriveId,int nSysDriveId,CString sDefaultValue,
		float fCoefficient,float fOffset,CString sMark,
		int &nItemId,int &nItem_SharedId);
	static bool Save_ProtoclData_toConfigData(int nDlgId,CString sRecSend,
		CString sDataName,CString sDataType,int nDataLen,int nProtocolId,int nIndex,
		int nDriveId,int nSysDriveId,CString sDefaultValue,
		float fCoefficient,float fOffset,CString sMark,
		int &nItemId,int &nItem_SharedId,int nShiftBitNum=8,int nDealMode=0);

	static void ResetProtocolId() {m_nProtocolId = 0;}
	static void ResetDataId() {m_nDataId = 0;}

protected:
	static int m_nDataId;
	static int m_nProtocolId;
	static int m_nProCommCtrlId;


	
};

