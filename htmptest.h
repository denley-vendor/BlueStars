#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// chtmptest 对话框

class chtmptest : public CDHtmlDialog
{
	DECLARE_DYNCREATE(chtmptest)

public:
	chtmptest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~chtmptest();
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_DIALOG_CONTENT6, IDH = IDR_HTML_HTMPTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
