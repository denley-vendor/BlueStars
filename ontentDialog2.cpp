/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define CONTENTDIALOG2_CPP

#include "stdafx.h"
#include "today.h"
#include "TodayMain.h"
#include "ontentDialog2.h"
#include "ShowItemImage.h"
#include "DialogRetweeted.h"

UINT	__cdecl Thread_ReFreshNewCContent2(LPVOID pParam)
{
		CString strId;
		WeiBoApi api;
		BOOL b = g_WeiBoData.GetAtItemMaxId(strId);
		if((!b) || strId.IsEmpty()) {
			//TRACE("Thread Info: Thread_ReFreshNewCContent2  GetAtItemMinId return \n");
			return 0;
		}

		InStreamA instrA;
		CString strApi = api.GetAtLoginUserItem() + _T("&count=12") + _T("&since_id=") + strId;
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
			SAFE_DELETE(pXml);
			return 0;
		}

		if(pXml) g_WeiBoData.AddAtLoginUserItem(pXml);

		pXml2 = (TiXmlDocument *)pXml->RootElement();
		if(pXml2){
			g_lockStr.LockWrite();
			pXml3 = pXml2->FirstChild(pInfo);
			if(pXml3){
				g_WeiBoEngine.AddAttentionRefreshCount(1);
			}
			while(pXml3){
				g_WeiBoEngine.AnalysisAttentionItem((TiXmlElement*)pXml3,g_str);
				g_contentDialog2.SendMessage(MSG_ADDHTMLSTR,(WPARAM)g_str.GetBuffer(0));
				pXml3 = (TiXmlElement*)pXml3->NextSibling();
				bUpDate = TRUE;
			}
			if(bUpDate){
				g_str.Empty();
				g_str.Format(_T("b_RoundCurve(\"imgtitle%d\", \"#C0D0F1\", \"#C0D0F1\", 2);"),g_WeiBoEngine.GetAttentionRefreshCount());
				g_contentDialog2.SendMessage(MSG_EXECSCRIPT,(WPARAM)&g_str);
			}

			g_lockStr.UnlockWrite();
			g_contentDialog2.SendMessage(MSG_HTMLREFRESH);
		}
		SAFE_DELETE(pXml);

		return 0;
}

UINT	__cdecl Thread_ReFreshPreviousCContent2(LPVOID pParam)
{
		if(g_TodayThreadLock.IsBStart(THREAD_LOCK_REFRESH_CONTENT2_PREVIOUS)) {
			TRACE("Thread Info: Thread_ReFreshPreviousCContent1 return \n");
			return 0;
		}
		g_TodayThreadLock.SetBStart(THREAD_LOCK_REFRESH_CONTENT2_PREVIOUS,2);

		CString strId;
		WeiBoApi api;
		BOOL b = g_WeiBoData.GetAtItemMinId(strId);
		if((!b) || strId.IsEmpty()) {
			g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CONTENT2_PREVIOUS);
			return 0;
		}

		LONG cid = 0;
		cid = _wtoi64(strId.GetBuffer(0));
		cid --;
		strId.Format(_T("%d"),cid);

		g_lockStr.LockWrite();
		InStreamA instrA;
		CString strApi = api.GetAtLoginUserItem() + _T("&count=12") + _T("&max_id=") + strId;
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
			g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CONTENT2_PREVIOUS);
			SAFE_DELETE(pXml);
			return 0;
		}

		if(pXml) g_WeiBoData.AddAtPreviousUserItem(pXml);

		pXml2 = (TiXmlDocument *)pXml->RootElement();
		if(pXml2){
			pXml3 = pXml2->LastChild(pInfo);
			if(pXml3){
				g_WeiBoEngine.AddAttentionRefreshCount(1);
			}
			while(pXml3){
				g_WeiBoEngine.AnalysisAttentionItem((TiXmlElement*)pXml3,g_str);
				g_contentDialog2.SendMessage(MSG_ADDHTMLSTR,(WPARAM)g_str.GetBuffer(0),(LPARAM)TRUE);
				pXml3 = (TiXmlElement*)pXml3->PreviousSibling();
				bUpDate = TRUE;
			}
			if(bUpDate){
				g_str.Empty();
				g_str.Format(_T("b_RoundCurve(\"imgtitle%d\", \"#C0D0F1\", \"#C0D0F1\", 2);"),g_WeiBoEngine.GetAttentionRefreshCount());
				g_contentDialog2.SendMessage(MSG_EXECSCRIPT,(WPARAM)&g_str);
			}

			g_contentDialog2.SendMessage(MSG_HTMLREFRESH);
		}
		SAFE_DELETE(pXml);

		g_lockStr.UnlockWrite();
		g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CONTENT2_PREVIOUS);
		return 0;
}

