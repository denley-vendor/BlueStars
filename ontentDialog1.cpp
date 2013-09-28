/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define CONTENTDIALOG1_CPP

#include "stdafx.h"
#include "today.h"
#include "ontentDialog1.h"
#include "TodayMain.h"
#include "ShowItemImage.h"
#include "todayDlg.h"
#include "Net/Mail.h"


UINT	__cdecl Thread_ReFreshPreviousCContent1(LPVOID pParam)
{
		if(g_TodayThreadLock.IsBStart(THREAD_LOCKREFRESHPREVIOUSCCONTENT1)) {
			return 0;
		}
		g_TodayThreadLock.SetBStart(THREAD_LOCKREFRESHPREVIOUSCCONTENT1,2);

		CString strId;
		WeiBoApi api;
		BOOL b = g_WeiBoData.GetAttentionItemMinId(strId);
		if((!b) || strId.IsEmpty()) {
			return 0;
		}
		LONG cId = _wtoi64(strId.GetBuffer(0));
		cId --;
		strId.Format(_T("%d"),cId);

		g_lockStr.LockWrite();

		InStreamA instrA;
		CString strApi = api.GetAttentionUserItem() + _T("&count=12") + _T("&max_id=") + strId;
		instrA.PutWChar(strApi.GetBuffer(0));
		CString strTemp = g_weiBoHttp.OauthRequestString(instrA);
		
		instrA.Empty();
		instrA.PutWCharToUTF8(strTemp.GetBuffer(0));

		const CHAR *pInfo = "status";
		BOOL  bUpDate = FALSE;

		const TiXmlNode *pXml2 = NULL;
		const TiXmlNode *pXml3 = NULL;
		TiXmlDocument *pXml = new TiXmlDocument;
		pXml->Parse(instrA.GetBuf());

		b = g_WeiBoData.IsItemBValidate(pXml);
		if(!b) {
			g_lockStr.UnlockWrite();
			SAFE_DELETE(pXml);
			g_TodayThreadLock.SetBStop(THREAD_LOCKREFRESHPREVIOUSCCONTENT1);
			return 0;
		}

		if(pXml) g_WeiBoData.AddPreviousAttentionItem(pXml);

		pXml2 = (TiXmlDocument *)pXml->RootElement();
		if(pXml2){
			pXml3 = pXml2->LastChild(pInfo);
			if(pXml3){
				g_WeiBoEngine.AddAttentionRefreshCount(1);
			}
			while(pXml3){
				g_WeiBoEngine.AnalysisAttentionItem((TiXmlElement*)pXml3,g_str);
				g_contentDialog1.SendMessage(MSG_ADDHTMLSTR,(WPARAM)g_str.GetBuffer(0),(LPARAM)TRUE);
				pXml3 = (TiXmlElement*)pXml3->PreviousSibling();
				bUpDate = TRUE;
			}
			if(bUpDate){
				g_str.Empty();
				g_str.Format(_T("b_RoundCurve(\"imgtitle%d\", \"#C0D0F1\", \"#C0D0F1\", 2);"),g_WeiBoEngine.GetAttentionRefreshCount());
				g_contentDialog1.SendMessage(MSG_EXECSCRIPT,(WPARAM)&g_str);
			}

			g_contentDialog1.SendMessage(MSG_HTMLREFRESH);
		}
		g_lockStr.UnlockWrite();
		SAFE_DELETE(pXml);

		TRACE("Thread Info: Thread_ReFreshPreviousCContent1 end \n");
		g_TodayThreadLock.SetBStop(THREAD_LOCKREFRESHPREVIOUSCCONTENT1);
		return 0;
}

IMPLEMENT_DYNCREATE(ContentDialog1, TodayHtmlDialog)

ContentDialog1::ContentDialog1(CWnd* pParent /*=NULL*/)
	: TodayHtmlDialog(ContentDialog1::IDD, ContentDialog1::IDH, pParent)
{
		m_curPt.x = 0;
		m_curPt.y = 0;
}

ContentDialog1::~ContentDialog1()
{
}

void	ContentDialog1::DoDataExchange(CDataExchange* pDX)
{
		TodayHtmlDialog::DoDataExchange(pDX);
}

