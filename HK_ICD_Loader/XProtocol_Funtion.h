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
	
	//�����ڴ漯�ϣ���Ҫ�Ƕ�ӦЭ���У�Э���Ӧ�Ĺ���ID����ӦЭ��������Ĺ���ID
	static void  Save_Data_Sets(int nProtoclId,  int nProtocol_ConfigId,int nProtocol_SharedId,int nChild_ConfigId,int nChild_SharedId);

	//ɾ��ĳһ��Э������е��빲���ڴ��й�ϵ����Ϣ�������nProtocolId
	static void DeleteProtoclRecod(int nDlgId,CString sProtocolName,int nDriveId,int nSysDriveId,int nRecSend,int nDataType,
		int& nProtocolId,CString sTableName="t_1553B_protocol");
	
	//����Э��
	static void SaveProtocol(int nDlgId, 
		int nProtocolId,
		CString sRecSend,
		CString sName,
		int nSysDriveId,
		CString sInfo,
		CString sBlockName,			//���ݿ�����
		CString sProtocolNo,		//�淶��
		CString sBlockType,			//���ݿ�����
		int nATAddr,			//AT��ַ
		int nATsuAddr,			//AT�ӵ�ַ
		CString sBusType);			//sBusType		);

	static void SaveProtocol(int nDlgId, 
		int nProtocolId,
		CString sRecSend,
		CString sName,
		int nSysDriveId,
		CString sInfo,
		BOOL bUpdate);

	// ����Э��ؼ���Ϣ
	static void SaveProCommCtrl(int nDlgId, 
		int nProtocolId,
		CString sRecSend,
		CString sName,
		int nProType,
		int nGroupId,
		CString sInfo,
		BOOL bUpdate);

	//����CANЭ��
	static void SaveCANProtocol(
		int nDlgId, 
		int nProtocolId,			//��Э��ID
		CString sRecSend,			//����/����
		CString sName,				//Э������
		int nSysDriveId,			//ϵͳ����ID
		CString sInfo,				//Э�����ԣ�xml��ʽ�ı���
		CString sBlockName,			//���ݿ�����
		CString sProtocolNo,		//�淶��
		CString sBlockType,			//���ݿ�����
		int nMailAddr,				//�����ַ
		int nMaskLen,				//��ʶ������
		CString sFrameType,			//֡����
		CString sFrameFormat,		//֡��ʽ
		int nFrameLen,				//֡����
		CString sCanStandard,		//CAN�淶
		CString sRate);				//����

	//����Э�鵽�����ڴ����������ΪDATA_TYPE_PROTOCL(99)
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

