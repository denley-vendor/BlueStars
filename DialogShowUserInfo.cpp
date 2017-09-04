/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define DIALOG_SHOW_USERINFO_CPP

#include "stdafx.h"
#include "today.h"
#include "DialogShowUserInfo.h"

UINT	__cdecl Thread_ShowUserInfo(LPVOID pParam)
{
		if(g_TodayThreadLock.IsBStart(THREAD_LOCK_SETHTMLSTR)) {
			//TRACE("Thread Info: Thread_ShowUserInfo return \n");
			return 0;
		}
		g_TodayThreadLock.SetBStart(THREAD_LOCK_SETHTMLSTR,1);
		//TRACE("Thread Info: Thread_ShowUserInfo start \n");

		CString strUserId;
		WCHAR *pUserID = (WCHAR*)pParam;
		if(!pUserID) return 1;
		strUserId.Format(_T("%s"),pUserID);


		CString strTemp;
		CString strApi;
		WeiBoApi api;
		InStreamA instrA;
		strApi = api.GetOtherUserInfo() + _T("&user_id=") + strUserId;
		instrA.PutWChar(strApi.GetBuffer(0));
		strTemp = g_weiBoHttp.OauthRequestString(instrA);

		instrA.Empty();
		instrA.PutWCharToUTF8(strTemp.GetBuffer(0));

		const TiXmlElement *pXml1 = NULL;
		TiXmlDocument *pXml = new TiXmlDocument;
		pXml->Parse(instrA.GetBuf());
		if(!pXml) return 1;

		pXml1 = pXml->RootElement();
		if(!pXml1) {
			g_TodayThreadLock.SetBStop(THREAD_LOCK_SETHTMLSTR);
			SAFE_DELETE(pXml);
			return 0;
		}
		BOOL b = FALSE;
		b = g_WeiBoData.IsItemBValidate(pXml);
		if(!b) {
			g_TodayThreadLock.SetBStop(THREAD_LOCK_SETHTMLSTR);
			SAFE_DELETE(pXml);
			return 0;
		}
		g_WeiBoEngine.AnalysisUserInfo(pXml1,strTemp);
		SAFE_DELETE(pXml);

		g_lockStr.LockWrite();
		g_str.Empty();
		g_str = strTemp;
		g_showUserInfo.SendMessage(MSG_SETHTMLSTR,(WPARAM)g_str.GetBuffer(0));
		g_lockStr.UnlockWrite();

	//	g_str.Empty();
	//	g_str = _T("b_RoundCurve(\"imgtitle\", \"#C0D0F1\", \"#C0D0F1\", 2);");
	//	g_showUserInfo.SendMessage(MSG_EXECSCRIPT,(WPARAM)&g_str);

		//TRACE("Thread Info: Thread_ShowUserInfo end \n");
		g_TodayThreadLock.SetBStop(THREAD_LOCK_SETHTMLSTR);
		return 0;
}

IMPLEMENT_DYNCREATE(CDialogShowUserInfo, TodayHtmlDialog)

CDialogShowUserInfo::CDialogShowUserInfo(CWnd* pParent /*=NULL*/)
	: TodayHtmlDialog(CDialogShowUserInfo::IDD, CDialogShowUserInfo::IDH, pParent)
{
		m_bMouseOn = FALSE;
}

CDialogShowUserInfo::~CDialogShowUserInfo()
{
		m_strID.Empty();
}

void	CDialogShowUserInfo::DoDataExchange(CDataExchange* pDX)
{
		TodayHtmlDialog::DoDataExchange(pDX);
}

