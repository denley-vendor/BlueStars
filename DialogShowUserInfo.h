#pragma once
#ifndef DIALOG_SHOW_USERINFO_H
#define DIALOG_SHOW_USERINFO_H

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

#include "control/TodayHtmlDialog.h"
#define  SHOWUSERONFO_CLOSE_TIME 888
#define  SHOWUSERONFO_SHOW_TIME  666

class CDialogShowUserInfo : public TodayHtmlDialog
{
	DECLARE_DYNCREATE(CDialogShowUserInfo)

public:
	CDialogShowUserInfo(CWnd* pParent = NULL);  
	virtual ~CDialogShowUserInfo();

	enum { IDD = IDD_DIALOG_SHOW_USERINFO, IDH = IDR_HTML_DIALOGSHOWUSERINFO };

protected:
	virtual		void		DoDataExchange(CDataExchange* pDX);   
	virtual		BOOL		OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg		void		OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
	afx_msg		BOOL		OnEraseBkgnd(CDC* pDC);
	afx_msg		int			OnCreate(LPCREATESTRUCT lpCreateStruct);
				void		SetWindowShow(BOOL b = FALSE);
				BOOL		CanClose();

				void		ShowUserInfo(WCHAR *pCharUserId);

public:
	BOOL		m_bMouseOn;
	CString		m_strID;
};

extern CDialogShowUserInfo g_showUserInfo;
#endif