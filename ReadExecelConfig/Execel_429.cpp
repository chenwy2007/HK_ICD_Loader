#include "StdAfx.h"
#include "Execel_429.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool AnalyzeData_string(VARIANT& val,string &sValue,int nPrecision)
{	
	char lpbuf[64];
	bool b =true;
	CString sFormat;
	sFormat.Format("%%.%df",nPrecision);
	sValue = "";
	if(val.vt == VT_R8)
	{
		sprintf(lpbuf,sFormat,val.dblVal);
		sValue = lpbuf;
	}
	else if(val.vt ==  VT_I4)
	{
		sprintf(lpbuf,"%d",val.intVal);
		sValue = lpbuf;
	}
	else if(val.vt ==VT_BSTR)
	{
		CString csValue = (CString)val.bstrVal;
		sValue=csValue.GetBuffer();
	}
	else if(val.vt == VT_DATE)
	{
		sValue="";
	}
	else 
		b =false;
	return b;
}
Execel_429::Execel_429(void)
{
}


Execel_429::~Execel_429(void)
{
	//COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	//objBook.Close(COleVariant((short)FALSE),VOptional,VOptional);
	//objApp.Quit();
}
 
void Execel_429::ReadInfo(int nSheetIndex, __protocol_429 &Data)
{
	objSheet = objSheets.GetItem(COleVariant((short)nSheetIndex));

	//Range usedRange;
	//����Ѿ�ʹ�õ�������������

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  


	long   iRowNum=objRange.GetCount()+1;//�Ѿ�ʹ�õ�����
	objRange.AttachDispatch(usedRange.GetColumns());
	long iColumnNum=objRange.GetCount();//�Ѿ�ʹ�õ�����


	VARIANT  ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	VARIANT val;
	long index[2];	
	CString str;
 		
 
	////����
	//index[0]=1;
	//index[1]=2;
 //	sa.GetElement(index, &val);
	//Data.sName =(CString)val.bstrVal;
	//
	////�汾	 
	//index[1]=4;
	//sa.GetElement(index, &val);
	//Data.sVer =(CString)val.bstrVal;
	//
	////���ݵ�λ 
	//index[1]=6;
	//sa.GetElement(index, &val);
	//Data.sUnitType =(CString)val.bstrVal;

	////ͨ��	 
	//index[0]=2;	
	//index[1]=2;
	//sa.GetElement(index, &val);
	//Data.nChannel =val.intVal;

	////����/����
	//index[1]=4;
	//sa.GetElement(index, &val);
	//Data.nRecSend =val.intVal;
	//
	////���ͻ���
	//index[1]=6;
	//sa.GetElement(index, &val);
	//AnalyzeData_int
	//Data.nSendMode =val.intVal;

	// index[0]=3;	
	////��������(Kbps)	
	//index[1]=2;
	//sa.GetElement(index, &val);
	//Data.nSendSpeed =val.intVal;

	////ʱ����(ms)
	//index[1]=4;
	//sa.GetElement(index, &val);
	//Data.nTimes =val.intVal;

	////У��λ
	//index[1]=6;
	//sa.GetElement(index, &val);
	//Data.nParity =val.intVal;

	////�����Э�����11�п�ʼ
	//__protocol_Item item;
	//string nSim;	
	//int nCol;
	//for(int k =11;k<iRowNum;k++)
	//{
	//	nSim="";
	//	nCol=1;
	//	index[0]=k;	
	//	index[1]=nCol++;
	//	sa.GetElement(index, &val);



	//}
	//for(int k =1;k<iRowNum;k++)
	//{
	//	__ConfigInfo info;

	//	index[0]=k;
	//	index[1]=1;
	//	sa.GetElement(index, &val);
	//	str =(CString)val.bstrVal;
	//	if(str=="")
	//		continue;
	//	info.sInfo[0] =str;

	//	index[0]=k;
	//	index[1]=2;
	//	sa.GetElement(index, &val);
	//	str =(CString)val.bstrVal;
	//	info.sInfo[1] =str;
	//	vec.push_back(info); 

	//}
}
void Execel_429::ReadInfo(int nSheetIndex, vector<vector<string>>&vec)
{
	
	objSheet = objSheets.GetItem(COleVariant((short)nSheetIndex));

	//Range usedRange;
	//����Ѿ�ʹ�õ�������������

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  


	long   iRowNum=objRange.GetCount()+1;//�Ѿ�ʹ�õ�����
	objRange.AttachDispatch(usedRange.GetColumns());
	long iColumnNum=objRange.GetCount();//�Ѿ�ʹ�õ�����


	VARIANT  ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	VARIANT val;
	long index[2];	
	string str;
	for(int k=1;k<iRowNum;k++)
	{
		vector<string>info;
		for(int i=1;i<iColumnNum && i<11;i++)
		{
			index[0]=k;
			index[1]=i;
			sa.GetElement(index, &val);
			AnalyzeData_string(val,str);
			info.push_back(str);
		}
		vec.push_back(info);
	}


}