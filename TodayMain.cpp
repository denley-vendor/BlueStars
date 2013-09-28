/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define CTODAYMAIN_CPP
#include "stdafx.h"
#include "today.h"
#include "todayDlg.h"
#include "TodayMain.h"
#include "ontentDialog1.h"
#include "ontentDialog2.h"
#include "ontentDialog3.h"
#include "ontentDialog4.h"
#include "ontentDialog5.h"
#include "ontentDialog6.h"
#include "ShowItemImage.h"
#include "DialogRetweeted.h"

IMPLEMENT_DYNAMIC(CTodayMain, CDialog)

CTodayMain::CTodayMain(CWnd* pParent /*=NULL*/)
	: CDialog(CTodayMain::IDD, pParent)
{
		m_pStatic = NULL;
		m_pCurrentWnd = NULL;
		m_pColorDlg = NULL;
		m_curIndex = 0;
}

CTodayMain::~CTodayMain()
{
}

void	CTodayMain::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_LONGIN_HAEDPIC, m_TitleImage);
		//DDX_Control(pDX,IDC_BUTTON_CLOSE,m_Colse);
		//DDX_Control(pDX, IDC_BUTTON_MAX, m_btMax);
		//DDX_Control(pDX, IDC_BUTTON_MINI, m_btMini);
}


BEGIN_MESSAGE_MAP(CTodayMain, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONTENT1, &CTodayMain::OnBnClickedButtonContent1)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CTodayMain::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_CONTENT2, &CTodayMain::OnBnClickedButtonContent2)
	ON_BN_CLICKED(IDC_BUTTON_CONTENT3, &CTodayMain::OnBnClickedButtonContent3)
	ON_BN_CLICKED(IDC_BUTTON_CONTENT4, &CTodayMain::OnBnClickedButtonContent4)
	ON_BN_CLICKED(IDC_BUTTON_CONTENT5, &CTodayMain::OnBnClickedButtonContent5)
	ON_BN_CLICKED(IDC_BUTTON_CONTENT6, &CTodayMain::OnBnClickedButtonContent6)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_MINI, &CTodayMain::OnBnClickedButtonMini)
	ON_BN_CLICKED(IDC_BUTTON_MAX, &CTodayMain::OnBnClickedButtonMax)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_COLERSET, &CTodayMain::OnBnClickedButtonColerset)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_PUBLISH, &CTodayMain::OnBnClickedButtonPublish)
END_MESSAGE_MAP()


CTodayMain	g_todayMainDialog;

void	CTodayMain::HideAllContentDialg()
{
		g_contentDialog1.ShowWindow(SW_HIDE);
		g_contentDialog2.ShowWindow(SW_HIDE);
		g_contentDialog3.ShowWindow(SW_HIDE);
		g_contentDialog4.ShowWindow(SW_HIDE);
		g_contentDialog5.ShowWindow(SW_HIDE);
		g_contentDialog6.ShowWindow(SW_HIDE);
}

void	CTodayMain::SwitchDialog(DWORD s)
{
		m_curIndex = s;
		TodayHtmlDialog *pWnd = NULL;
		switch (s){
		case 1:
			pWnd = &g_contentDialog1;
			break;
		case 2:
			pWnd = &g_contentDialog2;
			break;
		case 3:
			pWnd = &g_contentDialog3;
			break;
		case 4:
			pWnd = &g_contentDialog4;
			break;
		case 5:
			pWnd = &g_contentDialog5;
			break;
		case 6:
			pWnd = &g_contentDialog6;
			break;
		default: 
			if(m_pCurrentWnd) m_pCurrentWnd->ShowWindow(SW_HIDE);
			break;
		}
		if(pWnd != m_pCurrentWnd){
			if(m_pStatic && pWnd){
				CRect rect;
				m_pStatic->GetWindowRect(&rect);
				pWnd->MoveWindow(&rect);
				pWnd->ShowWindow(SW_SHOW);
			}
			if(m_pCurrentWnd) m_pCurrentWnd->ShowWindow(SW_HIDE);
			m_pCurrentWnd = pWnd;
		}
		else{
			if(m_pStatic && m_pCurrentWnd){
				CRect rect;
				m_pStatic->GetWindowRect(&rect);
				m_pCurrentWnd->MoveWindow(&rect);
				m_pCurrentWnd->ShowWindow(SW_SHOW);
			}
		}
}

