#pragma once

bool AnalyzeData_int(VARIANT& val,int &sValue);
bool AnalyzeData_string(VARIANT& val,string &sValue,int nPrecision = 0);
class Execel_Base
{
public:
	Execel_Base(void);
	~Execel_Base(void);

	//打开EXCEL文件，并获取分页的名称
	virtual BOOL OpenFile(CString sFilePath, vector<CString>&vec);

	BOOL OpenFile(CString sFilePath);

	void LoadSheetName(vector<CString>& vtSheetName);

	void close_excel();

	void ReadInfo_Sub_Item(int nSheetIndex, vector<vector<string>>&vec,int nStartRow =11,int nRowCount=-1,int nColCount =10,int nPrecision = 0);

	void ReadRowInfo(int nSheetIndex,vector<string>&vec,int nStartRow,int nColCount);

	void Read_One_RowInfo(int nSheetIndex,vector<string>&vec,int nRowNo,int nColNo,int nColCount);

public:
	_Application objApp; 
	Workbooks objBooks;
	_Workbook objBook;
	Worksheets objSheets;
	_Worksheet  objSheet;
	Range usedRange;
	Range objRange;
};

