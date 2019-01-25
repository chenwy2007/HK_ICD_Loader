#pragma once

//通信设备运行状态
struct  __DriveRunState
{
	u16 nDriveId;//通信设备id
	u16 nState;	//设备运行状态，0:未连接，1：连接成功，2：连接失败
};

//共享内存值，详细内容
struct __SysInfoMemory
{
	int nOk; //0：准备中，1：OK
	int nShareDCount;//一共多少个共享内存
	int nReCreate ;//0：表示重建,数据处理程序在重新构建数据时，写入，1：上位程序写入，表示上位程序，已经同步了数据
	int nDutId[5];
	__DriveRunState  state[16]; 
};

struct __SharedM
{
	__SharedM()
	{
		lpTickCount=NULL;
		lpUpdate =NULL;
		lpData=NULL;
	};
	int nId;	
	int nStartAddr;
	u8 nDataType;
	u8 nOutIn; //1：输入，2：输出
	u16 nSize;	
	u8 cReserve;
	DWORD * lpTickCount;
	u8 * lpUpdate;
	void * lpData;
};

/*共享内存的存放顺序：
1.64 byte，系统信息  
2.所有的共享内存的存储信息（vector<__SharedM>）
3.具体的信息（时间DWORD(4 BYTE)，是否更新(1BYTE),输入输出(1byte),具体信息,）
*/

class XSharedMemoryManage
{
public:
	//XSharedMemoryManage(string sShareName,int nMemorySize);
	XSharedMemoryManage();
	~XSharedMemoryManage(void);
	static XSharedMemoryManage &GetInstance();

public:

	//创建共享内存
	bool CreateShareMemory(string sShareName="HK_Shared_Memory",int nShareMemorySize=5);
	bool OpenShareMemory(string sShareName="HK_Shared_Memory");
	
	//判断打开是否成功
	bool IsOpenShareMemory();	 
	void QueryConfigData();

	bool ReadSysInfo();
	void ClearShareMemory();
	void InitShareMemory(int nDlgId,vector<__DATA_CONFIG>&vec);

	void GetSysInfo(char * lpbuf);
	void WriteSysInfo(char * lpData,int nLen);

	bool WriteDataToMemory(int nAddr ,void * lpData,int nSize);
	
	bool WriteUpDateTime(__SharedM & share);
	bool WriteUpdateFlag(__SharedM & share);

	bool WriteData_Int(int nId,int  nValue);
	bool WriteData_Float(int nId, float fValue);
	bool WriteData_Double(int nId, double dValue);
	
	bool WriteData_String(int nId, string strData);
	bool WriteData_Buf(int nId,string strData); //用于存放原始协议数据

	bool ReadData_Buf(int nId,string &sValue,int &bUpdateFlag,DWORD &dwUpdateTime,int& nReSize);	//用于读取原始缓存区数据
		
	bool ReadDataFromMemory(int nAddr,void * lpData,int nSize);
	
	bool ReadData_ToString(int nId,string & sValue,int &nDatanType);
	bool ReadData_ToString(int nId,string & sValue,int &nDatanType,int &bUpdateFlag,DWORD &dwUpdateTime);
	
	bool ReadData_Info(int nId,char *lpBuf,int &nDataType,int  &nDAtaSize,int &bUpdateFlag,DWORD &dwUpdateTime);
	bool ReadData_Attr(int nId,int &nDataType,int &nDataSize,int &nInOut);
	
	bool ReadData_Int(int nId,int &nValue);
	bool ReadData_Int(int nId,int &nValue,int &bUpdateFlag,DWORD &dwUpdateTime);

	bool ReadData_Float(int nId,float &fValue);
	bool ReadData_Float(int nId,float &fValue,int &bUpdateFlag,DWORD &dwUpdateTime);
	
	bool ReadData_double(int nId,double &fValue);
	bool ReadData_double(int nId,double &fValue,int &bUpdateFlag,DWORD &dwUpdateTime);
	
	bool ReadData_String(int nId,string & sValue);
	bool ReadData_String(int nId,string &sValue,int &bUpdateFlag,DWORD &dwUpdateTime);

//	bool ReadData_char(int nId, u8  &uValue);

public:
	__SysInfoMemory  m_sysInfo;
	map<int ,__SharedM>m_mapShare; 
	
	int m_nRectify;//0：不校定，1：校定中
protected:	
	//共享内存的名称 、内存大小(单位:M)
	string m_sShareName;
	int m_nMemorySize;

	HANDLE m_hMapFile;
	void *  m_pMapVoid;

	int m_nAllUseShareSize;
};

#define  g_XShared  XSharedMemoryManage::GetInstance()

extern XSharedMemoryManage *g_pShared;