#include "StdAfx.h"
#include "XSharedMemoryManage.h"


//XSharedMemoryManage::XSharedMemoryManage(string sShareName,int nMemorySize)
//	:m_sShareName(sShareName),m_nMemorySize(nMemorySize)
//{
//	m_hMapFile =NULL;
//}

#define ADDR_DATA_SIZE  5
 XSharedMemoryManage *g_pShared = NULL;
XSharedMemoryManage::XSharedMemoryManage()
{
	m_hMapFile =NULL;
	m_sShareName= "HK_Shared_Memory";
	m_nMemorySize = 5;
	m_pMapVoid =NULL;
	m_nRectify=1;		// 默认采集数据校正
}
XSharedMemoryManage::~XSharedMemoryManage(void)
{
	UnmapViewOfFile(m_pMapVoid);	
 	m_pMapVoid =NULL;
	CloseHandle(m_hMapFile);
	m_hMapFile =NULL;

	map<int,__SharedM>().swap(m_mapShare);
	 
}

XSharedMemoryManage & XSharedMemoryManage::GetInstance()
{
	static XSharedMemoryManage manage;

	return   manage;
}

bool XSharedMemoryManage::CreateShareMemory(string sShareName,int nShareMemorySize)
{
	m_sShareName = sShareName;
	m_nMemorySize = nShareMemorySize;
	if(!OpenShareMemory(m_sShareName))
	{ 
		m_hMapFile =CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,m_nMemorySize*1024*1024,m_sShareName.c_str());
		
	}

	if(m_hMapFile==NULL)
	{
		// LOG_Out_3(LOG_NAME_HK,__FUNCTION__,__LINE__,"CreateFileMapping(%s) return NULL",m_sShareName.c_str());
		return false;
	}
	m_pMapVoid = ::MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if(m_pMapVoid ==NULL)
	{
	//	LOG_Out_3(LOG_NAME_HK,__FUNCTION__,__LINE__,"MapViewOfFile() return NULL");
		return false;
	}
	return true;
}
bool XSharedMemoryManage::OpenShareMemory(string sShareName)
{ 
	m_sShareName = sShareName;
	m_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE,m_sShareName.c_str());
	if(m_hMapFile==NULL)
	{
		return false;
	}
	
	m_pMapVoid = ::MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS, 0,0,0);
	if(m_pMapVoid ==NULL)
	{
	
		return false;
	}
	return true;

}
bool XSharedMemoryManage::IsOpenShareMemory()
{
	return m_pMapVoid !=NULL;
};
void XSharedMemoryManage::QueryConfigData()
{

}
bool XSharedMemoryManage::ReadSysInfo()
{	 
	if(!OpenShareMemory())
	{
		LOG_Out(LOG_NAME_HK,"OpenShareMemory() return false");
		return false;
	}
 
	ReadDataFromMemory(0,(void *)&m_sysInfo,sizeof(__SysInfoMemory));
	if(m_sysInfo.nShareDCount==0)
	{
		return false;
	}
	int nAddr =sizeof(__SysInfoMemory);
	int nSize = sizeof(__SharedM);
	map<int,__SharedM>SharedMap;

	//if(m_sysInfo.nReCreate ==0 )
 
	for(int i =0;i<m_sysInfo.nShareDCount;i++)
	{
		__SharedM share;
		ReadDataFromMemory(nAddr,(void *)&share,nSize);

		share.lpData = (char*)m_pMapVoid+share.nStartAddr+ADDR_DATA_SIZE;
		share.lpTickCount = (DWORD*)((char *)m_pMapVoid+share.nStartAddr);
		share.lpUpdate = (u8*)((char *)m_pMapVoid+share.nStartAddr+sizeof(DWORD));

		SharedMap[share.nId] = share;	
			
		nAddr+=nSize;
	}
	m_mapShare.swap(SharedMap);
	 
	 
	return true;

}


