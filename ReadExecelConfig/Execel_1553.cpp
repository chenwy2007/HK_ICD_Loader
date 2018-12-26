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
	//获得已经使用的行数和列数。

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  

	long   iRowNum=objRange.GetCount()+1;//已经使用的行数
	objRange.AttachDispatch(usedRange.GetColumns());
	long iColumnNum=objRange.GetCount();//已经使用的列数

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
	//获得已经使用的行数和列数。

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  
	 
	VARIANT  ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	VARIANT val;
	long index[2];

	string str;
	//数据单位
	index[0]=3;
	index[1]=2;
	sa.GetElement(index, &val);
	AnalyzeData_string(val,str);
	vec.push_back(str);

	//数据长度
	index[1]=4;
	sa.GetElement(index, &val);
	AnalyzeData_string(val,str);
	vec.push_back(str);

	//内容类型	数值
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
	//获得已经使用的行数和列数。

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  

	long   iRowNum=objRange.GetCount()+1;//已经使用的行数
	objRange.AttachDispatch(usedRange.GetColumns());
	long iColumnNum=objRange.GetCount();//已经使用的列数

	VARIANT ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	
	CString str;
	VARIANT val;
	long index[2];	

	//1553B标志
	index[0]=1;
	index[1]=1;
	sa.GetElement(index, &val);
	str = (CString)val.bstrVal;
	if(str!="1553B-主")
	{
		AfxMessageBox("没有发现协议标志:1553B-主");
		return false;
	}
	//发送/接受
	index[0]=1;
	index[1]=3;
	sa.GetElement(index, &val);
	Data.sRecSend = (CString)val.bstrVal;
	
	//版本
	index[0]=1;
	index[1]=5;
	sa.GetElement(index, &val);
	Data.sVer = (CString)val.bstrVal;

	//名称
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
	
	//数据单位
	index[0]=4;
	index[1]=1;
	sa.GetElement(index, &val);
	Data.sUnitType =  (CString)val.bstrVal;
 
	//数据长度
	index[1]=3;
	sa.GetElement(index, &val);
	AnalyzeData_int(val,Data.nDataLen);
	
	//刷新时间
	index[1]=5;
	sa.GetElement(index, &val);
	AnalyzeData_int(val,Data.nRefreshTimes);

	//中断
	index[0]=5;
	index[1]=1;
	sa.GetElement(index, &val);
	Data.sInterrupt =  (CString)val.bstrVal;
	
	//源
	index[1]=3;
	sa.GetElement(index, &val);
	Data.sSource =  (CString)val.bstrVal;

	//目标
	index[1]=5;
	sa.GetElement(index, &val);
	Data.sDest =  (CString)val.bstrVal;

	return true;
}*/
