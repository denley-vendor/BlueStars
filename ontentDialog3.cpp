/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define CONTENTDIALOG3_CPP

#include "stdafx.h"
#include "today.h"
#include "ontentDialog3.h"
#include "ShowItemImage.h"
#include "DialogRetweeted.h"

UINT	__cdecl Thread_ReFreshPreviousCContent3(LPVOID pParam)
{	
		if(g_TodayThreadLock.IsBStart(THREAD_LOCK_REFRESH_CONTENT3_PREVIOUS)) {
			return 0;
		}
		g_TodayThreadLock.SetBStart(THREAD_LOCK_REFRESH_CONTENT3_PREVIOUS,1);

		CString strId;
		CString strUserId;
		const CHAR *pInfo = "id";
		const CHAR *pId = g_WeiBoData.GetLoginLastItemId();
		const CHAR *pUserId = g_WeiBoData.GetLonginserInfo(pInfo);
		if(pId){
			strId.Format(_T("%S"),pId);
			LONG lId = _wtoi64(strId.GetBuffer(0));
			strId.Empty();
			lId --;
			strId.Format(_T("%d"),lId);
		}
		if(pUserId){
			strUserId.Format(_T("%S"),pUserId);
		}

		CString strApi;
		CString strTemp;
		InStreamA instrA;
		WeiBoApi api;
		g_lockStr.LockWrite();
		strApi = api.GetUserItem() + _T("&user_id=") + strUserId + _T("&count=18") + _T("&max_id=") + strId;
		instrA.PutWChar(strApi.GetBuffer(0));
		strTemp = g_weiBoHttp.OauthRequestString(instrA);

		instrA.Empty();
		instrA.PutWCharToUTF8(strTemp.GetBuffer(0));

		const TiXmlNode *pXml2 = NULL;
		const TiXmlNode *pXml3 = NULL;
		const CHAR *pInfo2 = "status";
		TiXmlDocument *pXml = new TiXmlDocument;
		pXml->Parse(instrA.GetBuf());
		if(!pXml) {
			g_lockStr.UnlockWrite();
			g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CONTENT3_PREVIOUS);
			return 0;
		}

		BOOL b = FALSE;
		b = g_WeiBoData.IsItemBValidate(pXml);
		if(!b) {
			g_lockStr.UnlockWrite();
			g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CONTENT3_PREVIOUS);
			SAFE_DELETE(pXml);
			return 0;
		}

		if(pXml) {
			g_WeiBoData.AddPreviousLongingItem(pXml);
			pXml2 = pXml->RootElement();
		}
		if(pXml2){
			pXml3 = pXml2->LastChild(pInfo2);
			while(pXml3){
				g_WeiBoEngine.AnalysisLoginItem((TiXmlElement*)pXml3,g_str);
				g_contentDialog3.SendMessage(MSG_ADDHTMLSTR,(WPARAM)g_str.GetBuffer(0),TRUE);
				if(!pXml3) break;
				pXml3 = pXml3->PreviousSibling();
			}
			g_contentDialog3.SendMessage(MSG_HTMLREFRESH);
		}

		g_lockStr.UnlockWrite();
		SAFE_DELETE(pXml);
		g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CONTENT3_PREVIOUS);
		return 0;
}

