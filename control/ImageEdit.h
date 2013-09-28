#pragma once


// CImageEdit

class CImageEdit : public CEdit
{
	DECLARE_DYNAMIC(CImageEdit)

public:
	CImageEdit();
	virtual ~CImageEdit();

	bool		setBkBitmap(const wchar_t *strBitmap);
	bool		setBkBitmap(HBITMAP hBitmap);
	void		freeResource();
	void		setPos(int x,int y);

	WORD		getWidth();
	WORD		getHeight();

private:
	HBITMAP		m_hBkBitmap;

	RECT		 m_rect;

	WORD		 m_wdButtonWidth;
	WORD		 m_wdButtonHeight;

	bool		m_mousedown;
	bool		m_mousemove;

	int			m_istartSel;
	int			m_iendSel;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnEnChange();
	afx_msg void OnEnUpdate();
	afx_msg void OnEnErrspace();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


