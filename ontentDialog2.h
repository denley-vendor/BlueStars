#pragma once
#ifndef CONTENTDIALOG2_H
#define CONTENTDIALOG2_H
#include "control/TodayHtmlDialog.h"

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

class ContentDialog2 : public TodayHtmlDialog
{
	DECLARE_DYNCREATE(ContentDialog2)

public:
	ContentDialog2(CWnd* pParent = NULL);  
	virtual ~ContentDialog2();

	enum { IDD = IDD_DIALOG_CONTENT2, IDH = IDR_HTML_ONTENTDIALOG2 };

protected:
	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);

	virtual		void		DoDataExchange(CDataExchange* pDX); 
	virtual		BOOL		OnInitDialog();
	virtual		BOOL		PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
public:
	afx_msg		int			OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg		void		ReFreshItem();
	afx_msg		BOOL		OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg		void		OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg		void		OnRetweetedItem();
	afx_msg		void		OnLookItem();
	afx_msg		void		OnCopyItem();
	afx_msg		void		OnDalogue();
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);

				void		OnShowUserInfo(LPCSTR value);
				void		OnUShowUserInfo();

				void		OnShowItemImage(LPCSTR value,LPCSTR value1);
				void		OnUShowItemImage();
				void		OnShowBigImage(WCHAR *value);
				void		ItemInit();


public:
	BOOL		m_bInit;
	CPoint		m_curPt;
	CString		m_curItemId;
};
extern ContentDialog2 g_contentDialog2;
#endif