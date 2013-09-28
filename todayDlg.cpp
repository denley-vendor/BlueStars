/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define  CTODAYDLG_CPP
#include "stdafx.h"
#include "OAuth/oauth.h"
#include "today.h"
#include "todayDlg.h"
#include "TodayMain.h"
#include "ontentDialog1.h"
#include "ontentDialog2.h"
#include "ontentDialog3.h"
#include "Attach\ImageTransform.h"
#include "WeiBoEngine/Data/IniFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT	__cdecl  Thread_Login(LPVOID pParam)
{
		if(g_TodayThreadLock.IsBStart(THREAD_LOCK_TODAYLIGON)) {
			TRACE("Thread Info: Thread_Login TodayThreadLock return \n");
			return 0;
		}
		g_TodayThreadLock.SetBStart(THREAD_LOCK_TODAYLIGON,2);

		TRACE("Thread Info: Thread_Login start \n");
		CString strAccount;
		CString strPwd;
		CString strName;
		CString strImgUrl;

		WeiBoApi api;
		InStreamA inApi;
		inApi.PutWChar(api.GetUserInfo().GetBuffer(0));
		CString instrTemp = g_weiBoHttp.OauthRequestString(inApi);

		InStreamA inStrA;
		TiXmlDocument *pXml = new TiXmlDocument;
		inStrA.PutWChar(instrTemp.GetBuffer(0));

		char *pParse = inStrA.GetBuf();
		pXml->Parse(pParse);
		g_WeiBoData.SetLongingUserXml(pXml);

		CString strTemp;
		const CHAR *pInfo2 = ("id");
		const CHAR *pInfo3 = ("user");
		const CHAR *pInfo4 = ("profile_image_url");
		const CHAR *pInfo5 = ("screen_name");
		const CHAR *pInfo6 = "followers_count";
		const CHAR *pInfo7 = "statuses_count";
		const CHAR *pInfo8 = "favourites_count";
		const TiXmlElement *pUser = pXml->RootElement();
		const TiXmlElement *pId = g_WeiBoData.FirstChildElenent(pInfo2,pUser);
		const TiXmlElement *pImage = g_WeiBoData.FirstChildElenent(pInfo4,pUser);
		const TiXmlElement *pName = g_WeiBoData.FirstChildElenent(pInfo5,pUser);
		const TiXmlElement *pFolls = g_WeiBoData.FirstChildElenent(pInfo6,pUser);
		const TiXmlElement *pStatuses = g_WeiBoData.FirstChildElenent(pInfo7,pUser);
		const TiXmlElement *pFavour = g_WeiBoData.FirstChildElenent(pInfo8,pUser);
		if(pId){
			CtodayDlg *pWnd = (CtodayDlg*)theApp.GetMainWnd();
			const char *pUserId = pId->GetText();
			if(pImage){
				strImgUrl.Format(_T("%S"),pImage->GetText());
			}
			if(pName){
				InStreamW inStrW;
				InStreamW inStrW2;
				inStrA.Empty();
				inStrA << pName->GetText();
				inStrW2 = inStrW.PutChar(inStrA.GetBuf());
				strName.Format(_T("%s"),inStrW2.GetBuf());
			}
			if(!pUserId){
				if(pWnd){
					pWnd->ShowLoginError();
				}
			}
			else{
				ImageTransform imageTrans;
				if(pWnd) {
					CRect rect;
					pWnd->ShowWindow(SW_HIDE);
					pWnd->GetWindowRect(&rect);
					rect.right -= 30;
					rect.bottom -= 20;
					g_todayMainDialog.MoveWindow(&rect);
					g_todayMainDialog.ShowWindow(SW_SHOW);
					g_todayMainDialog.SwitchDialog(1);
				}
				strTemp = _T("image");
				g_weiBoHttp.GetUserImage(strAccount,pUserId,strImgUrl);
				imageTrans.SaveToImage(strAccount,_T("bmp"));
				g_todayMainDialog.SetUserImage(&strAccount);
				g_todayMainDialog.SetUserName(&strName);
				if(pStatuses){
					strName.Format(_T("%S"),pStatuses->GetText());
					g_todayMainDialog.SetStatuses(strName);
				}
				if(pFavour){
					strName.Format(_T("%S"),pFavour->GetText());
					g_todayMainDialog.SetFavourites(strName);
				}
				if(pFolls){
					strName.Format(_T("%S"),pFolls->GetText());
					g_todayMainDialog.SetFollowers(strName);
				}

				g_lockStr.LockWrite();
				g_WeiBoEngine.FillAttentionItem();
				if(pWnd){
					pWnd->SendMessage(REFRESHCCONTENT1);
					pWnd->SetBLogin(TRUE);
				}
				g_lockStr.UnlockWrite();
			}
		}
		else{
			CtodayDlg *pWnd = (CtodayDlg*)theApp.GetMainWnd();
			if(pWnd){
				pWnd->ShowLoginError();
			}
		}
		TRACE("Thread Info: Thread_Login end \n");
		g_TodayThreadLock.SetBStop(THREAD_LOCK_TODAYLIGON);
		return 0;
}

