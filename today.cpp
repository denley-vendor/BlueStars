/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/

#include "stdafx.h"
#include "today.h"
#include "todayDlg.h"
#include "control/TodayMenu.h"
#include "WeiBoEngine/Data/IniFile.h"
#include <WinVer.h>
#include "ExceptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CtodayApp

BEGIN_MESSAGE_MAP(CtodayApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CtodayApp 构造

CtodayApp::CtodayApp()
{
}


// 唯一的一个 CtodayApp 对象

CtodayApp theApp;

// CtodayApp 初始化

BOOL	CtodayApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
		INITCOMMONCONTROLSEX InitCtrls;
		InitCtrls.dwSize = sizeof(InitCtrls);
		InitCtrls.dwICC = ICC_WIN95_CLASSES;
		InitCommonControlsEx(&InitCtrls);

		AfxEnableControlContainer();
		InitContextMenuManager ();
		CMFCVisualManager::SetDefaultManager (RUNTIME_CLASS (TodayMenu));

		SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

		EnableAutomation();

		g_Exception.InitException();

		if(!dsLoadSkin( _T("data\\control"), LOAD_FROM_FOLDER)){
			AfxMessageBox(_T("dsLoadSkin error"));
			return FALSE;
		}


		//if(!dsLoadSkin( _T("control.t"), LOAD_FROM_DB)){
		//	AfxMessageBox(_T("dsLoadSkin error"));
		//	return FALSE;
		//}

		SetHtmlCss();
		g_WeiBoEngine.Initialization();

		Detours();
		CtodayDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{

		}
		else if (nResponse == IDCANCEL)
		{

		}

		return FALSE;
}

//static BOOL (AFXAPI *RAfxAssertFailedLine)(LPCSTR lpszFileName, int nLine) = AfxAssertFailedLine;
//BOOL	AFXAPI	MAfxAssertFailedLine(LPCSTR lpszFileName, int nLine)
//{
//		//return FALSE;
//		return RAfxAssertFailedLine(lpszFileName,nLine);
//}

static __gdi_entry BOOL (WINAPI *RLineTo)( __in HDC hdc, __in int x, __in int y) = LineTo;
BOOL	__gdi_entry WINAPI MLineTo(__in HDC hdc, __in int x, __in int y)
{
		return RLineTo(hdc,x,y);
}
static size_t (__cdecl *Rstrlen)(_In_z_ const char * _Str) = strlen;
size_t __cdecl	Mstrlen(_In_z_ const char * _Str)
{
		return Rstrlen(_Str);
}
static DWORD (WINAPI *RGetPrivateProfileStringA)(
						 __in_opt LPCSTR lpAppName,
						 __in_opt LPCSTR lpKeyName,
						 __in_opt LPCSTR lpDefault,
						 __out_ecount_part_opt(nSize, return + 1) LPSTR lpReturnedString,
						 __in     DWORD nSize,
						 __in_opt LPCSTR lpFileName
						 ) = GetPrivateProfileStringA;
DWORD	WINAPI MGetPrivateProfileStringA(__in_opt LPCSTR lpAppName,
										  __in_opt LPCSTR lpKeyName,
										  __in_opt LPCSTR lpDefault,
										  __out_ecount_part_opt(nSize, return + 1) LPSTR lpReturnedString,
										  __in     DWORD nSize,
										  __in_opt LPCSTR lpFileName)
{
		return RGetPrivateProfileStringA(lpAppName,lpKeyName,lpDefault,lpReturnedString,nSize,lpFileName);
}
static	int		(WINAPI *RDrawTextA)(__in HDC hdc,__inout_ecount_opt(cchText) LPCSTR lpchText,__in int cchText,__inout LPRECT lprc,__in UINT format) = DrawTextA;
int		WINAPI MDrawTextA(__in HDC hdc,__inout_ecount_opt(cchText) LPCSTR lpchText,__in int cchText,__inout LPRECT lprc,__in UINT format)
{

		return RDrawTextA(hdc,lpchText,cchText,lprc,format);
}

