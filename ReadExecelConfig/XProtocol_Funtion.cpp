#include "StdAfx.h"
#include "XProtocol_Funtion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int XProtocol_Funtion::m_nDataId = 0;
int XProtocol_Funtion::m_nProtocolId = 0;
int XProtocol_Funtion::m_nProCommCtrlId = 0;

XProtocol_Funtion::XProtocol_Funtion(void)
{
}


XProtocol_Funtion::~XProtocol_Funtion(void)
{
}
int XProtocol_Funtion::GetNewConfigDatId()
{
	 //t_data_config_1 nid
	int nId =g_DB.GetMaxId("select max(nId) from t_data_config_1 ");
	if(nId!=0)
		nId++;
	else 
	{
		nId =g_DB.GetMaxId("select max(nId) from t_data_config");
		nId++;
	}
	return nId;
}

int XProtocol_Funtion::GetNewDataId()
{
	CString strFormat = "select nId from t_data_config where nId=%d;";
	CString strSql = "";
	int nId = 0;
	while(TRUE)
	{
		strSql.Format(strFormat,m_nDataId);
		if(!g_DB.QueryDataId(strSql.GetBuffer(),nId))
			break;
		++ m_nDataId;
	}

	return m_nDataId;
}

int XProtocol_Funtion::GetNewProtocolId()
{
	CString strFormat = "select nId from t_comm_protocol where nId=%d;";
	CString strSql = "";
	int nId = 0;
	while(TRUE)
	{
		strSql.Format(strFormat,m_nProtocolId);
		if(!g_DB.QueryDataId(strSql.GetBuffer(),nId))
			break;
		++ m_nProtocolId;
	}

	return m_nProtocolId;
}

int XProtocol_Funtion::GetNewProCommCtrlId()
{
	CString strFormat = "select nId from t_comm_ctrl where nId=%d;";
	CString strSql = "";
	int nId = 0;
	while(TRUE)
	{
		strSql.Format(strFormat,m_nProCommCtrlId);
		if(!g_DB.QueryDataId(strSql.GetBuffer(),nId))
			break;
		++ m_nProCommCtrlId;
	}

	return m_nProCommCtrlId;
}

BOOL XProtocol_Funtion::GetConfigDataId(int nDlgId,CString sProtocolName,
	int nDriveId,int nSysDriveId,CString sRecSend,int& nConfigId)
{
	// ���ҹ����ڴ���Ϣ���ݱ�������ڸü�¼��ʹ��ԭ����ID
	int nId = 0;
	int nInOut = sRecSend=="����"? 1:2;
	CString strFormat = "select nId from t_data_config where nDlgId=%d and sName='%s' and \
					 nDriveId=%d and nSysDriveId=%d and nInOut=%d;";

	CString strSql = "";

	strSql.Format(strFormat,nDlgId,sProtocolName,nDriveId,nSysDriveId,nInOut);
	if(g_DB.QueryDataId(strSql.GetBuffer(),nConfigId))
		return TRUE;

	// �����ڸü�¼���������ݱ���δռ�õ�ID
	nConfigId = XProtocol_Funtion::GetNewDataId();

	return FALSE;
}

BOOL XProtocol_Funtion::GetProtocolId(int nDlgId,int nSysDriveId,CString sProtocolName,
	CString sRecSend,int& nProtocolId)
{
	// ���ҹ����ڴ���Ϣ���ݱ�������ڸü�¼��ʹ��ԭ����ID
	int nId = 0;
	CString strFormat = "select nId from t_comm_protocol where nDlgId=%d and nSysDriveId=%d \
						and sName='%s' and nType=%d;";

	CString strSql = "";
	int nType = sRecSend==PROTOCOL_RecSend_Rec? 1: 2;
	strSql.Format(strFormat,nDlgId,nSysDriveId,sProtocolName,nType);
	if(g_DB.QueryDataId(strSql.GetBuffer(),nProtocolId))
		return TRUE;

	// �����ڸü�¼���������ݱ���δռ�õ�ID
	nProtocolId = XProtocol_Funtion::GetNewProtocolId();

	return FALSE;
}