UINT	__cdecl  Thread_ReFreshCContent1(LPVOID pParam)
{
		if(g_TodayThreadLock.IsBStart(THREAD_LOCK_REFRESH_CCONTENT1)) {
			return 0;
		}
		g_TodayThreadLock.SetBStart(THREAD_LOCK_REFRESH_CCONTENT1,5);
		g_str.Empty();

		CString strId;
		g_WeiBoData.GetAttentionItemMaxId(strId);
		if(strId.IsEmpty()){
			return 0;
		}

		g_lockStr.LockWrite();

		WeiBoApi api;
		InStreamA inApi;
		CString strApi = api.GetAttentionUserItem() + _T("&count=18") + _T("&since_id=") + strId;
		inApi.PutWChar(strApi.GetBuffer(0));
		CString strTemp = g_weiBoHttp.OauthRequestString(inApi);

		InStreamA instrA;
		instrA.PutWCharToUTF8(strTemp.GetBuffer(0));

		const CHAR *pInfo = "status";
		BOOL  bUpDate = FALSE;

		TiXmlNode *pXml2 = NULL;
		TiXmlNode *pXml3 = NULL;
		TiXmlDocument *pXml = new TiXmlDocument;
		pXml->Parse(instrA.GetBuf());

		BOOL b = FALSE;
		b = g_WeiBoData.IsItemBValidate(pXml);
		if(!b) {
			SAFE_DELETE(pXml);
			g_lockStr.UnlockWrite();
			g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CCONTENT1);
			return 0;
		}

		if(pXml) g_WeiBoData.AddNewAttentionItem(pXml);

		pXml2 = (TiXmlDocument *)pXml->RootElement();
		if(pXml2){
			pXml3 = pXml2->FirstChildElement(pInfo);
			if(pXml3){
				g_WeiBoEngine.AddAttentionRefreshCount(1);
			}
			
			while(pXml3){
				g_WeiBoEngine.AnalysisAttentionItem((TiXmlElement*)pXml3,g_str);
				g_contentDialog1.SendMessage(MSG_ADDHTMLSTR,(WPARAM)g_str.GetBuffer(0));
				pXml3 = (TiXmlElement*)pXml3->NextSibling();
				bUpDate = TRUE;
			}
			if(bUpDate){
				g_str.Empty();
				g_str.Format(_T("b_RoundCurve(\"imgtitle%d\", \"#C0D0F1\", \"#C0D0F1\", 2);"),g_WeiBoEngine.GetAttentionRefreshCount());
				g_contentDialog1.SendMessage(MSG_EXECSCRIPT,(WPARAM)&g_str);
			}
		}
		g_lockStr.UnlockWrite();
		SAFE_DELETE(pXml);
		//TRACE("Thread Info: Thread_ReFreshCContent1 end \n");
		g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CCONTENT1);
		return 0;
}
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual		void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual		BOOL	OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void	CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
END_MESSAGE_MAP()

BOOL	CAboutDlg::OnInitDialog()
{
		CDialog::OnInitDialog();
		CStatic *pStatic = (CStatic*)GetDlgItem(IDC_STATIC_VERSION);
		CString strTemp;
		GetExeFileVersion(strTemp);
		strTemp = _T("当前版本:") + strTemp;
		pStatic->SetWindowText(strTemp.GetBuffer(0));

		CString strApi;
		WeiBoApi api;
		InStreamA instrA;
		strApi = api.GetOtherUserInfo() + _T("&user_id=2102273063");
		instrA.PutWChar(strApi.GetBuffer(0));
		strTemp = g_weiBoHttp.OauthRequestString(instrA);
		
		instrA.Empty();
		instrA.PutWCharToUTF8(strTemp.GetBuffer(0));

		TiXmlElement *pEle = NULL;
		TiXmlDocument *pXml = new TiXmlDocument;
		pXml->Parse(instrA.GetBuf());
		if(!pXml) return 1;

		const CHAR *pInfo = "description";
		pEle = pXml->RootElement();
		if(pEle){
			pEle = pEle->FirstChildElement(pInfo);
		}
		if(pEle){
			strTemp.Format(_T("最新版本%S"),pEle->GetText());
		}
		SAFE_DELETE(pXml);

		pStatic = (CStatic *)GetDlgItem(IDC_STATIC_NOWVERSION);
		if(pStatic){
			pStatic->SetWindowText(strTemp.GetBuffer(0));
		}
		return TRUE;
}

