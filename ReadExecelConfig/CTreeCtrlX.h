#pragma once

class CTreeCtrlX : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlX)
public:
	CTreeCtrlX();
	virtual ~CTreeCtrlX();

	//添 设置字体、颜色、粗体的函数
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

	//添 WM_PAINT 信息监控，重载OnPaint()函数来实现绘制

protected:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

protected:
	//首先定义一个字体、颜色的结构体：
	struct Color_Font
	{
		COLORREF color;
		LOGFONT  logfont;
	};

	//使用CMap来保存各个Item对应的字体、颜色信息
	CMap< void*, void*, Color_Font, Color_Font& > m_mapColorFont ;

};
