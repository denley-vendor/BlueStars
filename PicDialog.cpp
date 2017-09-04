/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define PICDIALOG_CPP
#include "stdafx.h"
#include "today.h"
#include "PicDialog.h"

IMPLEMENT_DYNAMIC(CPicDialog, CDialog)

CPicDialog::CPicDialog(CWnd* pParent /*=NULL*/)
: CDialog(CPicDialog::IDD, pParent)
{
		m_bSelect = FALSE;
		m_bDclk = FALSE;
		m_selectRect = CRect(0,0,0,0);
}

CPicDialog::~CPicDialog()
{
}

void	CPicDialog::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPicDialog, CDialog)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CPicDialog 消息处理程序

BOOL	CPicDialog::OnInitDialog()
{
		CDialog::OnInitDialog();
		ShowWindow(SW_HIDE);
		CDC dcScreen;
		dcScreen.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
		m_size.cx = GetDeviceCaps(dcScreen.GetSafeHdc(), HORZRES);
		m_size.cy = GetDeviceCaps(dcScreen.GetSafeHdc(), VERTRES);

		m_dcMem.CreateCompatibleDC(&dcScreen);
		m_bmpScreen.CreateCompatibleBitmap(&dcScreen, m_size.cx,m_size.cy);
		CBitmap* pOldBmp = m_dcMem.SelectObject(&m_bmpScreen);
		m_dcMem.BitBlt(0,0,m_size.cx, m_size.cy, &dcScreen, 0, 0, SRCCOPY);

		MoveWindow(0,0,m_size.cx,m_size.cy);
		ShowWindow(SW_SHOW);
		ShowSelectInfo();
		return TRUE;  
}

void	CPicDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
		if(!m_bSelect)
			return;

		//if(m_selectRect.IsRectEmpty() || m_selectRect.IsRectNull())
		//	return;
		if(m_selectRect.left > m_selectRect.right){
			DWORD temp = m_selectRect.left;
			m_selectRect.left = m_selectRect.right;
			m_selectRect.right = temp;
		}
		if(m_selectRect.top > m_selectRect.bottom){
			DWORD temp = m_selectRect.top;
			m_selectRect.top = m_selectRect.bottom;
			m_selectRect.bottom = temp;
		}

		CDC dcSave;
		dcSave.CreateCompatibleDC(NULL);
		CBitmap bitmap;
		BITMAP bm;
		m_bmpScreen.GetObject(sizeof(bm), &bm);
		bm.bmWidth = m_selectRect.Width();
		bm.bmHeight = m_selectRect.Height();
		bitmap.CreateBitmapIndirect(&bm);
		CBitmap* pOldBmp = dcSave.SelectObject(&bitmap);

		dcSave.StretchBlt(0,0, m_selectRect.Width(), m_selectRect.Height(),
			&m_dcMem, m_selectRect.left, m_selectRect.top,
			m_selectRect.Width(), m_selectRect.Height(), SRCCOPY);

		GetExePath(m_strPath);
		m_strPath.Replace(_T("today.exe"),_T("tempitem\\cut.jpg"));
		//AfxMessageBox(m_strPath);
		SaveToJpg(m_strPath,&bitmap);
		HBITMAP hBmp = (HBITMAP)bitmap.Detach();
		//	dcSave.SelectObject(pOldBmp);
	//	SaveBitmapToFile(hBmp,_T("D:\\okok.bmp"));

		// save to the clipboard
		if(::OpenClipboard(m_hWnd))
		{
			::EmptyClipboard();
			::SetClipboardData(CF_BITMAP, hBmp);
			::CloseClipboard();
		}
		m_bDclk = FALSE;
		m_bSelect = FALSE;
		OnOK();
		//CDialog::OnLButtonDblClk(nFlags, point);
}

void	CPicDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
		if(!m_bSelect){
			m_selectRect.left = point.x;
			m_selectRect.top = point.y;
			m_selectRect.right = point.x;
			m_selectRect.bottom = point.y;
			m_bSelect = TRUE;
			m_bDclk = FALSE ;
		}

		CDialog::OnLButtonDown(nFlags, point);
}

void	CPicDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
		if(m_bSelect){
			m_bDclk = TRUE;
		}

		CDialog::OnLButtonUp(nFlags, point);
}

void	CPicDialog::OnRButtonDown(UINT nFlags, CPoint point)
{
		if(!m_bSelect){
			OnOK();
		}
		m_bSelect = FALSE; 
		ShowSelectInfo();
		CDialog::OnRButtonDown(nFlags, point);
} 

void	CPicDialog::OnMouseMove(UINT nFlags, CPoint point)
{
		if(m_bSelect && !m_bDclk){
			ShowSelectInfo();
			m_selectRect.right = point.x;
			m_selectRect.bottom = point.y;
		}
		CDialog::OnMouseMove(nFlags, point);
}

void	CPicDialog::ShowSelectInfo()
{
		CClientDC dc(this);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(&dc, m_size.cx, m_size.cy);
		CBitmap* pOldBmp = dcMem.SelectObject(&bmp);
		dcMem.BitBlt(0,0,m_size.cx, m_size.cy,&m_dcMem,0,0,SRCCOPY);

		if(m_bSelect){
			CPen penLine(PS_SOLID, 2, RGB(0,0,255));
			CPen* pOldPen = dcMem.SelectObject(&penLine);	
			dcMem.MoveTo(m_selectRect.left, m_selectRect.top);
			dcMem.LineTo(m_selectRect.right, m_selectRect.top);
			dcMem.LineTo(m_selectRect.right, m_selectRect.bottom);
			dcMem.LineTo(m_selectRect.left, m_selectRect.bottom);
			dcMem.LineTo(m_selectRect.left, m_selectRect.top);
			dcMem.SelectObject(pOldPen);
		}

		dc.StretchBlt(0,0,m_size.cx, m_size.cy, 
			&dcMem, 
			0,0,m_size.cx, m_size.cy,
			SRCCOPY);

		dcMem.SelectObject(pOldBmp);
}

