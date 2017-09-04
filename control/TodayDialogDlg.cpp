
// TodayDialogDlg.cpp : 实现文件
//

#include "..//stdafx.h"
#include "TodayDialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTodayDialogDlg::CTodayDialogDlg(UINT nIDTemplate, CWnd* pParentWnd /* = NULL */)
: CDialog(nIDTemplate,pParentWnd)
{
		m_backColor = 0;
		m_bMemDc = FALSE;
		pimage = NULL;
		m_rect.SetRectEmpty();
}

CTodayDialogDlg::~CTodayDialogDlg()
{
}

void	CTodayDialogDlg::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTodayDialogDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()

BOOL	CTodayDialogDlg::OnInitDialog()
{
		CDialog::OnInitDialog();
		return TRUE;  
}

void	CTodayDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
		CDialog::OnSysCommand(nID, lParam);
}

void	CTodayDialogDlg::OnPaint()
{
		if (IsIconic()){
			CPaintDC dc(this);

			SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
			int cxIcon = GetSystemMetrics(SM_CXICON);
			int cyIcon = GetSystemMetrics(SM_CYICON);
			CRect rect;
			GetClientRect(&rect);
			int x = (rect.Width() - cxIcon + 1) / 2;
			int y = (rect.Height() - cyIcon + 1) / 2;

			dc.DrawIcon(x, y, m_hIcon);
		}
		else{
			CDialog::OnPaint();
		}
}

HCURSOR	CTodayDialogDlg::OnQueryDragIcon()
{
		return static_cast<HCURSOR>(m_hIcon);
}


void	CTodayDialogDlg::OnSize(UINT nType, int cx, int cy)
{
		CDialog::OnSize(nType, cx, cy);
		CRgn m_rgn;
		CRect rc;
		GetWindowRect(&rc);
		rc-=rc.TopLeft();
		m_rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom,7,7);
		SetWindowRgn(m_rgn,TRUE);

		if(m_bMemDc){
			InvalidateRect(&rc);
			DrawMemDc();
		}
		else{
			CRect rect;
			rect.CopyRect(m_rect);
			rect.left = rect.right - 2;
			rect.top = 0;
			rect.bottom = rc.bottom;
			if(rect.right >= rc.right){
				rect.right = rc.right - 2;
			}
			else{
				rect.right += 2;
			}
			InvalidateRect(&rect);

			rect.CopyRect(m_rect);
			rect.top = rect.bottom - 2;
			rect.left = 0;
			rect.right = rc.right;
			if(rect.bottom >= rc.bottom){
				rect.bottom = rc.bottom - 2;
			}
			else{
				rect.bottom += 2;
			}
			InvalidateRect(&rect);
		}
}

BOOL	CTodayDialogDlg::OnEraseBkgnd(CDC* pDC)
{		
		CRect rect; 
		GetClientRect(&rect);
		pDC->SelectObject(m_boderPen);
		//pDC->FillRgn(&m_rgn,&m_backBrush);
		pDC->SelectObject(m_backBrush);
		rect.right -= 1;
		rect.bottom -= 1;
		pDC->RoundRect(&rect,CPoint(7,7));
		if(m_bMemDc){
			if(!m_memDc){
				m_memDc.CreateCompatibleDC(pDC);
				bitmap.DeleteObject();
				bitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom);
				m_memDc.SelectObject(bitmap);
				DrawMemDc();
			}
			pDC->BitBlt(0,0,rect.right,rect.bottom,&m_memDc,0,0,SRCCOPY);
		}
		else{
			rect.left += 1;
			rect.top += 1;
			rect.bottom -= 2;
			rect.right -= 2;
			//pDC->FillRect(&rect,&m_backBrush);
			Graphics graphics( pDC->m_hDC);
			if(!pimage) return TRUE;
			DWORD pW = pimage->GetWidth();
			DWORD pH = pimage->GetHeight();
			RectF frect(rect.left,rect.top,rect.right,rect.bottom);
			Rect rcImage(0, 0, pW, pH);
			//Blur blur;
			//BlurParams bp;
			//bp.expandEdge = FALSE;
			//bp.radius = 5.0f;
			//blur.SetParameters(&bp);
			//pimage->ApplyEffect(&blur,rect);;
			TextureBrush brush(pimage, WrapModeClamp  , rcImage);
			graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			graphics.FillRectangle(&brush, frect);

			//m_pGraphics->TranslateTransform(rect.Width()+rect.left,0);
			//m_pGraphics->SetInterpolationMode(InterpolationModeNearestNeighbor); 

			//graphics.DrawImage(pimage,frect,0,0,frect.Width,frect.Height,UnitPixel);
		}
		return TRUE;
}

