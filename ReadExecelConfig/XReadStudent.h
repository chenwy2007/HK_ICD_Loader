// XReadStudent.h: interface for the XReadStudent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XREADSTUDENT_H__914CB079_126A_48C4_839F_E4772C133CCE__INCLUDED_)
#define AFX_XREADSTUDENT_H__914CB079_126A_48C4_839F_E4772C133CCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XReadStudent  
{
public:
	XReadStudent();
	virtual ~XReadStudent();
public:
	BOOL ReadStudentInfoFromFile(CString strFile,vector<struct_Student>&vec,CString & strClassName);
	BOOL ReadStudentInfo(  vector<struct_Student>&vec,COleSafeArray &sa);


private:
	 _Application objApp; 
    Workbooks objBooks;
    _Workbook objBook;
    Worksheets objSheets;

};

#endif // !defined(AFX_XREADSTUDENT_H__914CB079_126A_48C4_839F_E4772C133CCE__INCLUDED_)
