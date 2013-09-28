#pragma once
#include "afxbutton.h"
#ifndef TODAY_COLOR_DLG_H
#define TODAY_COLOR_DLG_H


class TodayColorDlg : public CDialog
{
	DECLARE_DYNAMIC(TodayColorDlg)

public:
	TodayColorDlg(CWnd* pParent = NULL);  
	virtual ~TodayColorDlg();

	enum { IDD = IDD_DIALOG_COLORSET };

protected:
	virtual		void		DoDataExchange(CDataExchange* pDX);   
	DECLARE_MESSAGE_MAP()
private:
public:
	virtual		BOOL		OnInitDialog();
	afx_msg		void		OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg		BOOL		OnNcActivate(BOOL bActive);
	afx_msg		BOOL		OnEraseBkgnd(CDC* pDC);
				void		RGBtoHSI(BYTE   r,BYTE  g,BYTE b,double& h,double& s,double& i) ;
	afx_msg		void		OnBnClickedButtonColor1();
	afx_msg		void		OnBnClickedButtonColor2();
	afx_msg		void		OnBnClickedButtonColor3();
	afx_msg		void		OnBnClickedButtonColor4();
	afx_msg		void		OnBnClickedButtonColor5();
	afx_msg		void		OnBnClickedButtonColor6();
	afx_msg		void		OnBnClickedButtonColor7();
	afx_msg		void		OnBnClickedButtonColor8();
	afx_msg		void		OnBnClickedButtonColorreset();
};

#endif