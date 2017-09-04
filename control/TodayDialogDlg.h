
// TodayDialogDlg.h : 头文件
//

#pragma once
#include <gdiplus.h>

using namespace Gdiplus;


// CTodayDialogDlg 对话框
class CTodayDialogDlg : public CDialog
{
public:
	CTodayDialogDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	~CTodayDialogDlg();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

public:
	HICON		m_hIcon;
	CPen		m_boderPen;
	CBrush		m_backBrush;
	DWORD		m_backColor;

	Rect		m_nineRect[9];
	Rect		m_wNineRect[9];

	Bitmap		*pimage;

	CDC			m_memDc;
	CBitmap		bitmap;

	DWORD		m_imageW;
	DWORD		m_imageH;
	BOOL		m_bMemDc;
	virtual		BOOL		OnInitDialog();
	afx_msg		void		OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg		void		OnPaint();
	afx_msg		HCURSOR		OnQueryDragIcon();
	afx_msg		void		OnSize(UINT nType, int cx, int cy);
	afx_msg		BOOL		OnEraseBkgnd(CDC* pDC);
	afx_msg		void		OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg		void		OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg		void		OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg		void		OnMouseMove(UINT nFlags, CPoint point);
	afx_msg		LRESULT		OnNcHitTest(CPoint point);
	afx_msg		BOOL		OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg		void		OnNcLButtonDown(UINT nHitTest, CPoint point);

				void		FillNine(FLOAT h,FLOAT w);
				void		FillWnine(FLOAT h,FLOAT w);
				void		DrawMemDc();
				void		GaussianFilter2 (unsigned  char * corrupted, unsigned  char * smooth,  int  width,  int  height);
				void		SetBackImage(CString str);
	DECLARE_MESSAGE_MAP()
	CRect		m_rect;
};
