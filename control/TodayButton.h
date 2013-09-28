#pragma once
#ifndef TODAYBUTTON_H
#define TODAYBUTTON_H
#include <gdiplus.h>
#include "todaybutton.h"
using namespace Gdiplus;
#define  TODAYBUTTON_TIMER  888

enum {
	TODAY_BUTTONSTATE_NORMAL,
	TODAY_BUTTONSTATE_DOWN,
	TODAY_BUTTONSTATE_MOVE,
	TODAY_BUTTONSTATE_FOCUS,
};
class TodayButton : public CButton
{
public:
			TodayButton(void);
			~TodayButton(void);
public:
	afx_msg		void		OnPaint();
	afx_msg		void		OnMouseLeave();
	afx_msg		void		OnMouseMove(UINT nFlags, CPoint point);
	afx_msg		void		OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg		void		OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg		void		OnSetFocus(CWnd* pOldWnd);
	afx_msg		void		OnKillFocus(CWnd* pNewWnd);
	afx_msg		BOOL		OnEraseBkgnd(CDC* pDC);
	afx_msg		void		OnSize(UINT nType, int cx, int cy);
				BOOL		SetNormalImage(CString strPath);
				BOOL		SetFocusImage(CString strPath);
				BOOL		SetDownImage(CString strPath);
				BOOL		SetDisableImage(CString strPath);
				BOOL		SetMoveImage(CString strPath);
	inline		void		SetRedrawBack(){m_bR = FALSE;}

	DECLARE_MESSAGE_MAP()
protected:
				DWORD		m_state;
				Image		*m_pNormal;
				Image		*m_pOnMove;
				Image		*m_pOnDown;
				Image		*m_pOnFoucs;
				BOOL		m_bMuseMove;
				BOOL		m_bMouseFocus;

				 CDC		m_memDC2;
			 CBitmap		m_bitmap;
			 BOOL			m_bR;
};
#endif
