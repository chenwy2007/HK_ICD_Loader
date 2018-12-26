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
	// 查找共享内存信息数据表，如果存在该记录则使用原来的ID
	int nId = 0;
	int nInOut = sRecSend=="接收"? 1:2;
	CString strFormat = "select nId from t_data_config where nDlgId=%d and sName='%s' and \
					 nDriveId=%d and nSysDriveId=%d and nInOut=%d;";

	CString strSql = "";

	strSql.Format(strFormat,nDlgId,sProtocolName,nDriveId,nSysDriveId,nInOut);
	if(g_DB.QueryDataId(strSql.GetBuffer(),nConfigId))
		return TRUE;

	// 不存在该记录，则获得数据表中未占用的ID
	nConfigId = XProtocol_Funtion::GetNewDataId();

	return FALSE;
}

BOOL XProtocol_Funtion::GetProtocolId(int nDlgId,int nSysDriveId,CString sProtocolName,
	CString sRecSend,int& nProtocolId)
{
	// 查找共享内存信息数据表，如果存在该记录则使用原来的ID
	int nId = 0;
	CString strFormat = "select nId from t_comm_protocol where nDlgId=%d and nSysDriveId=%d \
						and sName='%s' and nType=%d;";

	CString strSql = "";
	int nType = sRecSend==PROTOCOL_RecSend_Rec? 1: 2;
	strSql.Format(strFormat,nDlgId,nSysDriveId,sProtocolName,nType);
	if(g_DB.QueryDataId(strSql.GetBuffer(),nProtocolId))
		return TRUE;

	// 不存在该记录，则获得数据表中未占用的ID
	nProtocolId = XProtocol_Funtion::GetNewProtocolId();

	return FALSE;
}

BOOL XProtocol_Funtion::GetProCommCtrlId(int nDlgId,int nProType,CString sProtocolName,
	CString sRecSend,int& nProtocolId)
{
	// 查找共享内存信息数据表，如果存在该记录则使用原来的ID
	int nId = 0;
	CString strFormat = "select nId from t_comm_ctrl where nDlgId=%d and nProType=%d \
						and sName='%s' and nType=%d;";

	CString strSql = "";
	int nType = sRecSend== "显示" ? 1: 2;
	strSql.Format(strFormat,nDlgId,nProType,sProtocolName,nType);
	if(g_DB.QueryDataId(strSql.GetBuffer(),nProtocolId))
		return TRUE;

	// 不存在该记录，则获得数据表中未占用的ID
	nProtocolId = XProtocol_Funtion::GetNewProCommCtrlId();

	return FALSE;
}

/**
@brief      共享内存集合
@param[in]  nParendId :			对应t_comm_protocol中nId
@param[in]  nProtocolSharedId： 对应t_comm_protocol中nSharedId

@param[in]  nchildId :			对应t_comm_protocol中nId
@param[in]  nchild_SharedId：	对应t_comm_protocol中nSharedId
*/
void  XProtocol_Funtion::Save_Data_Sets(int nProtoclId,  int nProtocol_ConfigId,int nProtocol_SharedId,int nChild_ConfigId,int nChild_SharedId)
{
	CString  strSql;
	strSql.Format("insert into t_Data_Sets values((select max(nid)+1 from t_Data_Sets),%d,%d,%d,%d,%d,'')",nProtoclId,nProtocol_ConfigId,nProtocol_SharedId,nChild_ConfigId,nChild_SharedId);
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
}

/**
@brief     删除某一个协议的所有的与共享内存有关系的信息

@param[in]  nDlgId			对应产品ID
@param[in]  sProtocolName			数据名称
@param[in]  nDriveId		t_sys_drive中nDriveId
@param[in]  nSysDriveId		t_sys_drive中nId
@param[out]  nProtocolId	项目项对应的协议ID，t_comm_protocol表中nid,存在，返回现有值，不存在，则返回新的值
@param[in]	nRecSend		接收/发送数据
@param[in]	nDataType		数据块类型
*/
void XProtocol_Funtion::DeleteProtoclRecod(int nDlgId,CString sProtocolName,int nDriveId,
	int nSysDriveId,int nRecSend,int nDataType,int& nProtocolId,CString sTableName)
{
	CString strSql;

	//strSql.Format("delete from t_data_config_1;");
	//CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	//先判断当前是否存在该协议，以协议名称、板卡、产品为判断
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
		 
	//删除共享内存集合,
	//strSql.Format("delete from t_Data_Sets where nProtocolId  =%d ",nProtocolId);
	//CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	//删除共享内存
	strSql.Format("delete from t_data_config where nSysDriveId = %d and nDlgId = %d and nChannelIndex = %d;"
		,nSysDriveId,nDlgId,nProtocolId);
	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());

	//删除协议
	if(nDataType == 1)
		strSql.Format("delete  from %s where nDlgId = %d and nSysDriveId =%d and nRecOrSend=%d and sProtocolNo = '%s';"
		,sTableName,nDlgId,nSysDriveId,nRecSend,sProtocolName);
	else
		strSql.Format("delete  from %s where nDlgId = %d and nSysDriveId =%d and sProtocolNo = '%s';"
		,sTableName,nDlgId,nSysDriveId,sProtocolName);

	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
}