void	CTodayDialogDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
		PostMessage(WM_NCLBUTTONDOWN ,HTCAPTION,MAKELPARAM(point.x,point.y));
		CDialog::OnLButtonDown(nFlags,point);
}

void	CTodayDialogDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
		//SendMessage(WM_MBUTTONDOWN,MK_LBUTTON,MAKELPARAM(point.x,point.y));
		CDialog::OnMButtonDown(nFlags,point);
}

void	CTodayDialogDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
		//SendMessage(WM_LBUTTONDOWN,MK_LBUTTON,MAKELPARAM(point.x,point.y));
		CDialog::OnRButtonDown(nFlags,point);
}


void	CTodayDialogDlg::OnMouseMove(UINT nFlags, CPoint point)
{
		CDialog::OnMouseMove(nFlags, point);
}

LRESULT CTodayDialogDlg::OnNcHitTest(CPoint point)
{
		CRect rect;
		GetWindowRect(&rect);
		if(point.x <= rect.left+3){
			return HTLEFT;
		}
		else if(point.x >= rect.right-3){
			return HTRIGHT;
		}
		else if(point.y <= rect.top+3){
			return HTTOP;
		}
		else if(point.y >= rect.bottom-3){
			return HTBOTTOM;
		}    
		else if(point.x <= rect.left+10 && point.y <= rect.top+10){
			return HTTOPLEFT;
		}
		else if(point.x >= rect.right-10 && point.y <= rect.top+10){
			return HTTOPRIGHT;
		}
		else if(point.x <= rect.left+10 && point.y >= rect.bottom-10){
			return HTBOTTOMLEFT;
		}
		else if(point.x >= rect.right-10 && point.y >= rect.bottom-10){
			return HTBOTTOMRIGHT;
		}
		return HTCLIENT;
}

BOOL	CTodayDialogDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
		if(nHitTest == HTCAPTION || nHitTest == HTSYSMENU ||
			nHitTest == HTMENU || nHitTest == HTCLIENT){
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		}
		else if(nHitTest == HTTOP || nHitTest == HTBOTTOM){
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
		}
		else if(nHitTest == HTLEFT || nHitTest == HTRIGHT){
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
		}
		else if(nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMRIGHT){
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
		}
		else if(nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMLEFT){
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
		}
		else{
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		}
		return TRUE;
}

void	CTodayDialogDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
		GetClientRect(m_rect);
		if(nHitTest == HTTOP)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTBOTTOM)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTLEFT)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTRIGHT)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTTOPLEFT)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTTOPRIGHT)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTBOTTOMLEFT)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTBOTTOMRIGHT)
			SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));
		CDialog::OnNcLButtonDown(nHitTest, point);
}


void	CTodayDialogDlg::FillNine(FLOAT h,FLOAT w)
{
		FLOAT tempH = h/3;
		FLOAT tempW = w/3;
		FLOAT Hh = 0;
		for(DWORD i = 0; i < 9; i++){
			m_nineRect[i].X = (i)%3 * tempW;
			m_nineRect[i].Y = Hh;
			m_nineRect[i].Width = m_nineRect[i].X + tempW;
			m_nineRect[i].Height = Hh + tempH;
			if(!((i+1)%3)){
				Hh += tempH;
			}
		}
}

