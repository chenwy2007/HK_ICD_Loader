
// ReadExecelConfig.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#define GRID_SPARE 0

// CReadExecelConfigApp:
// �йش����ʵ�֣������ ReadExecelConfig.cpp
//

class CReadExecelConfigApp : public CWinApp
{
public:
	CReadExecelConfigApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CReadExecelConfigApp theApp;