#pragma once
#ifndef CTODAYDLG_H
#define CTODAYDLG_H
class CtodayDlg : public CDialog
{
public:
	CtodayDlg(CWnd* pParent = NULL);	
	virtual	~CtodayDlg();

	enum { IDD = IDD_TODAY_DIALOG };

protected:
	virtual		void	DoDataExchange(CDataExchange* pDX);	
public:
	virtual		BOOL	OnInitDialog();
	virtual		BOOL	PreTranslateMessage(MSG* pMsg);
	afx_msg		void	OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg		void	OnPaint();
	afx_msg		BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg		HCURSOR OnQueryDragIcon();
	afx_msg		LRESULT	OnTrayNotify(WPARAM wp, LPARAM lp);		
	afx_msg		void	OnClickedLogin();
	afx_msg		void	OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg		void	OnBnClickedButtonClose();
	afx_msg		void	OnBnClickedButtonMini();
	afx_msg		void	ReFreshCContent1();
	afx_msg		void	ReFreshCContent2();
				void	OnTrayContextMenus();
				void	OnReLogin();
	afx_msg		void	OnShowMainWindow();
	afx_msg		LRESULT	InitContent1Item(WPARAM wp, LPARAM lp);
	afx_msg		void	OnPublish();
	afx_msg		void	About();
	afx_msg		void	OnNMClickSyslinkLogin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg		void	OnStnClickedSyslinkLogin();
	afx_msg		void	OnStnClickedStaticReg();
	afx_msg		void	OnStnClickedStaticFor();
	afx_msg		void	OnStnClickedStaticAbout();
	inline		void	SetBLogin(BOOL b){m_bLogin = b;}
				void	ShowLoginError();
				void	RememberPW();
				void	RememberOauth();
				void	ShowPW();
				void	ShowOauth();
				CString	XXXPW(WCHAR *pIn,WCHAR *pIn1);
				CString	YYYPW(WCHAR *pIn,WCHAR *pIn1);
				CString CCCPW(WCHAR *pIn);
				void	MachineStartShow();
				void	MachineStartSet();

DECLARE_MESSAGE_MAP()
public:
	virtual		void	OnOK();
protected:
			HICON		m_hIcon;
	NOTIFYICONDATA		m_nid;
			CImage		m_image;
			BOOL		m_bInit1;
			int			m_bRemeberPw;
			int			m_bAutoLogin;
			int			m_bMachine;
			BOOL		m_bLogin;

			InStreamA	m_tkey;
			InStreamA   m_tsecret ; 
};
#endif