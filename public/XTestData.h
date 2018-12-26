#pragma once
class XTestData
{
public:
	XTestData(void);
	~XTestData(void);
public:

	void Delete_dlg(int nDlgId);

	int InitMainInterface_CPA();


	//383a
	int InitInterface383A(int nW,int nH);
	int  InitInterface383A_Sub(int nParentId,int nLeft,int nTop,int nW,int nH,CString strTitle);


	//415
	int InitInterface415(int nW,int nH);
	int  InitInterface415_Sub(int nParentId,int nLeft,int nTop,int nW,int nH,CString strTitle);

	//604
	int InitInterface604(int nW,int nH);
	int  InitInterface604_Sub(int nParentId,int nLeft,int nTop,int nW,int nH,CString strTitle);
	//434
	int InitInterface434(int nW,int nH);
	int  InitInterface434_Sub(int nParentId,int nLeft,int nTop,int nW,int nH,CString strTitle);
	//382
	int InitInterface382(int nW,int nH);
	int  InitInterface382_Sub(int nParentId,int nLeft,int nTop,int nW,int nH,int nIndex);
	//
	int  InitInterface391(int nW ,int nH );
	int  InitInterface391_Sub(int nParentId,int nLeft,int nTop,int nW,int nH,int nIndex);
	//427
	int  InitInterface427(int nParentId,int nW ,int nH );
	int  InitInterface427_Sub(int nParentId,int nLeft, int nTop ,int nW ,int nH ,int nIndex);
	int  InitInterface427_2_Sub(int nParentId,int nLeft, int nTop ,int nW ,int nH ,int nIndex);

	//528
	int  InitInterface528(int nParentId,int nW ,int nH );
	int  InitInterface528_Sub(int nParentId,int nLeft, int nTop ,int nW ,int nH ,int nIndex);
	int  InitInterface528_2_Sub(int nParentId,int nLeft, int nTop ,int nW ,int nH ,int nIndex);

	//430
	int  InitInterface430(int nParentId,int nW ,int nH );
	int  InitInterface430_Sub(int nParentId,int nLeft, int nTop ,int nW ,int nH ,int nIndex);
	
	int  InitInterface433(int nW,int nH);
	int  InitInterface433_Sub(int nParentId,int nLeft, int nTop ,int nW ,int nH ,int nIndex);


public:
	int m_nSysId;
};

