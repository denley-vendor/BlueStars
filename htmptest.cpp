/* ����:����˧
   ���˲���:http://lanxingxing.net/
   лл��������ע�ҵ���վ:http://shanpao.info/ ɽ����,д�������������
*/
// htmptest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "today.h"
#include "htmptest.h"


// chtmptest �Ի���

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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(chtmptest, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(chtmptest)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// chtmptest ��Ϣ�������

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