BOOL XProtocol_Funtion::GetProCommCtrlId(int nDlgId,int nProType,CString sProtocolName,
	CString sRecSend,int& nProtocolId)
{
	// ���ҹ����ڴ���Ϣ���ݱ�������ڸü�¼��ʹ��ԭ����ID
	int nId = 0;
	CString strFormat = "select nId from t_comm_ctrl where nDlgId=%d and nProType=%d \
						and sName='%s' and nType=%d;";

	CString strSql = "";
	int nType = sRecSend== "��ʾ" ? 1: 2;
	strSql.Format(strFormat,nDlgId,nProType,sProtocolName,nType);
	if(g_DB.QueryDataId(strSql.GetBuffer(),nProtocolId))
		return TRUE;

	// �����ڸü�¼���������ݱ���δռ�õ�ID
	nProtocolId = XProtocol_Funtion::GetNewProCommCtrlId();

	return FALSE;
}

/**
@brief      �����ڴ漯��
@param[in]  nParendId :			��Ӧt_comm_protocol��nId
@param[in]  nProtocolSharedId�� ��Ӧt_comm_protocol��nSharedId

@param[in]  nchildId :			��Ӧt_comm_protocol��nId
@param[in]  nchild_SharedId��	��Ӧt_comm_protocol��nSharedId
*/
void  XProtocol_Funtion::Save_Data_Sets(int nProtoclId,  int nProtocol_ConfigId,int nProtocol_SharedId,int nChild_ConfigId,int nChild_SharedId)
{
	CString  strSql;
	strSql.Format("insert into t_Data_Sets values((select max(nid)+1 from t_Data_Sets),%d,%d,%d,%d,%d,'')",nProtoclId,nProtocol_ConfigId,nProtocol_SharedId,nChild_ConfigId,nChild_SharedId);
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
}

/**
@brief     ɾ��ĳһ��Э������е��빲���ڴ��й�ϵ����Ϣ

@param[in]  nDlgId			��Ӧ��ƷID
@param[in]  sProtocolName			��������
@param[in]  nDriveId		t_sys_drive��nDriveId
@param[in]  nSysDriveId		t_sys_drive��nId
@param[out]  nProtocolId	��Ŀ���Ӧ��Э��ID��t_comm_protocol����nid,���ڣ���������ֵ�������ڣ��򷵻��µ�ֵ
@param[in]	nRecSend		����/��������
@param[in]	nDataType		���ݿ�����
*/
void XProtocol_Funtion::DeleteProtoclRecod(int nDlgId,CString sProtocolName,int nDriveId,
	int nSysDriveId,int nRecSend,int nDataType,int& nProtocolId,CString sTableName)
{
	CString strSql;

	//strSql.Format("delete from t_data_config_1;");
	//CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	//���жϵ�ǰ�Ƿ���ڸ�Э�飬��Э�����ơ��忨����ƷΪ�ж�
	if(nDataType == 1)
		strSql.Format("select nId from %s where nDlgId = %d and nSysDriveId =%d and nRecOrSend=%d and sProtocolNo = '%s';"
		,sTableName,nDlgId,nSysDriveId,nRecSend,sProtocolName);
	else
		strSql.Format("select nId from %s where nDlgId = %d and nSysDriveId =%d and sProtocolNo = '%s';"
		,sTableName,nDlgId,nSysDriveId,sProtocolName);

	nProtocolId= CManageDb_Sqllite::GetInstance().GetMaxId(strSql.GetBuffer());
	if(nProtocolId ==0)
	{
		CString strMaxSql;
		strMaxSql.Format("select max(nId) from %s;",sTableName);
		nProtocolId = CManageDb_Sqllite::GetInstance().GetMaxId(strMaxSql.GetBuffer())+1;
		return;
	}
		 
	//ɾ�������ڴ漯��,
	//strSql.Format("delete from t_Data_Sets where nProtocolId  =%d ",nProtocolId);
	//CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	//ɾ�������ڴ�
	strSql.Format("delete from t_data_config where nSysDriveId = %d and nDlgId = %d and nChannelIndex = %d;"
		,nSysDriveId,nDlgId,nProtocolId);
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	//ɾ��Э��
	if(nDataType == 1)
		strSql.Format("delete  from %s where nDlgId = %d and nSysDriveId =%d and nRecOrSend=%d and sProtocolNo = '%s';"
		,sTableName,nDlgId,nSysDriveId,nRecSend,sProtocolName);
	else
		strSql.Format("delete  from %s where nDlgId = %d and nSysDriveId =%d and sProtocolNo = '%s';"
		,sTableName,nDlgId,nSysDriveId,sProtocolName);

	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
}

