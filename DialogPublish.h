#pragma once
#ifndef DIALOGPUBLISH_H
#define DIALOGPUBLISH_H

class DialogPublish : public CDialog
{
	DECLARE_DYNAMIC(DialogPublish)

public:
	DialogPublish(CWnd* pParent = NULL);   
	virtual ~DialogPublish();

	enum { IDD = IDD_DIALOG_PUBLISH };

protected:
	virtual		void		DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	virtual		BOOL		OnInitDialog();
	afx_msg		BOOL		OnEraseBkgnd(CDC* pDC);
	afx_msg		void		OnBnClickedOk();
	afx_msg		void		OnBnClickedButtonClose();
	afx_msg		void		OnBnClickedButtonCutinsert();
	afx_msg		void		OnBnClickedButtonCut();

	inline		CString		&GetText(){return m_strText;}
	inline		CString		&GetPath(){return m_strPath;}
				void		SetPathText(WCHAR *pStr);
				void		CurlPublish();
				void		DoOk();
DECLARE_MESSAGE_MAP()
private:
				CString		m_strText;
				CString		m_strPath;
				CRichEditCtrl m_richEdit;

};

#endif