void	CTodayDialogDlg::FillWnine(FLOAT h,FLOAT w)
{
		memcpy(&m_wNineRect[0],&m_nineRect[0],sizeof(RectF));
		m_wNineRect[2].X = w - (m_nineRect[0].Width-m_nineRect[0].X);
		m_wNineRect[2].Y = m_nineRect[2].Y;
		m_wNineRect[2].Width = w;
		m_wNineRect[2].Height = m_nineRect[2].Height;

		m_wNineRect[6].X = m_nineRect[6].X;
		m_wNineRect[6].Y = h - (m_nineRect[6].Height - m_nineRect[6].Y);
		m_wNineRect[6].Width = m_nineRect[6].Width;
		m_wNineRect[6].Height = h;

		m_wNineRect[8].X = m_wNineRect[2].X;
		m_wNineRect[8].Y = m_wNineRect[6].Y;
		m_wNineRect[8].Width = w;
		m_wNineRect[8].Height = h;

		m_wNineRect[1].X = m_wNineRect[0].Width;
		m_wNineRect[1].Y = m_wNineRect[0].Y;
		m_wNineRect[1].Width = m_wNineRect[2].X;
		m_wNineRect[1].Height = m_wNineRect[0].Height;

		m_wNineRect[3].X = m_wNineRect[0].X;
		m_wNineRect[3].Y = m_wNineRect[0].Height;
		m_wNineRect[3].Width = m_wNineRect[0].Width;
		m_wNineRect[3].Height = m_wNineRect[6].Y;

		m_wNineRect[5].X = m_wNineRect[8].X;
		m_wNineRect[5].Y = m_wNineRect[2].Height;
		m_wNineRect[5].Width = w;
		m_wNineRect[5].Height = m_wNineRect[8].X;

		m_wNineRect[7].X = m_wNineRect[6].Width;
		m_wNineRect[7].Y = m_wNineRect[6].Y;
		m_wNineRect[7].Width = m_wNineRect[8].X;
		m_wNineRect[7].Height = m_wNineRect[8].Height;

		m_wNineRect[4].X = m_wNineRect[3].Width;
		m_wNineRect[4].Y = m_wNineRect[3].Y;
		m_wNineRect[4].Width = m_wNineRect[5].X;
		m_wNineRect[4].Height = m_wNineRect[3].Height;
}

