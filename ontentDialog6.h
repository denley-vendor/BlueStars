#pragma once
#ifndef CONTENTDIALOG6_H
#define CONTENTDIALOG6_H
#include "control/TodayHtmlDialog.h"
#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// ContentDialog6 对话框

class ContentDialog6 : public TodayHtmlDialog
{
	DECLARE_DYNCREATE(ContentDialog6)

public:
	ContentDialog6(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ContentDialog6();
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_DIALOG_CONTENT6, IDH = IDR_HTML_ONTENTDIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
extern ContentDialog6 g_contentDialog6;
#endif