void XSharedMemoryManage::InitShareMemory(int nDlgId,vector<__DATA_CONFIG>&vec)
{
  
	int nBaseAddr = sizeof(__SysInfoMemory);
	int nAddr  =vec.size()*sizeof(__SharedM)+sizeof(__SysInfoMemory);

	map<int ,__SharedM>().swap(m_mapShare);
	int nLastSize =0;
	int nSize=0;
	for(int i =0,end =vec.size();i<end;i++)
	{
		__SharedM share;
		share.nId = vec[i].nSharedID;
		share.nDataType = vec[i].nDataType;
		share.nOutIn = vec[i].nInOut;
		// 所有输入和输出信号都建立共享内存
		//if(vec[i].nInOut ==2 && vec[i].nDataType!= DATA_TYPE_PROTOCL)
		//	continue;

		if(vec[i].nDataType == DATA_TYPE_INT)
		{
			share.nSize =sizeof(int);
		} 
		else if(vec[i].nDataType == DATA_TYPE_FLOAT)
		{
			share.nSize =sizeof(float);
		}
		else if(vec[i].nDataType == DATA_TYPE_DOUBLE)
		{
			share.nSize =sizeof(double);
		}
		else
			share.nSize = vec[i].nDataSize;

		share.nStartAddr = nAddr+nLastSize;	
		nAddr+=nLastSize;
		nLastSize = share.nSize+sizeof(DWORD)+1; //数据长度+数据更新时间(4byte)+是否更新（1byte)

		//写入索引信息
		WriteDataToMemory(nBaseAddr,(void*)&share,sizeof(__SharedM));

		nBaseAddr +=sizeof(__SharedM);

		WriteUpDateTime(share);
		WriteUpdateFlag(share);
		string strData;
		share.lpData = (char*)m_pMapVoid+share.nStartAddr+ADDR_DATA_SIZE;
		if(vec[i].nDataType == DATA_TYPE_INT)
		{
		
			int nValue = atoi(vec[i].sDefaultValue.c_str());
			
			WriteDataToMemory(share.nStartAddr+ADDR_DATA_SIZE,&nValue,sizeof(int));
		}
		else if(vec[i].nDataType == DATA_TYPE_FLOAT)
		{
			float fValue =atof(vec[i].sDefaultValue.c_str());
			WriteDataToMemory(share.nStartAddr+ADDR_DATA_SIZE,&fValue,sizeof(float));
		}
		else if(vec[i].nDataType == DATA_TYPE_DOUBLE)
		{
			double dValue =atof(vec[i].sDefaultValue.c_str());
			WriteDataToMemory(share.nStartAddr,&dValue,sizeof(double));
		}
		else if(vec[i].nDataType ==DATA_TYPE_STRING || vec[i].nDataType ==DATA_TYPE_CHAR || vec[i].nDataType == DATA_TYPE_STRING_UNICODE)
		{
			 strData=vec[i].sDefaultValue;
			strData.resize(share.nSize);
			WriteDataToMemory(share.nStartAddr+ADDR_DATA_SIZE,(void *)strData.c_str(),share.nSize);
		}
		else if(vec[i].nDataType ==DATA_TYPE_PROTOCL) //协议
		{
			if(vec[i].nDataSize>0)
			{
				strData =vec[i].sDefaultValue;
				strData.resize(share.nSize);
				WriteDataToMemory(share.nStartAddr+ADDR_DATA_SIZE,(void *)strData.c_str(),share.nSize);
			}
		}
		// 缓存区共享内存初始化为零
		else if(vec[i].nDataType == DATA_TYPE_BUFFER)
		{
			if(vec[i].nDataSize > 0)
			{
				if(m_pMapVoid)
				{
					memset((char*)m_pMapVoid+share.nStartAddr+ADDR_DATA_SIZE,0,share.nSize);
				}
			}
		}

		m_mapShare[share.nId] =share;
		nSize++;
	}

	m_sysInfo.nShareDCount = nSize;//vec.size();
	WriteDataToMemory(0,(void *)&m_sysInfo,sizeof(__SysInfoMemory));
}
void XSharedMemoryManage::WriteSysInfo(char * lpData,int nLen)
{
	if(m_pMapVoid)
		memcpy(m_pMapVoid,lpData,nLen);
}
void XSharedMemoryManage::GetSysInfo(char * lpbuf)
{	
	ReadDataFromMemory(0,lpbuf,64);
	
}
bool XSharedMemoryManage::WriteDataToMemory(int nAddr ,void * lpData,int nSize)
{
	if(!m_pMapVoid)
		return false;
	memcpy((char*)m_pMapVoid+nAddr,lpData,nSize);
	return true;
}