static	BOOL	(APIENTRY *RGetTextExtentPoint32W)(__in HDC hdc,__in_ecount(c) LPCWSTR lpString,__in int c,__out LPSIZE psizl) = GetTextExtentPoint32W;
BOOL	APIENTRY	MGetTextExtentPoint32W(__in HDC hdc,__in_ecount(c) LPCWSTR lpString,__in int c,__out LPSIZE psizl)
{
		CString strTemp;
		strTemp.Format(_T("%s"),lpString);

		CString str;
		strTemp.MakeUpper();
		str = _T("EASY");
		if(strTemp.Find(str) != -1){
			return TRUE;
		}
		return RGetTextExtentPoint32W(hdc,lpString,c,psizl);
}
static	int		(WINAPI *RDrawTextW)(__in HDC hdc,__inout_ecount_opt(cchText) LPCWSTR lpchText,__in int cchText,__inout LPRECT lprc,__in UINT format) = DrawTextW;
int		WINAPI MDrawTextW(__in HDC hdc,__inout_ecount_opt(cchText) LPCWSTR lpchText,__in int cchText,__inout LPRECT lprc,__in UINT format)
{
		if(cchText < 20){
			CString strTemp;
			strTemp.Format(_T("%s"),lpchText);

			CString str;
			strTemp.MakeUpper();
			str = _T("EASY");
			if(strTemp.Find(str) != -1){
				return TRUE;
			}
		}
		return RDrawTextW(hdc,lpchText,cchText,lprc,format);
}
static int   (WINAPI *RStretchDIBitsR)(__in HDC hdc, __in int xDest, __in int yDest, __in int DestWidth, __in int DestHeight, __in int xSrc, __in int ySrc, __in int SrcWidth, __in int SrcHeight,
						   __in_opt CONST VOID * lpBits, __in CONST BITMAPINFO * lpbmi, __in UINT iUsage, __in DWORD rop) = StretchDIBits;
int		WINAPI	MStretchDIBitsR(__in HDC hdc, __in int xDest, __in int yDest, __in int DestWidth, __in int DestHeight, __in int xSrc, __in int ySrc, __in int SrcWidth, __in int SrcHeight,
								__in_opt CONST VOID * lpBits, __in CONST BITMAPINFO * lpbmi, __in UINT iUsage, __in DWORD rop)
{
		
		return 0;
}
static	BOOL  (WINAPI *RBitBlt)( __in HDC hdc, __in int x, __in int y, __in int cx, __in int cy, __in_opt HDC hdcSrc, __in int x1, __in int y1, __in DWORD rop) = BitBlt;
BOOL	WINAPI	MBitBlt(__in HDC hdc, __in int x, __in int y, __in int cx, __in int cy, __in_opt HDC hdcSrc, __in int x1, __in int y1, __in DWORD rop)
{
	//	TextOutW(hdcSrc,x,y,_T("TTT"),3);
		return RBitBlt(hdc,x,y,cx,cy,hdcSrc,x1,y1,rop);
}

static BOOL  (WINAPI *RStretchBlt)(__in HDC hdcDest, __in int xDest, __in int yDest, __in int wDest, __in int hDest, __in_opt HDC hdcSrc, __in int xSrc, __in int ySrc, __in int wSrc, __in int hSrc, __in DWORD rop) = StretchBlt;
BOOL	WINAPI	MStretchBlt(__in HDC hdcDest, __in int xDest, __in int yDest, __in int wDest, __in int hDest, __in_opt HDC hdcSrc, __in int xSrc, __in int ySrc, __in int wSrc, __in int hSrc, __in DWORD rop)
{
		TextOutW(hdcDest,xDest,yDest,_T("TTT"),3);
		return RStretchBlt(hdcDest,xDest,yDest,wDest,hDest,hdcSrc,xSrc,ySrc,wSrc,hSrc,rop);
}


