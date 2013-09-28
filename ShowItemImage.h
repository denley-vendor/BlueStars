#pragma once
#ifndef SHOWITEMIMAGE_H
#define SHOWITEMIMAGE_H
#include "control/TodayHtmlDialog.h"
#ifdef _WIN32_WCE
#error "Windows CE ²»Ö§³Ö CDHtmlDialog¡£"
#endif 

#define  SHOWUSERONFO_CLOSE_TIME 888
#define  SHOWUSERONFO_SHOW_TIME  666
class ShowItemImage : public TodayHtmlDialog
{
	DECLARE_DYNCREATE(ShowItemImage)

public:
	ShowItemImage(CWnd* pParent = NULL);   
	virtual ~ShowItemImage();

	enum { IDD = IDD_DIALOG_SHOWPIC, IDH = IDR_HTML_SHOWITEMIMAGE };

protected:
	virtual		void		DoDataExchange(CDataExchange* pDX);   
	virtual		BOOL		OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
public:
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
				void		SetWindowShow(BOOL b = FALSE);
				BOOL		CanClose();
				void		OnShowBigImage(WCHAR *pInUrl);

				void		OnShowImage(WCHAR *pChar);

				CString		m_strImage;
				CString		m_strImage2;
};

extern ShowItemImage g_showItemImage;
#endif