BOOL	ContentDialog1::OnInitDialog()
{
		TodayHtmlDialog::OnInitDialog();
		HINSTANCE hInst = LoadLibrary(_T("UxTheme.dll"));
		if (hInst){
			typedef HRESULT (WINAPI *PFUN_SetWindowTheme)(HWND, LPCWSTR, LPCWSTR);
			PFUN_SetWindowTheme pFun = (PFUN_SetWindowTheme)GetProcAddress(hInst, "SetWindowTheme");
			if (pFun)
				pFun(GetSafeHwnd(), L"", L""); 
			FreeLibrary(hInst);
		}

		CString strTemp;
		GetExePath(strTemp);
		strTemp.Replace(_T("today.exe"),_T("show.t1"));
		m_pBrowserApp->Navigate(strTemp.GetBuffer(0),NULL,NULL,NULL,NULL);
		UpdateVscroll();
		return TRUE; 
}

int		ContentDialog1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (TodayHtmlDialog::OnCreate(lpCreateStruct) == -1)
			return -1;

		BOOL bSu = FALSE;
		bSu = dsSkinWindow( GetSafeHwnd(), SKIN_TYPE_SCROLLWIN, _T("scrollwindow"), FALSE);
		if(!bSu) AfxMessageBox(_T("sSkinWindow( GetSafeHwnd(), SKIN_TYPE_SCROLLWIN,error"));
		return 0;
}

BEGIN_MESSAGE_MAP(ContentDialog1, TodayHtmlDialog)
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(MSG_ADDHTMLSTR,AddHtmlString)
	ON_MESSAGE(MSG_HTMLREFRESH,ReFresh)
	ON_MESSAGE(MSG_EXECSCRIPT,ExecScript)

	ON_COMMAND(ID_RETWEETED_CON1,OnRetweetedItem)
	ON_COMMAND(ID_LOOKITEM_CON1,OnLookItem)
	ON_COMMAND(ID_COPY_ITEM1,OnCopyItem)
	ON_COMMAND(ID_DIALOGUE1,OnDalogue)

	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(ContentDialog1)

END_DHTML_EVENT_MAP()

