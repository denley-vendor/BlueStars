#pragma once
#ifndef TODAYHTMLDIALOG_H
#define TODAYHTMLDIALOG_H

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

class TodayHtmlDialog : public CDHtmlDialog
{
	/*DECLARE_DYNCREATE(TodayHtmlDialog)*/

public:
	/*TodayHtmlDialog(CWnd* pParent = NULL); */
	TodayHtmlDialog(UINT nIDTemplate, UINT nHtmlResID = 0, CWnd *pParentWnd = NULL);
	virtual ~TodayHtmlDialog();

public:
	STDMETHOD(GetExternal)(IDispatch **ppDispatch);
	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);

	virtual		void		DoDataExchange(CDataExchange* pDX); 
	virtual		BOOL		OnInitDialog();
	virtual		void		UpdateVscroll();
	virtual		void		UpdateWebWidth();
				LONG		GetHtmlHeight();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	//DECLARE_DISPATCH_MAP()
public:
	afx_msg		void		OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg		BOOL		OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg		int			OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg		void		ReFresh();
	afx_msg		LRESULT		ReFresh(WPARAM wp, LPARAM lp);
	afx_msg		BOOL		OnEraseBkgnd(CDC* pDC);
	afx_msg		LRESULT		ExecScript(WPARAM wp, LPARAM lp);
				void		ExecScript(CString *pStrJS);
	afx_msg		LRESULT		AddHtmlString(WPARAM wp, LPARAM lp);	
				void		AddHtmlString(WCHAR *pStr,BOOL bEnd = FALSE);
	afx_msg		LRESULT		SetHtmlString(WPARAM wp, LPARAM lp);
				void		SetHtmlString(WCHAR *pStr);
				void		AddHeadString(CString *pInStr);
				BOOL		GetElementByName(CString strName,IHTMLElement **pElement);

				void		CloseMenu();
				void		Clear();

public:
	 CMFCPopupMenu		*m_pMenu;
			 BOOL		m_bMenu;
};
#endif