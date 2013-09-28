#pragma once
#ifndef	 PICDIALOG_H
#define  PICDIALOG_H
#include <gdiplus.h>
using namespace Gdiplus;

class CPicDialog : public CDialog
{
	DECLARE_DYNAMIC(CPicDialog)

public:
	CPicDialog(CWnd* pParent = NULL);   
	virtual ~CPicDialog();
	enum { IDD = IDD_DIALOG_CUTPIC };

protected:
	virtual		void	DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	virtual		BOOL	OnInitDialog();
	afx_msg		void	OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg		void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg		void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg		void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg		void	OnRButtonDown(UINT nFlags, CPoint point);
				void	ShowSelectInfo();
				int		GetCodecClsid(const WCHAR* format,ImageCodecInfo* pImageCodecInfo,CLSID* pClsid);
				void	SaveToJpg(CString strPath,CBitmap *pBitMap);
				BOOL	SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpFileName);
	inline		CString	&GetImagePath(){return m_strPath;}
private:
	CRect	m_selectRect;
	BOOL	m_bSelect;
	BOOL	m_bDclk;
	CDC		m_dcMem;
	CSize	m_size; 
	CBitmap	m_bmpScreen;
	CString m_strPath;
};
#endif