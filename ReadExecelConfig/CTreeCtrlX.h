#pragma once

class CTreeCtrlX : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlX)
public:
	CTreeCtrlX();
	virtual ~CTreeCtrlX();

	//�� �������塢��ɫ������ĺ���
	void CTreeCtrlX::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
	{
		Color_Font cf;
		if( !m_mapColorFont.Lookup( hItem, cf ) )
			cf.color = (COLORREF)-1;
		cf.logfont = logfont;
		m_mapColorFont[hItem] = cf;
	}

	void CTreeCtrlX::SetItemBold(HTREEITEM hItem, BOOL bBold)
	{
		SetItemState( hItem, bBold ? TVIS_BOLD: 0, TVIS_BOLD );
	}

	void CTreeCtrlX::SetItemColor(HTREEITEM hItem, COLORREF color)
	{
		Color_Font cf;
		if( !m_mapColorFont.Lookup( hItem, cf ) )
			cf.logfont.lfFaceName[0] = '\0';
		cf.color = color;
		m_mapColorFont[hItem] = cf;
	}

	BOOL CTreeCtrlX::GetItemFont(HTREEITEM hItem, LOGFONT * plogfont)
	{
		Color_Font cf;
		if( !m_mapColorFont.Lookup( hItem, cf ) )
			return FALSE;
		if( cf.logfont.lfFaceName[0] == '\0' ) 
			return FALSE;
		*plogfont = cf.logfont;
		return TRUE;
	}

	BOOL CTreeCtrlX::GetItemBold(HTREEITEM hItem)
	{
		return GetItemState( hItem, TVIS_BOLD ) & TVIS_BOLD;
	}

	COLORREF CTreeCtrlX::GetItemColor(HTREEITEM hItem)
	{
		// Returns (COLORREF)-1 if color was not set
		Color_Font cf;
		if( !m_mapColorFont.Lookup( hItem, cf ) )
			return (COLORREF)-1;
		return cf.color;
	}

	//�� WM_PAINT ��Ϣ��أ�����OnPaint()������ʵ�ֻ���

protected:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

protected:
	//���ȶ���һ�����塢��ɫ�Ľṹ�壺
	struct Color_Font
	{
		COLORREF color;
		LOGFONT  logfont;
	};

	//ʹ��CMap���������Item��Ӧ�����塢��ɫ��Ϣ
	CMap< void*, void*, Color_Font, Color_Font& > m_mapColorFont ;

};
