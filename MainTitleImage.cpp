/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define MAINTITLEIMAGE_CPP
#include "stdafx.h"
#include "today.h"
#include "DialogShowUserInfo.h"
#include "MainTitleImage.h"

MainTitleImage::MainTitleImage(void)
{
}

MainTitleImage::~MainTitleImage(void)
{
}
BEGIN_MESSAGE_MAP(MainTitleImage, CButton)
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void	MainTitleImage::OnMouseHover(UINT nFlags, CPoint point)
{

		CButton::OnMouseHover(nFlags, point);
}

void	 MainTitleImage::OnMouseLeave()
{
		g_showUserInfo.SetWindowShow();
		CButton::OnMouseLeave();
}

void	MainTitleImage::OnMouseMove(UINT nFlags, CPoint point)
{
		const CHAR *pInfo = "id";
		const CHAR *pUserId = g_WeiBoData.GetLonginserInfo(pInfo);
		CString stemp;
		stemp.Format(_T("%S"),pUserId);

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
		g_showUserInfo.ShowUserInfo(stemp.GetBuffer(0));

		CButton::OnMouseMove(nFlags, point);
}
