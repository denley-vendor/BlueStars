#pragma once
#ifndef CTODAYMAIN_H
#define CTODAYMAIN_H
#include "control/TodayHtmlDialog.h"
#include "DialogShowUserInfo.h"
#include "TodayColorDlg.h"
#include "MainTitleImage.h"
#include "DialogPublish.h"

class CTodayMain : public CDialog
{
		DECLARE_DYNAMIC(CTodayMain)

public:
		CTodayMain(CWnd* pParent = NULL); 
		virtual ~CTodayMain();


		enum { IDD = IDD_TODAY_MAIN };

protected:
	virtual		void		DoDataExchange(CDataExchange* pDX);   

		DECLARE_MESSAGE_MAP()
private:
				void		HideAllContentDialg();
				void		OnMoveWindow();
				void		OnMoveSelectButton();
				void		OnMoveEdit();
public:
	afx_msg		void		OnBnClickedButtonMini();
	afx_msg		void		OnBnClickedButtonMax();
	afx_msg		void		OnBnClickedButtonContent1();
	afx_msg		void		OnBnClickedButtonContent2();
	afx_msg		void		OnBnClickedButtonContent3();
	afx_msg		void		OnBnClickedButtonContent4();
	afx_msg		void		OnBnClickedButtonContent5();
	afx_msg		void		OnBnClickedButtonContent6();
	afx_msg		void		OnBnClickedButtonClose();
	afx_msg		void		OnBnClickedButtonPublish();
	virtual		BOOL		OnInitDialog();
	virtual		BOOL		PreTranslateMessage(MSG* pMsg);
	afx_msg		void		OnMove(int x, int y);
	afx_msg		void		OnSize(UINT nType, int cx, int cy);
	afx_msg		HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg		void		OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg		LRESULT		OnNcHitTest(CPoint point);
	afx_msg		BOOL		OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg		void		OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg		BOOL		OnEraseBkgnd(CDC* pDC);

	afx_msg		void		OnBnClickedButtonColerset();
	afx_msg		void		OnDestroy();
	inline		DWORD		GetSwitchIndex(){return m_curIndex;}
				void		SwitchDialog(DWORD s);
				void		SetUserName(CString *pStrName);
				void		SetUserImage(CString *pPath);
				void		SetFollowers(CString &str);
				void		SetStatuses(CString &str);
				void		SetFavourites(CString &str);
				void		ReFreshNow();
				void		SetWin7();
private:
				TodayColorDlg		*m_pColorDlg;
				TodayHtmlDialog		*m_pCurrentWnd;
				CStatic				*m_pStatic;
				CButton				m_Colse;
				CButton				m_btMax;
				CButton 			m_btMini;
				DWORD				m_curIndex;

				MainTitleImage		m_TitleImage;
				DialogPublish		m_Publish;

};

extern	CTodayMain		g_todayMainDialog;

#endif