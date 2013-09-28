#define  TODAYMENU_CPP
#include "..//stdafx.h"
#include "TodayMenu.h"

IMPLEMENT_DYNCREATE(TodayMenu, CMFCVisualManagerOffice2007)
TodayMenu::TodayMenu(void)
{
}

TodayMenu::~TodayMenu(void)
{
}

void	TodayMenu::OnFillBarBackground(CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, BOOL bNCArea /* = FALSE */)
{		
		CPoint pt;
		pt.x = 5;
		pt.y = 5;
		rectClient.left -= 2;
		rectClient.top -= 2;
		rectClient.right += 2;
		rectClient.bottom += 2;

		BOOL bQuickMode = FALSE;
		CMFCPopupMenuBar* pMenuBar = DYNAMIC_DOWNCAST(CMFCPopupMenuBar, pBar);
		if (!pMenuBar->m_bDisableSideBarInXPMode){
			CWnd* pWnd = pMenuBar->GetParent();

			if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CMFCPopupMenu))){
				CMFCPopupMenu* pMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, pWnd);

				if (pMenu->IsCustomizePane()){
					bQuickMode = TRUE;
				}
			}

			pDC->RoundRect(&rectClient,pt);
			CRect rectImages = rectClient;
			if (bQuickMode){
				rectImages.right = rectImages.left + 2 * CMFCToolBar::GetMenuImageSize().cx + 4 * GetMenuImageMargin() + 4;
			}
			else{
				rectImages.right = rectImages.left + CMFCToolBar::GetMenuImageSize().cx + 2 * GetMenuImageMargin() + 2;
			}

			pDC->SelectObject(m_brBarBkgnd);
			pDC->RoundRect(rectImages,pt);
			CRect rect1;
			rect1.CopyRect(rectImages);
			rect1.right += 5;
			pDC->FillRect(&rect1,&m_brBarBkgnd);
			CRect rect(rectImages);
			rectImages.left = rectImages.right;
			rectImages.right += 2;
			DrawSeparator(pDC, rectImages, FALSE);
		}

}

void	TodayMenu::OnDrawMenuBorder(CDC* pDC, CMFCPopupMenu* pMenu, CRect rect)
{
		CRect tempRect;
		tempRect.CopyRect(&rect);

		CPoint pt;
		pt.x = 5;
		pt.y = 5;
		//pDC->SelectObject(m_penMenuBorder);
		//pDC->RoundRect(&tempRect,pt);
		pDC->SelectObject(m_penMenuItemBorder);
		pDC->SetBkMode(TRANSPARENT);
		pDC->RoundRect(&tempRect,pt);
}

void	TodayMenu::OnDrawButtonSeparator(CDC* pDC, CMFCToolBarButton* pButton, CRect rect, CMFCVisualManager::AFX_BUTTON_STATE state, BOOL bHorz)
{
		CMFCVisualManagerOffice2007::OnDrawButtonSeparator(pDC,pButton,rect,state,bHorz);
}

void	TodayMenu::OnHighlightMenuItem(CDC *pDC, CMFCToolBarMenuButton* pButton, CRect rect, COLORREF& clrText)
{
		pDC->FillRect(&rect,&m_brHighlightDn);
}

void	TodayMenu::OnFillBackground(CDC* pDC)
{
		return;
}

void	TodayMenu::OnDrawMenuShadow(CDC* pDC, const CRect& rectClient, const CRect& rectExclude, int nDepth, int iMinBrightness, int iMaxBrightness, CBitmap* pBmpSaveBottom, CBitmap* pBmpSaveRight, BOOL bRTL)
{
		return;
}

void	TodayMenu::DrawSeparator(CDC* pDC, const CRect& rect, BOOL bHorz)
{
		DrawSeparator(pDC, rect, m_penSeparator, m_penSeparator2, bHorz);
}

void	TodayMenu::DrawSeparator(CDC* pDC, const CRect& rect, CPen& pen1, CPen& pen2, BOOL bHorz)
{
		CRect rect1(rect);
		CRect rect2;

		if (bHorz){
			rect1.top += rect.Height() / 2 - 1;
			rect1.bottom = rect1.top;
			rect2 = rect1;
			rect2.OffsetRect(0, 1);
		}
		else{
			rect1.left += rect.Width() / 2 - 1;
			rect1.right = rect1.left;
			rect2 = rect1;
			rect2.OffsetRect(1, 0);
		}

		if (CMFCToolBarImages::m_bIsDrawOnGlass){
			CDrawingManager dm(*pDC);

			LOGPEN logpen;

			pen1.GetLogPen(&logpen);
			dm.DrawLine(rect1.left, rect1.top, rect1.right, rect1.bottom, logpen.lopnColor);

			pen2.GetLogPen(&logpen);
			dm.DrawLine(rect2.left, rect2.top, rect2.right, rect2.bottom, logpen.lopnColor);
		}
}