UINT	__cdecl Thread_ReFreshNewCContent3(LPVOID pParam)
{
		//TRACE("Thread Info: Thread_ReFreshPreviousCContent1 start \n");

		CString strId;
		CString strUserId;
		const CHAR *pInfo = "id";
		const CHAR *pId = g_WeiBoData.GetLoginFirstItemId(); 
		if(!pId) return 0;
		const CHAR *pUserId = g_WeiBoData.GetLonginserInfo(pInfo);
		if(!pUserId) return 0;
		if(pId){
			strId.Format(_T("%S"),pId);
		}
		if(pUserId){
			strUserId.Format(_T("%S"),pUserId);
		}

		InStreamA instrA;
		CString strApi;
		CString strTemp;
		WeiBoApi api;
		strApi = api.GetUserItem() + _T("&user_id=") + strUserId + _T("&count=18") + _T("&since_id=") + strId;
		instrA.PutWChar(strApi.GetBuffer(0));

		g_lockStr.LockWrite();
		strTemp = g_weiBoHttp.OauthRequestString(instrA);
		instrA.Empty();
		instrA.PutWCharToUTF8(strTemp.GetBuffer(0));

		const TiXmlNode *pXml2 = NULL;
		const TiXmlNode *pXml3 = NULL;
		const CHAR *pInfo2 = "status";
		TiXmlDocument *pXml = new TiXmlDocument;
		pXml->Parse(instrA.GetBuf());
		if(!pXml) {
			g_lockStr.UnlockWrite();
			return 0;
		}

		BOOL b = FALSE;
		b = g_WeiBoData.IsItemBValidate(pXml);
		if(!b) {
			g_lockStr.UnlockWrite();
			SAFE_DELETE(pXml);
			return 0;
		}

		if(pXml) {
			g_WeiBoData.AddNewLongingItem(pXml);
			pXml2 = pXml->RootElement();
		}
		if(pXml2){
			pXml3 = (TiXmlElement*)pXml2->LastChild(pInfo2);
			while(pXml3){
				g_WeiBoEngine.AnalysisLoginItem((TiXmlElement*)pXml3,g_str);
				g_contentDialog3.SendMessage(MSG_ADDHTMLSTR,(WPARAM)g_str.GetBuffer(0));
				if(!pXml3) break;
				pXml3 = pXml3->PreviousSibling();
			}
			g_contentDialog3.SendMessage(MSG_HTMLREFRESH);
		}

		g_lockStr.UnlockWrite();
		SAFE_DELETE(pXml);

		return 0;
}
UINT	__cdecl Thread_ReFreshCContent3(LPVOID pParam)
{
		if(g_TodayThreadLock.IsBStart(THREAD_LOCK_REFRESH_CONTENT3)) {
			//TRACE("Thread Info: Thread_ReFreshPreviousCContent1 return \n");
			return 0;
		}
		g_TodayThreadLock.SetBStart(THREAD_LOCK_REFRESH_CONTENT3,2);

		const CHAR *pInfo = "id";
		const CHAR *pUserId = g_WeiBoData.GetLonginserInfo(pInfo);

		CString strUserId;
		CString strApi;
		CString strTemp;
		InStreamA instrA;
		WeiBoApi api;
		strUserId.Format(_T("%S"),pUserId);
		strApi = api.GetUserItem() + _T("&user_id=") + strUserId + _T("&count=18");
		instrA.PutWChar(strApi.GetBuffer(0));

		g_lockStr.LockWrite();
		strTemp = g_weiBoHttp.OauthRequestString(instrA);
		instrA.Empty();
		instrA.PutWCharToUTF8(strTemp.GetBuffer(0));

		const TiXmlNode *pXml2 = NULL;
		const TiXmlNode *pXml3 = NULL;
		const CHAR *pInfo2 = "status";
		TiXmlDocument *pXml = new TiXmlDocument;
		pXml->Parse(instrA.GetBuf());
		if(!pXml) {
			g_lockStr.UnlockWrite();
			g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CONTENT3);
			return 0;
		}

		BOOL b = FALSE;
		b = g_WeiBoData.IsItemBValidate(pXml);
		if(!b) {
			g_lockStr.UnlockWrite();
			g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CONTENT3);
			SAFE_DELETE(pXml);
			return 0;
		}

		if(pXml) {
			g_WeiBoData.SetLongingUserItem(pXml);
			pXml2 = pXml->RootElement();
		}
		if(pXml2){
			pXml3 = (TiXmlElement*)pXml2->LastChild(pInfo2);
			while(pXml3){
				g_WeiBoEngine.AnalysisLoginItem((const TiXmlElement*)pXml3,g_str);
				pXml3 = pXml3->PreviousSibling();
				g_contentDialog3.SendMessage(MSG_ADDHTMLSTR,(WPARAM)g_str.GetBuffer(0));
			}
			g_contentDialog3.SendMessage(MSG_HTMLREFRESH);
		}

		g_lockStr.UnlockWrite();
		g_TodayThreadLock.SetBStop(THREAD_LOCK_REFRESH_CONTENT3);
		return 0;
}

IMPLEMENT_DYNCREATE(ContentDialog3, TodayHtmlDialog)