static	BOOL  (WINAPI *RExtTextOutA)( __in HDC hdc, __in int x, __in int y, __in UINT options, __in_opt CONST RECT * lprect, __in_ecount_opt(c) LPCSTR lpString, __in UINT c, __in_ecount_opt(c) CONST INT * lpDx) = ExtTextOutA;
BOOL	WINAPI	MExtTextOutA(__in HDC hdc, __in int x, __in int y, __in UINT options, __in_opt CONST RECT * lprect, __in_ecount_opt(c) LPCSTR lpString, __in UINT c, __in_ecount_opt(c) CONST INT * lpDx)
{
		return TRUE;
		//return RExtTextOutA(hdc,x,y,options,lprect,lpString,c,lpDx);
}
static	int (WINAPI *RDrawTextExW)(__in HDC hdc,__inout_ecount_opt(cchText) LPWSTR lpchText,__in int cchText,__inout LPRECT lprc,__in UINT format,__in_opt LPDRAWTEXTPARAMS lpdtp) = DrawTextExW;
int		WINAPI MDrawTextExW(__in HDC hdc,__inout_ecount_opt(cchText) LPWSTR lpchText,__in int cchText,__inout LPRECT lprc,__in UINT format,__in_opt LPDRAWTEXTPARAMS lpdtp)
{	
		if(cchText == 13){
			CString strTemp;
			strTemp.Format(_T("%s"),lpchText);

			CString str = _T("TRIAL");
			strTemp.MakeUpper();
			if(strTemp.Find(str) != -1){
				return TRUE;
			}
		}

		//if(cchText > 5) return TRUE;
		
		return RDrawTextExW(hdc,lpchText,cchText,lprc,format,lpdtp);
}

static	BOOL	(WINAPI *RExtTextOutW)(__in HDC hdc, __in int x, __in int y, __in UINT options, __in_opt CONST RECT * lprect, __in_ecount_opt(c) LPCWSTR lpString, __in UINT c, __in_ecount_opt(c) CONST INT * lpDx)=ExtTextOutW;
BOOL	WINAPI MExtTextOutW( __in HDC hdc, __in int x, __in int y, __in UINT options, __in_opt CONST RECT * lprect, __in_ecount_opt(c) LPCWSTR lpString, __in UINT c, __in_ecount_opt(c) CONST INT * lpDx)
{
		if((c >= 13) && (c <= 20)){
			InStreamW instrw;
			instrw << lpString;
			CString strTemp;
			strTemp.Format(_T("%s"),(WCHAR*)instrw.GetBuf());
			CString str = _T("TRIAL");
			strTemp.MakeUpper();
			if(strTemp.Find(str) != -1){
				return TRUE;
			}

			CStdioFile file;
			file.Open(_T("d:\\today.txt"),CStdioFile::modeCreate | CStdioFile::modeWrite);
			file.WriteString(strTemp);
		}

		return RExtTextOutW(hdc,x,y,options,lprect,lpString,c,lpDx);
}

int		CtodayApp::ExitInstance()
{
		dsExitSkin();
		g_str.Empty();

		//DetourDetach(&(PVOID&)RGetTextExtentPoint32W, MGetTextExtentPoint32W);
		//DetourDetach(&(PVOID&)RDrawTextW, MDrawTextW);
	//	DetourDetach(&(PVOID&)RDrawTextExW, MDrawTextExW);
		//DetourDetach(&(PVOID&)RStretchDIBitsR,MStretchDIBitsR);
		//DetourDetach(&(PVOID&)RAfxAssertFailedLine,MAfxAssertFailedLine);
		DetourDetach(&(PVOID&)RDrawTextExW, MDrawTextExW);
		return CWinAppEx::ExitInstance();
}

void	CtodayApp::Detours()
{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread()); 
		//DetourAttach(&(PVOID&)RGetTextExtentPoint32W, MGetTextExtentPoint32W);
		//DetourAttach(&(PVOID&)RDrawTextW, MDrawTextW);
	//	DetourAttach(&(PVOID&)RDrawTextExW, MDrawTextExW);
		//DetourAttach(&(PVOID&)RStretchDIBitsR,MStretchDIBitsR);
		//DetourAttach(&(PVOID&)RAfxAssertFailedLine,MAfxAssertFailedLine);
		DetourAttach(&(PVOID&)RDrawTextExW, MDrawTextExW);
		DetourTransactionCommit();
}

