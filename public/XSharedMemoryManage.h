#pragma once

//ͨ���豸����״̬
struct  __DriveRunState
{
	u16 nDriveId;//ͨ���豸id
	u16 nState;	//�豸����״̬��0:δ���ӣ�1�����ӳɹ���2������ʧ��
};

//�����ڴ�ֵ����ϸ����
struct __SysInfoMemory
{
	int nOk; //0��׼���У�1��OK
	int nShareDCount;//һ�����ٸ������ڴ�
	int nReCreate ;//0����ʾ�ؽ�,���ݴ�����������¹�������ʱ��д�룬1����λ����д�룬��ʾ��λ�����Ѿ�ͬ��������
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
	u8 nOutIn; //1�����룬2�����
	u16 nSize;	
	u8 cReserve;
	DWORD * lpTickCount;
	u8 * lpUpdate;
	void * lpData;
};

/*�����ڴ�Ĵ��˳��
1.64 byte��ϵͳ��Ϣ  
2.���еĹ����ڴ�Ĵ洢��Ϣ��vector<__SharedM>��
3.�������Ϣ��ʱ��DWORD(4 BYTE)���Ƿ����(1BYTE),�������(1byte),������Ϣ,��
*/

class XSharedMemoryManage
{
public:
	//XSharedMemoryManage(string sShareName,int nMemorySize);
	XSharedMemoryManage();
	~XSharedMemoryManage(void);
	static XSharedMemoryManage &GetInstance();

public:

	//���������ڴ�
	bool CreateShareMemory(string sShareName="HK_Shared_Memory",int nShareMemorySize=5);
	bool OpenShareMemory(string sShareName="HK_Shared_Memory");
	
	//�жϴ��Ƿ�ɹ�
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
	bool WriteData_Buf(int nId,string strData); //���ڴ��ԭʼЭ������

	bool ReadData_Buf(int nId,string &sValue,int &bUpdateFlag,DWORD &dwUpdateTime,int& nReSize);	//���ڶ�ȡԭʼ����������
		
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
	
	int m_nRectify;//0����У����1��У����
protected:	
	//�����ڴ������ ���ڴ��С(��λ:M)
	string m_sShareName;
	int m_nMemorySize;

	HANDLE m_hMapFile;
	void *  m_pMapVoid;

	int m_nAllUseShareSize;
};

#define  g_XShared  XSharedMemoryManage::GetInstance()

extern XSharedMemoryManage *g_pShared;