bool XSharedMemoryManage::WriteUpDateTime(__SharedM & share)
{ 
	if(share.lpTickCount)
		*share.lpTickCount =GetTickCount();
	else
	{
		DWORD dwTime = GetTickCount();
		share.lpTickCount = (DWORD*)((char *)m_pMapVoid+share.nStartAddr);
		WriteDataToMemory(share.nStartAddr,&dwTime,sizeof(dwTime));
	}
	return true;
}
bool XSharedMemoryManage::WriteUpdateFlag(__SharedM & share)
{
	int nUpdateValue = 1;
	// 如果是输出类型的共享内存，更新标志初始为零
	if(share.nOutIn == 2)
		nUpdateValue = 0;
	if(share.lpUpdate)
		*share.lpUpdate = nUpdateValue;
	else
	{	
		u8 nV8 = nUpdateValue;
		share.lpUpdate = (u8*)((char *)m_pMapVoid+share.nStartAddr+sizeof(DWORD));
		WriteDataToMemory(share.nStartAddr+sizeof(DWORD),&nV8,sizeof(u8));
	}
	return true;
}
bool XSharedMemoryManage::WriteData_Int(int nId,int  nValue)
{	  
	map<int ,__SharedM>::iterator it = m_mapShare.find(nId);
	if(it ==m_mapShare.end())
		return false;
	__SharedM& share = it->second;
	WriteUpDateTime(share);

	if(share.lpData!=0)
	{
		int *nV = (int *)share.lpData;
		if(*nV != nValue)
		{
			WriteUpdateFlag(share);
			memcpy(share.lpData,&nValue,sizeof(int));			 
		}
	}
	else
	{
		WriteUpdateFlag(share);
		share.lpData   = (void*)((char *)m_pMapVoid+share.nStartAddr+sizeof(DWORD)+1);
		memcpy(share.lpData,&nValue,sizeof(int));	
	}
	
	return true;
}

bool XSharedMemoryManage::WriteData_Float(int nId, float fValue)
{
	map<int ,__SharedM>::iterator it = m_mapShare.find(nId);
	if(it ==m_mapShare.end())
		return false;
	__SharedM&  share = it->second;
	WriteUpDateTime(share);
 
	if(share.lpData!=0)
	{
		float *fV = (float *)share.lpData;
		if(abs(*fV-fValue)>ZERO_VALUE)		
		{
			WriteUpdateFlag(share);
			memcpy(share.lpData,&fValue,sizeof(float));			 
		}
	}
	else
	{
		WriteUpdateFlag(share);
		share.lpData   = (void*)((char *)m_pMapVoid+share.nStartAddr+sizeof(DWORD)+1);
		memcpy(share.lpData,&fValue,sizeof(float));	
	}

	return true;
}

bool XSharedMemoryManage::WriteData_Double(int nId, double dValue)
{
	map<int ,__SharedM>::iterator it = m_mapShare.find(nId);
	if(it ==m_mapShare.end())
	{
		LOG_Out(LOG_NAME_HK,"未发现共享变量:%d",nId);
		return false;	

	}
	__SharedM&  share = it->second;
	

	if(share.nDataType == DATA_TYPE_FLOAT)
	{ 
		return WriteData_Float(nId,dValue);	
	}
	else if(share.nDataType == DATA_TYPE_DOUBLE)
	{
			WriteUpDateTime(share);
			if(share.lpData!=0)
			{
				double *fV = (double *)share.lpData;
				if(abs(*fV-dValue)>ZERO_VALUE)		
				{
					WriteUpdateFlag(share);
					memcpy(share.lpData,&dValue,sizeof(double));			 
				}
			}
			else
			{
				WriteUpdateFlag(share);
				share.lpData   = (void*)((char *)m_pMapVoid+share.nStartAddr+sizeof(DWORD)+1);
				memcpy(share.lpData,&dValue,sizeof(double));	
			}
	}
	else 
		return false;
	 
	return true;
}

bool XSharedMemoryManage::WriteData_String(int nId, string strData)
{
	map<int ,__SharedM>::iterator it = m_mapShare.find(nId);
	if(it ==m_mapShare.end())
		return false;
	__SharedM&  share = it->second;
	return WriteDataToMemory(share.nStartAddr,(void *)strData.c_str(),strData.size()>share.nSize?share.nSize:strData.size());	
}
bool XSharedMemoryManage::WriteData_Buf(int nId,string strData)
{
	map<int ,__SharedM>::iterator it = m_mapShare.find(nId);
	if(it ==m_mapShare.end())
		return false;
	__SharedM&  share = it->second;
	WriteUpDateTime(share);
	WriteUpdateFlag(share);

	if(share.lpData!=0)
	{
		memcpy(share.lpData,strData.c_str(),strData.size());			 	 
	}
	else
	{		 
		share.lpData   = (void*)((char *)m_pMapVoid+share.nStartAddr+sizeof(DWORD)+1);
		memcpy(share.lpData,strData.c_str(),strData.size());	
	}

	return true;
}