void	CtodayApp::SetHtmlCss()
{
		CString strPath ;
		IniFile ini;
		CStdioFile file;
		DWORD bCSS = 0;
		GetExePath(strPath);
		strPath.Replace(_T("today.exe"),_T(""));

		CString iniPath = strPath + _T("TODAY.I");
		ini.InitFile(iniPath.GetBuffer(0));
		bCSS = ini.GetValue(_T("TODASET"),_T("BCSS"),0);

		DWORD count = 0;
		CString htmPath = strPath + _T("ontentDialog1.ht");
		CString strWrite;
		CString strWrite1;
		CStringArray arrStr;
		if(!bCSS){
			if(file.Open(htmPath.GetBuffer(0),CStdioFile::modeReadWrite)){

				while(file.ReadString(htmPath)){
					arrStr.Add(htmPath);
					if(htmPath.Find(_T("</title>")) != -1){
						strWrite1 = strPath + _T("\\today\\style.css");
						strWrite.Format(_T("<link href='%s' rel='stylesheet' type='text/css' />"),strWrite1);
						arrStr.Add(strWrite);
					}
				}
				file.SeekToBegin();
				count = arrStr.GetSize();
				for(DWORD i = 0; i < count; i++){
					strWrite = arrStr.GetAt(i);
					file.WriteString(strWrite + _T("\n"));
				}
				file.Close();
				ini.SetValue(_T("TODASET"),_T("BCSS"),1);
			}
		}
}

HRESULT GetExeFileVersion(CString &strVersion)
{
		strVersion = _T("");
		HINSTANCE hInst = NULL; 
		hInst=AfxGetApp()->m_hInstance; 
		WCHAR path_buffer[_MAX_PATH];
		GetModuleFileName(hInst,path_buffer,sizeof(path_buffer));//得到exe文件的全路径

		VS_FIXEDFILEINFO vs_finfo;
		HRESULT err = GetFileVersion(path_buffer,&vs_finfo);
		if (err) 
			return err;

		strVersion.Format(_T("%d.%d.%d Build %d"),HIWORD(vs_finfo.dwFileVersionMS),
			LOWORD(vs_finfo.dwFileVersionMS),
			HIWORD(vs_finfo.dwFileVersionLS),
			LOWORD(vs_finfo.dwFileVersionLS));

		return S_OK;
}

BOOL	GetExePath(CString &strPath)
{
		strPath = _T("");
		HINSTANCE hInst = NULL; 
		hInst=AfxGetApp()->m_hInstance; 
		WCHAR path_buffer[_MAX_PATH];
		GetModuleFileName(hInst,path_buffer,sizeof(path_buffer));//得到exe文件的全路径

		strPath.Format(_T("%s"),path_buffer);
		return TRUE;
}

HRESULT	GetFileVersion(WCHAR *filename, VS_FIXEDFILEINFO *pvsf)
{
		DWORD dwHandle;
		DWORD cchver = ::GetFileVersionInfoSize(filename,&dwHandle);
		if (cchver == 0)
			return LastError();
		BYTE* pver = (BYTE*)malloc(cchver);
		BOOL bret = ::GetFileVersionInfo(filename,dwHandle,cchver,pver);
		if (!bret) {
			free(pver);
			return LastError();
		}
		UINT uLen;
		void *pbuf;
		bret = ::VerQueryValue(pver,_T("\\"),&pbuf,&uLen);
		if (!bret) {
			free(pver);
			return LastError();
		}
		memcpy(pvsf,pbuf,sizeof(VS_FIXEDFILEINFO));
		free(pver);
		return S_OK;
}

HRESULT		LastError() 
{
		HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
		if (SUCCEEDED(hr)) 
			return E_FAIL;
		return hr;
}

BOOL	SetWindowTop(CWnd* pWnd)
{
		if(!pWnd){
			return FALSE;
		}
		if(pWnd->GetExStyle()&WS_EX_TOPMOST){
			return TRUE;
		}
		else{
			int i= pWnd->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			if (i==0)
				return FALSE;
			else
				return TRUE;
		}
}

BOOL	CancelWindowTop(CWnd* pWnd)
{
		if(pWnd){
			int x=pWnd->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			if(x==0)
				return FALSE;
			else
				return TRUE;
		}
		else{
			return FALSE;
		}
}