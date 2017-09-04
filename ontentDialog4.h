#pragma once
#ifndef CONTENTDIALOG4_H
#define CONTENTDIALOG4_H
#include "control/TodayHtmlDialog.h"

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// ContentDialog4 对话框

class ContentDialog4 : public TodayHtmlDialog
{
	DECLARE_DYNCREATE(ContentDialog4)

public:
	ContentDialog4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ContentDialog4();
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_DIALOG_CONTENT4, IDH = IDR_HTML_ONTENTDIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
extern ContentDialog4 g_contentDialog4;
#endif