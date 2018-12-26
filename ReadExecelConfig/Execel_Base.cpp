#include "StdAfx.h"
#include "Execel_Base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool AnalyzeData_int(VARIANT& val,int &nValue)
{
 
	bool b =true;
	if(val.vt == VT_R8)	
		nValue = (int)val.dblVal;
	else if(val.vt ==  VT_I4)
			nValue = val.intVal;
	else
		return false;	
	return true;
}

Execel_Base::Execel_Base(void)
{

}

Execel_Base::~Execel_Base(void)
{
	close_excel();
	//if(objApp.m_lpDispatch)
	//{
	//	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	//	objBook.Close(COleVariant((short)FALSE),VOptional,VOptional);
	//	objApp.Quit();
	//	objApp.ReleaseDispatch();
	//}
}

BOOL Execel_Base::OpenFile(CString sFilePath,vector<CString>&vec)
{
	CFileFind filefind; 
	if( !filefind.FindFile(sFilePath) ) 
	{ 
		AfxMessageBox(_T("文件不存在"));
		return false;
	}

	if(objApp.m_lpDispatch)
	{
		COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

		objBook.Close(COleVariant((short)FALSE),VOptional,VOptional);
		objApp.Quit();
		objApp.ReleaseDispatch();
	}

	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	CString cstr="";

	//检测电脑上是否安装了Excel程序。
	if(!objApp.CreateDispatch("Excel.Application"))     
	{  
		return false;     
	}     

	objApp.SetVisible(false);  
	objBooks = objApp.GetWorkbooks();
	LPDISPATCH lpDisp; //接口指针

	lpDisp = objBooks.Open(sFilePath,VOptional, _variant_t(true), VOptional, VOptional,  VOptional, 
		VOptional, VOptional, VOptional,  VOptional, VOptional, VOptional, VOptional);
	objBook.AttachDispatch(lpDisp);

	objSheets = objBook.GetWorksheets();

	short nIndex =1;
	while(1)
	{
		try
		{	
			objSheet = objSheets.GetItem(COleVariant(nIndex++));
			if(objSheet)
			{
				CString strName = objSheet.GetName();
				vec.push_back(strName);
				//objSheet.ReleaseDispatch();
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

	objSheet = objSheets.GetItem(COleVariant((short)1));

	return true;
}

BOOL Execel_Base::OpenFile(CString sFilePath)
{
	CFileFind filefind; 
	if( !filefind.FindFile(sFilePath) ) 
	{ 
		//AfxMessageBox(_T("文件不存在"));
		return false;
	}

	//close_excel();

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	if (!objApp.CreateDispatch(_T("Excel.Application")))
	{
		//AfxMessageBox("无法创建Excel应用！");
		return FALSE;
	}

	objApp.SetVisible(FALSE);
	//objApp.SetUserControl(TRUE);

	//objBooks = objApp.GetWorkbooks();
	objBooks.AttachDispatch(objApp.GetWorkbooks());
	LPDISPATCH lpDisp = objBooks.Open(sFilePath, covOptional,  _variant_t(true), 
		covOptional, covOptional, covOptional, covOptional, 
		covOptional, covOptional, covOptional, covOptional, 
		covOptional, covOptional);
	objBook.AttachDispatch(lpDisp);

	return true;
}

void Execel_Base::LoadSheetName(vector<CString>& vtSheetName)
{
	//objSheets = objBook.GetWorksheets();
	objSheets.AttachDispatch(objBook.GetWorksheets());
	long lSheetNum = objSheets.GetCount()+1;

	short nIndex =1;
	while(nIndex < lSheetNum)
	{
		try
		{	
			objSheet.AttachDispatch(objSheets.GetItem(COleVariant(nIndex++)));
			if(objSheet.m_lpDispatch)
			{
				CString strName = objSheet.GetName();
				//objSheet.ReleaseDispatch();

				if(strName.IsEmpty())
					break;

				vtSheetName.push_back(strName);
			}
			else
				break;
		}
		catch (...)
		{
			break;
		}
	}
}

void Execel_Base::close_excel()
{
	if(objApp.m_lpDispatch)
	{
		COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		objBook.Close(COleVariant((short)FALSE),VOptional,VOptional);
		//释放对象（相当重要！）   
		objRange.ReleaseDispatch();
		usedRange.ReleaseDispatch();
		objSheet.ReleaseDispatch();
		objSheets.ReleaseDispatch();
		objBook.ReleaseDispatch();
		objBooks.ReleaseDispatch();
		//退出程序   
		objApp.Quit();
		//m_ExlApp一定要释放，否则程序结束后还会有一个Excel进程驻留在内存中，而且程序重复运行的时候会出错   
		objApp.ReleaseDispatch();
	}
}

void Execel_Base::ReadInfo_Sub_Item(int nSheetIndex, vector<vector<string>>&vec,int nStartRow,int nRowCout,int nColCount,int nPrecision)
{
	objSheet = objSheets.GetItem(COleVariant((short)nSheetIndex));

	//获得已经使用的行数和列数。

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  

	long iRowNum=objRange.GetCount()+1;//已经使用的行数
	objRange.AttachDispatch(usedRange.GetColumns());
	long iColumnNum=objRange.GetCount();//已经使用的列数

	VARIANT  ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	VARIANT val;
	long index[2];	
	string str;
	for(int k=nStartRow;k<iRowNum;k++)
	{
		vector<string>info;
		for(int i=1;i<=iColumnNum && i<=nColCount;i++)
		{
			index[0]=k;
			index[1]=i;
			sa.GetElement(index, &val);
			AnalyzeData_string(val,str,nPrecision);
			info.push_back(str);
		}
		vec.push_back(info);
		if(nRowCout!=-1 && vec.size()==nRowCout)
			break;
	}
}

void Execel_Base::ReadRowInfo(int nSheetIndex,vector<string>&vec,int nStartRow,int nColCount)
{
	objSheet = objSheets.GetItem(COleVariant((short)nSheetIndex));

	//Range usedRange;
	//获得已经使用的行数和列数。

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  

	long iRowNum=objRange.GetCount()+1;//已经使用的行数
	objRange.AttachDispatch(usedRange.GetColumns());
	long iColumnNum=objRange.GetCount();//已经使用的列数

	VARIANT  ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	VARIANT val;
	long index[2];	
	string str;
	 
 	for(int i=1;i<=iColumnNum && i<=nColCount;i++)
	{
		index[0]=nStartRow;
		index[1]=i;
		sa.GetElement(index, &val);
		AnalyzeData_string(val,str);
		vec.push_back(str);
	}
}
void Execel_Base::Read_One_RowInfo(int nSheetIndex,vector<string>&vec,int nRowNo,int nColNo,int nColCount)
{
	objSheet = objSheets.GetItem(COleVariant((short)nSheetIndex));

	//Range usedRange;
	//获得已经使用的行数和列数。

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  

	long iRowNum=objRange.GetCount()+1;//已经使用的行数
	objRange.AttachDispatch(usedRange.GetColumns());
	long iColumnNum=objRange.GetCount();//已经使用的列数

	VARIANT ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	VARIANT val;
	long index[2];	
	string str;

	for(int i=nColNo;i<=iColumnNum && vec.size()<nColCount;i++)
	{
		index[0]=nRowNo;
		index[1]=i;
		sa.GetElement(index, &val);
		AnalyzeData_string(val,str);
		vec.push_back(str);
	}
}