void	CTodayMain::OnMoveWindow()
{
		CRect rect;
		CRect rect1;
		CRect rect2;
		CRect rect3;
		CRect rect4;
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_CLOSE);
		CButton *pButtonMax = (CButton*)GetDlgItem(IDC_BUTTON_MAX);
		CButton *pButtonMin = (CButton*)GetDlgItem(IDC_BUTTON_MINI);
		GetWindowRect(rect1);
		if(pButton){
			pButton->GetWindowRect(rect);
			rect.left = rect1.right - rect.right + rect.left;
			rect.right = rect1.right;
			ScreenToClient(rect);
			rect.top = -1;
			rect2.CopyRect(&rect);
		}
		if(pButtonMax && pButton){
			rect1.CopyRect(&rect);
			ClientToScreen(&rect1);
			pButtonMax->GetWindowRect(rect);
			rect.left = rect1.left - (rect.right - rect.left);
			rect.right = rect1.left;
			ScreenToClient(rect);
			rect.top =  -1;
			rect3.CopyRect(&rect);
		}
		if(pButtonMin && pButtonMax){
			rect1.CopyRect(&rect);
			ClientToScreen(&rect1);
			pButtonMin->GetWindowRect(rect);
			rect.left = rect1.left - (rect.right - rect.left);
			rect.right = rect1.left;
			ScreenToClient(rect);
			rect.top =  -1;
			rect4.CopyRect(&rect);
		}
		if(pButtonMin) {
			pButtonMin->MoveWindow(&rect4);
		}

		if(pButtonMax) {
			pButtonMax->MoveWindow(&rect3);
		} 
		if(pButton){
			pButton->MoveWindow(&rect2);
		}

		//发布
		pButton = (CButton *)GetDlgItem(IDC_BUTTON_PUBLISH);
		if(pButton){
			GetWindowRect(rect1);
			//ClientToScreen(&rect1);
			rect1.right -= 2;
			pButton->GetWindowRect(rect);
			rect.left = rect1.right - (rect.right - rect.left);
			rect.right = rect1.right;
			ScreenToClient(&rect);
			pButton->MoveWindow(rect);
		}
}

void	CTodayMain::OnMoveSelectButton()
{
		CRect rect1;
		CRect rect2;
		CButton *pButton1 = (CButton*)GetDlgItem(IDC_BUTTON_CONTENT6);
		//CButton *pButton2 = (CButton*)GetDlgItem(IDC_BUTTON_CONTENT3);
		//if(pButton2  && pButton1){
		//	GetWindowRect(rect1);
		//	pButton2->GetWindowRect(rect2);
		//	rect1.left = rect2.right;
		//	rect1.top = rect2.top;
		//	rect1.bottom = rect2.bottom;
		//	ScreenToClient(&rect1);
		//	pButton1->MoveWindow(rect1);
		//}

		pButton1 = (CButton *)GetDlgItem(IDC_BUTTON_COLERSET);
		if(pButton1){
			DWORD w = 0;
			GetWindowRect(rect1);
			pButton1->GetWindowRect(rect2);
			w = rect2.right - rect2.left; 
			rect2.left = rect1.right - 16 - w;
			rect2.right = rect2.left + w;
			ScreenToClient(rect2);
			pButton1->MoveWindow(&rect2);
		}
}

void	CTodayMain::OnMoveEdit()
{
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
		if(pEdit){
			CRect rect;
			CRect rect1;
			GetWindowRect(rect1);
			pEdit->GetWindowRect(rect);
			rect.right = rect1.right - 5;
			ScreenToClient(rect);
			pEdit->MoveWindow(rect);
		}
}

void	CTodayMain::OnBnClickedButtonContent1()
{		
		SwitchDialog(1);
		g_contentDialog1.ReFreshItem();
}

void	CTodayMain::OnBnClickedButtonMini()
{
		ShowWindow(SW_HIDE);
		if(m_pCurrentWnd) m_pCurrentWnd->ShowWindow(SW_HIDE);
}

void	CTodayMain::OnBnClickedButtonMax()
{
		if(IsZoomed()){
			ShowWindow(SW_RESTORE);
		}
		else{
			ShowWindow(SW_SHOWMAXIMIZED);
		}
}

