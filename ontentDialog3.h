#pragma once
#ifndef CONTENTDIALOG3_H
#define CONTENTDIALOG3_H
#include "control/TodayHtmlDialog.h"

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// ContentDialog3 对话框

class ContentDialog3 : public TodayHtmlDialog
{
	DECLARE_DYNCREATE(ContentDialog3)

public:
	ContentDialog3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ContentDialog3();

	enum { IDD = IDD_DIALOG_CONTENT3, IDH = IDR_HTML_ONTENTDIALOG3 };

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
	afx_msg		void		OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg		BOOL		OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg		void		ReFreshItem();

	afx_msg		void		OnLookItem();
	afx_msg		void		OnRetweetedItem();
	afx_msg		void		OnCopyItem();
	afx_msg		void		OnDalogue();
	afx_msg		void		OnTimer(UINT_PTR nIDEvent);
	afx_msg		void		OnDel();

				BOOL		FindHtmlItem(IHTMLElement **pElement);
				void		OnShowItemImage(LPCSTR value,LPCSTR value1);
				void		OnUShowItemImage();
				void		OnShowBigImage(WCHAR *value);
				void		InitItem();
public:
	BOOL		m_bInit;
	CPoint		m_curPt;
	CString		m_curItemId;
	IHTMLElement	*m_pEle;
};
extern ContentDialog3 g_contentDialog3;
#endif