#include "StdAfx.h"
#include "Execel_1553.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Execel_1553::Execel_1553(void)
{

}

Execel_1553::~Execel_1553(void)
{

}
void Execel_1553::ReadInfo_Item(int nSheetIndex, vector<vector<string>>&BaseInfovec,vector<vector<string>>&ItemInfovec)
{
	objSheet = objSheets.GetItem(COleVariant((short)nSheetIndex));

	//Range usedRange;
	//����Ѿ�ʹ�õ�������������

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  

	long   iRowNum=objRange.GetCount()+1;//�Ѿ�ʹ�õ�����
	objRange.AttachDispatch(usedRange.GetColumns());
	long iColumnNum=objRange.GetCount();//�Ѿ�ʹ�õ�����

	VARIANT ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	VARIANT val;
	long index[2];	
	string str;
	for(int k=1;k<10;k++)
	{
		vector<string>info;
		for(int i=1;i<=iColumnNum && i<=6;i++)
		{
			index[0]=k;
			index[1]=i;
			sa.GetElement(index, &val);
			AnalyzeData_string(val,str);
			info.push_back(str);
		}
		BaseInfovec.push_back(info);
	}

	for(int k=11;k<iRowNum;k++)
	{
		vector<string>info;
		for(int i=1;i<=iColumnNum && i<=5;i++)
		{
			index[0]=k;
			index[1]=i;
			sa.GetElement(index, &val);
			AnalyzeData_string(val,str);
			info.push_back(str);
		}
		ItemInfovec.push_back(info);
	}
}

void Execel_1553::ReadInfo_Item_SubInfo(int nSheetIndex,vector<string>&vec)
{
	objSheet = objSheets.GetItem(COleVariant((short)nSheetIndex));

	//Range usedRange;
	//����Ѿ�ʹ�õ�������������

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  
	 
	VARIANT  ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	VARIANT val;
	long index[2];

	string str;
	//���ݵ�λ
	index[0]=3;
	index[1]=2;
	sa.GetElement(index, &val);
	AnalyzeData_string(val,str);
	vec.push_back(str);

	//���ݳ���
	index[1]=4;
	sa.GetElement(index, &val);
	AnalyzeData_string(val,str);
	vec.push_back(str);

	//��������	��ֵ
	index[0]=1;
	index[1]=4;
	sa.GetElement(index, &val);
	AnalyzeData_string(val,str);
	vec.push_back(str);  
}

/*
bool Execel_1553::ReadInfo(int nSheetIndex, __protocol_1553 &Data)
{
	_Worksheet  objSheet = objSheets.GetItem(COleVariant((short)nSheetIndex));
	Range objRange,usedRange;
	//����Ѿ�ʹ�õ�������������

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  

	long   iRowNum=objRange.GetCount()+1;//�Ѿ�ʹ�õ�����
	objRange.AttachDispatch(usedRange.GetColumns());
	long iColumnNum=objRange.GetCount();//�Ѿ�ʹ�õ�����

	VARIANT ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	
	CString str;
	VARIANT val;
	long index[2];	

	//1553B��־
	index[0]=1;
	index[1]=1;
	sa.GetElement(index, &val);
	str = (CString)val.bstrVal;
	if(str!="1553B-��")
	{
		AfxMessageBox("û�з���Э���־:1553B-��");
		return false;
	}
	//����/����
	index[0]=1;
	index[1]=3;
	sa.GetElement(index, &val);
	Data.sRecSend = (CString)val.bstrVal;
	
	//�汾
	index[0]=1;
	index[1]=5;
	sa.GetElement(index, &val);
	Data.sVer = (CString)val.bstrVal;

	//����
	index[0]=2;
	index[1]=1;
	sa.GetElement(index, &val);
	Data.sName = (CString)val.bstrVal;

	//sAliasName
	index[0]=2;
	index[1]=3;
	sa.GetElement(index, &val);
	Data.sAliasName = (CString)val.bstrVal;

	//nRtAddr
	index[0]=3;
	index[1]=1;
	sa.GetElement(index, &val);
	AnalyzeData_int(val,Data.nRtAddr);
	
	index[1]=3;
	sa.GetElement(index, &val);
	AnalyzeData_int(val,Data.nSubRtAddr);
	
	//���ݵ�λ
	index[0]=4;
	index[1]=1;
	sa.GetElement(index, &val);
	Data.sUnitType =  (CString)val.bstrVal;
 
	//���ݳ���
	index[1]=3;
	sa.GetElement(index, &val);
	AnalyzeData_int(val,Data.nDataLen);
	
	//ˢ��ʱ��
	index[1]=5;
	sa.GetElement(index, &val);
	AnalyzeData_int(val,Data.nRefreshTimes);

	//�ж�
	index[0]=5;
	index[1]=1;
	sa.GetElement(index, &val);
	Data.sInterrupt =  (CString)val.bstrVal;
	
	//Դ
	index[1]=3;
	sa.GetElement(index, &val);
	Data.sSource =  (CString)val.bstrVal;

	//Ŀ��
	index[1]=5;
	sa.GetElement(index, &val);
	Data.sDest =  (CString)val.bstrVal;

	return true;
}*/