/**
@brief      保存协议到共享内存表，数据类型为DATA_TYPE_PROTOCL(99)

@param[in]  nDlgId			对应产品ID
@param[in]  sProtocolName			数据名称
@param[in]  nDriveId		t_sys_drive中nDriveId
@param[in]  nSysDriveId		t_sys_drive中nId
@param[in]  nProtocolId			项目项对应的协议ID，t_comm_protocol表中nid
@param[in]  nIndex				协议项序号,每一个协议中唯一
@param[out]  nItemId			对应新数据的NID(t_data_config)
@param[out]  nItem_SharedId		对应新数据的nSharedId(t_data_config)
@return		false,保存失败，或不需要保存，true,保存成功
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
@brief      保存协议到共享内存表，数据类型为DATA_TYPE_PROTOCL(99)

@param[in]  nDlgId			对应产品ID
@param[in]  sProtocolName			数据名称
@param[in]  nDriveId		t_sys_drive中nDriveId
@param[in]  nSysDriveId		t_sys_drive中nId
@param[in]  nProtocolId			项目项对应的协议ID，t_comm_protocol表中nid
@param[in]  nIndex				协议项序号,每一个协议中唯一
@param[out]  nItemId			对应新数据的NID(t_data_config)
@param[out]  nItem_SharedId		对应新数据的nSharedId(t_data_config)
@return		false,保存失败，或不需要保存，true,保存成功
*/
bool XProtocol_Funtion::Save_Protocl_ShareDataInfor(int nDlgId,CString sProtocolName,int nProtocolId,
	int nDriveId,int nSysDriveId,CString sRecSend,int nDataLen,int &nConfigId,int &nConfigSharedId,int nShiftBitNum)
{
	if(XProtocol_Funtion::GetConfigDataId(nDlgId,sProtocolName,nDriveId,nSysDriveId,sRecSend,nConfigId))
	{
		// 更新记录
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
		// 插入新记录
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
@brief      生成协议项共享内存值，并保存到共享内存表中

@param[in]  nDlgId  对应产品ID
@param[in]  sRecSend : 发送，接收
@param[in]  sDataName  数据名称
@param[in]  sDataType  数据类型
@param[in]  nDataLen   数据长度，只对字符有效
@param[in]  nProtocolId			项目项对应的协议ID，t_comm_protocol表中nid
@param[in]  nIndex				协议项序号,每一个协议中唯一
fCoefficient fOffset  :系数，补偿值
@param[out]  nItemId			对应新数据的NID(t_data_config)


@param[out]  nItem_SharedId		对应新数据的nSharedId(t_data_config)
@return		false,保存失败，或不需要保存，true,保存成功
*/
bool  XProtocol_Funtion::Save_ProtoclItem_toConfigData(int nDlgId,CString sRecSend,CString sDataName,CString sDataType,int nDataLen,int nProtocolId,int nIndex,int nDriveId,int nSysDriveId,CString sDefaultValue,
		float fCoefficient,float fOffset,CString sMark,
		int &nItemId,int &nItem_SharedId)
{ 
	int nDataType;
	int nDataSize;
	
	//数据类型
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
	
	//共享内存值
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
@brief      生成协议项共享内存值，并保存到共享内存表中

@param[in]  nDlgId  对应产品ID
@param[in]  sRecSend : 发送，接收
@param[in]  sDataName  数据名称
@param[in]  sDataType  数据类型
@param[in]  nDataLen   数据长度，只对字符有效
@param[in]  nProtocolId			项目项对应的协议ID，t_comm_protocol表中nid
@param[in]  nIndex				协议项序号,每一个协议中唯一
fCoefficient fOffset  :系数，补偿值
@param[out]  nItemId			对应新数据的NID(t_data_config)


@param[out]  nItem_SharedId		对应新数据的nSharedId(t_data_config)
@return		false,保存失败，或不需要保存，true,保存成功
*/
bool  XProtocol_Funtion::Save_ProtoclData_toConfigData(int nDlgId,CString sRecSend,
	CString sDataName,CString sDataType,int nDataLen,int nProtocolId,int nIndex,
	int nDriveId,int nSysDriveId,CString sDefaultValue,
	float fCoefficient,float fOffset,CString sMark,
	int &nItemId,int &nItem_SharedId,int nShiftBitNum)
{ 
	int nDataType;
	int nDataSize;

	//数据类型
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
		// 更新记录
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
		// 插入新记录
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
	CString sBlockName,			//数据块名称
	CString sProtocolNo,		//规范号
	CString sBlockType,			//数据块类型
	int nATAddr,			//AT地址
	int nATsubAddr,			//AT子地址
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
		// 更新已有记录
		strSql.Format("update t_comm_protocol set nDlgId=%d,nSysDriveId=%d, \
					  sName='%s',nType=%d,sProtocol='%s',sMark='%s' where nId=%d;",
			nDlgId,nSysDriveId,sName,nRecSend,sInfo,"",nProtocolId);	
	}
	else
	{
		// 插入新记录
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
		nRecSend = sRecSend =="显示" ? 1:2;

	if(bUpdate)
	{
		// 更新已有记录
		strSql.Format("update t_comm_ctrl set nDlgId=%d,nProType=%d,nGroupId=%d, \
					  sName='%s',nType=%d,sCtrl='%s',sMark='%s' where nId=%d;",
					  nDlgId,nProType,nGroupId,sName,nRecSend,sInfo,"",nProtocolId);	
	}
	else
	{
		// 插入新记录
		strSql.Format("insert into t_comm_ctrl values(%d,'%s',%d,%d,%d,%d,'%s','');",
			nProtocolId,sName,nDlgId,nProType,nGroupId,
			nRecSend/*sRecSend ==PROTOCOL_RecSend_Rec ? 0:1*/,sInfo);	
	}

	CManageDb_Sqllite::GetInstance().ExecDML(strSql.GetBuffer());
}


//保存CAN协议
void XProtocol_Funtion::SaveCANProtocol(
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
	CString sRate)				//速率
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