UINT	__cdecl Thread_ReFreshCContent2(LPVOID pParam)
{
		if(g_TodayThreadLock.IsBStart(THREAD_LOCK_REFRESH_CCONTENT2)) {
			TRACE("Thread Info: Thread_ReFreshPreviousCContent1 return \n");
			return 0;
		}
		g_TodayThreadLock.SetBStart(THREAD_LOCK_REFRESH_CCONTENT2,2);

		CString strApi;
		CString strTemp;
		WeiBoApi api;
		strApi = api.GetAtLoginUserItem() + _T("&count=18");


		g_lockStr.LockWrite();
		InStreamA instrA;
		instrA.PutWChar(strApi.GetBuffer(0));
		strTemp = g_weiBoHttp.OauthRequestString(instrA);
		instrA.Empty();
		instrA.PutWCharToUTF8(strTemp.GetBuffer(0));

		const TiXmlNode *pXml2 = NULL;
		const TiXmlNode *pXml3 = NULL;
		const CHAR *pInfo2 = "status";
		TiXmlDocument *pXml = new TiXmlDocument;
		pXml->Parse(instrA.GetBuf());
		if(!pXml){
			g_lockStr.UnlockWrite();
			g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CCONTENT2);
			return 0;
		}

		BOOL b = FALSE;
		b = g_WeiBoData.IsItemBValidate(pXml);
		if(!b) {
			g_lockStr.UnlockWrite();
			g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CCONTENT2);
			SAFE_DELETE(pXml);
			return 0;
		}
		if(pXml) {
			g_WeiBoData.SetAtLoginUserItem(pXml);
			pXml2 = pXml->RootElement();
		}
		if(pXml2){
			BOOL  bUpDate = FALSE;

			pXml3 = pXml2->LastChild(pInfo2);
			if(pXml3){
				g_WeiBoEngine.AddAttentionRefreshCount(1);
			}
			while(pXml3){
				g_WeiBoEngine.AnalysisAttentionItem((TiXmlElement*)pXml3,g_str);
				g_contentDialog2.SendMessage(MSG_ADDHTMLSTR,(WPARAM)g_str.GetBuffer(0));
				if(!pXml3) break;
				pXml2 = NULL;
				pXml2 = pXml3->PreviousSibling();
				pXml3 = pXml2;
				bUpDate = TRUE;
			}
			if(bUpDate){
				g_str.Empty();
				g_str.Format(_T("b_RoundCurve(\"imgtitle%d\", \"#C0D0F1\", \"#C0D0F1\", 2);"),g_WeiBoEngine.GetAttentionRefreshCount());
				g_contentDialog2.SendMessage(MSG_EXECSCRIPT,(WPARAM)&g_str);
			}
			g_contentDialog2.SendMessage(MSG_HTMLREFRESH);
		}
		g_lockStr.UnlockWrite();
		g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CCONTENT2);
		return 0;
}
IMPLEMENT_DYNCREATE(ContentDialog2, TodayHtmlDialog)

ContentDialog2::ContentDialog2(CWnd* pParent /*=NULL*/)
	: TodayHtmlDialog(ContentDialog2::IDD, ContentDialog2::IDH, pParent)
{
		m_bInit = FALSE;
}

ContentDialog2::~ContentDialog2()
{
}

STDMETHODIMP	ContentDialog2::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
{
		CComBSTR   str; 
		CString strTemp;
		IHTMLElement *pElement1 = NULL;
		IHTMLElement *pElement = NULL;
		CComQIPtr <IHTMLElement,   &IID_IHTMLElement> pHTMLElement(pdispReserved); 
		pElement = pHTMLElement;
		if(pElement){
			pElement->get_className(&str);
			strTemp.Format(_T("%s"),str.m_str);
		}

		DWORD index = 0;
		m_curItemId.Empty();
		if(strTemp != _T("lh") && pElement){
			index = 10;
		} 
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
		menu.LoadMenu (IDR_CONTENT_MENU2);

		HMENU hMenu = menu.GetSubMenu (0)->Detach ();
		m_pMenu = theApp.GetContextMenuManager()->ShowPopupMenu(hMenu, point.x, point.y,this,TRUE,FALSE,FALSE);
		m_pMenu->SetForegroundWindow();

		m_bMenu = TRUE;
		return S_OK;
}

void	ContentDialog2::DoDataExchange(CDataExchange* pDX)
{
		TodayHtmlDialog::DoDataExchange(pDX);
}

BOOL	ContentDialog2::OnInitDialog()
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
		strTemp.Replace(_T("today.exe"),_T("show.t2"));
		m_pBrowserApp->Navigate(strTemp.GetBuffer(0),NULL,NULL,NULL,NULL);
		return TRUE;  
}

BOOL	ContentDialog2::PreTranslateMessage(MSG* pMsg)
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

BEGIN_MESSAGE_MAP(ContentDialog2, TodayHtmlDialog)
	ON_MESSAGE(MSG_ADDHTMLSTR,AddHtmlString)
	ON_MESSAGE(MSG_HTMLREFRESH,ReFresh)
	ON_MESSAGE(MSG_EXECSCRIPT,ExecScript)
	ON_WM_CREATE()
	ON_COMMAND(ID_REFRESH_CONTENT2,ReFreshItem)
	ON_COMMAND(ID_RETWEETED_CON2,OnRetweetedItem)
	ON_COMMAND(ID_LOOKITEM_CON2,OnLookItem)
	ON_COMMAND(ID_CONTENT2_COPY,OnCopyItem)
	ON_COMMAND(ID_DIALOGUE2,OnDalogue)

	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(ContentDialog2)