BOOL	CTodayMain::OnInitDialog()
{
		CDialog::OnInitDialog();
		SetWin7();
		SetWindowTop(this);
		AfxInitRichEdit2();
		m_Publish.Create(IDD_DIALOG_PUBLISH,this);
		SetWindowTop(&m_Publish);

		g_dialogRetweeted.Create(IDD_DIALOG_RETWEETED);
		SetWindowTop(&g_dialogRetweeted);

		BOOL bSu = FALSE;
		bSu = dsSkinWindow( GetSafeHwnd(), SKIN_TYPE_DIALOG, _T("mainframe"), FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow( GetSafeHwnd(), SKIN_TYPE_DIALOG,"));

		bSu =dsSkinWindow( GetDlgItem(IDC_BUTTON_CLOSE)->GetSafeHwnd(), SKIN_TYPE_BUTTON, _T("dialogButtonClose"), FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow( GetDlgItem(IDC_BUTTON_CLOSE) error"));

		bSu = dsSkinWindow(GetDlgItem(IDC_EDIT3)->GetSafeHwnd(),SKIN_TYPE_EDIT,_T("Editdialog"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_EDIT3) error "));

		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_MAX)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonMax"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_MAX) error "));

		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_MINI)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonMin"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_MINI) error "));

		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_CONTENT1)->GetSafeHwnd(),SKIN_TYPE_RADIOBUTTON,_T("dialogButtonSelectMsg"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDD_DIALOG_CONTENT1) error "));

		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_CONTENT2)->GetSafeHwnd(),SKIN_TYPE_RADIOBUTTON,_T("dialogButtonSelectMan"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_CONTENT2)"));

		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_CONTENT3)->GetSafeHwnd(),SKIN_TYPE_RADIOBUTTON,_T("dialogButtonSelectGroup"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_CONTENT3)"));

		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_CONTENT4)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonSelectBlank"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_CONTENT4)"));

		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_CONTENT5)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonSelectBlank"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_CONTENT5)"));

		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_CONTENT6)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonSelectBlank"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_CONTENT6)"));

		bSu = dsSkinWindow(GetDlgItem(IDC_LONGIN_HAEDPIC)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonTittle"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_LONGIN_HAEDPIC)->GetSafeHwnd() error"));

		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_MYNAME)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonMyNameBold"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_MYNAME) error"));

		bSu = dsSetDrawItemValue(GetDlgItem(IDC_LONGIN_HAEDPIC)->GetSafeHwnd(),_T("buttontittle"),_T("D:\\WEBO.bmp"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSetDrawItemValue(GetDlgItem(IDC_LONGIN_HAEDPIC)->GetSafeHwnd()"));
		
		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_FANS)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonMyName"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSetDrawItemValue(GetDlgItem(IDC_BUTTON_FANS)->GetSafeHwnd()"));

		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_BOLG)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonMyName"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSetDrawItemValue(GetDlgItem(IDC_BUTTON_BOLG)"));

		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_ATTENTION)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonMyName"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSetDrawItemValue(GetDlgItem(IDC_BUTTON_ATTENTION)"));

		bSu = dsSkinWindow(GetDlgItem(IDC_STATIC_TITLE)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("StaticSoftTitle"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_STATIC_TITLE) error"));

		bSu = dsSkinWindow(GetDlgItem(IDC_BUTTON_COLERSET)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("buttonColor"),FALSE);
		if(!bSu) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_COLERSET) error"));

		dsSkinWindow(GetDlgItem(IDC_BUTTON_PUBLISH)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("buttonPublish"),FALSE);
		OnMoveWindow();
		m_pStatic = (CStatic *)GetDlgItem(IDC_STATIC_CONTENT);
		g_showUserInfo.Create(IDD_DIALOG_SHOW_USERINFO,this);
		g_showItemImage.Create(IDD_DIALOG_SHOWPIC,this);
		if(m_pStatic){
			g_contentDialog1.Create(IDD_DIALOG_CONTENT1,m_pStatic);
			g_contentDialog2.Create(IDD_DIALOG_CONTENT2,m_pStatic);
			g_contentDialog3.Create(IDD_DIALOG_CONTENT3,m_pStatic);
			g_contentDialog4.Create(IDD_DIALOG_CONTENT4,m_pStatic);
			g_contentDialog5.Create(IDD_DIALOG_CONTENT5,m_pStatic);
			g_contentDialog6.Create(IDD_DIALOG_CONTENT6,m_pStatic);
			HideAllContentDialg();

			CRect rect;
			CRect rect1;
			GetWindowRect(rect1);
			m_pStatic->GetWindowRect(rect);
			rect.right = rect1.right - 2;
			rect.bottom = rect1.bottom - 3;
			ScreenToClient(&rect);
			m_pStatic->MoveWindow(rect);

			CRect stempRect;
			m_pStatic->GetWindowRect(&stempRect);
			g_contentDialog1.MoveWindow(&stempRect);
			//g_contentDialog1.ShowWindow(SW_SHOW);
		}
		dsSetSelfPaint( GetSafeHwnd(), TRUE);
		//m_LonGinHeadPic.SetImage(_T("D:\\WEBO.JPG"));
		return TRUE; 
}

