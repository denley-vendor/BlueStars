/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
// htmptest.cpp : 实现文件
//

#include "stdafx.h"
#include "today.h"
#include "htmptest.h"


// chtmptest 对话框

IMPLEMENT_DYNCREATE(chtmptest, CDHtmlDialog)

chtmptest::chtmptest(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(chtmptest::IDD, chtmptest::IDH, pParent)
{

}

chtmptest::~chtmptest()
{
}

void chtmptest::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL chtmptest::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(chtmptest, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(chtmptest)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// chtmptest 消息处理程序

HRESULT chtmptest::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT chtmptest::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}
