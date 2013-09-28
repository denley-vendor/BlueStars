/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define EXCEPTIONDG_CPP

#include "stdafx.h"
#include "today.h"
#include "ExceptionDlg.h"
#include <DbgHelp.h>
#include <sstream>
#include <iomanip>
#include "Net/Mail.h"
#pragma comment( lib, "Dbghelp.lib" )

Exception g_Exception;
Exception::Exception()
{

}

Exception::~Exception()
{

}

void	Exception::InitException()
{
		::SetUnhandledExceptionFilter(Exception::OnUnhandledException);
}

LONG	Exception::OnHandledException(EXCEPTION_POINTERS *pExceptionInfo)
{
	#ifdef _DEBUG
		return EXCEPTION_CONTINUE_SEARCH;
	#endif

		if(pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW) {
			void* pOldStack = NULL;
			void *pVoid = malloc(1048576);
			BYTE *pNewStack = (BYTE*)pVoid;
			pNewStack += 1048576;
			__asm mov pOldStack,esp;
			__asm mov esp,pNewStack;

			g_Exception.SaveException(pExceptionInfo);

			__asm mov esp,pOldStack;
			free(pVoid);

			return EXCEPTION_CONTINUE_SEARCH;
		}
		else {
			g_Exception.SaveException(pExceptionInfo,EXCEPTION_TRIGGER_RUN);
		}

		return EXCEPTION_EXECUTE_HANDLER;
}

LONG	Exception::OnUnhandledException(EXCEPTION_POINTERS *pExceptionInfo)
{
	#ifdef _DEBUG
		return EXCEPTION_CONTINUE_SEARCH;
	#endif

		if(pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW) {
			void* pOldStack = NULL;
			void *pVoid = malloc(1048576);
			BYTE *pNewStack = (BYTE*)pVoid;
			pNewStack += 1048576;
			__asm mov pOldStack,esp;
			__asm mov esp,pNewStack;

			g_Exception.SaveException(pExceptionInfo);

			__asm mov esp,pOldStack;
			free(pVoid);

			return EXCEPTION_CONTINUE_SEARCH;
		}
		else {
			g_Exception.SaveException(pExceptionInfo,EXCEPTION_TRIGGER_INIT);
		}

		return EXCEPTION_EXECUTE_HANDLER;
}

BOOL	Exception::SaveException(EXCEPTION_POINTERS *pExceptionInfo,LONG triggerPosition /* = EXCEPTION_TRIGGER_INIT */)
{
		CString sVersion;
		GetExeFileVersion(sVersion);

		::CreateDirectory(_T("Log\\"),NULL);
		CTime t = CTime::GetCurrentTime();

		using namespace std;
		wostringstream filename;
		filename << _T("Log\\")
			<< _T("Dmp_") << sVersion.GetBuffer(0)
			<< _T("_") << setw(4) << setfill(_T('0')) << t.GetYear()
			<< _T("-") << setw(2) << setfill(_T('0')) << t.GetMonth()
			<< _T("-") << setw(2) << setfill(_T('0')) << t.GetDay()
			<< _T("-") << setw(2) << setfill(_T('0')) << t.GetHour()
			<< _T("-") << setw(2) << setfill(_T('0')) << t.GetMinute()
			<< _T("-") << setw(2) << setfill(_T('0')) << t.GetSecond() 
			<< _T("_") << setw(5) << setfill(_T('0')) << _T(".dmp") << ends;

		WCHAR errstr[MAX_PATH];
		memset(errstr,0,sizeof(errstr));

		HANDLE hFile = ::CreateFile(filename.str().c_str(),GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hFile == INVALID_HANDLE_VALUE) {
			swprintf_s(errstr,MAX_PATH,_T("Failed to create dump file '%s' (error %d)"),filename.str().c_str(),GetLastError());
		}
		else {
			_MINIDUMP_EXCEPTION_INFORMATION ExInfo;
			ExInfo.ThreadId = ::GetCurrentThreadId();
			ExInfo.ExceptionPointers = pExceptionInfo;
			ExInfo.ClientPointers = NULL;

			if(!::MiniDumpWriteDump(GetCurrentProcess(),GetCurrentProcessId(),hFile,MiniDumpWithIndirectlyReferencedMemory,&ExInfo,NULL,NULL)) {
				swprintf_s(errstr,MAX_PATH,_T("Failed to save dump file to '%s' (error %d)"),filename.str().c_str(),GetLastError());
			}
			::CloseHandle(hFile);
		}

		ExceptionDlg dlg;
		dlg.DoModal();
		if(dlg.m_bSendReprot){
			const CHAR *pInfo = "id";
			const CHAR *pUserId = g_WeiBoData.GetLonginserInfo(pInfo);
			CString strVersion;
			GetExeFileVersion(strVersion);
			InStreamW stream,streamSubject;
			streamSubject << strVersion.GetBuffer(0);

			strVersion.Format(_T("USERID:%S\n"),pUserId);
			MAIL_DATA mailData;
			mailData.sMailFromNickname = strVersion.GetBuffer(0);
			mailData.sSubject = (WCHAR*)streamSubject.GetBuf();
			mailData.sMessage = strVersion + dlg.m_strText.GetBuffer(0);
			mailData.sMailFilePath = filename.str().c_str();

			MailClient mail;		
			if (mail.InitData(NULL,&mailData)) {
				mail.MailSend();			
			}
		}
		if(dlg.m_bRestart){
			CString strPath;
			GetExePath(strPath);
			ShellExecute(NULL,_T("open"),strPath.GetBuffer(0),NULL,NULL,SW_SHOW);
		}
		return TRUE;
}

// ExceptionDlg 对话框
IMPLEMENT_DYNAMIC(ExceptionDlg, CDialog)

ExceptionDlg::ExceptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ExceptionDlg::IDD, pParent)
	, m_bRestart(TRUE)
	, m_bSendReprot(TRUE)
{
}

ExceptionDlg::~ExceptionDlg()
{
}

void	ExceptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_RESTART, m_bRestart);
	DDX_Check(pDX, IDC_CHECK_SEMDREPROT, m_bSendReprot);
}


BEGIN_MESSAGE_MAP(ExceptionDlg, CDialog)
	ON_BN_CLICKED(IDOK, &ExceptionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


void	ExceptionDlg::OnBnClickedOk()
{
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
		if(pEdit){
			pEdit->GetWindowText(m_strText);
		}
		OnOK();
}

BOOL	ExceptionDlg::OnInitDialog()
{
		CDialog::OnInitDialog();

		CString strErrorDes;
		AfxLoadString(IDS_ERROR_DES,strErrorDes.GetBuffer(0));
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
		if(pEdit){
			pEdit->SetWindowText(strErrorDes);
		}

		return TRUE;  
}
