/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define CONTENTDIALOG6_CPP
#include "stdafx.h"
#include "today.h"
#include "ontentDialog6.h"


// ContentDialog6 对话框

IMPLEMENT_DYNCREATE(ContentDialog6, CDHtmlDialog)

ContentDialog6::ContentDialog6(CWnd* pParent /*=NULL*/)
	: TodayHtmlDialog(ContentDialog6::IDD, ContentDialog6::IDH, pParent)
{

}

ContentDialog6::~ContentDialog6()
{
}

void	ContentDialog6::DoDataExchange(CDataExchange* pDX)
{
		TodayHtmlDialog::DoDataExchange(pDX);
}

BOOL	ContentDialog6::OnInitDialog()
{
		TodayHtmlDialog::OnInitDialog();
		return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(ContentDialog6, TodayHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(ContentDialog6)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()

HRESULT ContentDialog6::OnButtonOK(IHTMLElement* /*pElement*/)
{
		OnOK();
		return S_OK;
}

HRESULT ContentDialog6::OnButtonCancel(IHTMLElement* /*pElement*/)
{
		OnCancel();
		return S_OK;
}

ContentDialog6 g_contentDialog6;