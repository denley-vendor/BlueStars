/* ����:����˧
   ���˲���:http://lanxingxing.net/
   лл��������ע�ҵ���վ:http://shanpao.info/ ɽ����,д�������������
*/
#define CONTENTDIALOG5_CPP

#include "stdafx.h"
#include "today.h"
#include "ontentDialog5.h"


// ContentDialog5 �Ի���

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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(ContentDialog5, TodayHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(ContentDialog5)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// ContentDialog5 ��Ϣ�������

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