void	TodayMenu::OnUpdateSystemColors()
{
		CMFCVisualManagerOffice2007::OnUpdateSystemColors();

		//竖线
		m_penSeparator.DeleteObject();
		m_clrSeparator = RGB(200,200,255);
		m_penSeparator.CreatePen(PS_NULL, 1, m_clrSeparator);

		//ITEM间横线
		m_penSeparator2.DeleteObject();
		m_penSeparator2.CreatePen(PS_INSIDEFRAME,1,m_clrSeparator);

		//ITEM鼠标停放颜色
		m_clrHighlightDn = RGB(100,100,180);
		m_brHighlightDn.DeleteObject();
		m_brHighlightDn.CreateSolidBrush(m_clrHighlightDn);

		//菜单边框
		m_clrMenuItemBorder = RGB(180,180,180);
		m_penMenuItemBorder.DeleteObject();
		m_penMenuItemBorder.CreatePen(PS_SOLID, 1, m_clrMenuItemBorder);

		//边框
		m_penMenuBorder.DeleteObject();
		m_penMenuBorder.CreatePen(PS_SOLID, 3, RGB(200,200,200));


		m_clrMenuText = RGB(0,0,50);
		m_itemTextFont.DeleteObject();
		m_itemTextFont.CreateFont(
			15,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("Arial"));                 // lpszFacename()
		LOGFONT pF;
		m_itemTextFont.GetLogFont(&pF);
		afxGlobalData.SetMenuFont(&pF,TRUE);
		m_ctrlMenuHighlighted[0].CleanUp();
		m_ctrlMenuHighlighted[1].CleanUp();
}

//////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(TodayPopupMenu, CMFCPopupMenu)
TodayPopupMenu::TodayPopupMenu()
{
		//m_iShadowSize = 0;
}

TodayPopupMenu::~TodayPopupMenu()
{

}

void	TodayPopupMenu::DoPaint(CDC* pPaintDC)
{
		m_iShadowSize = 0;
		CMFCPopupMenu::DoPaint(pPaintDC);
}

///////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(TodayContextMenuManager, CContextMenuManager)
TodayContextMenuManager::TodayContextMenuManager()
{
}

TodayContextMenuManager::~TodayContextMenuManager()
{

}

CMFCPopupMenu	*TodayContextMenuManager::ShowPopupMenu(HMENU hmenuPopup, int x, int y, CWnd* pWndOwner, BOOL bOwnMessage /* = FALSE */, BOOL bAutoDestroy /* = TRUE */, BOOL bRightAlign /* = FALSE */)
{
		if (pWndOwner != NULL && pWndOwner->IsKindOf(RUNTIME_CLASS(CDialogEx)) && !bOwnMessage){
			ASSERT(FALSE);
			return NULL;
		}

		if (pWndOwner != NULL && pWndOwner->IsKindOf(RUNTIME_CLASS(CMFCPropertyPage)) && !bOwnMessage){
			ASSERT(FALSE);
			return NULL;
		}

		if (m_bTrackMode){
			bOwnMessage = TRUE;
		}

		if (!bOwnMessage){
			while (pWndOwner != NULL && pWndOwner->GetStyle() & WS_CHILD){
				pWndOwner = pWndOwner->GetParent();
			}
		}

		TodayPopupMenu* pPopupMenu = new TodayPopupMenu;
		pPopupMenu->SetAutoDestroy(FALSE);

		pPopupMenu->SetTrackMode(m_bTrackMode);
		pPopupMenu->SetRightAlign(bRightAlign);

		CMFCPopupMenu* pMenuActive = TodayPopupMenu::GetActiveMenu();
		if (!m_bDontCloseActiveMenu && pMenuActive != NULL){
			pMenuActive->SendMessage(WM_CLOSE);
		}

		if (!pPopupMenu->Create(pWndOwner, x, y, hmenuPopup, FALSE, bOwnMessage)){
			return NULL;
		}

		return pPopupMenu;
}