/**
@brief      ����Э�鵽�����ڴ����������ΪDATA_TYPE_PROTOCL(99)

@param[in]  nDlgId			��Ӧ��ƷID
@param[in]  sProtocolName			��������
@param[in]  nDriveId		t_sys_drive��nDriveId
@param[in]  nSysDriveId		t_sys_drive��nId
@param[in]  nProtocolId			��Ŀ���Ӧ��Э��ID��t_comm_protocol����nid
@param[in]  nIndex				Э�������,ÿһ��Э����Ψһ
@param[out]  nItemId			��Ӧ�����ݵ�NID(t_data_config)
@param[out]  nItem_SharedId		��Ӧ�����ݵ�nSharedId(t_data_config)
@return		false,����ʧ�ܣ�����Ҫ���棬true,����ɹ�
*/
bool XProtocol_Funtion::Save_Protocl_toConfigData(int nDlgId,CString sProtocolName,int nProtocolId,
	int nDriveId,int nSysDriveId,CString sRecSend,int nDataLen,int &nConfigId,int &nConfigSharedId)
{
	
	nConfigSharedId =  (nDriveId<<24) +(nSysDriveId<<16) +(nProtocolId<<8);
	nConfigId = GetNewConfigDatId();

	CString strSql;
	strSql.Format("insert into t_data_config_1 values(%d,%d,'%s',0,%d,%d,%d,%d,%d,%d,0,0,%d,'',0,'','')",
		nConfigId,nDlgId,sProtocolName,nSysDriveId,nDriveId,nProtocolId,nConfigSharedId,DATA_TYPE_PROTOCL,nDataLen,sRecSend==PROTOCOL_RecSend_Rec?1:2);

	return g_DB.ExecDML(strSql.GetBuffer());	
}

/**
@brief      ����Э�鵽�����ڴ����������ΪDATA_TYPE_PROTOCL(99)

@param[in]  nDlgId			��Ӧ��ƷID
@param[in]  sProtocolName			��������
@param[in]  nDriveId		t_sys_drive��nDriveId
@param[in]  nSysDriveId		t_sys_drive��nId
@param[in]  nProtocolId			��Ŀ���Ӧ��Э��ID��t_comm_protocol����nid
@param[in]  nIndex				Э�������,ÿһ��Э����Ψһ
@param[out]  nItemId			��Ӧ�����ݵ�NID(t_data_config)
@param[out]  nItem_SharedId		��Ӧ�����ݵ�nSharedId(t_data_config)
@return		false,����ʧ�ܣ�����Ҫ���棬true,����ɹ�
*/
bool XProtocol_Funtion::Save_Protocl_ShareDataInfor(int nDlgId,CString sProtocolName,int nProtocolId,
	int nDriveId,int nSysDriveId,CString sRecSend,int nDataLen,int &nConfigId,int &nConfigSharedId,int nShiftBitNum)
{
	if(XProtocol_Funtion::GetConfigDataId(nDlgId,sProtocolName,nDriveId,nSysDriveId,sRecSend,nConfigId))
	{
		// ���¼�¼
		if(nShiftBitNum > 8)
			nConfigSharedId =  (nDriveId<<24) +(nSysDriveId<<16) +((nProtocolId<<nShiftBitNum)&0xffff);
		else
			nConfigSharedId =  (nDriveId<<24) +(nSysDriveId<<16) +(nProtocolId<<8);

		CString strFormat = "update t_data_config set nDutIndex=%d,nChannelIndex=%d,nSharedId=%d,nDataType=%d,nDataSize=%d,\
			fCoefficient=%f,fOffset=%f,mark='%s',nDealMode=%d,sDefaultValue='%s',ExtendedAttr='%s' \
		where nId=%d;";
		CString strSql;
		strSql.Format(strFormat,0,nProtocolId,nConfigSharedId,DATA_TYPE_PROTOCL,nDataLen,1.0,0.0,"",0,"","",nConfigId);

		return g_DB.ExecDML(strSql.GetBuffer());	
	}
	else
	{
		// �����¼�¼
		if(nShiftBitNum > 8)
			nConfigSharedId =  (nDriveId<<24) +(nSysDriveId<<16) +((nProtocolId<<nShiftBitNum)&0xffff);
		else
			nConfigSharedId =  (nDriveId<<24) +(nSysDriveId<<16) +(nProtocolId<<8);

		CString strSql;
		strSql.Format("insert into t_data_config values(%d,%d,'%s',0,%d,%d,%d,%d,%d,%d,1.0,0.0,%d,'',0,'','');",
			nConfigId,nDlgId,sProtocolName,nSysDriveId,nDriveId,nProtocolId,nConfigSharedId,DATA_TYPE_PROTOCL,nDataLen,sRecSend==PROTOCOL_RecSend_Rec?1:2);

		return g_DB.ExecDML(strSql.GetBuffer());	
	}

	return false;
}