bool XSharedMemoryManage::ReadData_Buf(int nId,string &sValue,int &bUpdateFlag,DWORD &dwUpdateTime,int& nReSize)	//用于读取原始缓存区数据
{
	map<int ,__SharedM>::iterator it = m_mapShare.find(nId);
	if(it ==m_mapShare.end())
		return false;

	__SharedM&  share = it->second;
	if(share.lpTickCount)
		dwUpdateTime = *share.lpTickCount;
	if(share.lpUpdate)
	{
		bUpdateFlag = *share.lpUpdate;
		*share.lpUpdate=0;
	}
	if(share.lpData)
	{
		sValue =string((char*)share.lpData,share.nSize);
		//memcpy(pValue,(char*)share.lpData,share.nSize);
		nReSize = share.nSize;
	}

	return true;
}

bool XSharedMemoryManage::ReadDataFromMemory(int nAddr,void * lpData,int nSize)
{
	 if(!m_pMapVoid)
		 return false;

	memcpy(lpData,(char*)m_pMapVoid+nAddr,nSize);
	return true;
}

bool XSharedMemoryManage::ReadData_ToString(int nId,string & sValue,int &nDatanType)
{
	int bUpdateFlag;
	DWORD dwUpdateTime;
	return ReadData_ToString(nId,sValue,nDatanType,bUpdateFlag,dwUpdateTime);
}
bool XSharedMemoryManage::ReadData_ToString(int nId,string & sValue,int &nDataType,int &bUpdateFlag,DWORD &dwUpdateTime)
{
	char lpbuf[256];
	memset(lpbuf,0,256);
	nDataType =-1;
	int nDataSize=0;
	if(!ReadData_Info(nId,lpbuf,nDataType,nDataSize,bUpdateFlag,dwUpdateTime))
		return false;

	if(nDataType ==DATA_TYPE_INT)
	{
		int  nValue =0;
		memcpy(&nValue,lpbuf,sizeof(int)); 
		sprintf(lpbuf,"%d",nValue);
		sValue = lpbuf;		 
	}
	else if(nDataType == DATA_TYPE_FLOAT)
	{
		float fValue;
		memcpy(&fValue,lpbuf,sizeof(fValue));
		sprintf(lpbuf,"%.2f",fValue);
		sValue = lpbuf;	
	}
	else if(nDataType ==DATA_TYPE_DOUBLE)
	{
		double dValue;
		memcpy(&dValue,lpbuf,sizeof(double));
		sprintf(lpbuf,"%.2f",dValue);
		sValue = lpbuf;			
	} 
	else 
	{
		 sValue= string(lpbuf,nDataSize);
	}
	 
	return true;

}
bool XSharedMemoryManage::ReadData_Info(int nId,char *lpBuf,int &nDataType,int  &nDataSize,int &bUpdateFlag,DWORD &dwUpdateTime)
{
	map<int ,__SharedM>::iterator it = m_mapShare.find(nId);
	if(it ==m_mapShare.end())
		return false;
	__SharedM&  share = it->second;
	nDataType = share.nDataType;
	nDataSize =share.nSize;
	bool bRe =false;
	if(share.nDataType ==DATA_TYPE_INT)
	{
		int nValue=0;
		bRe = ReadData_Int(nId,nValue,bUpdateFlag,dwUpdateTime);
		memcpy(lpBuf,&nValue,sizeof(int));
	}
	else if(share.nDataType == DATA_TYPE_FLOAT)
	{
		float fValue=0;
		bRe = ReadData_Float(nId,fValue,bUpdateFlag,dwUpdateTime);
		memcpy(lpBuf,&fValue,sizeof(float));
	}
	else if(share.nDataType == DATA_TYPE_DOUBLE)
	{
		double dValue=0;
		bRe = ReadData_double(nId,dValue,bUpdateFlag,dwUpdateTime);
		memcpy(lpBuf,&dValue,sizeof(double));
	}
	else if(share.nDataType == DATA_TYPE_STRING || share.nDataType ==DATA_TYPE_CHAR|| share.nDataType ==DATA_TYPE_STRING_UNICODE ||share.nDataType == DATA_TYPE_PROTOCL)
	{
		string sValue;
		//int nDataBufSize = 0;
		bRe = ReadData_String(nId,sValue,bUpdateFlag,dwUpdateTime);
		memcpy(lpBuf,sValue.c_str(),sValue.size());
		nDataSize = sValue.size();
	}

	 
	return bRe;

}

