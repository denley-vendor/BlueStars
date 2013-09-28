// control/ImageButton.cpp : implementation file
//

#include "../stdafx.h"
#include "../today.h"
#include "ImageButton.h"


// CImageButton

IMPLEMENT_DYNAMIC(CImageButton, CButton)

CImageButton::CImageButton()
{
	m_hBitmapNormal = NULL;
	m_hBitmapHot = NULL;
	m_hBitmapClick = NULL;

	m_hCurrentBitmap = NULL;

	m_bMouseOnButton = false;
	m_bClickOnButton = false;

	
}

CImageButton::~CImageButton()
{
	freeResource();
}


BEGIN_MESSAGE_MAP(CImageButton, CButton)
	ON_WM_DRAWITEM()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEMOVE()
	//ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()

	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SIZE()
	ON_WM_MOVE()
END_MESSAGE_MAP()



// CImageButton message handlers

void CImageButton::freeResource()
{
	if (m_hBitmapNormal != NULL)
	{
		::DeleteObject(m_hBitmapNormal);
		m_hBitmapNormal = NULL;
	}
	if (m_hBitmapHot != NULL)
	{
		::DeleteObject(m_hBitmapHot);
		m_hBitmapHot = NULL;
	}
	if (m_hBitmapClick != NULL)
	{
		::DeleteObject(m_hBitmapClick);
		m_hBitmapClick = NULL;
	}

	m_hCurrentBitmap = NULL;

}
bool CImageButton::setBitmaps(const wchar_t *strBitmapNormal,const wchar_t *strBitmapHot,const wchar_t *strBitmapClicked)
{
	HBITMAP hBitmapNormal = (HBITMAP)::LoadImage(0,strBitmapNormal, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(!hBitmapNormal) return false;
	HBITMAP hBitmapHot = (HBITMAP)::LoadImage(0,strBitmapHot, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(!hBitmapHot) return false;
	HBITMAP hBitmapClick = (HBITMAP)::LoadImage(0,strBitmapClicked, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(!hBitmapClick) return false;

	return setBitmaps(hBitmapNormal,hBitmapHot,hBitmapClick);
}
bool CImageButton::setBitmaps(HBITMAP hBitmapNormal,HBITMAP hBitmapHot,HBITMAP hBitmapClick)
{
	//1.首先释放掉资源
	freeResource();
	
	//2.设置变量
	m_hBitmapNormal = hBitmapNormal;
	m_hBitmapHot = hBitmapHot;
	m_hBitmapClick = hBitmapClick;
	GetWindowRect(&m_rect);

	//3.设置初始值
	return setCurrentBitmap(m_hBitmapNormal);

}

bool CImageButton::setCurrentBitmap(HBITMAP hCurrentBitmap)
{
	BITMAP	csBitmapSize;
	int		nRetValue;



	if (hCurrentBitmap)
	{
		m_hCurrentBitmap = hCurrentBitmap;

		nRetValue = ::GetObject(hCurrentBitmap, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			freeResource();
			return false;
		} 
		m_wdButtonWidth = (DWORD)csBitmapSize.bmWidth;
		m_wdButtonHeight = (DWORD)csBitmapSize.bmHeight;
	}

	Invalidate();

	
	
 	this->MoveWindow(m_rect.left,m_rect.top, m_wdButtonWidth, m_wdButtonHeight);

	

	return true;
}
void CImageButton:: setPos(int x,int y)
{
	if(!m_hCurrentBitmap) return ;

	m_rect.left = x;
	m_rect.top = y;


	this->MoveWindow(m_rect.left,m_rect.top, m_wdButtonWidth, m_wdButtonHeight);
}
WORD CImageButton::getWidth()
{
	return m_wdButtonWidth;
}
WORD CImageButton::getHeight()
{
	return m_wdButtonHeight;
}
void CImageButton::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default

	//CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);

}

void CImageButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  Add your code to draw the specified item
	//1. 获得dc
	CDC *pDc = CDC::FromHandle(lpDrawItemStruct->hDC);

	CDC dc;
	HBITMAP    pbmpOldBmp = NULL;
	dc.CreateCompatibleDC(pDc);
	pbmpOldBmp = (HBITMAP)::SelectObject(dc.m_hDC, m_hCurrentBitmap);

	pDc->TransparentBlt(0, 0,m_wdButtonWidth, m_wdButtonHeight, &dc, 0, 0,m_wdButtonWidth, m_wdButtonHeight, RGB(204,0,255));





}

LRESULT CImageButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if (message == WM_LBUTTONDBLCLK)
	{
		message = WM_LBUTTONDOWN;
	} // if

	return CButton::DefWindowProc(message, wParam, lParam);
}

void CImageButton::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CButton::PreSubclassWindow();
}

BOOL CImageButton::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CButton::PreTranslateMessage(pMsg);
	return true;
}

HBRUSH CImageButton::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

void CImageButton::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CButton::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
}

void CImageButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

//	CButton::OnMouseMove(nFlags, point);

	CWnd*				wndUnderMouse = NULL;
	CWnd*				wndActive = this;
	TRACKMOUSEEVENT		csTME;


	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);

	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_bMouseOnButton)
		{
			m_bMouseOnButton = TRUE;

			Invalidate();

			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);

			setCurrentBitmap(m_hBitmapHot);
		} 
	}

}

void CImageButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CButton::OnPaint() for painting messages
	

}
LRESULT CImageButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bMouseOnButton = false;
	m_bClickOnButton = false;
	setCurrentBitmap(m_hBitmapNormal);
	return 0;
}
void CImageButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CButton::OnLButtonDblClk(nFlags, point);
}

void CImageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	
	CWnd*				wndUnderMouse = NULL;
	CWnd*				wndActive = this;


	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);

	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_bClickOnButton)
		{
			m_bClickOnButton = TRUE;
			setCurrentBitmap(m_hBitmapClick);
			//CButton::OnLButtonDown(nFlags, point);
		} 
	}
}

void CImageButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	
	CWnd*				wndUnderMouse = NULL;
	CWnd*				wndActive = this;
	TRACKMOUSEEVENT		csTME;


	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);

	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (m_bClickOnButton)
		{
			m_bMouseOnButton = true;
			m_bClickOnButton = false;
			setCurrentBitmap(m_hBitmapHot);
			//::SendDlgItemMessage(GetParent()->m_hWnd,GetDlgCtrlID(),BN_CLICKED,0,0);
			/*::PostMessage()*/
			::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),BN_CLICKED),NULL);
			//CButton::OnLButtonUp(nFlags, point);
		} 
	}
	else
	{
		OnMouseLeave(MAKEWPARAM(0,0),LPARAM(NULL));
	}
}

void CImageButton::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CButton::OnMButtonDblClk(nFlags, point);
}

void CImageButton::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CButton::OnMButtonDown(nFlags, point);
}

void CImageButton::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CButton::OnMButtonUp(nFlags, point);
}

BOOL CImageButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class

	m_rect = rect;

	return CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);

}

void CImageButton::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);

	

	// TODO: Add your message handler code here
}

void CImageButton::OnMove(int x, int y)
{
	CButton::OnMove(x, y);

	m_rect.left = x;
	m_rect.top = y;
	// TODO: Add your message handler code here
}
