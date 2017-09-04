#pragma once
#ifndef CONTENTDIALOG1_H
#define CONTENTDIALOG1_H
#include "control/TodayHtmlDialog.h"
#include "DialogRetweeted.h"

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

class ContentDialog1 : public TodayHtmlDialog
{
	DECLARE_DYNCREATE(ContentDialog1)

public:
	ContentDialog1(CWnd* pParent = NULL);   
	virtual		~ContentDialog1();

				void		OnCloseMenu();
				void		OnShowUserInfo(LPCSTR value);
				void		OnUShowUserInfo();
				void		OnOpenUrl(LPCSTR value);
				void		OnShowItemImage(LPCSTR value,LPCSTR value1);
				void		OnUShowItemImage();
				void		OnShowBigImage(WCHAR *value);
				STDMETHOD(GetExternal)(IDispatch **ppDispatch);
				STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);

	enum { IDD = IDD_DIALOG_CONTENT1, IDH = IDR_HTML_ONTENTDIALOG1 };

public:
	virtual		void		DoDataExchange(CDataExchange* pDX);    
	virtual		BOOL		OnInitDialog();
	virtual		BOOL		PreTranslateMessage(MSG* pMsg);
	afx_msg		int			OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg		void		OnKillFocus(CWnd* pNewWnd);
	afx_msg		void		OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg		void	    OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg		BOOL		OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg		void		OnRetweetedItem();
	afx_msg		void		OnLookItem();
	afx_msg		void		OnCopyItem();
	afx_msg		void		OnDalogue();
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);

				void		ReFreshItem();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
private:
		CPoint			m_curPt;
		CString			m_curItemId;
};
extern ContentDialog1 g_contentDialog1;
#endif
