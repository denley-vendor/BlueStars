/* ����:����˧
   ���˲���:http://lanxingxing.net/
   лл��������ע�ҵ���վ:http://shanpao.info/ ɽ����,д�������������
*/
#define CONTENTDIALOG4_CPP

#include "stdafx.h"
#include "today.h"
#include "ontentDialog4.h"


// ContentDialog4 �Ի���

IMPLEMENT_DYNCREATE(ContentDialog4, TodayHtmlDialog)

ContentDialog4::ContentDialog4(CWnd* pParent /*=NULL*/)
	: TodayHtmlDialog(ContentDialog4::IDD, ContentDialog4::IDH, pParent)
{
}

ContentDialog4::~ContentDialog4()
{
}

void ContentDialog4::DoDataExchange(CDataExchange* pDX)
{
	TodayHtmlDialog::DoDataExchange(pDX);
}

BOOL ContentDialog4::OnInitDialog()
{
	TodayHtmlDialog::OnInitDialog();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(ContentDialog4, TodayHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(ContentDialog4)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// ContentDialog4 ��Ϣ�������

HRESULT ContentDialog4::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT ContentDialog4::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}
ContentDialog4 g_contentDialog4;
