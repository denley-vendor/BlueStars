#pragma once
#ifndef TODAYMENU_H
#define TODAYMENU_H


class TodayMenu : public CMFCVisualManagerOffice2007
{ 
	DECLARE_DYNCREATE(TodayMenu)
public:
	TodayMenu(void);
	~TodayMenu(void);
public:
	virtual		void		OnFillBarBackground(CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, BOOL bNCArea = FALSE);	
	virtual		void		OnDrawMenuBorder(CDC* pDC, CMFCPopupMenu* pMenu, CRect rect);
	virtual		void		OnDrawButtonSeparator(CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state, BOOL bHorz);
	virtual		void		OnHighlightMenuItem(CDC *pDC, CMFCToolBarMenuButton* pButton, CRect rect, COLORREF& clrText);
	virtual		void		OnFillBackground(CDC* pDC);
	virtual		void		OnDrawMenuShadow(CDC* pDC, const CRect& rectClient, const CRect& rectExclude, int nDepth,
								int iMinBrightness, int iMaxBrightness, CBitmap* pBmpSaveBottom,  CBitmap* pBmpSaveRight, BOOL bRTL);
				void		DrawSeparator(CDC* pDC, const CRect& rect, BOOL bHorz);
				void		DrawSeparator(CDC* pDC, const CRect& rect, CPen& pen1, CPen& pen2, BOOL bHorz);
	virtual		void		OnUpdateSystemColors();
private:
	CPen	m_penMenuBorder;
	CFont	m_itemTextFont;
};

///////////////////////////
class TodayPopupMenu : public CMFCPopupMenu
{
	DECLARE_DYNCREATE(TodayPopupMenu)
public:
	TodayPopupMenu();
	~TodayPopupMenu();
public:
	inline		void		SetTrackMode(BOOL b){m_bTrackMode = b;}
	virtual		void		DoPaint(CDC* pPaintDC);
};

///////////////////////////
class TodayContextMenuManager : public CContextMenuManager
{
	DECLARE_DYNCREATE(TodayContextMenuManager)
public:
	TodayContextMenuManager();
	~TodayContextMenuManager();
public:
	virtual		CMFCPopupMenu		*ShowPopupMenu(HMENU hmenuPopup, int x, int y, CWnd* pWndOwner, BOOL bOwnMessage = FALSE,
										BOOL bAutoDestroy = TRUE, BOOL bRightAlign = FALSE);
};
#endif