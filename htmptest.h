#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CDHtmlDialog��"
#endif 

// chtmptest �Ի���

class chtmptest : public CDHtmlDialog
{
	DECLARE_DYNCREATE(chtmptest)

public:
	chtmptest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~chtmptest();
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_DIALOG_CONTENT6, IDH = IDR_HTML_HTMPTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
