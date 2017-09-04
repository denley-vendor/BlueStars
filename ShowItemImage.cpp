/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define SHOWITEMIMAGE_CPP

#include "stdafx.h"
#include "today.h"
#include "ShowItemImage.h"


UINT	__cdecl Thread_ShowItemImage(LPVOID pParam)
{
		WCHAR *pUrl = (WCHAR*)pParam;
		if(!pUrl) return 0;

		CString strTemp;
		CString strTemp1;
		CString strTemp2;
		CString strTemp3;
		strTemp.Format(_T("%s"),pUrl);
		strTemp2 = strTemp.Right(strTemp.GetLength() - strTemp.ReverseFind(_T('//')) - 1);
		g_weiBoHttp.GetInfoImage(strTemp1,strTemp2.GetBuffer(0),strTemp);

		g_WeiBoEngine.AnalysisItemPic(strTemp1,strTemp3,strTemp.GetBuffer(0));

		g_lockStr.LockWrite();
		g_str.Empty();
		g_str = strTemp3;
		g_showItemImage.SendMessage(MSG_SETHTMLSTR,(WPARAM)g_str.GetBuffer(0));
		g_lockStr.UnlockWrite();
		return 0;
}

UINT	__cdecl Thread_ShowBigImage(LPVOID pParam)
{
		WCHAR *pInUrl = (WCHAR*)pParam;
		if(!pInUrl) return 0;

		CString strTemp;
		CString strTemp1;
		CString strTemp2;
		strTemp.Format(_T("%s"),pInUrl);
		strTemp.Replace(_T("thumbnail"),_T("bmiddle"));
		strTemp2 = strTemp.Right(strTemp.GetLength() - strTemp.ReverseFind(_T('//')) - 1);
		strTemp2 = _T("big_") + strTemp2 ;

		g_lockStr.LockWrite();
		g_weiBoHttp.GetInfoImage(strTemp1,strTemp2.GetBuffer(0),strTemp);
		g_lockStr.UnlockWrite();
		::ShellExecute(NULL,_T("open"),strTemp1.GetBuffer(0),NULL,NULL,SW_SHOW);

		return 0;
}



IMPLEMENT_DYNCREATE(ShowItemImage, TodayHtmlDialog)
ShowItemImage::ShowItemImage(CWnd* pParent /*=NULL*/)
	: TodayHtmlDialog(ShowItemImage::IDD, ShowItemImage::IDH, pParent)
{

}

ShowItemImage::~ShowItemImage()
{
}

void	ShowItemImage::DoDataExchange(CDataExchange* pDX)
{
		TodayHtmlDialog::DoDataExchange(pDX);
}

BOOL	ShowItemImage::OnInitDialog()
{
		TodayHtmlDialog::OnInitDialog();
		CString strTemp;
		GetExePath(strTemp);
		strTemp.Replace(_T("today.exe"),_T("item.t"));
		m_pBrowserApp->Navigate(strTemp.GetBuffer(0),NULL,NULL,NULL,NULL);
		return TRUE; 
}

BEGIN_MESSAGE_MAP(ShowItemImage, TodayHtmlDialog)
	ON_WM_TIMER()
	ON_MESSAGE(MSG_SETHTMLSTR,SetHtmlString)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(ShowItemImage)

END_DHTML_EVENT_MAP()

BEGIN_DISPATCH_MAP(ShowItemImage,TodayHtmlDialog)
	DISP_FUNCTION(ShowItemImage,"ShowBigImage",OnShowBigImage,VT_EMPTY,VTS_BSTR) 
END_DISPATCH_MAP()

ShowItemImage g_showItemImage;


void	ShowItemImage::OnTimer(UINT_PTR nIDEvent)
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

void	ShowItemImage::SetWindowShow(BOOL b /* = FALSE */)
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

BOOL	ShowItemImage::CanClose()
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

void	ShowItemImage::OnShowBigImage(WCHAR *pInUrl)
{		
		if(pInUrl){
			m_strImage2.Format(_T("%s"),pInUrl);
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_ShowBigImage,m_strImage2.GetBuffer(0),THREAD_PRIORITY_NORMAL,0,0,NULL);

		}
}

void	ShowItemImage::OnShowImage(WCHAR *pChar)
{
		if(!pChar) return;
		m_strImage.Format(_T("%s"),pChar);
		CWinThread		*pThread = NULL;
		pThread = AfxBeginThread(Thread_ShowItemImage,m_strImage.GetBuffer(0),THREAD_PRIORITY_NORMAL,0,0,NULL);

}