/**
@brief      ����Э������ڴ�ֵ�������浽�����ڴ����

@param[in]  nDlgId  ��Ӧ��ƷID
@param[in]  sRecSend : ���ͣ�����
@param[in]  sDataName  ��������
@param[in]  sDataType  ��������
@param[in]  nDataLen   ���ݳ��ȣ�ֻ���ַ���Ч
@param[in]  nProtocolId			��Ŀ���Ӧ��Э��ID��t_comm_protocol����nid
@param[in]  nIndex				Э�������,ÿһ��Э����Ψһ
fCoefficient fOffset  :ϵ��������ֵ
@param[out]  nItemId			��Ӧ�����ݵ�NID(t_data_config)


@param[out]  nItem_SharedId		��Ӧ�����ݵ�nSharedId(t_data_config)
@return		false,����ʧ�ܣ�����Ҫ���棬true,����ɹ�
*/
bool  XProtocol_Funtion::Save_ProtoclItem_toConfigData(int nDlgId,CString sRecSend,CString sDataName,CString sDataType,int nDataLen,int nProtocolId,int nIndex,int nDriveId,int nSysDriveId,CString sDefaultValue,
		float fCoefficient,float fOffset,CString sMark,
		int &nItemId,int &nItem_SharedId)
{ 
	int nDataType;
	int nDataSize;
	
	//��������
	if(sDataType ==DATA_TYPE_INT_NAME || sDataType=="")
	{
		nDataType =DATA_TYPE_INT;
		nDataSize =DATA_TYPE_INT_LEN;
	}
	else if(sDataType==DATA_TYPE_FLOAT_NAME)
	{
		nDataType =DATA_TYPE_FLOAT;
		nDataSize =DATA_TYPE_FLOAT_LEN;
	}
	else if(sDataType==DATA_TYPE_STRING_NAME )
	{
		nDataType =DATA_TYPE_STRING;
		nDataSize =nDataLen;
	}
	else if(sDataType==DATA_TYPE_CHAR_NAME)
	{
		nDataType =DATA_TYPE_CHAR;
		nDataSize =DATA_TYPE_CHAR_LEN;
	}
	else if(sDataType==DATA_TYPE_DOUBLE_NAME)
	{
		nDataType =DATA_TYPE_DOUBLE;
		nDataSize =DATA_TYPE_DOUBLE_LEN;
	} 
	else if(sDataType ==DATA_TYPE_STRING_UNICODE_NAME)
	{
		nDataType =DATA_TYPE_STRING_UNICODE;
		nDataSize =nDataLen;
	}
	else 
		return false;
	
	//�����ڴ�ֵ
	nItem_SharedId =  (nDriveId<<24) +(nSysDriveId<<16) +(nProtocolId<<8)+nIndex;
	//t_data_config_1 nid
	nItemId =g_DB.GetMaxId("select max(nId) from t_data_config_1 ");
	if(nItemId!=0)
		nItemId++;
	else 
	{
		nItemId =g_DB.GetMaxId("select max(nId) from t_data_config");
		nItemId++;
	}

	CString strSql;
	strSql.Format("insert into t_data_config_1 values(%d,%d,'%s',0,%d,%d, \
				  %d,%d,%d,%d,%.2f,\
				  %.2f,%d,'%s',0,'%s','')",
		nItemId,nDlgId,sDataName,nSysDriveId,nDriveId,  
		nProtocolId,nItem_SharedId,nDataType,nDataSize,fCoefficient,
		fOffset,sRecSend==PROTOCOL_RecSend_Rec?1:2,sMark,sDefaultValue);

	return g_DB.ExecDML(strSql.GetBuffer());
}