void	CTodayMain::OnBnClickedButtonClose()
{	
		CtodayDlg *pWnd = (CtodayDlg*)theApp.GetMainWnd();
		if(pWnd){
			pWnd->OnOK();
		}
}

void	CTodayMain::OnBnClickedButtonContent2()
{
		SwitchDialog(2);
		g_contentDialog2.ReFreshItem();
		g_contentDialog2.ItemInit();
}

void	CTodayMain::OnBnClickedButtonContent3()
{
		SwitchDialog(3);
		g_contentDialog3.ReFreshItem();
		g_contentDialog3.InitItem();
}

void	CTodayMain::OnBnClickedButtonContent4()
{
		SwitchDialog(4);
}

void	CTodayMain::OnBnClickedButtonContent5()
{
		SwitchDialog(5);
}

void	CTodayMain::OnBnClickedButtonContent6()
{
		SwitchDialog(6);
}

void	CTodayMain::OnMove(int x, int y)
{
		if(m_pStatic && m_pCurrentWnd){
			CRect rect;
			m_pStatic->GetWindowRect(&rect);
			m_pCurrentWnd->MoveWindow(&rect);
		}
		CDialog::OnMove(x, y);
}

void	CTodayMain::OnSize(UINT nType, int cx, int cy)
{
		CDialog::OnSize(nType, cx, cy);
		OnMoveWindow();
		OnMoveSelectButton();
		OnMoveEdit();
		if(m_pStatic){
			CRect rect;
			CRect rect1;
			GetWindowRect(rect1);
			m_pStatic->GetWindowRect(rect);
			rect.right = rect1.right - 2;
			rect.bottom = rect1.bottom - 3;
			if(m_pCurrentWnd) {
				m_pCurrentWnd->MoveWindow(rect);
				m_pCurrentWnd->UpdateVscroll();
			}
			ScreenToClient(&rect);
			m_pStatic->MoveWindow(rect);
		}
}

HBRUSH	CTodayMain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
		HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		return hbr;
}

LRESULT CTodayMain::OnNcHitTest(CPoint point)
{
		CRect rect;
		GetWindowRect(&rect);
		if(point.x <= rect.left+3){
			return HTLEFT;
		}
		else if(point.x >= rect.right-3){
			return HTRIGHT;
		}
		else if(point.y <= rect.top+3){
			return HTTOP;
		}
		else if(point.y >= rect.bottom-3){
			return HTBOTTOM;
		}    
		else if(point.x <= rect.left+10 && point.y <= rect.top+10){
			return HTTOPLEFT;
		}
		else if(point.x >= rect.right-10 && point.y <= rect.top+10){
			return HTTOPRIGHT;
		}
		else if(point.x <= rect.left+10 && point.y >= rect.bottom-10){
			return HTBOTTOMLEFT;
		}
		else if(point.x >= rect.right-10 && point.y >= rect.bottom-10){
			return HTBOTTOMRIGHT;
		}
		return HTCLIENT;
}

BOOL	CTodayMain::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
		if(nHitTest == HTCAPTION || nHitTest == HTSYSMENU ||
			nHitTest == HTMENU || nHitTest == HTCLIENT){
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		}
		else if(nHitTest == HTTOP || nHitTest == HTBOTTOM){
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
		}
		else if(nHitTest == HTLEFT || nHitTest == HTRIGHT){
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
		}
		else if(nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMRIGHT){
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
		}
		else if(nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMLEFT){
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
		}
		else{
			SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		}
		return TRUE;
}

void	CTodayMain::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
		if(nHitTest == HTTOP)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTBOTTOM)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTLEFT)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTRIGHT)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTTOPLEFT)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTTOPRIGHT)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTBOTTOMLEFT)
			SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));
		else if(nHitTest == HTBOTTOMRIGHT)
			SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));
		CDialog::OnNcLButtonDown(nHitTest, point);
}

