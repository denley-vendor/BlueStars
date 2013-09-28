// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef FILEDIALG_H
#define FILEDIALG_H

#pragma once
#include <afximpl.h>
class CFileDialog : public CCommonDialog
{
	DECLARE_DYNAMIC(CFileDialog)

public:
// Attributes
	__declspec(property(get=GetOFN)) OPENFILENAME m_ofn;
	const OPENFILENAME& GetOFN() const;
	OPENFILENAME& GetOFN();
	LPOPENFILENAME m_pOFN;

// Constructors
	explicit CFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL,
		DWORD dwSize = 0,
		BOOL bVistaStyle = TRUE);
	virtual ~CFileDialog();	

// Operations
	virtual INT_PTR DoModal();

	// Helpers for parsing file name after successful return
	// or during Overridable callbacks if OFN_EXPLORER is set
	CString GetPathName() const;  // return full path and filename
	CString GetFileName() const;  // return only filename
	CString GetFileExt() const;   // return only ext
	CString GetFileTitle() const; // return file title
	BOOL GetReadOnlyPref() const; // return TRUE if readonly checked

	// Enumerating multiple file selections
	POSITION GetStartPosition() const;
	CString GetNextPathName(POSITION& pos) const;

	// Helpers for custom templates
	void SetTemplate(UINT nWin3ID, UINT nWin4ID);
	void SetTemplate(LPCTSTR lpWin3ID, LPCTSTR lpWin4ID);

	// Other operations available while the dialog is visible
	CString GetFolderPath() const; // return full path
	void SetControlText(int nID, LPCSTR lpsz);
	#ifdef UNICODE
	void SetControlText(int nID, const wchar_t  *lpsz);
	#endif 
	void HideControl(int nID);
	void SetDefExt(LPCSTR lpsz);
#if WINVER >= 0x0600
	void UpdateOFNFromShellDialog();
	void ApplyOFNToShellDialog();
	IFileOpenDialog* GetIFileOpenDialog() throw();
	IFileSaveDialog* GetIFileSaveDialog() throw();
	IFileDialogCustomize* GetIFileDialogCustomize() throw();
#endif

// Overridable callbacks
protected:

#if WINVER >= 0x0600
	HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void **ppv);
#endif

	friend UINT_PTR CALLBACK _AfxCommDlgProc(HWND, UINT, WPARAM, LPARAM);
	virtual UINT OnShareViolation(LPCTSTR lpszPathName);
	virtual BOOL OnFileNameOK();
	virtual void OnLBSelChangedNotify(UINT nIDBox, UINT iCurSel, UINT nCode);

	// only called back if OFN_EXPLORER is set
	virtual void OnInitDone();
	virtual void OnFileNameChange();
	virtual void OnFolderChange();
	virtual void OnTypeChange();

// Implementation
#ifdef _DEBUG
public:
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	BOOL m_bVistaStyle;
	DWORD m_dwCookie;
	void* m_pIFileDialog;
	void* m_pIFileDialogCustomize;

	BOOL m_bOpenFileDialog;       // TRUE for file open, FALSE for file save
	CString m_strFilter;          // filter string
						// separate fields with '|', terminate with '||\0'
	TCHAR m_szFileTitle[64];       // contains file title after return
	TCHAR m_szFileName[_MAX_PATH]; // contains full path name after return

	OPENFILENAME*  m_pofnTemp;

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

protected:
	DECLARE_INTERFACE_MAP()

#if WINVER >= 0x0600

	BEGIN_INTERFACE_PART(FileDialogEvents, IFileDialogEvents)
		STDMETHOD(OnFileOk)(IFileDialog *);
		STDMETHOD(OnFolderChange)(IFileDialog *);
		STDMETHOD(OnFolderChanging)(IFileDialog *, IShellItem *);
		STDMETHOD(OnHelp)(IFileDialog *);
		STDMETHOD(OnSelectionChange)(IFileDialog *);
		STDMETHOD(OnShareViolation)(
			IFileDialog *pfd,
			IShellItem *psi,
			FDE_SHAREVIOLATION_RESPONSE *pResponse);
		STDMETHOD(OnTypeChange)(IFileDialog *);
		STDMETHOD(OnOverwrite)(IFileDialog *, IShellItem *, FDE_OVERWRITE_RESPONSE *);
	END_INTERFACE_PART_OPTIONAL(FileDialogEvents)

	BEGIN_INTERFACE_PART(FileDialogControlEvents, IFileDialogControlEvents)
		STDMETHOD(OnItemSelected)(IFileDialogCustomize *, DWORD, DWORD);
		STDMETHOD(OnButtonClicked)(IFileDialogCustomize *, DWORD);
		STDMETHOD(OnCheckButtonToggled)(IFileDialogCustomize *, DWORD, BOOL);
		STDMETHOD(OnControlActivating)(IFileDialogCustomize *, DWORD);
	END_INTERFACE_PART_OPTIONAL(FileDialogControlEvents)

#else
	
	BEGIN_INTERFACE_PART(FileDialogEvents, IUnknown)
	END_INTERFACE_PART_OPTIONAL(FileDialogEvents)
	
	BEGIN_INTERFACE_PART(FileDialogControlEvents, IUnknown)
	END_INTERFACE_PART_OPTIONAL(FileDialogControlEvents)

#endif
};

