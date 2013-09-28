#pragma once
#ifndef CONTENTDIALOG4_H
#define CONTENTDIALOG4_H
#include "control/TodayHtmlDialog.h"

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CDHtmlDialog��"
#endif 

// ContentDialog4 �Ի���

class ContentDialog4 : public TodayHtmlDialog
{
	DECLARE_DYNCREATE(ContentDialog4)

public:
	ContentDialog4(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ContentDialog4();
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_DIALOG_CONTENT4, IDH = IDR_HTML_ONTENTDIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
extern ContentDialog4 g_contentDialog4;
#endif