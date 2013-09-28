#define TODAYBUTTON_CPP
#include "..//StdAfx.h"
#include "TodayButton.h"

TodayButton::TodayButton(void)
{
		m_state = TODAY_BUTTONSTATE_NORMAL;
		m_bMuseMove = FALSE;
		m_bMouseFocus = FALSE;
		m_pNormal = NULL;
		m_pOnDown = NULL;
		m_pOnFoucs = NULL;
		m_pOnMove = NULL;
		m_bR = FALSE;
}

TodayButton::~TodayButton(void)
{
}

BEGIN_MESSAGE_MAP(TodayButton, CButton)
	ON_WM_PAINT()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void	TodayButton::OnPaint()
{
		CRect rect;
		CPoint pt;
		pt.x = 0;
		pt.y = 0;
		GetClientRect(&rect);
		CPaintDC dc(this);
		Graphics graphics(dc.GetSafeHdc());

		if(m_memDC2){
			dc.BitBlt(0,0,rect.right,rect.bottom,&m_memDC2,0,0,SRCCOPY);
		}
	
		RectF rect1(0,0,rect.Width(),rect.Height());
		RectF rect2(0,0,0,0);
		if(m_pNormal){
			rect2.Width = m_pNormal->GetWidth();
			rect2.Height = m_pNormal->GetHeight();
		}
		switch(m_state){
			case TODAY_BUTTONSTATE_NORMAL:
				if(m_pNormal) graphics.DrawImage(m_pNormal,rect1,0,0,rect2.Width,rect2.Height,UnitPixel);
				break;
			case TODAY_BUTTONSTATE_MOVE:
				if(m_pOnMove) graphics.DrawImage(m_pOnMove,rect1,0,0,rect2.Width,rect2.Height,UnitPixel);
				break;
			case TODAY_BUTTONSTATE_FOCUS:
				if(m_pOnFoucs) graphics.DrawImage(m_pOnFoucs,rect1,0,0,rect2.Width,rect2.Height,UnitPixel);
				break;
			case TODAY_BUTTONSTATE_DOWN:
				if(m_pOnDown) graphics.DrawImage(m_pOnDown,rect1,0,0,rect2.Width,rect2.Height,UnitPixel);
				break;
		}

		CString str;
		GetWindowText(str);
		dc.SetBkMode(TRANSPARENT);
		dc.SelectObject(afxGlobalData.fontDefaultGUIBold);
		dc.DrawText(str,rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

BOOL	TodayButton::OnEraseBkgnd(CDC* pDC)
{
		if(!m_bR){
			CRect  clientRect;
			GetClientRect(&clientRect);
			int cx=clientRect.Width ();
			int cy=clientRect.Height ();
			CBitmap	bitmap;
			m_memDC2.DeleteDC();
			m_memDC2.CreateCompatibleDC (pDC);
			bitmap.CreateCompatibleBitmap (pDC,cx,cy);
			m_memDC2.SelectObject (&bitmap);
			m_memDC2.BitBlt (0,0,cx,cy,pDC,0,0,SRCCOPY);
			m_bR = TRUE;
		}
		OnPaint();
		return TRUE;
}

void	TodayButton::OnMouseLeave()
{
		if(m_bMuseMove){
			if(!m_bMouseFocus){
				m_state = TODAY_BUTTONSTATE_NORMAL;
			}
			OnPaint();
			m_bMuseMove = FALSE;
		}
		CButton::OnMouseLeave();
}

void	TodayButton::OnMouseMove(UINT nFlags, CPoint point)
{
		if(!m_bMuseMove){
			m_state = TODAY_BUTTONSTATE_MOVE;
			OnPaint();
			m_bMuseMove = TRUE;
		}
		CButton::OnMouseMove(nFlags,point);
}

void	TodayButton::OnLButtonDown(UINT nFlags, CPoint point)
{
		m_state = TODAY_BUTTONSTATE_DOWN;
		OnPaint();
		CButton::OnLButtonDown(nFlags,point);
}

void	TodayButton::OnLButtonUp(UINT nFlags, CPoint point)
{
		m_state = TODAY_BUTTONSTATE_FOCUS;
		OnPaint();
		CButton::OnLButtonUp(nFlags,point);
}

void	TodayButton::OnSetFocus(CWnd* pOldWnd)
{
		m_state = TODAY_BUTTONSTATE_FOCUS;
		m_bMouseFocus = TRUE;
		OnPaint();
		CButton::OnSetFocus(pOldWnd);
}


void	TodayButton::OnKillFocus(CWnd* pNewWnd)
{
		m_bMouseFocus = FALSE;
		m_state = TODAY_BUTTONSTATE_NORMAL;
		OnPaint();
		CButton::OnKillFocus(pNewWnd);
}

void	TodayButton::OnSize(UINT nType, int cx, int cy)
{
		CButton::OnSize(nType, cx, cy);
}

BOOL	TodayButton::SetNormalImage(CString strPath)
{
		m_pNormal = Image::FromFile(strPath.GetBuffer(0));
		if(!m_pNormal) return FALSE;
		return TRUE;
}

BOOL	TodayButton::SetFocusImage(CString strPath)
{
		m_pOnFoucs = Image::FromFile(strPath.GetBuffer(0));
		if(!m_pOnFoucs) return FALSE;
		return TRUE;
}

BOOL	TodayButton::SetDownImage(CString strPath)
{
		m_pOnDown = Image::FromFile(strPath.GetBuffer(0));
		if(!m_pOnDown) return FALSE;
		return TRUE;
}

BOOL	TodayButton::SetDisableImage(CString strPath)
{
		if(!m_pOnFoucs) return FALSE;
		return TRUE;
}

BOOL	TodayButton::SetMoveImage(CString strPath)
{
		m_pOnMove = Image::FromFile(strPath.GetBuffer(0));
		if(!m_pOnMove) return FALSE;
		return TRUE;
}