/**
@brief      ����Э������ڴ�ֵ�������浽�����ڴ����

@param[in]  nDlgId  ��Ӧ��ƷID
@param[in]  sRecSend : ���ͣ�����
@param[in]  sDataName  ��������
@param[in]  sDataType  ��������
@param[in]  nDataLen   ���ݳ��ȣ�ֻ���ַ���Ч
@param[in]  nProtocolId			��Ŀ���Ӧ��Э��ID��t_comm_protocol����nid
@param[in]  nIndex				Э�������,ÿһ��Э����Ψһ
fCoefficient fOffset  :ϵ��������ֵ
@param[out]  nItemId			��Ӧ�����ݵ�NID(t_data_config)


@param[out]  nItem_SharedId		��Ӧ�����ݵ�nSharedId(t_data_config)
@return		false,����ʧ�ܣ�����Ҫ���棬true,����ɹ�
*/
bool  XProtocol_Funtion::Save_ProtoclData_toConfigData(int nDlgId,CString sRecSend,
	CString sDataName,CString sDataType,int nDataLen,int nProtocolId,int nIndex,
	int nDriveId,int nSysDriveId,CString sDefaultValue,
	float fCoefficient,float fOffset,CString sMark,
	int &nItemId,int &nItem_SharedId,int nShiftBitNum)
{ 
	int nDataType;
	int nDataSize;

	//��������
	if(sDataType ==DATA_TYPE_INT_NAME || sDataType=="")
	{
		nDataType =DATA_TYPE_INT;
		nDataSize =DATA_TYPE_INT_LEN;
	}
	else if(sDataType==DATA_TYPE_FLOAT_NAME)
	{
		nDataType =DATA_TYPE_FLOAT;
		nDataSize =DATA_TYPE_FLOAT_LEN;
	}
	else if(sDataType==DATA_TYPE_STRING_NAME )
	{
		nDataType =DATA_TYPE_STRING;
		nDataSize =nDataLen;
	}
	else if(sDataType==DATA_TYPE_CHAR_NAME)
	{
		nDataType =DATA_TYPE_CHAR;
		nDataSize =DATA_TYPE_CHAR_LEN;
	}
	else if(sDataType==DATA_TYPE_DOUBLE_NAME)
	{
		nDataType =DATA_TYPE_DOUBLE;
		nDataSize =DATA_TYPE_DOUBLE_LEN;
	} 
	else if(sDataType ==DATA_TYPE_STRING_UNICODE_NAME)
	{
		nDataType =DATA_TYPE_STRING_UNICODE;
		nDataSize =nDataLen;
	}
	else 
		return false;

	if(XProtocol_Funtion::GetConfigDataId(nDlgId,sDataName,nDriveId,nSysDriveId,sRecSend,nItemId))
	{
		// ���¼�¼
		if(nShiftBitNum > 8)
			nItem_SharedId =  (nDriveId<<24) +(nSysDriveId<<16) +(((nProtocolId<<nShiftBitNum)+nIndex)&0xffff);
		else
			nItem_SharedId =  (nDriveId<<24) +(nSysDriveId<<16) +(nProtocolId<<8)+nIndex;

		CString strSql;
		strSql.Format("update t_data_config set nDutIndex=%d,nChannelIndex=%d,nSharedId=%d,nDataType=%d,nDataSize=%d,\
					  fCoefficient=%f,fOffset=%f,mark='%s',nDealMode=%d,sDefaultValue='%s',ExtendedAttr='%s' \
					  where nId=%d;",0,nProtocolId,nItem_SharedId,nDataType,nDataSize,fCoefficient,fOffset,"",0,"","",nItemId);

		return g_DB.ExecDML(strSql.GetBuffer());	
	}
	else
	{
		// �����¼�¼
		if(nShiftBitNum > 8)
			nItem_SharedId =  (nDriveId<<24) +(nSysDriveId<<16) +(((nProtocolId<<nShiftBitNum)+nIndex)&0xffff);
		else
			nItem_SharedId =  (nDriveId<<24) +(nSysDriveId<<16) +(nProtocolId<<8)+nIndex;

		CString strSql;
		strSql.Format("insert into t_data_config values(%d,%d,'%s',0,%d,%d,%d,%d,%d,%d,%f,%f,%d,'%s',0,'%s','');",
			nItemId,nDlgId,sDataName,nSysDriveId,nDriveId,nProtocolId,nItem_SharedId,nDataType,nDataSize,
			fCoefficient,fOffset,sRecSend==PROTOCOL_RecSend_Rec?1:2,sMark,sDefaultValue);

		return g_DB.ExecDML(strSql.GetBuffer());	
	}

	return false;
}