END_DHTML_EVENT_MAP()


BEGIN_DISPATCH_MAP(ContentDialog2,TodayHtmlDialog)
	DISP_FUNCTION(ContentDialog2,"ShowUserInfo",OnShowUserInfo,VT_EMPTY,VTS_BSTR) 
	DISP_FUNCTION(ContentDialog2,"UShowUserInfo",OnUShowUserInfo,VT_EMPTY,VTS_NONE) 
	DISP_FUNCTION(ContentDialog2,"ShowItemImage",OnShowItemImage,VT_EMPTY,VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(ContentDialog2,"UShowItemImage",OnUShowItemImage,VT_EMPTY,VTS_NONE)
	DISP_FUNCTION(ContentDialog2,"ShowBigImage",OnShowBigImage,VT_EMPTY,VTS_BSTR)
END_DISPATCH_MAP()

ContentDialog2 g_contentDialog2;
int		ContentDialog2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (TodayHtmlDialog::OnCreate(lpCreateStruct) == -1)
			return -1;

		BOOL bSu = FALSE;
		bSu = dsSkinWindow( GetSafeHwnd(), SKIN_TYPE_SCROLLWIN, _T("scrollwindow"), FALSE);
		if(!bSu) AfxMessageBox(_T("sSkinWindow( GetSafeHwnd(), SKIN_TYPE_SCROLLWIN,error"));
		return 0;
}

void	ContentDialog2::ReFreshItem()
{
		if(m_bInit){
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ReFreshNewCContent2,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
}

void	ContentDialog2::OnShowUserInfo(LPCSTR value)
{
		CString stemp;
		stemp.Format(_T("%s"),value);

		CString strId;
		g_WeiBoData.GetUserIDByAtItem(strId,stemp.GetBuffer(0));
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

void	ContentDialog2::OnUShowUserInfo()
{
		g_showUserInfo.SetWindowShow();
}

void	ContentDialog2::OnShowItemImage(LPCSTR value,LPCSTR value1)
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

void	ContentDialog2::OnUShowItemImage()
{
		g_showItemImage.SetWindowShow();
}

void	ContentDialog2::OnShowBigImage(WCHAR *value)
{
		g_showItemImage.OnShowBigImage(value);
}

void	ContentDialog2::ItemInit()
{
		if(!m_bInit){
			m_bInit = TRUE;
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ReFreshCContent2,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
}
BOOL	ContentDialog2::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
		TodayHtmlDialog::OnMouseWheel(nFlags, zDelta, pt);

		int max = 0;
		int min = 0;
		GetScrollRange(SB_VERT,&min,&max);
		LONG pos = GetScrollPos(SB_VERT);
		min = max - pos;
		if(min <= 10){
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ReFreshPreviousCContent2,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}

		return TRUE;
}

void	ContentDialog2::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
		TodayHtmlDialog::OnVScroll(nSBCode, nPos, pScrollBar);

		int max = 0;
		int min = 0;
		GetScrollRange(SB_VERT,&min,&max);
		LONG pos = GetScrollPos(SB_VERT);
		min = max - pos;
		if(min <= 10){
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ReFreshPreviousCContent2,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
}

void	ContentDialog2::OnRetweetedItem()
 {
		if(g_dialogRetweeted.IsWindowVisible()) return;
		if(m_curItemId.IsEmpty()) return;
		g_dialogRetweeted.ShowWindow(SW_SHOW);
		g_dialogRetweeted.InitData(m_curItemId.GetBuffer(0));
}

void	ContentDialog2::OnLookItem()
{
		CString strUserId;
		CString strUrl;
		WeiBoApi api;
		strUrl = api.LookItem();
		g_WeiBoData.GetUserIDByAtItem(strUserId,m_curItemId.GetBuffer(0));

		strUrl.Replace(_T("[USERID]"),strUserId.GetBuffer(0));
		strUrl.Replace(_T("[ITEMID]"),m_curItemId.GetBuffer(0));
		ShellExecute(NULL,_T("open"),strUrl.GetBuffer(0),NULL,NULL,SW_SHOWNORMAL);
}

void	ContentDialog2::OnCopyItem()
{		
		if(m_curItemId.IsEmpty()) return;
		CString strTemp;
		CString strTemp1;
		g_WeiBoData.GetAtItemText(m_curItemId,strTemp,strTemp1);
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

void	ContentDialog2::OnDalogue()
{

}
void	ContentDialog2::OnTimer(UINT_PTR nIDEvent)
{
		if(nIDEvent == REFRESH_TIME){
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ReFreshCContent2,this,THREAD_PRIORITY_NORMAL,0,0,NULL);

			KillTimer(nIDEvent);
		}

		TodayHtmlDialog::OnTimer(nIDEvent);
}