void	CTodayMain::OnLButtonDown(UINT nFlags, CPoint point)
{
		m_pCurrentWnd->CloseMenu();
		CDialog::OnLButtonDown(nFlags,point);
}


BOOL	CTodayMain::OnEraseBkgnd(CDC* pDC)
{
		CRect rect;
		CRect rect1;
		rect1.left  = 10;
		rect1.top = 10;
		rect1.right = 180;
		rect1.bottom = 60;
		dsDrawImage(_T("dialog.main.bk"),pDC->GetSafeHdc(),&rect1,&rect1);
		return TRUE;
}

void	CTodayMain::OnBnClickedButtonColerset()
{
		if(!m_pColorDlg){
			m_pColorDlg = new TodayColorDlg;
			m_pColorDlg->Create(TodayColorDlg::IDD,this);
		}

		CRect rect;
		GetDlgItem(IDC_BUTTON_COLERSET)->GetWindowRect( &rect);
		rect.left = rect.left ;
		rect.top  = rect.top + rect.Height();
		m_pColorDlg->SetWindowPos( &CWnd::wndTopMost, rect.left, rect.top, 0,0, SWP_NOSIZE|SWP_SHOWWINDOW);
		m_pColorDlg->ShowWindow( SW_SHOW);
}

void	CTodayMain::OnDestroy()
{
		if(m_pColorDlg){
			m_pColorDlg->DestroyWindow();
			delete m_pColorDlg;
		}
		CDialog::OnDestroy();
}

void	CTodayMain::SetUserName(CString *pStrName)
{
		if(!pStrName) return;
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_MYNAME);
		if(pButton){
			pButton->SetWindowText(pStrName->GetBuffer(0));
		}
}

void	CTodayMain::SetUserImage(CString *pPath)
{
		if(!pPath) return;
		dsSetDrawItemValue(GetDlgItem(IDC_LONGIN_HAEDPIC)->GetSafeHwnd(),_T("buttontittle"),pPath->GetBuffer(0),TRUE);
}

void	CTodayMain::SetFollowers(CString &str)
{
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_FANS);
		if(pButton){
			str = _T("粉丝") + str;
			pButton->SetWindowText(str.GetBuffer(0));
		}
}

void	CTodayMain::SetStatuses(CString &str)
{
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_BOLG);
		if(pButton){
			str = _T("发布") + str;
			pButton->SetWindowText(str.GetBuffer(0));
		}
}

void	CTodayMain::SetFavourites(CString &str)
{
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_ATTENTION);
		if(pButton){
			str = _T("关注") + str;
			pButton->SetWindowText(str.GetBuffer(0));
		}
}

BOOL	CTodayMain::PreTranslateMessage(MSG* pMsg)
{
		if(pMsg->message == WM_KEYDOWN){
			if(pMsg->wParam == VK_RETURN){
				return TRUE;
			}
			else if(pMsg->wParam == VK_END){
				return TRUE;
			}
		}
		return CDialog::PreTranslateMessage(pMsg);
}

void	CTodayMain::OnBnClickedButtonPublish()
{
		m_Publish.ShowWindow(SW_SHOW);
}

void	CTodayMain::ReFreshNow()
{
		if(m_pCurrentWnd){
			m_pCurrentWnd->SetTimer(REFRESH_TIME,2000,NULL);
		}
}

void	CTodayMain::SetWin7()
{
		HINSTANCE hInst = LoadLibrary(_T("UxTheme.dll"));
		if (hInst){
			typedef HRESULT (WINAPI *PFUN_SetWindowTheme)(HWND, LPCWSTR, LPCWSTR);
			PFUN_SetWindowTheme pFun = (PFUN_SetWindowTheme)GetProcAddress(hInst, "SetWindowTheme");
			if (pFun)
				pFun(GetSafeHwnd(), L"", L""); 
			FreeLibrary(hInst);
		}
		hInst = LoadLibrary(_T("dwmapi.dll"));
		if (hInst){
			typedef HRESULT (WINAPI * TmpFun)(HWND,DWORD,LPCVOID,DWORD);
			TmpFun DwmSetWindowAttributeEX = (TmpFun)::GetProcAddress(hInst, "DwmSetWindowAttribute");
			if (DwmSetWindowAttributeEX){
				DWORD dwAttr = 1;
				DwmSetWindowAttributeEX(GetSafeHwnd(), 2, &dwAttr, 4); 
			}
			FreeLibrary(hInst);
		}
}