void	CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	ShellExecute(NULL,_T("open"),_T("http://weibo.com/2102273063"),NULL,NULL,SW_SHOW);
}

CtodayDlg::CtodayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtodayDlg::IDD, pParent)
	, m_bRemeberPw(FALSE)
{
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

		memset(&m_nid, 0 , sizeof(m_nid));
		m_nid.cbSize = sizeof(m_nid);
		m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		m_bInit1 = FALSE;
		m_bLogin = FALSE;
}

CtodayDlg::~CtodayDlg()
{

}
void	CtodayDlg::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
		DDX_Check(pDX,IDC_CHECK_REMMEMBERPW,m_bRemeberPw);
		DDX_Check(pDX,IDC_CHECK_AUTOLOGIN,m_bAutoLogin);
		DDX_Check(pDX,IDC_CHECK_MASTART,m_bMachine);
}

BEGIN_MESSAGE_MAP(CtodayDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETTINGCHANGE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
	ON_MESSAGE(UM_TRAYNOTIFY, OnTrayNotify)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CtodayDlg::OnClickedLogin)
	ON_WM_SIZING()

	ON_COMMAND(ID_MAINMENU_EXIT,OnOK)
	ON_COMMAND(ID_MAINMENU_RELOGIN,OnReLogin)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CtodayDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_MINI, &CtodayDlg::OnBnClickedButtonMini)
	ON_COMMAND(ID_REFRESH_CONTENT1,ReFreshCContent1)
	ON_COMMAND(ID_TODAY_PUBLISH,OnPublish)
	ON_COMMAND(ID_MAINMENU_SHOWMAIN,OnShowMainWindow)
	ON_COMMAND(ID_TODAY_ABOUT,About)
/*	ON_COMMAND(ID_REFRESH_CONTENT2,ReFreshCContent2)*/
	ON_MESSAGE(REFRESHCCONTENT1,InitContent1Item)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_LOGIN, &CtodayDlg::OnNMClickSyslinkLogin)
	ON_STN_CLICKED(IDC_SYSLINK_LOGIN, &CtodayDlg::OnStnClickedSyslinkLogin)
	ON_STN_CLICKED(IDC_STATIC_REG, &CtodayDlg::OnStnClickedStaticReg)
	ON_STN_CLICKED(IDC_STATIC_FOR, &CtodayDlg::OnStnClickedStaticFor)
	ON_STN_CLICKED(IDC_STATIC_ABOUT, &CtodayDlg::OnStnClickedStaticAbout)
END_MESSAGE_MAP()

