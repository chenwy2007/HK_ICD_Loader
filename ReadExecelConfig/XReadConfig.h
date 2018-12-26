#pragma once

struct __ConfigInfo
{
	CString sInfo[8]; 

};
class XReadConfig
{
public:
	XReadConfig(void);
	~XReadConfig(void);

public:
	BOOL OpenFile(CString sFilePath,vector<CString>&vec);

	void ReadInfo_2(short nSheetIndex,vector<__ConfigInfo> &vec);

	void ReadInfo(short nSheetIndex,vector<__ConfigInfo> &vec,int nUnitNum =5);

	void Read(_Worksheet&  objSheet,int nCol,vector<__ConfigInfo> &vec,int nUnitNum );
public:
	_Application objApp; 
	Workbooks objBooks;
	_Workbook objBook;
	Worksheets objSheets;
};

