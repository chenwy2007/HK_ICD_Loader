// XReadStudent.cpp: implementation of the XReadStudent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFS3.h"
#include "XReadStudent.h"

#include "IFS3Dlg.h"
#include "Dlgmsg.h"

#include <COMUTIL.H>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XReadStudent::XReadStudent()
{

}

XReadStudent::~XReadStudent()
{
	
	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

    objBook.Close(COleVariant((short)FALSE),VOptional,VOptional);
    objApp.Quit();

}

BOOL XReadStudent::ReadStudentInfoFromFile(CString strFile,vector<struct_Student>&vec,CString & strClassName)
{
	 COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
 
    CString cstr=L"";
 
	//���������Ƿ�װ��Excel����
	if(!objApp.CreateDispatch("Excel.Application"))     
	{ 
 		
	//	g_dlg->m_dlgMsg.SetMsgInfo("�޷�����Excel������,��ȷ���Ƿ��Ѱ�װ",1);
	//	g_dlg->m_dlgMsg.DoModal();
		return false;     
	}     
  	
     objApp.SetVisible(false);  //��Excel����û����ʾ�κ����ݡ�	
	objBooks = objApp.GetWorkbooks();
    objBook = objBooks.Open(strFile,VOptional, _variant_t(true), VOptional, VOptional,  VOptional, 
	  VOptional, VOptional, VOptional,  VOptional, VOptional, VOptional, VOptional);
 
    objSheets = objBook.GetWorksheets();
	
	_Worksheet objSheet = objSheets.GetItem(COleVariant((short)1));
	
			
	Range objRange,usedRange;
	//����Ѿ�ʹ�õ�������������

	usedRange.AttachDispatch(objSheet.GetUsedRange());     
	objRange.AttachDispatch(usedRange.GetRows());  

	long   iRowNum=objRange.GetCount();//�Ѿ�ʹ�õ�����
	objRange.AttachDispatch(usedRange.GetColumns());
	long iColumnNum=objRange.GetCount();//�Ѿ�ʹ�õ�����
	if(iColumnNum<2  || iRowNum<5)
		return   false;

	//�༶����
	VARIANT  ret = objRange.GetValue( );
	COleSafeArray sa(ret);
	VARIANT val;
	long index[2];	
	index[0]=1;
	index[1]=2;
	sa.GetElement(index, &val);
	strClassName =(CString)val.bstrVal;
 
	return ReadStudentInfo(vec,sa);
	return true;
}
BOOL XReadStudent::ReadStudentInfo(vector<struct_Student>&vec,COleSafeArray &sa)
{

	vec.clear();
 
	VARIANT val;
	long index[2];
	CString str;
	struct_Student student;

	int nRow =5;
	long nId;	
	 
	for(int k =nRow;k<1000;k++)
	{
		memset(&student,0,sizeof(struct_Student));
		
		index[0]=k;
		index[1]=1;
		sa.GetElement(index, &val);
		str =(CString)val.bstrVal;
		if(str.GetLength()>30)
			str =str.Left(30);
		sprintf(student.cName,"%s",str);
		if(strlen(student.cName)==0)
			break;
		index[0]=k;
		index[1]=2;
		sa.GetElement(index, &val);
		if(val.vt ==VT_R8)
		{
		 	nId  =(long)val.dblVal;
			sprintf(student.cStudentId,"%d",nId);
		}
		else
		{
			str =(CString)val.bstrVal;
			if(str.GetLength()>15)
				str =str.Left(15);
			sprintf(student.cStudentId,"%s",str);
		} 	
		vec.push_back(student);
	}
	return true;
}