int		CPicDialog::GetCodecClsid(const WCHAR* format,ImageCodecInfo* pImageCodecInfo,CLSID* pClsid)
{
		UINT num = 0;  
		UINT size = 0; 
		pImageCodecInfo = NULL;
		GetImageEncodersSize(&num, &size);
		if(size == 0) return -1; 

		pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
		if(pImageCodecInfo == NULL) return -1; 
		GetImageEncoders(num, size, pImageCodecInfo);

		for(UINT j = 0; j < num; ++j)
		{
			if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j; // Success
			}
		} 
		return -1;
}

void	CPicDialog::SaveToJpg(CString strPath,CBitmap *pBitMap)
{
		if(!pBitMap) return;
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		Image *i;
		i=Bitmap::FromHBITMAP((HBITMAP)pBitMap->m_hObject,0);
		CLSID   gifcodec;   
		ImageCodecInfo* pImageCodecInfo = NULL;
		GetCodecClsid(L"image/jpeg",pImageCodecInfo,&gifcodec);

		//wchar_t *wstr = strPath.AllocSysString();
		i->Save(strPath.GetBuffer(0),&gifcodec,0); 

		free(pImageCodecInfo);
		pImageCodecInfo = NULL;
		delete i;
		GdiplusShutdown(gdiplusToken);
}

BOOL	CPicDialog::SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpFileName)
{
		HDC hDC; //设备描述表 
		int iBits; //当前显示分辨率下每个像素所占字节数 
		WORD wBitCount; //位图中每个像素所占字节数 
		DWORD dwPaletteSize=0, //定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数 
			dwBmBitsSize, 
			dwDIBSize, dwWritten; 
		BITMAP Bitmap; //位图属性结构 
		BITMAPFILEHEADER bmfHdr; //位图文件头结构 
		BITMAPINFOHEADER bi; //位图信息头结构 
		LPBITMAPINFOHEADER lpbi; //指向位图信息头结构 

		HANDLE fh, hDib, hPal,hOldPal=NULL; //定义文件，分配内存句柄，调色板句柄 

		//计算位图文件每个像素所占字节数 
		HDC hWndDC = CreateDC(_T("DISPLAY"),NULL,NULL,NULL); 
		hDC = ::CreateCompatibleDC( hWndDC ) ; 
		iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES); 
		DeleteDC(hDC); 

		if (iBits <= 1) 
			wBitCount = 1; 
		else if (iBits <= 4) 
			wBitCount = 4; 
		else if (iBits <= 8) 
			wBitCount = 8; 
		else if (iBits <= 24) 
			wBitCount = 24; 
		else 
			wBitCount = 24 ; 

		//计算调色板大小 
		if (wBitCount <= 8) 
			dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD); 

		//设置位图信息头结构 
		GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap); 
		bi.biSize = sizeof(BITMAPINFOHEADER); 
		bi.biWidth = Bitmap.bmWidth; 
		bi.biHeight = Bitmap.bmHeight; 
		bi.biPlanes = 1; 
		bi.biBitCount = wBitCount; 
		bi.biCompression = BI_RGB; 
		bi.biSizeImage = 0; 
		bi.biXPelsPerMeter = 0; 
		bi.biYPelsPerMeter = 0; 
		bi.biClrUsed = 0; 
		bi.biClrImportant = 0; 

		dwBmBitsSize = ((Bitmap.bmWidth * wBitCount+31)/32) * 4 * Bitmap.bmHeight ; 

		//为位图内容分配内存 
		hDib = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER)); 
		lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
		*lpbi = bi; 

		// 处理调色板 
		hPal = GetStockObject(DEFAULT_PALETTE); 
		if (hPal) 
		{ 
			hDC = ::GetDC(NULL); 
			hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
			RealizePalette(hDC); 
		} 

		// 获取该调色板下新的像素值 
		GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, 
			(LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
			+dwPaletteSize, 
			(LPBITMAPINFO ) 
			lpbi, DIB_RGB_COLORS); 

		//恢复调色板 
		if (hOldPal) 
		{ 
			SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
			RealizePalette(hDC); 
			::ReleaseDC(NULL, hDC); 
		} 

		//创建位图文件 
		fh = CreateFile(lpFileName, GENERIC_WRITE, 
			0, NULL, CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 

		if (fh == INVALID_HANDLE_VALUE) 
			return FALSE; 

		// 设置位图文件头 
		bmfHdr.bfType = 0x4D42; // "BM" 
		dwDIBSize = sizeof(BITMAPFILEHEADER) 
			+ sizeof(BITMAPINFOHEADER) 
			+ dwPaletteSize + dwBmBitsSize; 
		bmfHdr.bfSize = dwDIBSize; 
		bmfHdr.bfReserved1 = 0; 
		bmfHdr.bfReserved2 = 0; 
		bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) 
			+ (DWORD)sizeof(BITMAPINFOHEADER) 
			+ dwPaletteSize; 

		// 写入位图文件头 
		WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 

		// 写入位图文件其余内容 
		WriteFile(fh, (LPSTR)lpbi, dwDIBSize, 
			&dwWritten, NULL); 

		//清除 
		GlobalUnlock(hDib); 
		GlobalFree(hDib); 
		CloseHandle(fh); 

		return TRUE;
}