ContentDialog3::ContentDialog3(CWnd* pParent /*=NULL*/)
	: TodayHtmlDialog(ContentDialog3::IDD, ContentDialog3::IDH, pParent)
{
		m_bInit = FALSE;
		m_pEle = NULL;
}

ContentDialog3::~ContentDialog3()
{
}

STDMETHODIMP	ContentDialog3::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
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
			m_pEle = pElement;
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
		menu.LoadMenu (IDR_CONTENT_MENU3);

		HMENU hMenu = menu.GetSubMenu (0)->Detach ();
		m_pMenu = theApp.GetContextMenuManager()->ShowPopupMenu(hMenu, point.x, point.y,this,TRUE,FALSE,FALSE);
		m_pMenu->SetForegroundWindow();

		m_bMenu = TRUE;
		return S_OK;
}

void	ContentDialog3::DoDataExchange(CDataExchange* pDX)
{
		TodayHtmlDialog::DoDataExchange(pDX);
}

BOOL	ContentDialog3::OnInitDialog()
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
		strTemp.Replace(_T("today.exe"),_T("show.t3"));
		m_pBrowserApp->Navigate(strTemp.GetBuffer(0),NULL,NULL,NULL,NULL);
		UpdateVscroll();
		return TRUE;  
}

BOOL	ContentDialog3::PreTranslateMessage(MSG* pMsg)
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

BEGIN_MESSAGE_MAP(ContentDialog3, TodayHtmlDialog)
	ON_WM_CREATE()
	ON_MESSAGE(MSG_ADDHTMLSTR,AddHtmlString)
	ON_MESSAGE(MSG_HTMLREFRESH,ReFresh)
	ON_COMMAND(ID_REFRESH_CONTENT3,ReFreshItem)
	ON_COMMAND(ID_RETWEETED_CON3,OnRetweetedItem)
	ON_COMMAND(ID_LOOKITEM_CON3,OnLookItem)
	ON_COMMAND(ID_COPY_ITEM3,OnCopyItem)
	ON_COMMAND(ID_DIALOGUE3,OnDalogue)
	ON_COMMAND(ID_ITEM_DEL,OnDel)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(ContentDialog3)
END_DHTML_EVENT_MAP()

BEGIN_DISPATCH_MAP(ContentDialog3,TodayHtmlDialog)
	DISP_FUNCTION(ContentDialog3,"ShowItemImage",OnShowItemImage,VT_EMPTY,VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(ContentDialog3,"UShowItemImage",OnUShowItemImage,VT_EMPTY,VTS_NONE)
	DISP_FUNCTION(ContentDialog3,"ShowBigImage",OnShowBigImage,VT_EMPTY,VTS_BSTR)
END_DISPATCH_MAP()

ContentDialog3 g_contentDialog3;
int		ContentDialog3::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (TodayHtmlDialog::OnCreate(lpCreateStruct) == -1)
			return -1;

		BOOL bSu = FALSE;
		bSu = dsSkinWindow( GetSafeHwnd(), SKIN_TYPE_SCROLLWIN, _T("scrollwindow"), FALSE);
		if(!bSu) AfxMessageBox(_T("sSkinWindow( GetSafeHwnd(), SKIN_TYPE_SCROLLWIN,error"));

		return 0;
}

