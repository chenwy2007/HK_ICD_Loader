#pragma once
#include "afxwin.h"


// CDlgLoadIcdFile �Ի���

class CDlgLoadIcdFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLoadIcdFile)

public:
	CDlgLoadIcdFile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLoadIcdFile();

// �Ի�������
	enum { IDD = IDD_DLD_LOADICD };
	int GetCurProtocolType(void) {return m_nCurProtocolType;}
	int GetCurComChannelNo(void) {return m_nCurComChannelNo;}
	CString GetIdcFilePath(void) {return m_sIdcFilePath;}
	vector<__SYS_Drive> getCurDriveInfor(void) {return m_DriveVec;}
	int GetCurDriveNo(void) {return m_nCurDriveIdx;}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void InitialDriveInfor(int nCurProtocolType);
	void InitialRadio(int& nCurProtocolType);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	vector<__SYS_Drive> m_DriveVec;

	int m_nCurProtocolType;
	CEdit m_edit_comchannelno;
	int m_nCurComChannelNo;
	//CEdit m_edit_drivetype;
	CEdit m_edit_idcfile;
	CString m_sIdcFilePath;

	int m_nCurDriveIdx;

public:
	afx_msg void OnBnClickedRadio1553b();
	afx_msg void OnBnClickedRadioScom();
	afx_msg void OnBnClickedRadioArinc429();
	afx_msg void OnBnClickedRadioCan();
	afx_msg void OnBnClickedRadioDiscrete();
	afx_msg LRESULT OnDispInfor(WPARAM wParam, LPARAM lParam);
private:
	CComboBox m_comboDrive;
public:
	afx_msg void OnBnClickedBtnSelectfile();
	virtual void OnOK();
	afx_msg void OnCbnSelchangeComboDrivetype();
private:
	CStatic m_static_infor;
};