void XProtocol_Funtion::SaveProtocol(int nDlgId, 
	int nProtocolId,
	CString sRecSend,
	CString sName,
	int nSysDriveId,
	CString sInfo,
	CString sBlockName,			//���ݿ�����
	CString sProtocolNo,		//�淶��
	CString sBlockType,			//���ݿ�����
	int nATAddr,			//AT��ַ
	int nATsubAddr,			//AT�ӵ�ַ
	CString sBusType)			//sBusType
{
	CString strSql;
	
	int nRecSend = -1;
	if(!sRecSend.IsEmpty())
		nRecSend = sRecSend ==PROTOCOL_RecSend_Rec ? 1:2;
	strSql.Format("insert into t_1553B_protocol values(%d,%d,%d,'%s','%s','%s','%s',%d,%d,%d,'%s','%s','')",
		nProtocolId,nDlgId,nSysDriveId,sBlockName,sProtocolNo,sName,sBlockType,nATAddr,nATsubAddr,
		nRecSend/*sRecSend ==PROTOCOL_RecSend_Rec ? 0:1*/,sBusType,sInfo);	

	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

}

void XProtocol_Funtion::SaveProtocol(int nDlgId, 
	int nProtocolId,
	CString sRecSend,
	CString sName,
	int nSysDriveId,
	CString sInfo,
	BOOL bUpdate)
{
	CString strSql;
	int nRecSend = -1;
	if(!sRecSend.IsEmpty())
		nRecSend = sRecSend ==PROTOCOL_RecSend_Rec ? 1:2;

	if(bUpdate)
	{
		// �������м�¼
		strSql.Format("update t_comm_protocol set nDlgId=%d,nSysDriveId=%d, \
					  sName='%s',nType=%d,sProtocol='%s',sMark='%s' where nId=%d;",
			nDlgId,nSysDriveId,sName,nRecSend,sInfo,"",nProtocolId);	
	}
	else
	{
		// �����¼�¼
		strSql.Format("insert into t_comm_protocol values(%d,%d,%d,'%s',%d,'%s','');",
			nProtocolId,nDlgId,nSysDriveId,sName,
			nRecSend/*sRecSend ==PROTOCOL_RecSend_Rec ? 0:1*/,sInfo);	
	}

	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
}

void XProtocol_Funtion::SaveProCommCtrl(int nDlgId, 
	int nProtocolId,
	CString sRecSend,
	CString sName,
	int nProType,
	int nGroupId,
	CString sInfo,
	BOOL bUpdate)
{
	CString strSql;
	int nRecSend = -1;
	if(!sRecSend.IsEmpty())
		nRecSend = sRecSend =="��ʾ" ? 1:2;

	if(bUpdate)
	{
		// �������м�¼
		strSql.Format("update t_comm_ctrl set nDlgId=%d,nProType=%d,nGroupId=%d, \
					  sName='%s',nType=%d,sCtrl='%s',sMark='%s' where nId=%d;",
					  nDlgId,nProType,nGroupId,sName,nRecSend,sInfo,"",nProtocolId);	
	}
	else
	{
		// �����¼�¼
		strSql.Format("insert into t_comm_ctrl values(%d,'%s',%d,%d,%d,%d,'%s','');",
			nProtocolId,sName,nDlgId,nProType,nGroupId,
			nRecSend/*sRecSend ==PROTOCOL_RecSend_Rec ? 0:1*/,sInfo);	
	}

	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
}


//����CANЭ��
void XProtocol_Funtion::SaveCANProtocol(
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
	CString sRate)				//����
{
	CString strSql;

	int nRecSend = -1;
	if(!sRecSend.IsEmpty())
		nRecSend = sRecSend ==PROTOCOL_RecSend_Rec ? 1:2;
	strSql.Format("insert into t_CAN_protocol values(%d,%d,%d,'%s','%s','%s','%s',%d,%d,'%s','%s',%d,'%s','%s',%d,'%s','')",
		nProtocolId,nDlgId,nSysDriveId,sBlockName,sProtocolNo,sName,sBlockType,nMailAddr,
		nMaskLen,sFrameType,sFrameFormat,nFrameLen,sCanStandard,sRate,
		nRecSend/*sRecSend ==PROTOCOL_RecSend_Rec ? 0:1*/,sInfo);	

	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
}
