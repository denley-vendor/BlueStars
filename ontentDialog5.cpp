/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define CONTENTDIALOG5_CPP

#include "stdafx.h"
#include "today.h"
#include "ontentDialog5.h"


// ContentDialog5 对话框

IMPLEMENT_DYNCREATE(ContentDialog5, TodayHtmlDialog)

ContentDialog5::ContentDialog5(CWnd* pParent /*=NULL*/)
	: TodayHtmlDialog(ContentDialog5::IDD, ContentDialog5::IDH, pParent)
{

}

ContentDialog5::~ContentDialog5()
{
}

void ContentDialog5::DoDataExchange(CDataExchange* pDX)
{
	TodayHtmlDialog::DoDataExchange(pDX);
}

BOOL ContentDialog5::OnInitDialog()
{
	TodayHtmlDialog::OnInitDialog();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(ContentDialog5, TodayHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(ContentDialog5)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// ContentDialog5 消息处理程序

HRESULT ContentDialog5::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT ContentDialog5::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}
ContentDialog5 g_contentDialog5;