bool XSharedMemoryManage::ReadData_Attr(int nId,int &nDataType,int &nDataSize,int &nInOut)
{
	map<int ,__SharedM>::iterator it = m_mapShare.find(nId);
	if(it == m_mapShare.end())
		return false;

	__SharedM&  share = it->second;
	nDataType = share.nDataType;
	nDataSize =share.nSize;
	nInOut = share.nOutIn;

	return true;
}

bool XSharedMemoryManage::ReadData_Int(int nId,int &nValue)
{
	int bUpdateFlag;
	DWORD dwUpdateTime;
	return ReadData_Int(nId,nValue,bUpdateFlag,dwUpdateTime);
}

bool XSharedMemoryManage::ReadData_Int(int nId,int &nValue,int &bUpdateFlag,DWORD &dwUpdateTime)
{
	map<int ,__SharedM>::iterator it = m_mapShare.find(nId);
	if(it ==m_mapShare.end())
		return false;
	__SharedM&  share = it->second;
	if(share.lpTickCount)
		dwUpdateTime = *share.lpTickCount;
	if(share.lpUpdate)
	{
		bUpdateFlag = *share.lpUpdate;
		*share.lpUpdate=0;
	}
	if(share.lpData)
		memcpy(&nValue,share.lpData,sizeof(int));

	return true;
}

bool XSharedMemoryManage::ReadData_Float(int nId,float &fValue)
{
	int bUpdateFlag;
	DWORD dwUpdateTime;
	return ReadData_Float(nId,fValue,bUpdateFlag,dwUpdateTime);
}

bool XSharedMemoryManage::ReadData_Float(int nId,float &fValue,int &bUpdateFlag,DWORD &dwUpdateTime)
{
	map<int ,__SharedM>::iterator it = m_mapShare.find(nId);
	if(it ==m_mapShare.end())
		return false;
	__SharedM&  share = it->second;
	if(share.lpTickCount)
		dwUpdateTime = *share.lpTickCount;
	if(share.lpUpdate)
	{
		bUpdateFlag = *share.lpUpdate;
		*share.lpUpdate=0;
	}
	if(share.lpData)
		memcpy(&fValue,share.lpData,sizeof(float));

	return true;
	//return ReadDataFromMemory(it->second.nStartAddr,&fValue,it->second.nSize);	
}
bool XSharedMemoryManage::ReadData_double(int nId,double &fValue)
{
	int bUpdateFlag;
	DWORD dwUpdateTime;
	return ReadData_double(nId,fValue,bUpdateFlag,dwUpdateTime);
}
bool XSharedMemoryManage::ReadData_double(int nId,double &fValue,int &bUpdateFlag,DWORD &dwUpdateTime)
{
	map<int ,__SharedM>::iterator it = m_mapShare.find(nId);
	if(it ==m_mapShare.end())
		return false;
	__SharedM&  share = it->second;
	if(share.lpTickCount)
		dwUpdateTime = *share.lpTickCount;
	if(share.lpUpdate)
	{
		bUpdateFlag = *share.lpUpdate;
		*share.lpUpdate=0;
	}
	if(share.lpData)
		memcpy(&fValue,share.lpData,sizeof(double));

	return true;
}
bool XSharedMemoryManage::ReadData_String(int nId,string &sValue)
{
	int bUpdateFlag;
	DWORD dwUpdateTime;
	//int nDataSize = 0;
	return ReadData_String(nId,sValue,bUpdateFlag,dwUpdateTime);
}
bool XSharedMemoryManage::ReadData_String(int nId,string &sValue,int &bUpdateFlag,DWORD &dwUpdateTime)
{
	map<int ,__SharedM>::iterator it = m_mapShare.find(nId);
	if(it ==m_mapShare.end())
		return false;
	 
	__SharedM&  share = it->second;
	if(share.lpTickCount)
		dwUpdateTime = *share.lpTickCount;
	if(share.lpUpdate)
	{
		bUpdateFlag = *share.lpUpdate;
		*share.lpUpdate=0;
	}
	if(share.lpData)
	{
		sValue =string((char*)share.lpData,share.nSize);
		//nReSize = share.nSize;
	}
 
	return true;
}