BOOL	CDialogShowUserInfo::OnInitDialog()
{
		TodayHtmlDialog::OnInitDialog();

		CString strTemp;
		GetExePath(strTemp);
		strTemp.Replace(_T("today.exe"),_T("todayinfo.ht"));
		m_pBrowserApp->Navigate(strTemp.GetBuffer(0),NULL,NULL,NULL,NULL);

		BOOL bOk = FALSE;
		bOk = dsSkinWindow(GetSafeHwnd(),SKIN_TYPE_DIALOG,_T("showuserinfo"),FALSE);
		if(!bOk) AfxMessageBox(_T("bOk = dsSkinWindow(GetSafeHwnd(),SKIN_TYPE_DIALOG"));


		LONG h = 0;
		IHTMLDocument2*   pDoc = NULL;
		IHTMLElement2 *pElement = NULL;
		IHTMLElement *pEl = NULL;
		GetDHtmlDocument(&pDoc);
		if(pDoc){
			pDoc->get_body(&pEl);
			if(pEl){
				pEl->QueryInterface(IID_IHTMLElement2,( void** )&pElement);
			}
			if(pElement){
				pElement->get_scrollHeight(&h);
			}
		}

		CRect rect;
		GetWindowRect(&rect);
		MoveWindow(rect.left,rect.top,rect.right - rect.left, rect.bottom - rect.top);


		return TRUE;  
}

BEGIN_MESSAGE_MAP(CDialogShowUserInfo, TodayHtmlDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_MESSAGE(MSG_SETHTMLSTR,SetHtmlString)
	ON_MESSAGE(MSG_HTMLREFRESH,ReFresh)
	ON_MESSAGE(MSG_EXECSCRIPT,ExecScript)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDialogShowUserInfo)
END_DHTML_EVENT_MAP()

CDialogShowUserInfo g_showUserInfo;
void	CDialogShowUserInfo::SetWindowShow(BOOL b /* = FALSE */)
{
		if(!b){
			KillTimer(SHOWUSERONFO_CLOSE_TIME);
			KillTimer(SHOWUSERONFO_SHOW_TIME);
			SetTimer(SHOWUSERONFO_CLOSE_TIME,1000,NULL);
		}
		else{
			KillTimer(SHOWUSERONFO_CLOSE_TIME);
			KillTimer(SHOWUSERONFO_SHOW_TIME);
			SetTimer(SHOWUSERONFO_SHOW_TIME,1000,NULL);
		}
}

BOOL	CDialogShowUserInfo::CanClose()
{
		CRect rect;
		CPoint pt;
		GetCursorPos(&pt);
		GetWindowRect(&rect);
		if(rect.PtInRect(pt)){
			return FALSE;
		}
		return TRUE;
}

void	CDialogShowUserInfo::ShowUserInfo(WCHAR *pCharUserId)
{
		if(g_TodayThreadLock.IsBStart(THREAD_LOCK_SETHTMLSTR)) {
			return ;
		}

		if(!pCharUserId) return;
		m_strID.Empty();
		m_strID.Format(_T("%s"),pCharUserId);
		CWinThread		*pThread = NULL;
		pThread = AfxBeginThread(Thread_ShowUserInfo,m_strID.GetBuffer(0),THREAD_PRIORITY_NORMAL,0,0,NULL);

}

void	CDialogShowUserInfo::OnShowWindow(BOOL bShow, UINT nStatus)
{
		TodayHtmlDialog::OnShowWindow(bShow, nStatus);
}

void	CDialogShowUserInfo::OnTimer(UINT_PTR nIDEvent)
{
		if(nIDEvent == SHOWUSERONFO_CLOSE_TIME && CanClose()){
			ShowWindow(SW_HIDE);
			KillTimer(SHOWUSERONFO_CLOSE_TIME);
		}
		else if(nIDEvent == SHOWUSERONFO_SHOW_TIME){
			ShowWindow(SW_SHOW);
			KillTimer(SHOWUSERONFO_SHOW_TIME);
		}
		TodayHtmlDialog::OnTimer(nIDEvent);
}

BOOL	CDialogShowUserInfo::OnEraseBkgnd(CDC* pDC)
{
		//CPen pen;
		//pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
		//pDC->SelectObject(pen);
		//CRect rect; 
		//GetClientRect(&rect);
		//pDC->RoundRect(&rect,CPoint(7,7));
		//pDC->FillRect(&rect,NULL);

		return TRUE;
}

int		CDialogShowUserInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (TodayHtmlDialog::OnCreate(lpCreateStruct) == -1)
			return -1;

		//dsSetSelfPaint(GetSafeHwnd(),FALSE);
		return 0;
}
