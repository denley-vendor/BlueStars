// today.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "detours/detours.h"
//#pragma comment(lib,"Detoured.lib")
#pragma comment( lib, "version.lib" )

#define		UM_TRAYNOTIFY		(WM_USER + 100)
#define		REFRESHCCONTENT1	(WM_USER + 101)
#define		MSG_ADDHTMLSTR		(WM_USER + 102)
#define		MSG_EXECSCRIPT		(WM_USER + 103)
#define		MSG_HTMLREFRESH		(WM_USER + 104)
#define		MSG_SETHTMLSTR		(WM_USER + 105)
#define		MSG_RETWWEETED_ITEM	(WM_USER + 106)

#define		REFRESH_TIME		888

class CtodayApp : public CWinAppEx
{
public:
	CtodayApp();

// ��д
	public:
	virtual		BOOL		InitInstance();
	virtual		int			ExitInstance();
				void		SetHtmlCss();
				void		Detours();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtodayApp theApp;

extern	HRESULT 		GetExeFileVersion(CString &strVersion);
extern	HRESULT			GetFileVersion(WCHAR *filename, VS_FIXEDFILEINFO *pvsf);
extern	HRESULT			LastError();
extern	BOOL			GetExePath(CString &strPath);
extern	BOOL			SetWindowTop(CWnd* pWnd);
extern	BOOL			CancelWindowTop(CWnd* pWnd);