#pragma once
#ifndef INI_FILE_H
#define INI_FILE_H

class IniFile
{
private:
		CString		m_FileName;
public:
					IniFile();
virtual				~IniFile();

		BOOL		IsDirectoryExist(CString &dir);
		BOOL		CreateDir(CString &dir);
		BOOL		InitFile(WCHAR *pFileName);

		DWORD		GetValue(WCHAR *pSection,WCHAR *pKey,int defaultvalue=0);
		void		SetValue(WCHAR *pSection,WCHAR *pKey,DWORD a);

		void		GetString(WCHAR *pSection,WCHAR *pKey,CString &str);
		void		SetString(WCHAR *pSection,WCHAR *pKey,WCHAR *pStr);
		
};

#endif