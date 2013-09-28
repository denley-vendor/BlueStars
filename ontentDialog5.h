#pragma once
#ifndef CONTENTDIALOG5_H
#define CONTENTDIALOG5_H
#include "control/TodayHtmlDialog.h"

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// ContentDialog5 对话框

class ContentDialog5 : public TodayHtmlDialog
{
	DECLARE_DYNCREATE(ContentDialog5)

public:
	ContentDialog5(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ContentDialog5();
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_DIALOG_CONTENT5, IDH = IDR_HTML_ONTENTDIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
extern ContentDialog5 g_contentDialog5;
#endif