void	CTodayDialogDlg::DrawMemDc()
{
		if(!m_memDc) return;
		if(!pimage) return;

		CRect rect;
		CPoint pt;
		pt.x = 10;
		pt.y = 10;
		GetClientRect(&rect);
		CDC *pDC = GetDC();

		bitmap.DeleteObject();
		bitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom);
		m_memDc.SelectObject(bitmap);

		m_memDc.SelectObject(m_backBrush);
		m_memDc.SelectObject(m_boderPen);
		Graphics graphics( m_memDc.m_hDC);
		m_memDc.RoundRect(&rect,pt);

		INT wW = rect.right - rect.left;
		INT wH = rect.bottom - rect.top;

		FillNine(m_imageH,m_imageW);
		FillWnine(wH,wW);
		Rect inRect;

		inRect.X = m_wNineRect[1].X;
		inRect.Y = 0;
		inRect.Width = m_wNineRect[1].Width-100;
		inRect.Height = m_nineRect[1].Height;
		graphics.DrawImage(pimage,inRect,m_nineRect[1].X,0,110,m_nineRect[1].Height,UnitPixel);

		inRect.X = m_wNineRect[2].X;
		inRect.Y = 0;
		inRect.Width = m_nineRect[2].Width - 2;
		inRect.Height = m_nineRect[2].Height;
		graphics.DrawImage(pimage,inRect,m_nineRect[2].X,0,m_nineRect[2].Width,m_nineRect[2].Height,UnitPixel);

		inRect.X = m_wNineRect[0].X;
		inRect.Y = 0;
		inRect.Width = m_nineRect[0].Width;
		inRect.Height = m_nineRect[0].Height;
		graphics.DrawImage(pimage,inRect,m_nineRect[0].X,0,m_nineRect[0].Width,m_nineRect[0].Height,UnitPixel);

		inRect.X = m_wNineRect[3].X;
		inRect.Y = m_nineRect[3].Y;
		inRect.Width = m_nineRect[3].Width;
		inRect.Height = wH;
		graphics.DrawImage(pimage,inRect,m_nineRect[3].X,m_nineRect[3].Y,m_nineRect[3].Width,200,UnitPixel);

		inRect.X = m_wNineRect[6].X;
		inRect.Y = m_wNineRect[6].Y;
		inRect.Width = m_nineRect[6].Width;
		inRect.Height = m_nineRect[6].Height-4;
		graphics.DrawImage(pimage,inRect,m_nineRect[6].X,m_nineRect[6].Y,m_nineRect[6].Width,m_nineRect[6].Height,UnitPixel);

		inRect.X = m_wNineRect[4].X;
		inRect.Y = m_wNineRect[4].Y;
		inRect.Width = m_wNineRect[4].Width;
		inRect.Height = m_wNineRect[4].Height;
		graphics.DrawImage(pimage,inRect,m_nineRect[4].X,m_nineRect[4].Y,110,80,UnitPixel);

		inRect.X = m_wNineRect[7].X;
		inRect.Y = m_wNineRect[7].Y;
		inRect.Width = m_wNineRect[7].Width;
		inRect.Height = m_nineRect[7].Height - 4;
		graphics.DrawImage(pimage,inRect,m_nineRect[7].X,m_nineRect[7].Y,110,m_nineRect[7].Height,UnitPixel);

		inRect.X = m_wNineRect[8].X;
		inRect.Y = m_wNineRect[8].Y;
		inRect.Width = m_nineRect[8].Width - 2;
		inRect.Height = m_nineRect[8].Height-4;
		graphics.DrawImage(pimage,inRect,m_nineRect[8].X,m_nineRect[8].Y,m_nineRect[8].Width,m_nineRect[8].Height,UnitPixel);

		inRect.X = m_wNineRect[5].X;
		inRect.Y = m_wNineRect[5].Y;
		inRect.Width = m_nineRect[5].Width - 2;
		inRect.Height = wH - 100;
		graphics.DrawImage(pimage,inRect,m_nineRect[5].X,m_nineRect[5].Y,m_nineRect[5].Width,80,UnitPixel);

		m_memDc.BitBlt(0,0,rect.right,rect.bottom,NULL,0,0,SRCCOPY);
}

void	CTodayDialogDlg::GaussianFilter2(unsigned char * corrupted, unsigned char * smooth, int width, int height)
{
		int  templates[25] = { 1, 4, 7, 4, 1,   
			4, 16, 26, 16, 4,   
			7, 26, 41, 26, 7,  
			4, 16, 26, 16, 4,   
			1, 4, 7, 4, 1 };        

		memcpy ( smooth, corrupted, width*height*sizeof (unsigned  char ) );  
		for  ( int  j=2;j<height-2;j++)  {  
			for  ( int  i=2;i<width-2;i++)  {  
				int  sum = 0;  
				int  index = 0;  
				for  (  int  m=j-2; m<j+3; m++)  {  
					for  ( int  n=i-2; n<i+3; n++)  {  
						sum += corrupted [ m*width + n] * templates[index++] ;  
					}  
				}  
				sum /= 273;  
				if  (sum > 255)  
					sum = 255;  
				smooth [ j*width+i ] = sum;  
			}  
		} 
}

void	CTodayDialogDlg::SetBackImage(CString str)
{
		pimage = Bitmap::FromFile(str.GetBuffer(0));
		Color color;
		DWORD w = pimage->GetWidth();
		if(!pimage->GetPixel(w-10,10,&color)){
			m_backColor = RGB(color.GetR(),color.GetG(),color.GetB());
		}

		m_imageH = pimage->GetHeight();
		m_imageW = pimage->GetWidth();

		m_backBrush.DeleteObject();
		m_backBrush.CreateSolidBrush(m_backColor);

		m_boderPen.DeleteObject();
		m_boderPen.CreatePen(PS_SOLID,1,RGB(0,0,0));
}