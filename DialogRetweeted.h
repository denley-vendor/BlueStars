#pragma once
#include "afxcmn.h"
#ifndef  DIALOGRETWEETED_H
#define  DIALOGRETWEETED_H
// DialogRetweeted 对话框

class DialogRetweeted : public CDialog
{
	DECLARE_DYNAMIC(DialogRetweeted)

public:
	DialogRetweeted(CWnd* pParent = NULL);  
	virtual ~DialogRetweeted();

	enum { IDD = IDD_DIALOG_RETWEETED };

protected:
	virtual		void	DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	virtual		BOOL	OnInitDialog();
	afx_msg		void	OnBnClickedButtonClose();
	afx_msg		void	OnBnClickedOk();
	afx_msg		BOOL	OnEraseBkgnd(CDC* pDC);
	inline		CString &GetText(){return m_strText;}
	inline		CString &GetId(){return m_strId;}
				void	InitData(WCHAR *pItemID);
				void	DoOk();
public:

	CString			m_strId;
	CString			m_strText;
	CRichEditCtrl	m_richEdit;
};
extern DialogRetweeted g_dialogRetweeted;

#endif