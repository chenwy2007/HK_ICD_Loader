#pragma once


// COwnerDrawTabCtrl
/*
����
TABҳ:�߶ȣ������С����ѡ�б���ɫ��ѡ�б���ɫ

������ɫ��������ɫ

*/
class COwnerDrawTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(COwnerDrawTabCtrl)

public:
	COwnerDrawTabCtrl();
	virtual ~COwnerDrawTabCtrl();

public:
	void SetItemTabSize(int nW,int nH);
	void SetFontInfo(int nSize,COLORREF colorFont=RGB(255,255,255),CString sFontName="����");
	void SetItemTabColor(COLORREF  selectTabColor,COLORREF  unSelectTabColor);
public:
	void DrawInfo(CDC * pDC ,CRect rt);
	virtual void DrawInfoEx(CDC *pDC,CRect rt){};

public:

	//����ɫ ������ɫ����ǩ��ȣ���ǩ�߶ȡ�������ɫ�����塢�ֺš���ѡ�б�ǩ��ɫ��δѡ�б�ǩ��ɫ

	COLORREF  m_selectTabColor;
	
	COLORREF  m_unSelectTabColor;

	COLORREF  m_LineColor;

	COLORREF  m_backColor;

	COLORREF m_colorText;

	int m_nFontSize;

	string m_sFontName;

	int m_nTabWidth;

	int m_nTabHight;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	 
	 
};


