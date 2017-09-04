#pragma once
#ifndef EXCEPTIONDLG_H
#define EXCEPTIONDLG_H

enum {
	EXCEPTION_TRIGGER_INIT	=	0,
	EXCEPTION_TRIGGER_RUN	=	1,
};

class Exception
{
public:
			BOOL		m_bRestart;
public:
						Exception();
						~Exception();
			void		InitException();
	static	LONG WINAPI	OnUnhandledException(EXCEPTION_POINTERS *pExceptionInfo);
	static	LONG WINAPI	OnHandledException(EXCEPTION_POINTERS *pExceptionInfo);

			BOOL		SaveException(EXCEPTION_POINTERS *pExceptionInfo,LONG triggerPosition = EXCEPTION_TRIGGER_INIT);
};
extern Exception g_Exception;

// ExceptionDlg 对话框
class ExceptionDlg : public CDialog
{
	DECLARE_DYNAMIC(ExceptionDlg)

public:
	ExceptionDlg(CWnd* pParent = NULL);   
	virtual ~ExceptionDlg();

	enum { IDD = IDD_DIALOG_EXCEPTION };

protected:
	virtual		void		DoDataExchange(CDataExchange* pDX);  
	virtual		BOOL		OnInitDialog();
	afx_msg		void		OnBnClickedOk();

	DECLARE_MESSAGE_MAP()
public:
	BOOL	m_bRestart;
	BOOL	m_bSendReprot;
	CString m_strText;
};

#endif