BEGIN_DISPATCH_MAP(ContentDialog1,TodayHtmlDialog)
	DISP_FUNCTION(ContentDialog1,"ShowUserInfo",OnShowUserInfo,VT_EMPTY,VTS_BSTR) 
	DISP_FUNCTION(ContentDialog1,"UShowUserInfo",OnUShowUserInfo,VT_EMPTY,VTS_NONE) 
	DISP_FUNCTION(ContentDialog1,"ShowItemImage",OnShowItemImage,VT_EMPTY,VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(ContentDialog1,"UShowItemImage",OnUShowItemImage,VT_EMPTY,VTS_NONE)
	DISP_FUNCTION(ContentDialog1,"ShowBigImage",OnShowBigImage,VT_EMPTY,VTS_BSTR)
//	DISP_FUNCTION(ContentDialog1,"OpenUrl",OnOpenUrl,VT_EMPTY,VTS_BSTR) 
END_DISPATCH_MAP()

void	ContentDialog1::OnCloseMenu()
{
		CloseMenu();
}

void	ContentDialog1::OnShowUserInfo(LPCSTR value)
{
		CString stemp;
		stemp.Format(_T("%s"),value);
		
		CString strId;
		g_WeiBoData.GetUserIdByItem(strId,stemp.GetBuffer(0));
		CPoint pt;
		CRect rect;
		CRect rect1;
		GetWindowRect(&rect);
		GetCursorPos(&pt);
		g_showUserInfo.GetClientRect(&rect1);

		LONG y = pt.y - 30;
		LONG cy = pt.y + (rect1.bottom - rect1.top);
		LONG cx = rect.left;
		LONG x = rect.left - (rect1.right - rect1.left) - 7;
		g_showUserInfo.SetWindowShow(TRUE);
		g_showUserInfo.SetWindowPos(this,x,y,cx,cy,SW_SHOW);
		g_showUserInfo.ShowUserInfo(strId.GetBuffer(0));

}

void	ContentDialog1::OnUShowUserInfo()
{
		g_showUserInfo.SetWindowShow();
}

void	ContentDialog1::OnOpenUrl(LPCSTR value)
{
		CString strUrl;
		CString strUrl1;
		strUrl.Format(_T("%s"),value);
		strUrl.Remove('@');
		strUrl1.Format(_T("http://weibo.com/n/%s"),strUrl.GetBuffer(0));
		//g_weiBoHttp.OpenUrl(strUrl);
		COleVariant str(_T("_blank"));
		COleVariant strHead(g_weiBoHttp.GetStrKey());

		m_pBrowserApp->Navigate(strUrl1.GetBuffer(0),NULL,&str,&strHead,&strHead);
}

void	ContentDialog1::OnShowItemImage(LPCSTR value,LPCSTR value1)
{
		CString stemp;
		stemp.Format(_T("%s"),value);

		CPoint pt;
		CRect rect;
		CRect rect1;
		GetCursorPos(&pt);
		if(g_showItemImage.IsWindowVisible()){
			int xx = 0;
			int yy = 0;
			xx = abs(pt.x - m_curPt.x);
			yy = abs(pt.y - m_curPt.y);
			if((xx <= 18) && (yy <= 18)){
				return;
			}
		}
		GetWindowRect(&rect);
		g_showItemImage.GetClientRect(&rect1);

		LONG x = rect.left + 40;
		LONG y = pt.y + 10;
		LONG cx = x + (rect1.right - rect1.left);
		LONG cy = y + (rect1.bottom - rect1.top);
		g_showItemImage.SetWindowShow(TRUE);
		g_showItemImage.SetWindowPos(this,x,y,cx,cy,SW_SHOW);
		g_showItemImage.OnShowImage(stemp.GetBuffer(0));
		m_curPt.SetPoint(pt.x,pt.y);
}

void	ContentDialog1::OnUShowItemImage()
{
		g_showItemImage.SetWindowShow();
}

void	ContentDialog1::OnShowBigImage(WCHAR *value)
{
		g_showItemImage.OnShowBigImage(value);
}

STDMETHODIMP	ContentDialog1::GetExternal(IDispatch **ppDispatch)
{
		TodayHtmlDialog::GetExternal(ppDispatch);
		return S_OK;
}

STDMETHODIMP	ContentDialog1::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
{
		CComBSTR   str; 
		CString strTemp;
		IHTMLElement *pElement1 = NULL;
		IHTMLElement *pElement = NULL;
		CComQIPtr <IHTMLElement,   &IID_IHTMLElement> pHTMLElement(pdispReserved); 
		pElement = pHTMLElement;
		if(!pdispReserved) return S_OK;

		if(pElement){
			pElement->get_className(&str);
			strTemp.Format(_T("%s"),str.m_str);
		}

		DWORD index = 0;
		m_curItemId.Empty();
		if(strTemp != _T("lh") && pElement){
			index = 10;
		} 
		if(!pElement) return S_OK;
		for(DWORD i = 0; i < index; i ++){
			if(!pElement) continue;
			pElement->get_className(&str);
			strTemp.Format(_T("%s"),str.m_str);
			if(strTemp == _T("lh")) break;

			pElement->get_parentElement(&pElement1);
			pElement = pElement1;
		}
		if(strTemp == _T("lh")){
			pElement->get_id(&str);
			m_curItemId.Format(_T("%s"),str.m_str);
		}
		CloseMenu();
		CMFCPopupMenu::SetForceShadow (TRUE);
		
		CPoint point;
		::GetCursorPos (&point);
		CMenu menu;
		menu.LoadMenu (IDR_CONTENT_MENU);

		HMENU hMenu = menu.GetSubMenu (0)->Detach ();
		m_pMenu = theApp.GetContextMenuManager()->ShowPopupMenu(hMenu, point.x, point.y,this,TRUE,FALSE,FALSE);
		m_pMenu->SetForegroundWindow();

		m_bMenu = TRUE;
		return S_OK;
}

ContentDialog1 g_contentDialog1;
void	ContentDialog1::OnKillFocus(CWnd* pNewWnd)
{
		//CloseMenu();
		TodayHtmlDialog::OnKillFocus(pNewWnd);
}

void	ContentDialog1::OnLButtonDown(UINT nFlags, CPoint point)
{
		//CloseMenu();
		TodayHtmlDialog::OnLButtonDown(nFlags, point);
}

void	ContentDialog1::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
		TodayHtmlDialog::OnVScroll(nSBCode, nPos, pScrollBar);

		int max = 0;
		int min = 0;
		GetScrollRange(SB_VERT,&min,&max);
		LONG pos = GetScrollPos(SB_VERT);
		min = max - pos;
		if(min <= 60){
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ReFreshPreviousCContent1,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
}

BOOL	ContentDialog1::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
		TodayHtmlDialog::OnMouseWheel(nFlags, zDelta, pt);

		int max = 0;
		int min = 0;
		GetScrollRange(SB_VERT,&min,&max);
		LONG pos = GetScrollPos(SB_VERT);
		min = max - pos;
		if(min <= 10){
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ReFreshPreviousCContent1,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
		return TRUE;
}

BOOL	ContentDialog1::PreTranslateMessage(MSG* pMsg)
{
		if(pMsg->message == WM_LBUTTONDOWN){
			CloseMenu();
		}
		if(pMsg->message == WM_KEYDOWN){
			if(pMsg->wParam == VK_RETURN){
				return TRUE;
			}
			else if(pMsg->wParam == VK_END){
				return TRUE;
			}
		}
		return TodayHtmlDialog::PreTranslateMessage(pMsg);

}

void	ContentDialog1::OnRetweetedItem()
{
		if(m_curItemId.IsEmpty()) return;
		g_dialogRetweeted.ShowWindow(SW_SHOW);
		g_dialogRetweeted.InitData(m_curItemId.GetBuffer(0));
}

void	ContentDialog1::OnLookItem()
{
		CString strUserId;
		CString strUrl;
		WeiBoApi api;
		strUrl = api.LookItem();
		g_WeiBoData.GetUserIdByItem(strUserId,m_curItemId.GetBuffer(0));

		strUrl.Replace(_T("[USERID]"),strUserId.GetBuffer(0));
		strUrl.Replace(_T("[ITEMID]"),m_curItemId.GetBuffer(0));
		ShellExecute(NULL,_T("open"),strUrl.GetBuffer(0),NULL,NULL,SW_SHOWNORMAL);
}

void	ContentDialog1::OnCopyItem()
{		
		if(m_curItemId.IsEmpty()) return;
		CString strTemp;
		CString strTemp1;
		g_WeiBoData.GetAttentionItemText(m_curItemId.GetBuffer(0),strTemp,strTemp1);
		strTemp += strTemp1;
		if(OpenClipboard()){
			HANDLE hClip; 
			TCHAR *pBuf; 
			EmptyClipboard();
			hClip = GlobalAlloc(GMEM_MOVEABLE, (strTemp.GetLength()+1)*2); 
			pBuf = (TCHAR*)GlobalLock(hClip); 
			wcscpy(pBuf,strTemp); 
			GlobalUnlock(hClip); 
			SetClipboardData(CF_UNICODETEXT,hClip); 
			CloseClipboard(); 
		}
}

void	ContentDialog1::OnDalogue()
{
		//MAIL_DATA mailData;
		//mailData.sMailFromNickname = _T("蓝星星");
		//mailData.sSubject.Empty();
		//mailData.sSubject += _T("邮件测试");
		//mailData.sMessage = _T("发个邮件");

		//MailClient mail;		
		//if (mail.InitData(NULL,&mailData)) {
		//	mail.MailSend();			
		//}

		//DWORD n = 0;
		//DWORD m = 0;
		//DWORD g = n/m;
}

void	ContentDialog1::OnTimer(UINT_PTR nIDEvent)
{
		if(nIDEvent == REFRESH_TIME){

			CtodayDlg *pDlg = (CtodayDlg*)AfxGetApp()->GetMainWnd();
			if(pDlg){
				pDlg->ReFreshCContent1();
			}
			KillTimer(nIDEvent);
		}
		TodayHtmlDialog::OnTimer(nIDEvent);
}

void	ContentDialog1::ReFreshItem()
{
		CtodayDlg *pDlg = (CtodayDlg*)AfxGetApp()->GetMainWnd();
		if(pDlg){
			pDlg->ReFreshCContent1();
		}
}