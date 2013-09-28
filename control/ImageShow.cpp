#define  IMAGESHOW_CPP
#include "ImageShow.h"

ImageShow::ImageShow(WCHAR *pImagePath /* = NULL */)
{
		m_image.Load(pImagePath);
		m_bImageSize = FALSE;
		m_pen.CreatePen(PS_SOLID,1,RGB(222,222,222));
		m_pt.x = 5;
		m_pt.y = 5;
		m_frameColor = 0x97FFFF;
		m_bMouseMove = FALSE;
}

ImageShow::~ImageShow(void)
{
}

BEGIN_MESSAGE_MAP(ImageShow, CMFCButton)
		ON_WM_PAINT()
		ON_WM_MOUSELEAVE()
		ON_WM_MOUSEMOVE()
		//ON_WM_ERASEBKGND()
		//ON_WM_SIZE()
		//ON_WM_DRAWITEM()
END_MESSAGE_MAP()

void	ImageShow::OnPaint()
{
		CRect rect;
		this->GetWindowRect(&rect);
		rect.top += 5;
		rect.left += 5;
		rect.right -= 5;
		rect.bottom -= 5;
		if(m_bImageSize){
			DWORD h = m_image.GetHeight();
			DWORD w = m_image.GetWidth();
			SetWindowPos(GetParent(),rect.left,rect.top,w,h,SWP_DEFERERASE);
			rect.bottom = rect.top + h;
			rect.right = rect.left + w;
		}
		GetWindowRect(&rect);
		ScreenToClient(&rect);
		CPaintDC dc(this);
		dc.SelectObject(m_pen);
		dc.RoundRect(rect,m_pt);
		dc.SelectObject(m_framePen);
		dc.RoundRect(rect.left-1,rect.top-1,rect.right+1,rect.bottom+1,m_pt.x,m_pt.y);
		CRgn rgn;
		rgn.CreateRoundRectRgn(rect.left,rect.top,rect.right,rect.bottom,m_pt.x,m_pt.y);
		dc.SelectClipRgn(&rgn);
		m_image.StretchBlt(dc.GetSafeHdc(),rect);
}
void	ImageShow::OnMouseLeave()
{
		m_framePen.DeleteObject();
		m_framePen.CreatePen(PS_SOLID,1,RGB(222,222,222));
		OnPaint();
		m_bMouseMove = FALSE;
		//Invalidate();
		CMFCButton::OnMouseLeave();
}

void	ImageShow::OnMouseMove(UINT nFlags, CPoint point)
{
		if(!m_bMouseMove){
			m_framePen.DeleteObject();
			m_framePen.CreatePen(PS_SOLID,1,m_frameColor);
			OnPaint();
			m_bMouseMove = TRUE;
			//Invalidate();
		}
		CMFCButton::OnMouseMove(nFlags, point);
}


BOOL	ImageShow::OnEraseBkgnd(CDC* pDC)
{


		return TRUE;
}

void	ImageShow::OnSize(UINT nType, int cx, int cy)
{
		CMFCButton::OnSize(nType, cx, cy);

		CRgn m_rgn;
		CRect rc;
		GetWindowRect(&rc);
		rc-=rc.TopLeft();
		m_rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom,5,5);
		SetWindowRgn(m_rgn,TRUE);
}

void	ImageShow::OnFillBackground(CDC* pDC, const CRect& rectClient)
{
		//pDC->SetBkMode(TRANSPARENT);
		//pDC->RoundRect(rectClient,CPoint(10,10));
		return;
}

void	ImageShow::OnDrawFocusRect(CDC* pDC, const CRect& rectClient)
{
		return;
}

void	ImageShow::OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState)
{
		//pDC->SetBkMode(TRANSPARENT);
		return;
}

void	ImageShow::OnDraw(CDC* pDC, const CRect& rect, UINT uiState)
{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(m_pen);
		pDC->RoundRect(rect,m_pt);
		pDC->SelectObject(m_framePen);
		pDC->RoundRect(rect.left-1,rect.top-1,rect.right+1,rect.bottom+1,m_pt.x,m_pt.y);
		CRgn rgn;
		rgn.CreateRoundRectRgn(rect.left,rect.top,rect.right,rect.bottom,m_pt.x,m_pt.y);
		pDC->SelectClipRgn(&rgn);
		//m_image.StretchBlt(pDC->GetSafeHdc(),rect);
}

void	ImageShow::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
		CDC* pDCPaint = CDC::FromHandle(lpDrawItemStruct->hDC);
		pDCPaint->FillRect(&lpDrawItemStruct->rcItem,NULL);
}

void	ImageShow::SetImage(WCHAR *pImagePath)
{
		m_image.Load(pImagePath);
}