BOOL	CtodayDlg::OnInitDialog()
{
		CDialog::OnInitDialog();
		m_image.LoadFromResource(AfxGetInstanceHandle(),IDB_BITMAP_LIGON);
		SetIcon(m_hIcon, TRUE);			
		SetWindowTop(this);


		BOOL bOk = FALSE;
		bOk = dsSkinWindow(GetDlgItem(IDC_BUTTON_CLOSE)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonClose"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_CLOSE)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T(dialogButtonClose error"));
		bOk = dsSkinWindow(GetDlgItem(IDC_BUTTON_MINI)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonMin"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_MINI)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T(dialogButtonMin"));
		bOk = dsSkinWindow(this->GetSafeHwnd(),SKIN_TYPE_DIALOG,_T("mainframeLogin"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(this->GetSafeHwnd(),SKIN_TYPE_DIALOG,_T(mainframeLogin) error"));

		bOk = dsSkinWindow(GetDlgItem(IDC_CHECK1)->GetSafeHwnd(),SKIN_TYPE_CHECKBUTTON,_T("allcheckbutton"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_CHECK1)->GetSafeHwnd()"));
		bOk = dsSkinWindow(GetDlgItem(IDC_CHECK2)->GetSafeHwnd(),SKIN_TYPE_CHECKBUTTON,_T("allcheckbutton"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_CHECK2)->GetSafeHwnd()"));
		bOk = dsSkinWindow(GetDlgItem(IDC_CHECK3)->GetSafeHwnd(),SKIN_TYPE_CHECKBUTTON,_T("allcheckbutton"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_CHECK3)->GetSafeHwnd()"));

		bOk = dsSkinWindow(GetDlgItem(IDC_EDIT_ACCOUNT)->GetSafeHwnd(),SKIN_TYPE_EDIT,_T("EditUserName"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_EDIT1) error "));
		bOk = dsSkinWindow(GetDlgItem(IDC_EDIT_PASSWORD)->GetSafeHwnd(),SKIN_TYPE_EDIT,_T("EditUserPW"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_EDIT2) error "));

		bOk = dsSkinWindow(GetDlgItem(IDC_BUTTON_LOGIN)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("buttonLogin"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_LOGIN) error"));

		bOk = dsSkinWindow(GetDlgItem(IDC_STATIC_ACCOUNT)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("StaticLogin"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_STATIC_ACCOUNT) error"));
		bOk = dsSkinWindow(GetDlgItem(IDC_STATIC_PW)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("StaticLogin"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_STATIC_PW) error"));
		bOk = dsSkinWindow(GetDlgItem(IDC_STATIC_TITLE)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("StaticSoftTitle"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_STATIC_TITLE) error"));

		bOk = dsSkinWindow(GetDlgItem(IDC_STATIC_ERROR)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("StaticLoginError"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_STATIC_ERROR error"));

		//dsSkinWindow(GetDlgItem(IDC_STATIC_L1)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("StaticLogin"),FALSE);
		//dsSkinWindow(GetDlgItem(IDC_STATIC_L2)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("StaticLogin"),FALSE);
		//dsSkinWindow(GetDlgItem(IDC_STATIC_L3)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("StaticLogin"),FALSE);
		dsSkinWindow(GetDlgItem(IDC_SYSLINK_LOGIN)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("linkstatic"),FALSE);
		dsSkinWindow(GetDlgItem(IDC_EDIT_OAUTHTOKEN)->GetSafeHwnd(),SKIN_TYPE_EDIT,_T("EditUserPW"),FALSE);
		dsSkinWindow(GetDlgItem(IDC_STATIC_REG)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("linkstatic1"),FALSE);
		dsSkinWindow(GetDlgItem(IDC_STATIC_FOR)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("linkstatic1"),FALSE);
		dsSkinWindow(GetDlgItem(IDC_STATIC_ABOUT)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("linkstatic1"),FALSE);

		g_todayMainDialog.Create(IDD_TODAY_MAIN);
		g_todayMainDialog.ShowWindow(SW_HIDE);

		m_nid.hWnd = GetSafeHwnd ();
		m_nid.uCallbackMessage = UM_TRAYNOTIFY;
		m_nid.hIcon = m_hIcon;
		CString strToolTip = _T("Today微博");
		_tcsncpy_s (m_nid.szTip, strToolTip, strToolTip.GetLength ());
		Shell_NotifyIcon (NIM_ADD, &m_nid);

		dsSetSelfPaint( GetSafeHwnd(), TRUE);

		MachineStartShow();
		return TRUE;  
}

BOOL	CtodayDlg::PreTranslateMessage(MSG* pMsg)
{
		if(pMsg->message == WM_KEYDOWN){
			if(pMsg->wParam == VK_RETURN){
				OnClickedLogin();
				return TRUE;
			}
		}

		return CDialog::PreTranslateMessage(pMsg);
}

void	CtodayDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
		if ((nID & 0xFFF0) == IDM_ABOUTBOX)
		{
			CAboutDlg dlgAbout;
			dlgAbout.DoModal();
		}
		else
		{
			CDialog::OnSysCommand(nID, lParam);
		}
}

void	CtodayDlg::OnPaint()
{
		if (IsIconic())
		{
			CPaintDC dc(this); 

			SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
			int cxIcon = GetSystemMetrics(SM_CXICON);
			int cyIcon = GetSystemMetrics(SM_CYICON);
			CRect rect;
			GetClientRect(&rect);
			int x = (rect.Width() - cxIcon + 1) / 2;
			int y = (rect.Height() - cyIcon + 1) / 2;
			dc.DrawIcon(x, y, m_hIcon);
		}
		else
		{
			CDialog::OnPaint();
		}
}

BOOL	CtodayDlg::OnEraseBkgnd(CDC* pDC)
{
		CRect rect;
		GetWindowRect(&rect);
		m_image.TransparentBlt(pDC->GetSafeHdc(),0,0,rect.right - rect.left,rect.bottom - rect.top,RGB(255,0,255));
		m_image.TransparentBlt(pDC->GetSafeHdc(),0,0,20,20,0,0,20,20,RGB(255,0,255));
		ScreenToClient(&rect);
		m_image.TransparentBlt(pDC->GetSafeHdc(),rect.right - 20,0,20,20,238,0,20,20,RGB(255,0,255));
		m_image.TransparentBlt(pDC->GetSafeHdc(),0,rect.bottom - 20,20,20,0,560,20,20,RGB(255,0,255));
		m_image.TransparentBlt(pDC->GetSafeHdc(),rect.right - 20,rect.bottom - 20,20,20,238,560,20,20,RGB(255,0,255));

		CRect rect1;
		rect1.left  = 40;
		rect1.top = 40;
		rect1.right = 308;
		rect1.bottom = 305;
		dsDrawImage(_T("dialog.main.texture.star"),pDC->GetSafeHdc(),&rect1,&rect);
		//m_image2.TransparentBlt(pDC->GetSafeHdc(),40,40,128,128,RGB(0,0,0));
		//m_image2.Draw(pDC->GetSafeHdc(),40,40);
		return TRUE;
}

void	CtodayDlg::OnTrayContextMenus()
{
		if(IsWindowVisible()) return;
		CPoint point;
		::GetCursorPos (&point);
		point.x -= 100;


		CMenu menu;
		menu.LoadMenu (IDR_TRAY_MENU);

		CMFCPopupMenu::SetForceShadow (TRUE);

		HMENU hMenu = menu.GetSubMenu (0)->Detach ();
		CMFCPopupMenu* pMenu = theApp.GetContextMenuManager()->ShowPopupMenu(hMenu, point.x, point.y, this, TRUE);
		pMenu->SetForegroundWindow ();
}

void	CtodayDlg::OnReLogin()
{
		if(g_todayMainDialog){
			g_todayMainDialog.ShowWindow(SW_HIDE);
			g_todayMainDialog.SwitchDialog(0);
		}		
		//if(g_contentDialog1){
		//	//g_contentDialog1.CloseMenu();
		//}
		m_bLogin = FALSE;
		ShowWindow(SW_SHOW);
		g_WeiBoData.Clear();
		g_weiBoHttp.Clear();

		g_contentDialog1.Clear();
		g_contentDialog2.Clear();
		g_contentDialog3.Clear();
		g_contentDialog2.m_bInit = FALSE;
		g_contentDialog3.m_bInit = FALSE;
		m_bInit1 = FALSE;

		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_LOGIN);
		if(pButton){
			pButton->EnableWindow();
		}
}

void	CtodayDlg::OnShowMainWindow()
{
		if(!g_todayMainDialog.IsWindowVisible() && m_bLogin){
			g_todayMainDialog.ShowWindow(SW_SHOW);
			g_todayMainDialog.SwitchDialog(1);
		}
}

LRESULT	CtodayDlg::InitContent1Item(WPARAM wp, LPARAM lp)
{
		if(m_bInit1) return 0 ;
		CString strTemp;
		CString strHtml;
		DWORD n = 0;
		g_WeiBoEngine.AddAttentionRefreshCount(1);
		const TiXmlElement *pEle = g_WeiBoData.GetAttentionItem(0);
		while(pEle){
			g_WeiBoEngine.AnalysisAttentionItem(pEle,strTemp);
			strHtml += strTemp;

			n ++;
			pEle = g_WeiBoData.GetAttentionItem(n);
		}


		g_contentDialog1.AddHtmlString(strHtml.GetBuffer(0));

		CString strJS = _T("");
		strJS.Format(_T("b_RoundCurve(\"imgtitle%d\", \"#C0D0F1\", \"#C0D0F1\", 2);"),g_WeiBoEngine.GetAttentionRefreshCount());
		g_contentDialog1.ExecScript(&strJS);
		m_bInit1 = TRUE;

		g_contentDialog1.ReFresh();

		return 0;
}

void	CtodayDlg::ShowLoginError()
{
		CStatic *pCtrlError = (CStatic *)GetDlgItem(IDC_STATIC_ERROR);
		if(pCtrlError){
			pCtrlError->SetWindowText(_T("账号密码错误.."));
			pCtrlError->ShowWindow(SW_SHOW);
		}
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_LOGIN);
		if(pButton){
			pButton->EnableWindow(TRUE);
		}
}

void	CtodayDlg::RememberOauth()
{
		UpdateData(TRUE);
		CString strPath;
		IniFile ini;
		GetExePath(strPath);
		strPath.Replace(_T("today.exe"),_T(""));

		CString iniPath = strPath + _T("TODAY.I");
		ini.InitFile(iniPath.GetBuffer(0));

		if(m_bRemeberPw){
			CString strTemp;
			strTemp.Format(_T("%S"),m_tkey.GetBuf());
			ini.SetString(_T("LOGINITEM"),_T("KEY"),strTemp.GetBuffer(0));
			strTemp.Format(_T("%S"),m_tsecret.GetBuf());
			ini.SetString(_T("LOGINITEM"),_T("SECRET"),strTemp.GetBuffer(0));
			ini.SetValue(_T("LOGINITEM"),_T("BOOL"),1);
		}
		if(m_bAutoLogin){
			ini.SetValue(_T("LOGINITEM"),_T("LOGIN"),1);
		}
		else{
			ini.SetValue(_T("LOGINITEM"),_T("LOGIN"),0);
		}
}

void	CtodayDlg::RememberPW()
{
		UpdateData(TRUE);
		CString strPath;
		IniFile ini;
		GetExePath(strPath);
		strPath.Replace(_T("today.exe"),_T(""));

		CString iniPath = strPath + _T("TODAY.I");
		ini.InitFile(iniPath.GetBuffer(0));
		if(m_bRemeberPw){
			CString strAcc;
			CString strPw;
			CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ACCOUNT);
			if(pEdit){
				pEdit->GetWindowText(strAcc);
			}
			pEdit = (CEdit *)GetDlgItem(IDC_EDIT_PASSWORD);
			if(pEdit){
				pEdit->GetWindowText(strPw);
			}

			CString strSet = XXXPW(strPw.GetBuffer(0),_T("today8888"));
			strSet = CCCPW(strSet.GetBuffer(0));

			ini.SetString(_T("LOGINITEM"),_T("ACCOUT"),strAcc.GetBuffer(0));
			ini.SetString(_T("LOGINITEM"),_T("PWD"),strSet.GetBuffer(0));
			ini.SetValue(_T("LOGINITEM"),_T("BOOL"),1);
		}
		else{
			//ini.SetString(_T("LOGINITEM"),_T("ACCOUT"),NULL);
			ini.SetString(_T("LOGINITEM"),_T("PWD"),NULL);
			ini.SetValue(_T("LOGINITEM"),_T("BOOL"),0);
		}

		if(m_bAutoLogin){
			ini.SetValue(_T("LOGINITEM"),_T("LOGIN"),1);
		}
		else{
			ini.SetValue(_T("LOGINITEM"),_T("LOGIN"),0);
		}
}

void	CtodayDlg::ShowOauth()
{
		CString strPath;
		IniFile ini;
		GetExePath(strPath);
		strPath.Replace(_T("today.exe"),_T(""));
		CString iniPath = strPath + _T("TODAY.I");
		ini.InitFile(iniPath.GetBuffer(0));

		BOOL b = ini.GetValue(_T("LOGINITEM"),_T("BOOL"));
		if(b){
			m_bRemeberPw = TRUE;
		}
		b = ini.GetValue(_T("LOGINITEM"),_T("LOGIN"));
		if(b){
			m_bAutoLogin = TRUE;
		}
		UpdateData(FALSE);

		CString strAcc;
		CString strPw;
		ini.GetString(_T("LOGINITEM"),_T("KEY"),strAcc);
		ini.GetString(_T("LOGINITEM"),_T("SECRET"),strPw);
		
		m_tkey.PutWChar(strAcc.GetBuffer(0));
		m_tsecret.PutWChar(strPw.GetBuffer(0));
		g_weiBoHttp.SetOauthToken(m_tkey.GetBuf(),m_tsecret.GetBuf());
		if(m_bRemeberPw){
			strAcc = _T("已记住密码,可直接登录");
			CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_OAUTHTOKEN);
			if(pEdit){
				 pEdit->SetWindowText(strAcc);
			}
			if(m_bAutoLogin){
				CWinThread		*pThread = NULL;
				pThread = AfxBeginThread(Thread_Login,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,NULL);
				pThread->ResumeThread();
			}
		}
}

void	CtodayDlg::ShowPW()
{
		CString strPath;
		IniFile ini;
		GetExePath(strPath);
		strPath.Replace(_T("today.exe"),_T(""));
		CString iniPath = strPath + _T("TODAY.I");
		ini.InitFile(iniPath.GetBuffer(0));

		BOOL b = ini.GetValue(_T("LOGINITEM"),_T("BOOL"));
		if(b){
			m_bRemeberPw = TRUE;
		}
		CString strAcc;
		CString strPw;
		ini.GetString(_T("LOGINITEM"),_T("ACCOUT"),strAcc);
		ini.GetString(_T("LOGINITEM"),_T("PWD"),strPw);
		strPw = CCCPW(strPw.GetBuffer(0));
		strPw = YYYPW(strPw.GetBuffer(0),_T("today8888"));

		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ACCOUNT);
		if(pEdit){
			pEdit->SetWindowText(strAcc);
		}
		pEdit = (CEdit *)GetDlgItem(IDC_EDIT_PASSWORD);
		if(pEdit){
			pEdit->SetWindowText(strPw);
		}

		b = ini.GetValue(_T("LOGINITEM"),_T("LOGIN"));
		if(b){
			m_bAutoLogin = TRUE;
		}
		UpdateData(FALSE);

		if(b){
			OnClickedLogin();
		}
}

CString CtodayDlg::XXXPW(WCHAR *pIn,WCHAR *pIn1)
{
		if(!pIn1) return NULL;
		if(!pIn) return NULL;

		CString strTemp;
		DWORD len = wcslen(pIn);
		DWORD len1 = wcslen(pIn1);
		DWORD count = 0;
		DWORD i = 0;
		if(len < len1) count = len;
		else count = len1;

		for( i = 0; i < count; i ++){
			strTemp += pIn[i];
			strTemp += pIn1[i];
		}

		if(len > len1){
			for(i ; i < len; i ++){
				strTemp += pIn[i];
			}
		}
		else{
			for(i ; i < len1; i ++){
				strTemp += pIn1[i];
			}
		}

		return strTemp;
}

CString	CtodayDlg::YYYPW(WCHAR *pIn,WCHAR *pIn1)
{
		if(!pIn1) return NULL;
		if(!pIn) return NULL;

		LONG count = 0;
		LONG len = wcslen(pIn);
		LONG len1 = wcslen(pIn1);
		count = len;
		len = abs(len - len1);
		
		LONG i = 0;
		LONG i1 = 0;
		CString strTemp;
		for(i; i < count; i ++){
			if(!(i%2)){
				strTemp += pIn[i];
				i1 ++;
				if(i1 == len) break;
			}
		}

		return strTemp;
}

CString	CtodayDlg::CCCPW(WCHAR *pIn)
{
		if(!pIn) return NULL;
		CString strTemp;
		LONG len = wcslen(pIn);
		if(!len) return NULL;
		for(LONG i = len - 1; i > -1; i --){
			strTemp += pIn[i];
		}
		return strTemp;
}

void	CtodayDlg::MachineStartShow()
{
		HKEY   RegKey = NULL; 
		LRESULT   lRet;
		WCHAR         szValue[256];   
		memset(szValue,0,256);
		LONG	revalue = 0;
		CString strTemp = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
		RegOpenKeyEx(HKEY_LOCAL_MACHINE,strTemp, 0,KEY_READ,&RegKey);
		lRet = RegQueryValue(RegKey,_T("today"),szValue,&revalue);
		if(SUCCEEDED(lRet)){
			m_bMachine = TRUE;
		}
		else{
			m_bMachine = FALSE;
		}

		RegCloseKey(RegKey);
		UpdateData(FALSE);
}

void	CtodayDlg::MachineStartSet()
{
		UpdateData(TRUE);
		HKEY   RegKey = NULL; 
		LRESULT   lRet;
		CString strTemp ;
		GetExePath(strTemp);
		CString strPath = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
		RegOpenKey(HKEY_LOCAL_MACHINE,strPath,&RegKey); 
		if(m_bMachine){
			lRet = RegSetValueEx(RegKey,_T("today"),0,REG_SZ,(const BYTE*)strTemp.GetBuffer(0),strTemp.GetLength()*2+1);
		}  
		else{
			RegDeleteKey(RegKey,_T("today"));
		}

		RegCloseKey(RegKey);
}

void	CtodayDlg::OnPublish()
{
		g_todayMainDialog.OnBnClickedButtonPublish();
}

void	CtodayDlg::About()
{
		CAboutDlg dlg;
		dlg.DoModal();
}

LRESULT	CtodayDlg::OnTrayNotify(WPARAM wp, LPARAM lp)
{
		UINT uiMsg = (UINT) lp;

		switch (uiMsg)
		{
		case WM_RBUTTONUP:
			OnTrayContextMenus();
			return 0;

		case WM_LBUTTONDBLCLK:
			if(!IsWindowVisible() && m_bLogin){
				OnShowMainWindow();
			}
			else{
				ShowWindow(SW_SHOW);
			}
			return 0;
		}

		return 0;
}

void	CtodayDlg::OnClickedLogin()
{
		UpdateData(TRUE);
		CStatic *pCtrlError = (CStatic *)GetDlgItem(IDC_STATIC_ERROR);
		if(pCtrlError){
			//pCtrlError->SetWindowText(_T("登录中,wait..."));
			pCtrlError->ShowWindow(SW_HIDE);
		}
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_LOGIN);
		if(pButton){
			pButton->EnableWindow(FALSE);
		}

		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_OAUTHTOKEN);
		if(pEdit){
			InStreamA instra;
			CString strTemp;
			pEdit->GetWindowText(strTemp);
			strTemp.Trim();
			instra.PutWChar(strTemp.GetBuffer(0));

			char *req_url = NULL;
			char *reply   = NULL;
			char *postarg = NULL;

			const char *c_key         = "544758078";
			const char *c_secret      = "ec144cec7fadb6e3ee3c2f635de14c02";

			InStreamA instUrl;
			instUrl << "http://api.t.sina.com.cn/oauth/access_token";
			instUrl << "?oauth_verifier=" << instra.GetBuf();
			req_url = oauth_sign_url2(instUrl.GetBuf(), &postarg, OA_HMAC, "POST", c_key, c_secret,m_tkey.GetBuf(),m_tsecret.GetBuf());
			reply = oauth_http_post(req_url,postarg);
			SAFE_FREE(req_url);
			SAFE_FREE(postarg);

			CStringArray arrStr;
			split_str(reply,'&',arrStr);
			SAFE_FREE(reply);

			InStreamA instr1;
			InStreamA instr2;
			if(arrStr.GetSize() >= 2){
				strTemp = arrStr.GetAt(0);
				strTemp.Replace(_T("oauth_token="),NULL);
				instr1.PutWChar(strTemp.GetBuffer(0));
				strTemp = arrStr.GetAt(1);
				strTemp.Replace(_T("oauth_token_secret="),NULL);
				instr2.PutWChar(strTemp.GetBuffer(0));

				g_weiBoHttp.SetOauthToken(instr1.GetBuf(),instr2.GetBuf());

				CWinThread		*pThread = NULL;
				pThread = AfxBeginThread(Thread_Login,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,NULL);
				pThread->ResumeThread();
			}
			MachineStartSet();
		}
}

void	CtodayDlg::OnOK()
{
		Shell_NotifyIcon (NIM_DELETE, &m_nid);
		CDialog::OnOK();
}

void	CtodayDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
		CDialog::OnSizing(fwSide, pRect);
	
}

void	CtodayDlg::OnBnClickedButtonClose()
{
		OnOK();
}

void	CtodayDlg::OnBnClickedButtonMini()
{
		ShowWindow(SW_HIDE);
}

void	CtodayDlg::ReFreshCContent1()
{
		g_contentDialog1.m_pMenu = NULL;
		CWinThread		*pThread = NULL;
		pThread = AfxBeginThread(Thread_ReFreshCContent1,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
}

void	CtodayDlg::ReFreshCContent2()
{
		g_contentDialog2.ReFreshItem();
}

void	CtodayDlg::OnNMClickSyslinkLogin(NMHDR *pNMHDR, LRESULT *pResult)
{
		*pResult = 0;
}

void	CtodayDlg::OnStnClickedSyslinkLogin()
{	
	char *postarg = NULL;
	char *req_url = NULL;
	char *reply   = NULL;
	const char *c_key         = "544758078";
	const char *c_secret      = "ec144cec7fadb6e3ee3c2f635de14c02";
	const char *request_token_uri = "http://api.t.sina.com.cn/oauth/request_token";
	req_url = oauth_sign_url2(request_token_uri, &postarg, OA_HMAC, NULL, c_key, c_secret, NULL, NULL);
	reply = oauth_http_post(req_url,postarg);

	SAFE_FREE(postarg);
	SAFE_FREE(req_url);

	CString strTemp;
	CStringArray arrStr;
	split_str(reply,'&',arrStr);
	if(arrStr.GetSize() >= 2){
		strTemp = arrStr.GetAt(0);
		strTemp.Replace(_T("oauth_token="),_T(""));
		m_tkey.Empty();
		m_tkey.PutWChar(strTemp.GetBuffer(0));

		m_tsecret.Empty();
		strTemp = arrStr.GetAt(1);
		strTemp.Replace(_T("oauth_token_secret="),_T(""));
		m_tsecret.PutWChar(strTemp.GetBuffer(0));
		SAFE_FREE(reply);

		const char *access_token_uri = "http://api.t.sina.com.cn/oauth/authorize?oauth_callback=oob&oauth_token=";
		CString strUrl;
		strUrl.Format(_T("%S%S"),access_token_uri,m_tkey.GetBuf());
		ShellExecute(NULL,_T("open"),strUrl.GetBuffer(0),NULL,NULL,SW_SHOW);
	}
}

void	CtodayDlg::OnStnClickedStaticReg()
{
		CString strUrl = _T("http://weibo.com/reg.php?ps=u3&lang=zh");
		ShellExecute(NULL,_T("open"),strUrl.GetBuffer(0),NULL,NULL,SW_SHOW);
}

void	CtodayDlg::OnStnClickedStaticFor()
{
		CString strUrl = _T("http://login.sina.com.cn/member/getpwd/getpwd0.php?entry=sso");
		ShellExecute(NULL,_T("open"),strUrl.GetBuffer(0),NULL,NULL,SW_SHOW);
}

void	CtodayDlg::OnStnClickedStaticAbout()
{
		CString strUrl = _T("http://blog.sina.com.cn/u/2102273063");
		ShellExecute(NULL,_T("open"),strUrl.GetBuffer(0),NULL,NULL,SW_SHOW);
}
