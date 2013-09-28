#pragma once


// CImageButton

class CImageButton : public CButton
{
	DECLARE_DYNAMIC(CImageButton)

public:
	CImageButton();
	virtual ~CImageButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void	OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void	DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void	PreSubclassWindow();
public:
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH	CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void	OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnPaint();
	afx_msg void	OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void	 OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void	OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);



	void		 freeResource();
	bool		 setBitmaps(const wchar_t *strBitmapNormal,const wchar_t *strBitmapHot,const wchar_t *strBitmapClicked);
	bool		 setBitmaps(HBITMAP hBitmapNormal,HBITMAP hBitmapHot,HBITMAP hBitmapClick);

	bool		 setCurrentBitmap(HBITMAP hCurrentBitmap);

	void		 setPos(int x,int y);

	WORD		getWidth();
	WORD		getHeight();

private:
	HBITMAP		 m_hBitmapNormal;
	HBITMAP		 m_hBitmapHot;
	HBITMAP		 m_hBitmapClick;

	HBITMAP		 m_hCurrentBitmap;

	RECT		 m_rect;

	WORD		 m_wdButtonWidth;
	WORD		 m_wdButtonHeight;

	bool		 m_bMouseOnButton;
	bool		 m_bClickOnButton;
public:
	virtual BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
};


