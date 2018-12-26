#include "StdAfx.h"
#include "XReadConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

XReadConfig::XReadConfig(void)
{

}


XReadConfig::~XReadConfig(void)
{

	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	objBook.Close(COleVariant((short)FALSE),VOptional,VOptional);
	objApp.Quit();
}

BOOL XReadConfig::OpenFile(CString sFilePath,vector<CString>&vec)
{
	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	CString cstr="";

	//检测电脑上是否安装了Excel程序。
	if(!objApp.CreateDispatch("Excel.Application"))     
	{ 

		//	g_dlg->m_dlgMsg.SetMsgInfo("无法启动Excel服务器,请确认是否已安装",1);
		//	g_dlg->m_dlgMsg.DoModal();
		return false;     
	}     

	objApp.SetVisible(false);  //打开Excel程序，没有显示任何数据。	
	objBooks = objApp.GetWorkbooks();
	objBook = objBooks.Open(sFilePath,VOptional, _variant_t(true), VOptional, VOptional,  VOptional, 
		VOptional, VOptional, VOptional,  VOptional, VOptional, VOptional, VOptional);

	objSheets = objBook.GetWorksheets();

	short nIndex =1;
	while(1)
	{
		try
		{	_Worksheet objSheet = objSheets.GetItem(COleVariant(nIndex++));
			if(objSheet)
			{
				CString strName = objSheet.GetName();
				vec.push_back(strName);
				if(strName.IsEmpty())
					break;
			}
			else
				break;
		}
		catch (...)
		{
			break;
		}

	}


	return true;

}	
void XReadConfig::ReadInfo_2(short nSheetIndex,vector<__ConfigInfo> &vec)
{
	_Worksheet  objSheet = objSheets.GetItem(COleVariant((short)nSheetIndex));
	 
	Range objRange,usedRange;
	//获得已经使用的行数和列数。

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  


	long   iRowNum=objRange.GetCount()+1;//已经使用的行数
	objRange.AttachDispatch(usedRange.GetColumns());
	long iColumnNum=objRange.GetCount();//已经使用的列数


	VARIANT  ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	VARIANT val;
	long index[2];	
	CString str;
	bool bFind= false;
 
	CString sLastStr;
	for(int k =1;k<iRowNum;k++)
	{
		__ConfigInfo info;

		index[0]=k;
		index[1]=1;
		sa.GetElement(index, &val);
		str =(CString)val.bstrVal;
		if(str=="")
			 continue;
		info.sInfo[0] =str;
		 
		index[0]=k;
		index[1]=2;
		sa.GetElement(index, &val);
		str =(CString)val.bstrVal;
		info.sInfo[1] =str;
		 vec.push_back(info); 

	}


}

void XReadConfig::ReadInfo(short nSheetIndex,vector<__ConfigInfo> &vec,int nUnitNum)
{
 
	_Worksheet  objSheet = objSheets.GetItem(COleVariant((short)nSheetIndex));
	int nCol =4; //第一单位

	Read(objSheet,nCol,vec,nUnitNum);
	 
	//nCol = 16;
	//Read(objSheet,nCol,vec);

}
void XReadConfig::Read(_Worksheet&  objSheet,int nCol,vector<__ConfigInfo> &vec,int nUnitNum )
{

	Range objRange,usedRange;
	//获得已经使用的行数和列数。

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  


	long   iRowNum=objRange.GetCount()+1;//已经使用的行数
	objRange.AttachDispatch(usedRange.GetColumns());
	long iColumnNum=objRange.GetCount();//已经使用的列数
	 

	VARIANT  ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	VARIANT val;
	long index[2];	
	CString str;
	bool bFind= false;
	bool bData;

	CString sLastStr;
	 for(int k =1;k<iRowNum;k++)
	 {
		 index[0]=k;
		 index[1]=nCol;
		sa.GetElement(index, &val);

		str =(CString)val.bstrVal;

		if(bFind)
		{
			if(str=="")
				continue;

			__ConfigInfo info;
			 	
			bData = true;
			for(int i =0;i<nUnitNum+3;i++)
			{ 
				index[0]=k;
				index[1]=i+nCol;
				sa.GetElement(index, &val);
				info.sInfo[i] = (CString)val.bstrVal;	
				if(i<nUnitNum)
				{ 
					if(info.sInfo[i].Find("CH") ==-1)
					{
						bData =FALSE;
						break;
					}
					
				}
				else if(i == nUnitNum)
				{
					if(info.sInfo[i]=="")
						info.sInfo[i] = sLastStr;
					else
						sLastStr = info.sInfo[i];

				} 

			}
			if(bData)
				vec.push_back(info);

		}
		else if(str =="工控机一单元")
			bFind =true;
	 }
 





	

}