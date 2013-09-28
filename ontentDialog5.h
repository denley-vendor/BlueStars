#pragma once
#ifndef CONTENTDIALOG5_H
#define CONTENTDIALOG5_H
#include "control/TodayHtmlDialog.h"

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CDHtmlDialog��"
#endif 

// ContentDialog5 �Ի���

class ContentDialog5 : public TodayHtmlDialog
{
	DECLARE_DYNCREATE(ContentDialog5)

public:
	ContentDialog5(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ContentDialog5();
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_DIALOG_CONTENT5, IDH = IDR_HTML_ONTENTDIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
extern ContentDialog5 g_contentDialog5;
#endif