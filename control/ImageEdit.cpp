// control/ImageEdit.cpp : implementation file
//

#include "../stdafx.h"
#include "ImageEdit.h"


// CImageEdit

IMPLEMENT_DYNAMIC(CImageEdit, CEdit)

CImageEdit::CImageEdit()
{
	m_mousedown=FALSE;
	m_istartSel = 0xFFFFFFFF;

	m_hBkBitmap = NULL;
}

CImageEdit::~CImageEdit()
{
}


BEGIN_MESSAGE_MAP(CImageEdit, CEdit)
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(EN_CHANGE, &CImageEdit::OnEnChange)
	ON_CONTROL_REFLECT(EN_UPDATE, &CImageEdit::OnEnUpdate)
	ON_CONTROL_REFLECT(EN_ERRSPACE, &CImageEdit::OnEnErrspace)
	ON_WM_ERASEBKGND()
	ON_WM_GETDLGCODE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CImageEdit message handlers

bool CImageEdit::setBkBitmap(const wchar_t *strBitmap)
{
	HBITMAP hBkBitmap = (HBITMAP)::LoadImage(0,strBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(!hBkBitmap) return false;

	return setBkBitmap(hBkBitmap);
}
bool CImageEdit::setBkBitmap(HBITMAP hBitmap)
{
	freeResource();


	m_hBkBitmap = hBitmap;
	GetWindowRect(&m_rect);

	BITMAP	csBitmapSize;
	int		nRetValue;



	if (hBitmap)
	{
		m_hBkBitmap = hBitmap;

		nRetValue = ::GetObject(hBitmap, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			freeResource();
			return false;
		} 
		m_wdButtonWidth = (DWORD)csBitmapSize.bmWidth;
		m_wdButtonHeight = (DWORD)csBitmapSize.bmHeight;
	}



	setPos(m_rect.left,m_rect.top);



}
void CImageEdit::freeResource()
{
	if (m_hBkBitmap != NULL)
	{
		::DeleteObject(m_hBkBitmap);
		m_hBkBitmap = NULL;
	}
}
void CImageEdit::setPos(int x,int y)
{
	if (!m_hBkBitmap) return;


	m_rect.left = x;
	m_rect.top = y;

	this->MoveWindow(m_rect.left,m_rect.top,m_wdButtonWidth - 10 , m_wdButtonHeight);
}

WORD CImageEdit::getWidth()
{
	return m_wdButtonWidth;
}
WORD CImageEdit::getHeight()
{
	return m_wdButtonHeight;
}

void CImageEdit::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CEdit::OnPaint() for painting messages
	CDC tdc;
	HBITMAP    pbmpOldBmp = NULL;
	tdc.CreateCompatibleDC(&dc);
	pbmpOldBmp = (HBITMAP)::SelectObject(tdc.m_hDC, m_hBkBitmap);

	dc.TransparentBlt(-5, -5,m_wdButtonWidth, m_wdButtonHeight, &tdc, 0, 0,m_wdButtonWidth, m_wdButtonHeight, RGB(204,0,255));

	
	if (m_mousemove)
	{
		if (m_iendSel < m_istartSel)
		{
			SetSel(m_iendSel,m_istartSel);
		}
		else
			SetSel(m_istartSel,m_iendSel);
	}


	CString str;
	GetWindowText(str);
	dc.SelectObject(GetParent()->GetFont());
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(0,0,str);

}

void CImageEdit::OnEnChange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

}

void CImageEdit::OnEnUpdate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	
}

void CImageEdit::OnEnErrspace()
{
	// TODO: Add your control notification handler code here
	
}

BOOL CImageEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
//	CDC tdc;
//	HBITMAP    pbmpOldBmp = NULL;
//	tdc.CreateCompatibleDC(pDC);
//	pbmpOldBmp = (HBITMAP)::SelectObject(tdc.m_hDC, m_hBkBitmap);
//
//	pDC->TransparentBlt(-5, -5,m_wdButtonWidth, m_wdButtonHeight, &tdc, 0, 0,m_wdButtonWidth, m_wdButtonHeight, RGB(204,0,255));
//	
////
//	if (m_mousemove)
//	{
//		if (m_iendSel < m_istartSel)
//		{
//			SetSel(m_iendSel,m_istartSel);
//		}
//		else
//			SetSel(m_istartSel,m_iendSel);
//	}
//
//	/*SetSel(GetSel());*/
//// 		
//// 	}
//
//
//	CString str;
//	GetWindowText(str);
//	pDC->SelectObject(GetParent()->GetFont());
//	pDC->SetBkMode(TRANSPARENT);
//	pDC->TextOut(0,0,str);
	

	return CEdit::OnEraseBkgnd(pDC);
}

UINT CImageEdit::OnGetDlgCode()
{
	// TODO: Add your message handler code here and/or call default
	RedrawWindow(NULL, NULL,RDW_INVALIDATE | RDW_ERASE ); 

	return CEdit::OnGetDlgCode();
}

void CImageEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_mousedown = TRUE;
	SetCapture();
	
	SetFocus();

	

//	CEdit::OnLButtonDown(nFlags, point);
	
	m_istartSel = CharFromPos(point);//正准备选择的位置
	m_iendSel = m_istartSel;
	SetSel(m_istartSel,m_istartSel);
	
}

void CImageEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_mousedown) 
		ReleaseCapture();

	
	m_mousedown = FALSE;
	m_istartSel = 0xFFFFFFFF;
	m_mousemove = false;
//	CEdit::OnLButtonUp(nFlags, point);
}

void CImageEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_mousedown)
	{
		m_mousemove = true;
		m_iendSel = CharFromPos(point);
		
		if(0 > point.x)
			m_iendSel = 0;
		if(point.x > m_wdButtonWidth)
			m_iendSel = GetLimitText();

		Invalidate();
	}

	//CEdit::OnMouseMove(nFlags, point);
}

void CImageEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CImageEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	SetSel(0,GetLimitText());
}

void CImageEdit::OnMove(int x, int y)
{
	CEdit::OnMove(x, y);

	Invalidate();

	// TODO: Add your message handler code here
}

void CImageEdit::OnSize(UINT nType, int cx, int cy)
{
	CEdit::OnSize(nType, cx, cy);

	Invalidate();
	// TODO: Add your message handler code here
}