void	ContentDialog3::ReFreshItem()
{		
		if(m_bInit){
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ReFreshNewCContent3,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
}

void	ContentDialog3::OnLookItem()
{
		CString strUserId;
		CString strUrl;
		WeiBoApi api;
		strUrl = api.LookItem();
		const CHAR *pInfo = "id";
		const CHAR *pUserId = g_WeiBoData.GetLonginserInfo(pInfo);
		if(!pUserId) return;
		strUserId.Format(_T("%S"),pUserId);

		strUrl.Replace(_T("[USERID]"),strUserId.GetBuffer(0));
		strUrl.Replace(_T("[ITEMID]"),m_curItemId.GetBuffer(0));
		ShellExecute(NULL,_T("open"),strUrl.GetBuffer(0),NULL,NULL,SW_SHOWNORMAL);
}

void	ContentDialog3::OnRetweetedItem()
{
		if(m_curItemId.IsEmpty()) return;
		if(g_dialogRetweeted.IsWindowVisible()) return;
		g_dialogRetweeted.InitData(m_curItemId.GetBuffer(0));
		g_dialogRetweeted.ShowWindow(SW_SHOW);
}

void	ContentDialog3::OnCopyItem()
{		
		if(m_curItemId.IsEmpty()) return;
		CString strTemp ;
		CString strTemp1;
		g_WeiBoData.GetLoginItemText(m_curItemId.GetBuffer(0),strTemp,strTemp1);
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

void	ContentDialog3::OnDalogue()
{
		
}

BOOL	ContentDialog3::FindHtmlItem(IHTMLElement **pElement)
{
		IHTMLDocument2*   pDoc = NULL;
		GetDHtmlDocument(&pDoc);

		IHTMLElementCollection*   pIHTMLElementCollection=NULL; 
		if(pDoc-> get_all(&pIHTMLElementCollection)==S_OK) 
		{ 
			long   l; 
			pIHTMLElementCollection-> get_length(&l); 
			_variant_t index;  
			index.vt=VT_I4;   
			index.intVal=0;
			IDispatch*   pDisp; 

			CComBSTR   str; 
			CString strName = _T("lh");
			CString strStemp = _T("");
			for(DWORD i = 0; i < l; i ++){
				index.intVal = i;
				pIHTMLElementCollection-> item(index,index,&pDisp); 
				CComQIPtr <IHTMLElement,   &IID_IHTMLElement> pHTMLElement(pDisp); 
				if(pHTMLElement!=NULL) 
				{ 
					pHTMLElement->get_innerHTML(&str);
					//pHTMLElement->get_tagName(&str);
					strStemp.Format(_T("%s"),str);
					//strStemp.MakeUpper();
					if(strStemp.Find(strName) != -1){
						if(strStemp.Find(m_curItemId) != -1){
							pDisp->QueryInterface(IID_IHTMLElement,(void**)&*pElement);
						}
					}
					//if(strStemp == strName){
					//	pDisp->QueryInterface(IID_IHTMLElement,(void**)&*pElement);
					//	return TRUE;
					//}
				} 	
			}
		}
		return FALSE;
}

void	ContentDialog3::OnShowItemImage(LPCSTR value,LPCSTR value1)
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

void	ContentDialog3::OnUShowItemImage()
{
		g_showItemImage.SetWindowShow();
}

void	ContentDialog3::OnShowBigImage(WCHAR *value)
{
		g_showItemImage.OnShowBigImage(value);
}

void	ContentDialog3::InitItem()
{
		if(!m_bInit){
			m_bInit = TRUE;
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ReFreshCContent3,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
}
void	ContentDialog3::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
		TodayHtmlDialog::OnVScroll(nSBCode, nPos, pScrollBar);

		int max = 0;
		int min = 0;
		GetScrollRange(SB_VERT,&min,&max);
		LONG pos = GetScrollPos(SB_VERT);
		min = max - pos;
		if(min <= 10){
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ReFreshPreviousCContent3,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
}

BOOL	ContentDialog3::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
		TodayHtmlDialog::OnMouseWheel(nFlags, zDelta, pt);


		int max = 0;
		int min = 0;
		GetScrollRange(SB_VERT,&min,&max);
		LONG pos = GetScrollPos(SB_VERT);
		min = max - pos;
		if(min <= 10){
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ReFreshPreviousCContent3,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
		return TRUE;
}

void	ContentDialog3::OnTimer(UINT_PTR nIDEvent)
{
		if(nIDEvent == REFRESH_TIME){
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ReFreshNewCContent3,this,THREAD_PRIORITY_NORMAL,0,0,NULL);

			KillTimer(nIDEvent);
		}
		TodayHtmlDialog::OnTimer(nIDEvent);
}

void	ContentDialog3::OnDel()
{
		if(m_curItemId.IsEmpty()) return;

		CString strTemp;
		CString strApi;
		WeiBoApi api;
		strApi = api.DelMyItem();
		strApi.Replace(_T("[ITEMID]"),m_curItemId.GetBuffer(0));

		InStreamA instra;
		instra.PutWChar(strApi.GetBuffer(0));
		strTemp = g_weiBoHttp.OauthRequestString(instra);
		if(m_pEle){
			m_pEle->put_outerHTML(_T(""));
			m_pEle->Release();
		}
}